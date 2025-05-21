from typing import List
from collections import deque

# Assuming Transaction class is in blockchain_simulation.transaction
from .transaction import Transaction

class Mempool:
    def __init__(self):
        """
        Initializes the Mempool with an empty deque to store transactions.
        """
        self.transactions: deque[Transaction] = deque()

    def add_transaction(self, transaction: Transaction):
        """
        Adds a transaction to the right of the mempool's deque.

        Args:
            transaction: The Transaction object to add.
        """
        self.transactions.append(transaction)

    def get_transactions(self, count: int) -> List[Transaction]:
        """
        Retrieves up to 'count' transactions from the left of the mempool's deque.
        These transactions are not removed from the mempool by this method.

        Args:
            count: The maximum number of transactions to retrieve.

        Returns:
            A list of Transaction objects. If 'count' is larger than the
            number of transactions in the mempool, all transactions are returned.
        """
        if count <= 0:
            return []
        
        num_to_retrieve = min(count, len(self.transactions))
        
        # Convert the relevant part of the deque to a list
        retrieved_list: List[Transaction] = []
        for i in range(num_to_retrieve):
            retrieved_list.append(self.transactions[i])
            
        return retrieved_list

    def remove_transactions(self, transactions_to_remove: List[Transaction]):
        """
        Removes the specified transactions from the mempool.

        Args:
            transactions_to_remove: A list of Transaction objects to remove.
        """
        # Convert deque to list, filter, and convert back to deque
        # This approach is acceptable for typical simulation mempool sizes.
        # For very large deques and frequent removals, other strategies might be considered,
        # but this is clear and handles the non-trivial removal from deque.
        
        # Create a set of ids for efficient lookup if transactions_to_remove can be large
        # Assuming Transaction objects are unique and hashable if used directly in a set.
        # If not, or if equality is complex, comparing by a unique ID (tx.id) is safer.
        # For this implementation, we'll assume 'tx not in transactions_to_remove' works
        # as expected, which implies Transaction objects have a proper __eq__ or are unique instances.
        # If Transaction objects might be different instances with same data, compare by ID.
        # Let's assume for now that object identity or __eq__ is sufficient.
        
        current_transactions_list = list(self.transactions)
        
        # Filter out the transactions to remove
        # This creates a new list, excluding items present in transactions_to_remove
        updated_transactions_list = [
            tx for tx in current_transactions_list if tx not in transactions_to_remove
        ]
        
        self.transactions = deque(updated_transactions_list)

    def get_size(self) -> int:
        """
        Returns the current number of transactions in the mempool.

        Returns:
            The integer count of transactions.
        """
        return len(self.transactions)

if __name__ == '__main__':
    # This section is for demonstration and testing purposes.
    # It will not run when the Mempool class is imported as a module.
    import datetime # For mock transaction timestamp

    print("--- Mempool Class Demonstration ---")

    # Mock Transaction class for self-contained testing in __main__
    # The actual Mempool class uses `from .transaction import Transaction`
    class MockTransactionForMempoolTest:
        def __init__(self, sender: str, receiver: str, amount: float, id_suffix: str = ""):
            self.sender = sender
            self.receiver = receiver
            self.amount = amount
            # Create a somewhat unique ID for testing; real tx id is a hash
            self.id = f"tx_{sender}_{receiver}_{amount}_{id_suffix}" 
            self.timestamp = datetime.datetime.now(datetime.timezone.UTC).isoformat()

        def to_dict(self) -> dict: # For printing
            return {
                "id": self.id,
                "sender": self.sender,
                "receiver": self.receiver,
                "amount": self.amount,
                "timestamp": self.timestamp
            }
        
        # For `tx not in transactions_to_remove` to work correctly based on object identity
        # or a proper equality check. If instances are different but data is same,
        # __eq__ and __hash__ would be needed in the actual Transaction class.
        # For this mock, object identity will be used if __eq__ is not defined.
        def __eq__(self, other):
            if isinstance(other, MockTransactionForMempoolTest):
                return self.id == other.id
            return False

        def __hash__(self):
            return hash(self.id)


    # Create some dummy transactions using the mock class
    tx1 = MockTransactionForMempoolTest(sender="Alice", receiver="Bob", amount=10, id_suffix="1")
    tx2 = MockTransactionForMempoolTest(sender="Charlie", receiver="David", amount=20, id_suffix="2")
    tx3 = MockTransactionForMempoolTest(sender="Eve", receiver="Frank", amount=5, id_suffix="3")
    tx4 = MockTransactionForMempoolTest(sender="Grace", receiver="Heidi", amount=15, id_suffix="4")


    mempool = Mempool()
    print(f"Initial Mempool size: {mempool.get_size()}") # Expected: 0

    # Add transactions
    mempool.add_transaction(tx1)
    mempool.add_transaction(tx2)
    mempool.add_transaction(tx3)
    print(f"\nMempool size after adding 3 transactions: {mempool.get_size()}") # Expected: 3

    # Get transactions (should not remove them)
    retrieved_tx_list = mempool.get_transactions(2)
    print(f"\nRetrieved {len(retrieved_tx_list)} transactions (expected 2):")
    for tx in retrieved_tx_list:
        print(tx.to_dict())
    
    # Verify they are tx1 and tx2 (order of retrieval from left)
    assert len(retrieved_tx_list) == 2
    assert retrieved_tx_list[0].sender == "Alice" # tx1
    assert retrieved_tx_list[1].sender == "Charlie" # tx2

    print(f"\nMempool size after get_transactions: {mempool.get_size()}") # Expected: 3 (still there)

    # Remove specific transactions (tx1, tx3)
    # For remove_transactions to work correctly, the Transaction objects in the list
    # must be comparable to those in the deque. Using mock with __eq__ and __hash__.
    mempool.remove_transactions([tx1, tx3])
    print(f"\nMempool size after removing tx1, tx3: {mempool.get_size()}") # Expected: 1

    # Verify remaining transaction is tx2
    remaining_tx_list = mempool.get_transactions(mempool.get_size()) # Get all remaining
    print("\nRemaining transaction(s) in mempool:")
    if remaining_tx_list:
        for tx in remaining_tx_list:
            print(tx.to_dict())
        assert len(remaining_tx_list) == 1
        assert remaining_tx_list[0].sender == "Charlie" # tx2 should be the only one left
    else:
        print("None")
        assert False, "Mempool should not be empty."


    # Test get_transactions with count > size
    mempool.add_transaction(tx4) # tx2, tx4 in mempool
    print(f"\nMempool size after adding tx4: {mempool.get_size()}") # Expected: 2 (tx2, tx4)
    
    retrieved_all = mempool.get_transactions(5) # Request 5, but only 2 available
    print(f"\nRetrieved {len(retrieved_all)} transactions when asking for 5 (expected 2):")
    for tx in retrieved_all:
        print(tx.to_dict())
    assert len(retrieved_all) == 2
    assert retrieved_all[0].sender == "Charlie" # tx2
    assert retrieved_all[1].sender == "Grace" # tx4

    # Test removing transactions not in mempool (should not error)
    tx_not_in_mempool = MockTransactionForMempoolTest("NotIn", "Mempool", 1)
    mempool.remove_transactions([tx_not_in_mempool])
    print(f"\nMempool size after attempting to remove a non-existent tx: {mempool.get_size()}") # Expected: 2
    assert mempool.get_size() == 2

    # Test get_transactions with count = 0
    retrieved_zero = mempool.get_transactions(0)
    print(f"\nRetrieved {len(retrieved_zero)} transactions when asking for 0 (expected 0).")
    assert len(retrieved_zero) == 0

    print("\n--- Mempool Class Demonstration Finished ---")
