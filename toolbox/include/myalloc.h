
#pragma once

#include <iostream>
#include <vector>

template <typename T>
struct myalloc {
    using value_type = T;

    T* allocate(std::size_t n) const {
        std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl;
        auto p = std::malloc(n * sizeof(T));
        if (!p) throw std::bad_alloc();
        return reinterpret_cast<T*>(p);
    }

    void deallocate(T* p, std::size_t n) const {
        std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl;
        std::free(p);
    }
};
