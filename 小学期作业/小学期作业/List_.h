// List_.h
#pragma once
#include <stdexcept>

template <typename T>
class List_ {
public:
    struct Node {
        T data;
        Node* next;
        Node(const T& d) : data(d), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int size_;

public:
    List_() : head(nullptr), tail(nullptr), size_(0) {}
    ~List_();

    void push_back(const T& data);
    int size() const { return size_; }
    bool empty() const { return size_ == 0; }
    Node* get_head() const { return head; } // 提供头节点访问
};

template <typename T>
List_<T>::~List_() {
    Node* current = head;
    while (current) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

template <typename T>
void List_<T>::push_back(const T& data) {
    Node* newNode = new Node(data);
    if (!head) {
        head = tail = newNode;
    }
    else {
        tail->next = newNode;
        tail = newNode;
    }
    size_++;
}