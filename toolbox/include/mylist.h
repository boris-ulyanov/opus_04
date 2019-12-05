
#pragma once

#include <cassert>
#include <iostream>

template <typename T, typename Allocator = std::allocator<T> >
class MyList {
    using value_type = T;

   private:
    struct Node {
        T value;
        Node* next;
    };

    class iterator {
        const Node* node;

       public:
        explicit iterator(const Node* _node) : node(_node){};

        auto operator!=(const iterator& rhs) const {
            return node != rhs.node;
        };
        auto& operator++() {
            node = node->next;
            return *this;
        };

        auto operator*() const {
            return node->value;
        };
    };

    // https://stackoverflow.com/questions/48354682/how-to-use-rebind-with-custom-allocator-and-custom-list
    // https://en.cppreference.com/w/cpp/memory/allocator_traits
    using node_alloc_t = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;

    // create an object of type node allocator
    node_alloc_t allocator;

    Node* front = nullptr;
    Node* back = nullptr;

   public:
    void push_back(const T& value) {
        auto item = allocator.allocate(1);
        item->value = value;
        item->next = nullptr;

        if (back) {
            back->next = item;
            back = item;
        } else {
            back = item;
            front = item;
        }
    };

    auto begin() {
        return iterator(front);
    }
    auto end() {
        return iterator(nullptr);
    }
};
