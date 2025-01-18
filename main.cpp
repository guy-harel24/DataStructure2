#include <iostream>
#include "HashTable.h"
#include <random>
#include <cassert>

struct Data {
    int value;

    explicit Data(int v) : value(v) {}

    int getKey() const {
        return value;
    }
};

void stressTestInsertAndRemove() {
    const int NUM_OPERATIONS = 100;
    const int NUM_KEYS = 100;

    HashTable<Data> hashTable;

    std::cout << "Stress Test: Insert and Remove" << std::endl;

    // Random number generator
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> keyDist(0, NUM_KEYS - 1);

    // Insert random keys
    for (int i = 0; i < NUM_OPERATIONS; ++i) {
        int key = keyDist(rng);
        hashTable.insert(key, new Data(key));

        if (i % (NUM_OPERATIONS / 10) == 0) {
            std::cout << "Progress: Insert " << (i * 100 / NUM_OPERATIONS) << "%, "
                      << "Load Factor: " << hashTable.getLoadFactor() << ", "
                      << "Size: " << hashTable.getSize() << std::endl;
        }
    }

    // Remove random keys
    for (int i = 0; i < NUM_OPERATIONS; ++i) {
       // int key = keyDist(rng);
        hashTable.remove(i);

        if (i % (NUM_OPERATIONS / 10) == 0) {
            std::cout << "Progress: Remove " << (i * 100 / NUM_OPERATIONS) << "%, "
                      << "Load Factor: " << hashTable.getLoadFactor() << ", "
                      << "Size: " << hashTable.getSize() << std::endl;
        }
    }

    std::cout << "Stress Test Complete!" << std::endl;
}

void edgeCaseTest() {
    HashTable<Data> hashTable;

    std::cout << "Edge Case Tests" << std::endl;

    // Insert single element and remove it
    assert(hashTable.insert(42, new Data(42)) == StatusType::SUCCESS);
    assert(hashTable.remove(42) == StatusType::SUCCESS);
    assert(hashTable.getElementsCount() == 0);

    // Attempt to remove a non-existent element
    assert(hashTable.remove(100) == StatusType::FAILURE);

    // Test large numbers
    int largeKey = 1000000000;
    hashTable.find(largeKey);
    assert(hashTable.insert(largeKey, new Data(largeKey)) == StatusType::SUCCESS);
    assert(hashTable.find(largeKey)->getKey() == largeKey);
    assert(hashTable.remove(largeKey) == StatusType::SUCCESS);

    // Insert many elements with the same hash index
    for (int i = 0; i < 50; ++i) {
        assert(hashTable.insert(i * hashTable.getSize(), new Data(i)) == StatusType::SUCCESS);
    }

    // Ensure resizing occurs properly
    assert(hashTable.getLoadFactor() <= 0.5);
    assert(hashTable.getSize() >= INITIAL_SIZE);

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
