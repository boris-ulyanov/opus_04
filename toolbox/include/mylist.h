
#pragma once

#include <cassert>
#include <iostream>

template <typename T, typename Allocator = std::allocator<T> >
class MyList {
    using value_type = T;

   private:
    struct Node {
        template <typename... Args>
        Node(Args&&... args) : value(std::forward<Args>(args)...), next(nullptr) {}

        T value;
        Node* next;
    };

    class iterator {
        const Node* node;

       public:
        explicit iterator(const Node* _node) : node(_node) {};

        auto operator!=(const iterator& rhs) const {
            return node != rhs.node;
        };
        auto& operator++() {
            node = node->next;
            return *this;
        };

        const T& operator*() const {
            return node->value;
        };
    };

    // https://stackoverflow.com/questions/48354682/how-to-use-rebind-with-custom-allocator-and-custom-list
    // https://en.cppreference.com/w/cpp/memory/allocator_traits
    using node_alloc_t = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;

    // create an object of type node allocator
    node_alloc_t allocator;

    Node* front;
    Node* back;

   public:
    MyList() : front(nullptr), back(nullptr){};

    ~MyList() {
        for (auto p = front; p != nullptr;) {
            auto next = p->next;
            allocator.destroy(p);
            allocator.deallocate(p, 1);
            p = next;
        }
    };

    template <typename... Args>
    void emplace_back(Args&&... args) {
        auto node = allocator.allocate(1);
        allocator.construct(node, std::forward<Args>(args)...);
        
        if (back) {
            back->next = node;
            back = node;
        } else {
            back = node;
            front = node;
        }
    }

    auto begin() {
        return iterator(front);
    }
    auto end() {
        return iterator(nullptr);
    }
};
