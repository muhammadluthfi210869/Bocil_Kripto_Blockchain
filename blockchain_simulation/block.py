import datetime
from typing import List
import hashlib # sha256 is a method of hashlib, so import hashlib
import json # For __main__ testing

# Assuming Transaction class is in blockchain_simulation/transaction.py
from .transaction import Transaction
# Assuming calculate_merkle_root is available for testing, though not directly used by Block.from_dict
from .merkle_tree import calculate_merkle_root


class Block:
    def __init__(self, index: int, previous_hash: str, difficulty: int, timestamp: datetime.datetime = None):
        self.index: int = index
        self.timestamp: datetime.datetime = timestamp if timestamp is not None else datetime.datetime.now(datetime.timezone.UTC)
        self.transactions: List[Transaction] = []
        self.previous_hash: str = previous_hash
        self.nonce: int = 0
        self.merkle_root: str = ""
        self.difficulty: int = difficulty
        self.hash: str = "" 

    def calculate_hash(self) -> str:
        data_string = (
            str(self.index) +
            self.timestamp.isoformat() +
            self.previous_hash +
            str(self.nonce) +
            self.merkle_root
        )
        data_bytes = data_string.encode('utf-8')
        hash_object = hashlib.sha256(data_bytes)
        return hash_object.hexdigest()

    def mine_block(self):
        if not self.merkle_root:
            # print("Warning: Mining block without a Merkle root. Defaulting to hash of empty string.")
            # In a real scenario, Merkle root must be properly calculated from transactions before mining.
            # For Block class, if transactions are added, merkle_root should be set externally.
            # If transactions are empty, this is the correct Merkle root.
            self.merkle_root = calculate_merkle_root([])


        target_prefix = "0" * self.difficulty
        self.nonce = 0 
        while True:
            current_hash = self.calculate_hash()
            if current_hash.startswith(target_prefix):
                self.hash = current_hash
                break
            self.nonce += 1

    def to_dict(self) -> dict:
        return {
            'index': self.index,
            'timestamp': self.timestamp.isoformat(),
            'transactions': [tx.to_dict() for tx in self.transactions],
            'previous_hash': self.previous_hash,
            'nonce': self.nonce,
            'merkle_root': self.merkle_root,
            'difficulty': self.difficulty,
            'hash': self.hash
        }

    @classmethod
    def from_dict(cls, data: dict) -> 'Block':
        """
        Creates a Block instance from a dictionary representation.
        """
        # Create the block instance with core identifying information.
        # Timestamp is parsed from ISO format.
        block = cls(
            index=data['index'],
            previous_hash=data['previous_hash'],
            difficulty=data['difficulty'],
            timestamp=datetime.datetime.fromisoformat(data['timestamp'])
        )
        
        # Manually set the other attributes from the dictionary data
        # This includes reconstructing Transaction objects using Transaction.from_dict
        block.transactions = [Transaction.from_dict(tx_data) for tx_data in data['transactions']]
        block.nonce = data['nonce']
        block.merkle_root = data['merkle_root']
        block.hash = data['hash'] # Crucially, use the stored hash
        
        # Optional: Verify if the recalculated hash based on loaded data would match stored hash.
        # This depends on all components (timestamp, nonce, merkle_root reconstruction) being perfect.
        # current_hash_check = block.calculate_hash()
        # if current_hash_check != block.hash and block.hash != "": # Don't warn if hash wasn't set (e.g. pre-mining)
        #     print(f"Warning: Recalculated hash {current_hash_check} does not match stored hash {block.hash} for block {block.index}. Using stored hash.")
        # For persistence, trusting the stored hash is standard.
            
        return block

if __name__ == '__main__':
    print("--- Block Class Demonstration & from_dict Test ---")

    # --- Test Case 1: Mining a block (modified to use real Transaction) ---
    print("\n--- Test Case 1: Mining a block (difficulty 3) ---")
    genesis_previous_hash = "0" * 64 
    
    # Create actual Transaction objects for testing Block.from_dict properly
    # This requires Transaction class (and its from_dict) to be working.
    tx1_real = Transaction(sender="Alice", receiver="Bob", amount=10.0)
    # To make IDs more deterministic for simple test runs if needed, can alter timestamp slightly or add delay
    # For now, rely on their natural creation.
    tx2_real = Transaction(sender="Bob", receiver="Charlie", amount=5.0)

    block_orig = Block(index=0, previous_hash=genesis_previous_hash, difficulty=3)
    block_orig.transactions = [tx1_real, tx2_real]
    
    if block_orig.transactions:
        transaction_ids_for_merkle = [tx.id for tx in block_orig.transactions]
        block_orig.merkle_root = calculate_merkle_root(transaction_ids_for_merkle)
    else:
        block_orig.merkle_root = calculate_merkle_root([])

    print(f"Mining block {block_orig.index} with difficulty {block_orig.difficulty}...")
    block_orig.mine_block()
    print(f"Block {block_orig.index} mined. Hash: {block_orig.hash}, Nonce: {block_orig.nonce}")

    print("\nOriginal Block data (to_dict()):")
    block_orig_dict = block_orig.to_dict()
    print(json.dumps(block_orig_dict, indent=2))

    # --- Test Case 2: Block.from_dict() ---
    print("\n--- Test Case 2: Reconstructing block from dictionary using Block.from_dict() ---")
    loaded_block = Block.from_dict(block_orig_dict)

    print("\nLoaded Block data (to_dict()):")
    loaded_block_dict = loaded_block.to_dict()
    print(json.dumps(loaded_block_dict, indent=2))

    # Verifications
    assert loaded_block.index == block_orig.index
    assert loaded_block.timestamp.isoformat() == block_orig.timestamp.isoformat() # Compare ISO strings for safety
    assert loaded_block.previous_hash == block_orig.previous_hash
    assert loaded_block.nonce == block_orig.nonce
    assert loaded_block.merkle_root == block_orig.merkle_root
    assert loaded_block.difficulty == block_orig.difficulty
    assert loaded_block.hash == block_orig.hash, "Loaded block hash must match original block hash."

    # Verify transactions
    assert len(loaded_block.transactions) == len(block_orig.transactions)
    for i in range(len(loaded_block.transactions)):
        tx_loaded = loaded_block.transactions[i]
        tx_orig = block_orig.transactions[i]
        assert tx_loaded.id == tx_orig.id
        assert tx_loaded.sender == tx_orig.sender
        assert tx_loaded.receiver == tx_orig.receiver
        assert tx_loaded.amount == tx_orig.amount
        assert tx_loaded.timestamp.isoformat() == tx_orig.timestamp.isoformat()
    
    # Critical check: does the loaded block's calculate_hash() method produce the same hash?
    # This confirms that all constituent parts (nonce, merkle_root, timestamp etc.) were correctly restored.
    assert loaded_block.calculate_hash() == loaded_block.hash, \
        f"Recalculated hash {loaded_block.calculate_hash()} of loaded block does not match its stored hash {loaded_block.hash}."

    print("\nBlock.from_dict() tests passed successfully.")
    
    # --- Test with an empty block (e.g. genesis block) ---
    print("\n--- Test Case 3: from_dict with an empty block (like a Genesis Block) ---")
    genesis_block_orig = Block(index=0, previous_hash="0", difficulty=4)
    genesis_block_orig.merkle_root = calculate_merkle_root([]) # Empty transactions
    genesis_block_orig.mine_block() # Mine it to get a hash and nonce

    genesis_block_dict = genesis_block_orig.to_dict()
    print("\nOriginal Genesis Block data (to_dict()):")
    print(json.dumps(genesis_block_dict, indent=2))

    loaded_genesis_block = Block.from_dict(genesis_block_dict)
    print("\nLoaded Genesis Block data (to_dict()):")
    print(json.dumps(loaded_genesis_block.to_dict(), indent=2))

    assert loaded_genesis_block.hash == genesis_block_orig.hash
    assert loaded_genesis_block.merkle_root == genesis_block_orig.merkle_root
    assert len(loaded_genesis_block.transactions) == 0
    assert loaded_genesis_block.calculate_hash() == loaded_genesis_block.hash

    print("\nBlock.from_dict() test with empty block passed successfully.")
    print("\nBlock class demonstration and from_dict tests finished.")
