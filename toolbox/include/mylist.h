
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

    // copy same allocator
    MyList(const MyList& other) : front(nullptr), back(nullptr) {
         for (const auto& p : other)
             push_back(p);
    }

    // move same allocator
    MyList(MyList&& other) : front(nullptr), back(nullptr) {
        std::swap(allocator, other.allocator);
        std::swap(front, other.front);
        std::swap(back, other.back);
    }

    // copy different allocator
    template <typename OtherAllocator>
    MyList(const MyList<T, OtherAllocator>& other) : front(nullptr), back(nullptr) {
        for (const auto& p : other)
            push_back(p);
    }

    // move different allocator
    template <typename OtherAllocator>
    MyList(MyList<T, OtherAllocator> && other) : front(nullptr), back(nullptr) {
        // ??? просто копируем
        for (const auto& p : other)
            push_back(p);
        other.~MyList<T, OtherAllocator>();    // ??? имеет смысл
    }

    ~MyList() {
        for (auto p = front; p != nullptr;) {
            auto next = p->next;
            allocator.destroy(p);
            allocator.deallocate(p, 1);
            p = next;
        }
        front = nullptr;
        back = nullptr;
    }

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

    void push_back(const T& value) {
        auto node = allocator.allocate(1);
        node->value = value;

        if (back) {
            back->next = node;
            back = node;
        } else {
            back = node;
            front = node;
        }
    }

    auto begin() const {
        return iterator(front);
    }
    auto end() const {
        return iterator(nullptr);
    }
};
