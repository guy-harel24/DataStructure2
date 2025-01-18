#ifndef DATASTRUCTURE02_HASHTABLE_H
#define DATASTRUCTURE02_HASHTABLE_H

#include "GenericList.h"
#include "wet2util.h"

const int INITIAL_SIZE = 15;

class HashFunction {
protected:
    int size;
public:
   explicit HashFunction(int size) : size(size) {}
    virtual ~HashFunction() = default;
    virtual int operator()(int x) const = 0;
    void setSize(int new_size) {
        HashFunction::size = new_size;
    }
};

class ModuloHash : public HashFunction {
public:
   explicit ModuloHash(int size) : HashFunction(size) {}
    int operator()(int x) const override {
        return x % size;
    }
};

template <typename T>
class HashTable {
    GenericList<T>* array; //GenericList is not the owner of the data
    HashFunction* hash;
    int size;
    int elements_count;

public:
    int getSize() const {
        return size;
    }


    int getElementsCount() const {
        return elements_count;
    }

    double getLoadFactor() const {
        return (double)elements_count / (double) size;
    }
    HashTable() : size(INITIAL_SIZE), elements_count(0), hash(new ModuloHash(INITIAL_SIZE)) {
        array = new GenericList<T>[size];
    }

    ~HashTable() {
        delete[] array;
        delete hash;
    }

    GenericListNode<T>* find(int key, int* index) {
        *index = (*hash)(key);
        return array[*index].find(key);
    }

    void copyToNewArray(int new_size) {
        auto* new_array = new GenericList<T>[new_size];
        hash->setSize(new_size);

        for (int i = 0; i < size; ++i) {
            GenericListNode<T>* head = array[i].getHead();
            while (head) {
                int key = head->getKey();
                T* data = head->getData();
                int index = (*hash)(key);
                new_array[index].insert(key, data);
                GenericListNode<T>* next = head->getNext();
                array[i].removeNode(head);
                head = next;
            }
        }

        delete[] array; // Free old array memory
        array = new_array;
        size = new_size;
    }

    StatusType insert(int key, T* data) {
        int index;
        if (find(key, &index)){
            return StatusType::FAILURE;
        }
        try {
            if (getLoadFactor() >= 0.5) {
                copyToNewArray(size * 2); // Resize if load factor >= 0.5
            }
            array[index].insert(key, data);
        } catch (std::bad_alloc&) {
            return StatusType::ALLOCATION_ERROR;
        }

        elements_count++;
        return StatusType::SUCCESS;
    }

    StatusType remove(int key) {
        int index;
        GenericListNode<T>* node = find(key, &index);
        if (node) {
            array[index].removeNode(node);
            elements_count--;
            return StatusType::SUCCESS;
        }
        return StatusType::FAILURE;
    }
};


#endif //DATASTRUCTURE02_HASHTABLE_H
