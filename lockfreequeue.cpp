#include <iostream>
#include <atomic>

using namespace std;

template<class T>
struct Node {
    T value;
    std::atomic<Node*> next;
    Node(T value): value(value), next(nullptr) { }
    Node(): next(nullptr){ }
};

template<class T>
class LockFreeQueue {
public:
    LockFreeQueue();
    ~LockFreeQueue();

    void enqueue(Node<T> *node);
    Node<T>* dequeue();
private:
    std::atomic<Node<T>*> head;
    std::atomic<Node<T>*> tail;
};

template<class T>
LockFreeQueue<T>::LockFreeQueue() {
    Node<T> *dummy = new Node<T>( T() );
    dummy->next = nullptr;
    head.store(dummy);
    tail = head.load();
}

template<class T>
LockFreeQueue<T>::~LockFreeQueue() {}

template<class T>
void LockFreeQueue<T>::enqueue(Node<T> *node) {
    while(true) {
        Node<T> *last = tail.load();
        node->next = nullptr;

        Node<T>* next = last->next.load();
        if(next == nullptr) {
            if (tail.compare_exchange_strong(last, node)){
                if (atomic_compare_exchange_strong(&(last->next), &next, node)) {
                    return;
                }
            }
        }

    }
}

template<class T>
Node<T>* LockFreeQueue<T>::dequeue() {
    while(true) {
        if (head.load() == nullptr) {
            continue;
        }else {
            Node<T>* first = head.load();
            Node<T>* next = first->next.load();
            if(first == head.load() && next != nullptr){
                if(head.compare_exchange_strong(first,next)){
                    return first;
                }
            }
        }

    }
}