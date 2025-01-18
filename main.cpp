#include <iostream>
#include "HashTable.h" // Include your hash table implementation

struct Data {
    int value;
    Data(int v) : value(v) {}
    int getKey() const {
        return value; // Key is the same as the value for simplicity
    }
};

int main() {
    // Create a hash table
    HashTable<Data> hashTable;

    // Initial capacity
    int initialCapacity = 15; // INITIAL_SIZE in your implementation
    std::cout << "Initial Capacity: " << initialCapacity << std::endl;

    // Insert elements and check resizing
    int numElements = 20; // More than half of initialCapacity to trigger resizing
    for (int i = 0; i < numElements; ++i) {
        Data* data = new Data(i);
        StatusType status = hashTable.insert(i, data);
        if (status == StatusType::SUCCESS) {
            std::cout << "Inserted: " << i << std::endl;
        } else {
            std::cout << "Failed to insert: " << i << std::endl;
        }

        // Print the load factor and capacity after each insertion
        std::cout << "Load Factor: " << hashTable.getLoadFactor() << ", Capacity: " << hashTable.getSize() << std::endl;
    }

    // Verify all elements are still accessible
    std::cout << "\nVerifying elements after resizing..." << std::endl;
    for (int i = 0; i < numElements; ++i) {
        int index;
        GenericListNode<Data>* node = hashTable.find(i, &index);
        if (node && node->getData()->getKey() == i) {
            std::cout << "Found: " << i << " in index: " << index << std::endl;
        } else {
            std::cout << "Error: Element " << i << " not found!" << std::endl;
        }
    }

    return 0;
}
