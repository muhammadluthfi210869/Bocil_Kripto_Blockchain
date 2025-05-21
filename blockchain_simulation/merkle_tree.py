import hashlib
from typing import List

def calculate_merkle_root(transaction_hashes: List[str]) -> str:
    """
    Calculates the Merkle root for a list of transaction hashes.

    Args:
        transaction_hashes: A list of transaction hashes (hex strings).

    Returns:
        The Merkle root hash (hex string).
    """
    # Base case: Empty list of transactions
    if not transaction_hashes:
        return hashlib.sha256(b'').hexdigest()

    # Base case: Single transaction
    if len(transaction_hashes) == 1:
        return transaction_hashes[0]

    # If the number of hashes is odd, duplicate the last hash
    current_level_hashes = list(transaction_hashes) # Make a copy to avoid modifying the original list if it's passed multiple times
    if len(current_level_hashes) % 2 != 0:
        current_level_hashes.append(current_level_hashes[-1])

    next_level_hashes: List[str] = []
    for i in range(0, len(current_level_hashes), 2):
        hash1 = current_level_hashes[i]
        hash2 = current_level_hashes[i+1]
        
        # Concatenate the two hashes
        combined_hash_data = hash1 + hash2
        
        # Encode to UTF-8 bytes
        combined_hash_bytes = combined_hash_data.encode('utf-8')
        
        # Calculate the SHA-256 hash
        new_hash = hashlib.sha256(combined_hash_bytes).hexdigest()
        next_level_hashes.append(new_hash)

    # Recursively call with the new list of hashes
    return calculate_merkle_root(next_level_hashes)

if __name__ == '__main__':
    # Helper for example usage, not part of the function itself
    h = lambda x: hashlib.sha256(x.encode()).hexdigest()

    print("Starting Merkle Tree calculation examples...\n")

    # Example 1: Even number of transactions
    tx_hashes_even = [h("tx1"), h("tx2"), h("tx3"), h("tx4")]
    print(f"Transaction Hashes (Even): {tx_hashes_even}")
    root_even = calculate_merkle_root(tx_hashes_even)
    print(f"Merkle Root for 4 transactions: {root_even}\n")
    # Expected: Depends on specific hash values, but structure is:
    # root( H(H(tx1,tx2), H(tx3,tx4)) )

    # Example 2: Odd number of transactions
    tx_hashes_odd = [h("txA"), h("txB"), h("txC")]
    print(f"Transaction Hashes (Odd): {tx_hashes_odd}")
    root_odd = calculate_merkle_root(tx_hashes_odd)
    print(f"Merkle Root for 3 transactions: {root_odd}\n")
    # Expected: root( H(H(txA,txB), H(txC,txC)) )

    # Example 3: Empty list of transactions
    tx_hashes_empty = []
    print(f"Transaction Hashes (Empty): {tx_hashes_empty}")
    root_empty = calculate_merkle_root(tx_hashes_empty)
    print(f"Merkle Root for 0 transactions: {root_empty}")
    print(f"Expected empty hash: {hashlib.sha256(b'').hexdigest()}\n")
    assert root_empty == hashlib.sha256(b'').hexdigest()

    # Example 4: Single transaction
    tx_hash_single = [h("tx_single")]
    print(f"Transaction Hashes (Single): {tx_hash_single}")
    root_single = calculate_merkle_root(tx_hash_single)
    print(f"Merkle Root for 1 transaction: {root_single}")
    print(f"Expected single hash: {tx_hash_single[0]}\n")
    assert root_single == tx_hash_single[0]

    # Example 5: Two transactions
    tx_hashes_two = [h("tx_pair1"), h("tx_pair2")]
    print(f"Transaction Hashes (Two): {tx_hashes_two}")
    root_two = calculate_merkle_root(tx_hashes_two)
    expected_root_two = hashlib.sha256((tx_hashes_two[0] + tx_hashes_two[1]).encode('utf-8')).hexdigest()
    print(f"Merkle Root for 2 transactions: {root_two}")
    print(f"Expected root for two: {expected_root_two}\n")
    assert root_two == expected_root_two
    
    print("Merkle Tree calculation examples finished.")
