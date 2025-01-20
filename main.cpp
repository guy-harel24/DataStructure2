#include <iostream>
#include <cassert>
#include <random>
#include "HashTable.h"

// Test data structure
struct Data {
    int value;

    explicit Data(int v) : value(v) {}

    int getKey() const {
        return value;
    }
};

// Stress Test for Insert and Remove operations
void stressTestInsertAndRemove() {
    const int NUM_OPERATIONS = 1000; // Total number of insertions and removals
    const int NUM_KEYS = 1000; // Number of unique keys
    HashTable<Data> hashTable;

    std::cout << "Stress Test: Insert and Remove" << std::endl;

    // Random number generator for key generation
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> keyDist(0, NUM_KEYS - 1);

    // Insert random keys
    for (int i = 0; i < NUM_OPERATIONS; ++i) {
        int key = keyDist(rng);
        Data* data = new Data(key); // Dynamically allocate data
        StatusType st = hashTable.insert(key, data);
        if(st == StatusType::FAILURE)
        {
            delete data;
        }

        if (i % (NUM_OPERATIONS / 10) == 0) {
            std::cout << "Progress: Insert " << (i * 100 / NUM_OPERATIONS) << "%, "
                      << "Load Factor: " << hashTable.getLoadFactor() << ", "
                      << "Size: " << hashTable.getSize() << std::endl;
        }
    }

    // Remove random keys
    for (int i = 0; i < NUM_OPERATIONS; ++i) {
        int key = keyDist(rng);
        hashTable.remove(key);
        if (i % (NUM_OPERATIONS / 10) == 0) {
            std::cout << "Progress: Remove " << (i * 100 / NUM_OPERATIONS) << "%, "
                      << "Load Factor: " << hashTable.getLoadFactor() << ", "
                      << "Size: " << hashTable.getSize() << std::endl;
        }
    }

    std::cout << "Stress Test Complete!" << std::endl;
}

// Edge case tests
void edgeCaseTest() {
    HashTable<Data> hashTable;

    std::cout << "Edge Case Tests" << std::endl;

    // Insert and remove a single element
    assert(hashTable.insert(42, new Data(42)) == StatusType::SUCCESS);
    assert(hashTable.remove(42) == StatusType::SUCCESS);
    assert(hashTable.getElementsCount() == 0);

    // Attempt to remove a non-existent element
    assert(hashTable.remove(100) == StatusType::FAILURE);

    // Test large number key
    int largeKey = 1000000000;
    hashTable.insert(largeKey, new Data(largeKey));
    assert(hashTable.remove(largeKey) == StatusType::SUCCESS);

    std::cout << "Edge Case Tests Passed!" << std::endl;
}

int main() {
    std::cout << "Starting Stress and Edge Case Tests..." << std::endl;

    // Run stress test
    stressTestInsertAndRemove();

    // Run edge case tests
    edgeCaseTest();

    std::cout << "All Tests Completed Successfully!" << std::endl;
    return 0;
}
