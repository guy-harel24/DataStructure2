#ifndef DATASTRUCTURE02_HASHTABLE_H
#define DATASTRUCTURE02_HASHTABLE_H

#include "GenericList.h"
#include "wet2util.h"

const int INITIAL_SIZE = 24;
const double LOAD_FACTOR_UPPER_LIMIT = 0.5;
const double LOAD_FACTOR_BOTTOM_LIMIT = 0.25;
const int RESIZING_FACTOR = 2;

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
        return (x % size + size) % size; // Ensures non-negative result
    }
};

template <typename T>
class HashTable {
    HashFunction* hash;
    int size;
    int elements_count;
    GenericList<T>* array; //GenericList is not the owner of the data - Yet!!

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
    HashTable() : hash(new ModuloHash(INITIAL_SIZE)), size(INITIAL_SIZE),
                        elements_count(0) {
        array = new GenericList<T>[size];
    }

    ~HashTable() {
        delete[] array;
        delete hash;
    }

    GenericListNode<T>* find(int key, int* index = nullptr){
        if(!index){
            int tmp_index = (*hash)(key);
            return array[tmp_index].find(key);
        }
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
                head->loseData();
                head = next;
            }
        }

        delete[] array; // Free old array memory
        array = new_array;
        size = new_size;
    }

    StatusType insert(int key, T* data) {
        int index;
        GenericListNode<T>* node = find(key, &index);
        if (node != nullptr){
            return StatusType::FAILURE;
        }
        try {
            if (getLoadFactor() >= LOAD_FACTOR_UPPER_LIMIT) {
                copyToNewArray(size * RESIZING_FACTOR); // Resize if load factor >= 0.5
            }
            array[index].insert(key, data);
            elements_count++;
            return StatusType::SUCCESS;
        } catch (std::bad_alloc&){
            return StatusType::ALLOCATION_ERROR;
        }
    }

    StatusType remove(int key) {
        int index;
        GenericListNode<T> *node = find(key, &index);
        if (!node) {
            return StatusType::FAILURE;
        }
        try {
            array[index].removeNode(node);
            elements_count--;
            if (getLoadFactor() < LOAD_FACTOR_BOTTOM_LIMIT && size > INITIAL_SIZE) {
                copyToNewArray(size / RESIZING_FACTOR);
            }
            return StatusType::SUCCESS;
        } catch (std::bad_alloc &) {
            return StatusType::ALLOCATION_ERROR;
        }
    }
    GenericList<T>* getArray(){
        return array;
    }

};


#endif //DATASTRUCTURE02_HASHTABLE_H
