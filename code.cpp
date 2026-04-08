#include <iostream>
#include <memory>
#include <optional>

template <typename T>
class DoublyLinkedList {
private:
    // Node structure definition
    struct Node {
        T value;
        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev;

        Node(const T& val) : value(val), next(nullptr) {}
    };

    std::shared_ptr<Node> head;
    std::weak_ptr<Node> tail;
    size_t length;

public:
    // Iterator class
    class iterator {
    private:
        std::shared_ptr<Node> current;

    public:
        iterator(std::shared_ptr<Node> node) : current(node) {}

        // Dereference operator to get value
        T& operator*() {
            return current->value;
        }

        // Post-increment operator
        iterator operator++(int) {
            iterator temp = *this;
            current = current->next;
            return temp;
        }

        // Equality operators
        bool operator==(const iterator& other) const {
            return current == other.current;
        }

        bool operator!=(const iterator& other) const {
            return current != other.current;
        }
    };

    // Constructor
    DoublyLinkedList() : head(nullptr), length(0) {}

    // No destructor needed, memory managed by shared_ptr

    // Check if list is empty
    bool empty() const {
        return length == 0;
    }

    // Get the size of the list
    size_t size() const {
        return length;
    }

    // Add element to the front
    void push_front(const T& value) {
        std::shared_ptr<Node> newNode = std::make_shared<Node>(value);
        if (empty()) {
            head = newNode;
            tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        length++;
    }

    // Add element to the back
    void push_back(const T& value) {
        std::shared_ptr<Node> newNode = std::make_shared<Node>(value);
        if (empty()) {
            head = newNode;
            tail = newNode;
        } else {
            std::shared_ptr<Node> tailNode = tail.lock();
            tailNode->next = newNode;
            newNode->prev = tailNode;
            tail = newNode;
        }
        length++;
    }

    // Remove and return the first element
    std::optional<T> pop_front() {
        if (!head) {
            return std::nullopt;
        }
        T val = head->value;
        head = head->next;
        if (head) {
            head->prev.reset();
        } else {
            tail.reset();
        }
        length--;
        return val;
    }

    // Remove and return the last element
    std::optional<T> pop_back() {
        if (empty()) {
            return std::nullopt;
        }
        std::shared_ptr<Node> tailNode = tail.lock();
        T val = tailNode->value;
        std::shared_ptr<Node> prevNode = tailNode->prev.lock();
        if (prevNode) {
            prevNode->next = nullptr;
            tail = prevNode;
        } else {
            head = nullptr;
            tail.reset();
        }
        length--;
        return val;
    }

    // Iterators
    iterator begin() {
        return iterator(head);
    }

    iterator end() {
        return iterator(nullptr);
    }
};

int main() {
    return 0;
}
