import datetime
import hashlib
import json

class Transaction:
    def __init__(self, sender: str, receiver: str, amount: float):
        self.sender = sender
        self.receiver = receiver
        self.amount = amount
        # Timestamp is set with full precision including microseconds
        self.timestamp = datetime.datetime.now(datetime.timezone.UTC)
        # ID is calculated based on initial data, including the precise timestamp
        self.id = self.calculate_hash()

    def calculate_hash(self) -> str:
        """
        Calculates the SHA-256 hash of the transaction details.
        Uses ISO format timestamp for hashing.
        """
        transaction_data = {
            'sender': self.sender,
            'receiver': self.receiver,
            'amount': self.amount,
            'timestamp': self.timestamp.isoformat() # Timestamp converted to string for hashing
        }
        
        # Sort the dictionary by keys for consistent hash results
        sorted_transaction_data = sorted(transaction_data.items())
        
        transaction_json = json.dumps(dict(sorted_transaction_data), sort_keys=True)
        
        hash_object = hashlib.sha256(transaction_json.encode('utf-8'))
        return hash_object.hexdigest()

    def to_dict(self) -> dict:
        """
        Returns a dictionary representation of the transaction.
        """
        return {
            'id': self.id,
            'sender': self.sender,
            'receiver': self.receiver,
            'amount': self.amount,
            'timestamp': self.timestamp.isoformat() # Store timestamp in ISO format
        }

    @classmethod
    def from_dict(cls, data: dict) -> 'Transaction':
        """
        Creates a Transaction instance from a dictionary representation.
        """
        # Instantiate with basic data; __init__ will calculate a hash.
        # However, we will override timestamp and id with values from data
        # to preserve the original transaction's state.
        transaction = cls(
            sender=data['sender'],
            receiver=data['receiver'],
            amount=data['amount']
        )
        
        # Override timestamp and id with the values from the dictionary
        # This is crucial for restoring the exact state, including original hash (id).
        transaction.timestamp = datetime.datetime.fromisoformat(data['timestamp'])
        transaction.id = data['id'] # Use the original ID from the stored data
        
        # Optional: Verify if the recalculated hash based on loaded data would match stored id.
        # This depends on whether fromisoformat produces an identical timestamp object as original.
        # current_hash_check = transaction.calculate_hash()
        # if current_hash_check != transaction.id:
        #     print(f"Warning: Recalculated hash {current_hash_check} does not match stored id {transaction.id} for tx {transaction.id}. Using stored id.")
            # This might happen due to subtle differences in timestamp string representation or precision.
            # For persistence, trusting the stored hash (id) is standard.
            
        return transaction

if __name__ == '__main__':
    print("--- Transaction Class Demonstration ---")
    # Example Usage (for testing, not part of the class definition)
    tx1 = Transaction(sender="Alice", receiver="Bob", amount=50.0)
    print(f"\nOriginal Transaction 1 ID: {tx1.id}")
    print(f"Original Transaction 1 Data: {json.dumps(tx1.to_dict(), indent=2)}")

    tx2 = Transaction(sender="Bob", receiver="Charlie", amount=25.0)
    print(f"\nOriginal Transaction 2 ID: {tx2.id}")
    print(f"Original Transaction 2 Data: {json.dumps(tx2.to_dict(), indent=2)}")

    # Test from_dict
    print("\n--- Testing Transaction.from_dict() ---")
    tx1_data = tx1.to_dict()
    loaded_tx1 = Transaction.from_dict(tx1_data)

    print(f"\nLoaded Transaction 1 ID: {loaded_tx1.id}")
    print(f"Loaded Transaction 1 Data: {json.dumps(loaded_tx1.to_dict(), indent=2)}")

    # Verifications
    assert loaded_tx1.id == tx1.id, "Loaded ID should match original ID."
    assert loaded_tx1.sender == tx1.sender, "Loaded sender should match original sender."
    assert loaded_tx1.receiver == tx1.receiver, "Loaded receiver should match original receiver."
    assert loaded_tx1.amount == tx1.amount, "Loaded amount should match original amount."
    # Timestamp objects might be different in memory but represent the same time
    assert loaded_tx1.timestamp == tx1.timestamp, "Loaded timestamp should represent the same time as original."
    # Ensure the string representation is identical as well, as used in hashing
    assert loaded_tx1.timestamp.isoformat() == tx1.timestamp.isoformat(), "Loaded timestamp ISO format should match original."
    
    # Verify that if calculate_hash is called on loaded_tx1, it still produces the same id
    # This relies on fromisoformat() perfectly reconstructing the timestamp object
    # such that its .isoformat() method yields the exact same string as the original.
    assert loaded_tx1.calculate_hash() == loaded_tx1.id, "Hash recalculation on loaded tx should match its ID."

    print("\nTransaction.from_dict() tests passed.")
    print("\nNote: The example usage in __main__ is for demonstration and testing.")
    print("It won't run when this file is imported as a module.")
