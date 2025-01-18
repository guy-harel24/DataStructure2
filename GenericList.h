#pragma once
#include <iostream>

template <typename T>
class GenericListNode;

template <typename T>
class GenericList {
protected:
    GenericListNode<T>* head = nullptr;
    GenericListNode<T>* tail = nullptr;

public:
    GenericList() = default;

    void clearList() {
        while (head) {
            GenericListNode<T>* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
    }

    ~GenericList() {
        clearList();
    }

    GenericListNode<T>* getTail() const {
        return tail;
    }

    GenericListNode<T>* getHead() const {
        return head;
    }

    GenericListNode<T>* find(int key) {
        while (head) {
            if (head->getKey() == key)
            {
                return head;
            }
                head = head->next;
        }
        return nullptr;
    }

    GenericListNode<T>* insert(int key, T* val) {
        if (head == nullptr) {
            head = new GenericListNode<T>(key, val, nullptr, nullptr);
            tail = head;
            return head;
        }
        GenericListNode<T>* temp = new GenericListNode<T>(key, val, nullptr, tail);
        tail->next = temp;
        tail = temp;
        return temp;
    }

    void removeNode(GenericListNode<T>* node) {
        if (node == nullptr) return;

        if (node->next != nullptr) node->next->last = node->last;
        if (node->last != nullptr) node->last->next = node->next;

        if (node == head) head = node->next;
        if (node == tail) tail = node->last;

        delete node;
    }


    GenericListNode<T>* getNode(int place) {
        int counter = 0;
        GenericListNode<T>* temp = head;
        while (temp && counter < place) {
            temp = temp->next;
            ++counter;
        }
        return temp;
    }

    void print() const {
        GenericListNode<T>* current = head;
        while (current) {
            std::cout << current->getKey() << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};

template <class T>
class GenericListNode {
    const int key;
    T* data;
    GenericListNode<T>* next = nullptr;
    GenericListNode<T>* last = nullptr;

public:
    GenericListNode(int key, T* data, GenericListNode<T>* next = nullptr, GenericListNode<T>* last = nullptr)
            : key(key), data(data), next(next), last(last) {}

    ~GenericListNode() = default;

    int getKey() const {
        return key;
    }

    T* getData() const {
        return data;
    }

    GenericListNode<T>* getNext() const {
        return next;
    }

    template <class U>
    friend class GenericList;
};
