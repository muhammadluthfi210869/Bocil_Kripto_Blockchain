import datetime
from typing import List, Optional
import json # Added for save_to_file and load_from_file

# Required imports
from .block import Block
from .transaction import Transaction
from .merkle_tree import calculate_merkle_root
from .mempool import Mempool

class Blockchain:
    def __init__(self, difficulty: int = 4, create_genesis: bool = True):
        """
        Initializes the Blockchain.

        Args:
            difficulty: The difficulty level for mining blocks.
            create_genesis: If True, a genesis block is created automatically.
        """
        self.chain: List[Block] = []
        self.difficulty: int = difficulty
        if create_genesis:
            self.create_genesis_block()

    def create_genesis_block(self):
        """
        Creates and adds the genesis block to the blockchain.
        """
        genesis_block = Block(
            index=0,
            previous_hash="0", 
            difficulty=self.difficulty,
            timestamp=datetime.datetime.now(datetime.timezone.UTC)
        )
        genesis_block.merkle_root = calculate_merkle_root([])
        genesis_block.mine_block()
        self.chain.append(genesis_block)

    def get_latest_block(self) -> Optional[Block]:
        if not self.chain:
            return None
        return self.chain[-1]

    def add_block(self, new_block: Block, transactions: List[Transaction]):
        latest_block = self.get_latest_block()
        if not latest_block:
            raise Exception("Blockchain has no genesis block. Cannot add new block.")

        new_block.previous_hash = latest_block.hash
        new_block.transactions = transactions
        
        if transactions:
            transaction_ids = [tx.id for tx in transactions]
        else:
            transaction_ids = []
        new_block.merkle_root = calculate_merkle_root(transaction_ids)
        new_block.difficulty = self.difficulty 
        new_block.mine_block()
        self.chain.append(new_block)

    def mine_pending_transactions(self, mempool: Mempool) -> Optional[Block]:
        transactions_per_block = 5
        pending_transactions = mempool.get_transactions(transactions_per_block)

        if not pending_transactions:
            print("No transactions in the mempool to mine.")
            return None

        latest_block = self.get_latest_block()
        if not latest_block:
             print("Error: No genesis block found. Cannot mine.")
             return None

        new_block_index = latest_block.index + 1
        new_block = Block(
            index=new_block_index,
            previous_hash="", 
            difficulty=self.difficulty
        )
        self.add_block(new_block, pending_transactions)
        mempool.remove_transactions(pending_transactions)
        print(f"New block #{new_block.index} mined and added to the blockchain with {len(pending_transactions)} transactions.")
        return new_block

    def is_chain_valid(self) -> bool:
        if not self.chain:
            # An empty chain might be valid if starting from scratch, but an initialized one should have genesis.
            # If create_genesis=False led to an empty chain, it's up to subsequent ops to populate it.
            # For a chain with blocks, it must start with a valid genesis.
            return True if not self.chain else False # Empty chain is valid before blocks are added/loaded.

        # Validate Genesis Block if chain is not empty
        if self.chain:
            genesis_block = self.chain[0]
            if genesis_block.index != 0: return False
            if genesis_block.hash != genesis_block.calculate_hash(): return False
            if not genesis_block.hash.startswith("0" * genesis_block.difficulty): return False
            expected_genesis_merkle_root = calculate_merkle_root([])
            if genesis_block.merkle_root != expected_genesis_merkle_root: return False

        # Validate Subsequent Blocks
        for i in range(1, len(self.chain)):
            current_block = self.chain[i]
            previous_block = self.chain[i-1]

            if current_block.hash != current_block.calculate_hash(): return False
            if current_block.previous_hash != previous_block.hash: return False
            if not current_block.hash.startswith("0" * current_block.difficulty): return False
            
            transaction_ids = [tx.id for tx in current_block.transactions] if current_block.transactions else []
            expected_merkle_root = calculate_merkle_root(transaction_ids)
            if current_block.merkle_root != expected_merkle_root: return False
        
        return True

    def save_to_file(self, filename: str):
        """
        Saves the blockchain's state (difficulty and chain) to a JSON file.
        """
        data_to_save = {
            'difficulty': self.difficulty,
            'chain': [block.to_dict() for block in self.chain]
        }
        try:
            with open(filename, 'w') as f:
                json.dump(data_to_save, f, indent=4)
            print(f"Blockchain saved to {filename}")
        except IOError as e:
            print(f"Error saving blockchain to file {filename}: {e}")

    @classmethod
    def load_from_file(cls, filename: str) -> Optional['Blockchain']:
        """
        Loads the blockchain's state from a JSON file.
        """
        try:
            with open(filename, 'r') as f:
                data_loaded = json.load(f)
            
            difficulty = data_loaded['difficulty']
            # Create a new Blockchain instance without creating a genesis block initially
            blockchain = cls(difficulty=difficulty, create_genesis=False)
            
            # Reconstruct the chain
            if 'chain' in data_loaded:
                blockchain.chain = [Block.from_dict(block_data) for block_data in data_loaded['chain']]
            
            # Validate the loaded chain
            if not blockchain.is_chain_valid():
                print("Warning: Loaded blockchain is invalid according to is_chain_valid().")
                # Depending on policy, one might raise an error or return None/corrupted chain
                # For now, we print a warning and return the chain as loaded.
            else:
                print(f"Blockchain loaded successfully from {filename}.")
            
            # Ensure there's at least one block if chain data was present, otherwise create genesis
            # This handles case where file might exist but chain is empty or not in file.
            if not blockchain.chain and data_loaded.get('chain'): 
                 # This means chain was in file but empty, which is unusual if not genesis.
                 # If chain was not in file or empty, and we need a functional blockchain:
                 pass # blockchain.create_genesis_block() could be an option if rules require non-empty chain
            elif not blockchain.chain and not data_loaded.get('chain'):
                 # If no chain data at all, and we expect a functional chain, create genesis.
                 # However, load_from_file should primarily restore, not initialize if empty.
                 # Current logic: if file has empty chain, loaded chain is empty.
                 print("Loaded blockchain is empty (no blocks found in file or chain data was empty).")


            return blockchain
            
        except FileNotFoundError:
            print(f"Error: File {filename} not found. Cannot load blockchain.")
            return None
        except json.JSONDecodeError:
            print(f"Error: Could not decode JSON from {filename}. File might be corrupted.")
            return None
        except Exception as e: # Catch other potential errors during loading (e.g., missing keys)
            print(f"An unexpected error occurred while loading blockchain from {filename}: {e}")
            return None


if __name__ == '__main__':
    def print_block_json(block_data_dict): # Renamed to avoid conflict if json was not imported earlier
        # Assuming json is available in this __main__ scope
        print(json.dumps(block_data_dict, indent=2))

    print("--- Blockchain Simulation with Persistence ---")
    blockchain_filename = "blockchain_data.json"

    # Initialize Blockchain and Mempool
    original_blockchain = Blockchain(difficulty=3) # Lower difficulty for faster testing
    mempool = Mempool()

    print(f"\nOriginal Blockchain initialized with difficulty: {original_blockchain.difficulty}")
    print("Genesis Block created:")
    print_block_json(original_blockchain.get_latest_block().to_dict())
    
    # Add transactions to mempool and mine a block
    print("\n--- Adding Transactions and Mining First Block ---")
    tx1 = Transaction(sender="Satoshi", receiver="Alice", amount=100.0)
    tx2 = Transaction(sender="Alice", receiver="Bob", amount=10.0)
    mempool.add_transaction(tx1)
    mempool.add_transaction(tx2)
    
    mined_block_1 = original_blockchain.mine_pending_transactions(mempool)
    if mined_block_1:
        print(f"Mined Block #1: {mined_block_1.hash[:10]}...")
    print(f"Mempool size: {mempool.get_size()}")

    # Add more transactions and mine another block
    print("\n--- Adding More Transactions and Mining Second Block ---")
    tx3 = Transaction(sender="Bob", receiver="Charlie", amount=5.0)
    tx4 = Transaction(sender="Charlie", receiver="David", amount=2.5)
    mempool.add_transaction(tx3)
    mempool.add_transaction(tx4)

    mined_block_2 = original_blockchain.mine_pending_transactions(mempool)
    if mined_block_2:
        print(f"Mined Block #2: {mined_block_2.hash[:10]}...")
    print(f"Mempool size: {mempool.get_size()}")
    
    print(f"\nOriginal blockchain has {len(original_blockchain.chain)} blocks.")
    original_latest_block_hash = original_blockchain.get_latest_block().hash

    # Save the blockchain
    print("\n--- Saving Blockchain to File ---")
    original_blockchain.save_to_file(blockchain_filename)

    # Load the blockchain into a new instance
    print("\n--- Loading Blockchain from File ---")
    loaded_blockchain = Blockchain.load_from_file(blockchain_filename)

    if loaded_blockchain:
        print("\n--- Verifying Loaded Blockchain ---")
        assert loaded_blockchain.difficulty == original_blockchain.difficulty, "Loaded difficulty mismatch."
        assert len(loaded_blockchain.chain) == len(original_blockchain.chain), "Loaded chain length mismatch."
        
        # Check if the latest block hashes match
        loaded_latest_block = loaded_blockchain.get_latest_block()
        assert loaded_latest_block is not None, "Loaded chain has no latest block."
        assert loaded_latest_block.hash == original_latest_block_hash, "Latest block hash mismatch."
        
        # Validate the entire loaded chain's integrity
        is_valid = loaded_blockchain.is_chain_valid()
        print(f"Is loaded blockchain valid? {is_valid}")
        assert is_valid, "Loaded blockchain failed validation!"

        print("\nLoaded blockchain verified successfully.")
        
        # Optional: Compare all blocks (can be verbose)
        # for i in range(len(loaded_blockchain.chain)):
        #     assert loaded_blockchain.chain[i].to_dict() == original_blockchain.chain[i].to_dict(), \
        #            f"Mismatch in block {i}"
        # print("All blocks in loaded chain match original blocks.")

    else:
        print("Failed to load blockchain. Cannot perform verification.")

    # Test loading from a non-existent file
    print("\n--- Testing Load from Non-Existent File ---")
    non_existent_blockchain = Blockchain.load_from_file("non_existent_file.json")
    assert non_existent_blockchain is None, "Loading non-existent file should return None."
    print("Test for loading non-existent file passed (returned None).")

    print("\n--- Blockchain Persistence Test Finished ---")
