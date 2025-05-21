from typing import List, Any, TYPE_CHECKING

# Imports for type hinting and for objects stored in operation.data
from .transaction import Transaction 
from .block import Block

if TYPE_CHECKING:
    # These imports are for type hinting only for Blockchain and Mempool parameters
    from .blockchain import Blockchain
    from .mempool import Mempool

class UndoableOperation:
    def __init__(self, operation_type: str, data: Any):
        self.operation_type: str = operation_type
        self.data: Any = data

    def __repr__(self) -> str:
        data_repr = str(self.data)
        if len(data_repr) > 50: # Truncate long data representations
            data_repr = data_repr[:47] + "..."
        return f"UndoableOperation(type='{self.operation_type}', data='{data_repr}')"


class OperationStack:
    def __init__(self):
        self.undo_stack: List[UndoableOperation] = []
        self.redo_stack: List[UndoableOperation] = []

    def record_operation(self, operation_type: str, data: Any):
        operation = UndoableOperation(operation_type, data)
        self.undo_stack.append(operation)
        self.redo_stack.clear()

    def undo(self, blockchain: 'Blockchain', mempool: 'Mempool'):
        if not self.undo_stack:
            print("Nothing to undo.")
            return

        operation = self.undo_stack.pop()

        if operation.operation_type == "MINE_BLOCK":
            # Data: {'block': mined_block_object, 'transactions_in_block': list_of_tx_objects}
            mined_block: Block = operation.data['block']
            transactions_to_readd: List[Transaction] = operation.data['transactions_in_block']
            
            latest_block = blockchain.get_latest_block()
            if latest_block and latest_block.hash == mined_block.hash:
                blockchain.chain.pop()
                # Add transactions back to mempool. Reversing order and adding to front (if possible)
                # would be most accurate. For now, add to end (default for deque.append).
                # Using reversed() so if add_transaction appends, it's like pushing them back in order.
                for tx in reversed(transactions_to_readd):
                    # Assuming mempool.add_transaction takes tx. If it needs add_to_front,
                    # that's a Mempool change. For now, standard add.
                    mempool.add_transaction(tx) 
                print(f"Undid MINE_BLOCK operation for block #{mined_block.index}.")
            else:
                print(f"Error undoing MINE_BLOCK: Blockchain state inconsistent. Expected block hash {mined_block.hash[:10]}..., found {latest_block.hash[:10] if latest_block else 'None'}.")
                # If state is inconsistent, push operation back to undo_stack to avoid losing it.
                self.undo_stack.append(operation) 
                return # Stop processing to avoid further issues

        elif operation.operation_type == "ADD_TRANSACTION_TO_MEMPOOL":
            # Data: transaction_object
            transaction_to_remove: Transaction = operation.data
            mempool.remove_transactions([transaction_to_remove]) # remove_transactions expects a list
            print(f"Undid ADD_TRANSACTION_TO_MEMPOOL operation for tx {transaction_to_remove.id[:10]}...")
            
        else:
            print(f"Unknown operation type '{operation.operation_type}' for undo. Cannot process.")
            # If unknown, push back to undo_stack to avoid losing it.
            self.undo_stack.append(operation)
            return

        self.redo_stack.append(operation)

    def redo(self, blockchain: 'Blockchain', mempool: 'Mempool'):
        if not self.redo_stack:
            print("Nothing to redo.")
            return

        operation = self.redo_stack.pop()

        if operation.operation_type == "MINE_BLOCK":
            mined_block: Block = operation.data['block']
            transactions_in_block: List[Transaction] = operation.data['transactions_in_block']
            
            # Directly append the block, bypassing normal checks and mining, as we are restoring exact state.
            # This assumes the blockchain's state (previous block hash) is consistent with this block.
            # A more robust check could be added here if needed.
            latest_block = blockchain.get_latest_block()
            if latest_block and latest_block.hash == mined_block.previous_hash:
                blockchain.chain.append(mined_block)
                mempool.remove_transactions(transactions_in_block)
                print(f"Redid MINE_BLOCK operation for block #{mined_block.index}.")
            else:
                prev_hash_expected = mined_block.previous_hash[:10] if mined_block else 'N/A'
                latest_hash_found = latest_block.hash[:10] if latest_block else 'None'
                print(f"Error redoing MINE_BLOCK: Blockchain state inconsistent. Expected prev_hash {prev_hash_expected}..., found latest block hash {latest_hash_found}...")
                self.redo_stack.append(operation) # Push back if failed
                return

        elif operation.operation_type == "ADD_TRANSACTION_TO_MEMPOOL":
            transaction_to_add: Transaction = operation.data
            mempool.add_transaction(transaction_to_add)
            print(f"Redid ADD_TRANSACTION_TO_MEMPOOL operation for tx {transaction_to_add.id[:10]}...")
            
        else:
            print(f"Unknown operation type '{operation.operation_type}' for redo. Cannot process.")
            self.redo_stack.append(operation) # Push back if failed
            return

        self.undo_stack.append(operation)

if __name__ == '__main__':
    import datetime # For Transaction timestamp if not handled by default in mock

    print("--- OperationStack Full Undo/Redo Logic Test ---")

    # Dummy classes for Blockchain and Mempool for standalone testing
    class MockMempool:
        def __init__(self): self.transactions: List[Transaction] = [] # Store actual Transaction objects
        def add_transaction(self, tx: Transaction, add_to_front=False): # add_to_front is hypothetical
            if add_to_front: # Simulate adding to front for undo of MINE_BLOCK
                self.transactions.insert(0, tx)
                print(f"MockMempool: Adding to front: {tx.id[:7]}")
            else:
                self.transactions.append(tx)
                print(f"MockMempool: Adding {tx.id[:7]}")
        def remove_transactions(self, txs_to_remove: List[Transaction]):
            ids_to_remove = {tx.id for tx in txs_to_remove}
            self.transactions = [tx for tx in self.transactions if tx.id not in ids_to_remove]
            print(f"MockMempool: Removing {[tx.id[:7] for tx in txs_to_remove]}")
        def get_transactions(self, count): # For MINE_BLOCK simulation
            return self.transactions[:count]
        def __repr__(self): return f"MockMempool({[tx.id[:7] for tx in self.transactions]})"

    class MockBlockchain:
        def __init__(self): self.chain: List[Block] = []
        def get_latest_block(self) -> Block | None: return self.chain[-1] if self.chain else None
        def __repr__(self): return f"MockBlockchain(blocks={[b.index for b in self.chain]}, latest_hash={self.get_latest_block().hash[:7] if self.get_latest_block() else 'None'})"

    op_stack = OperationStack()
    mock_bc = MockBlockchain()
    mock_mp = MockMempool()

    # 1. User adds Transaction A
    print("\nStep 1: Add Transaction txA")
    txA = Transaction(sender="Alice", receiver="Bob", amount=10.0) # Real Transaction
    op_stack.record_operation("ADD_TRANSACTION_TO_MEMPOOL", txA)
    mock_mp.add_transaction(txA) # Simulate action
    print(f"Undo: {op_stack.undo_stack}, Redo: {op_stack.redo_stack}, Mempool: {mock_mp}")

    # 2. User adds Transaction B
    print("\nStep 2: Add Transaction txB")
    txB = Transaction(sender="Charlie", receiver="David", amount=20.0) # Real Transaction
    op_stack.record_operation("ADD_TRANSACTION_TO_MEMPOOL", txB)
    mock_mp.add_transaction(txB)
    print(f"Undo: {op_stack.undo_stack}, Redo: {op_stack.redo_stack}, Mempool: {mock_mp}")

    # 3. User mines a block (containing txA, txB)
    print("\nStep 3: Mine Block (Block0 with txA, txB)")
    # Simulate block creation for testing
    mined_block_0 = Block(index=0, previous_hash="0"*64, difficulty=1) # Real Block
    mined_block_0.transactions = [txA, txB] # Assume these were taken from mempool
    mined_block_0.hash = "dummyhash_block0_" + txA.id[:5] # Simulate a hash
    mined_block_0.merkle_root = "dummy_merkle_" + txB.id[:5] # Simulate merkle
    
    op_data_mine_block_0 = {'block': mined_block_0, 'transactions_in_block': [txA, txB]}
    op_stack.record_operation("MINE_BLOCK", op_data_mine_block_0)
    # Simulate action: add to chain, remove from mempool
    mock_bc.chain.append(mined_block_0)
    mock_mp.remove_transactions([txA, txB])
    print(f"Undo: {op_stack.undo_stack}, Redo: {op_stack.redo_stack}")
    print(f"Blockchain: {mock_bc}, Mempool: {mock_mp}")
    
    # 4. User calls UNDO (should undo MINE_BLOCK)
    print("\nStep 4: Undo (MINE_BLOCK)")
    op_stack.undo(mock_bc, mock_mp)
    print(f"Undo: {op_stack.undo_stack}, Redo: {op_stack.redo_stack}")
    print(f"Blockchain: {mock_bc}, Mempool: {mock_mp}")
    assert len(mock_bc.chain) == 0, "Blockchain should be empty after undoing MINE_BLOCK"
    assert txA in mock_mp.transactions and txB in mock_mp.transactions, "Transactions should be back in mempool"

    # 5. User calls UNDO (should undo ADD_TRANSACTION_TO_MEMPOOL for txB)
    print("\nStep 5: Undo (ADD_TX B)")
    op_stack.undo(mock_bc, mock_mp)
    print(f"Undo: {op_stack.undo_stack}, Redo: {op_stack.redo_stack}")
    print(f"Blockchain: {mock_bc}, Mempool: {mock_mp}")
    assert txB not in mock_mp.transactions, "txB should be removed from mempool"
    assert txA in mock_mp.transactions, "txA should still be in mempool"

    # 6. User calls REDO (should redo ADD_TRANSACTION_TO_MEMPOOL for txB)
    print("\nStep 6: Redo (ADD_TX B)")
    op_stack.redo(mock_bc, mock_mp)
    print(f"Undo: {op_stack.undo_stack}, Redo: {op_stack.redo_stack}")
    print(f"Blockchain: {mock_bc}, Mempool: {mock_mp}")
    assert txB in mock_mp.transactions, "txB should be re-added to mempool"

    # 7. User calls REDO (should redo MINE_BLOCK)
    print("\nStep 7: Redo (MINE_BLOCK)")
    op_stack.redo(mock_bc, mock_mp)
    print(f"Undo: {op_stack.undo_stack}, Redo: {op_stack.redo_stack}")
    print(f"Blockchain: {mock_bc}, Mempool: {mock_mp}")
    assert len(mock_bc.chain) == 1 and mock_bc.chain[0] == mined_block_0, "Block0 should be back in chain"
    assert not mock_mp.transactions, "Mempool should be empty after redoing MINE_BLOCK"
    
    print("\n--- Full Undo/Redo Logic Test Finished ---")
