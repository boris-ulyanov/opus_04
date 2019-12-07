
#pragma once

#include <cassert>
#include <iostream>

template <typename T, size_t capacity>
struct Bucket;

template <typename T, size_t capacity>
struct Chunk {
    Bucket<T, capacity>* bucket;
    T payload;
};

template <typename T, size_t capacity>
struct Bucket {
    size_t size = 0;
    Chunk<T, capacity> chunks[capacity];
};

template <typename T, size_t capacity>
struct MyAlloc {
    using value_type = T;
    Bucket<T, capacity>* filling = nullptr;

    T* allocate(std::size_t n) {
        if (n != 1) throw std::bad_alloc();

        if (!filling) {
            filling = (Bucket<T, capacity>*)malloc(sizeof(Bucket<T, capacity>));
            if (!filling) throw std::bad_alloc();
            filling->size = 0;
            for (size_t i = 0; i < capacity; ++i) filling->chunks[i].bucket = filling;
        }

        auto rv = &(filling->chunks[filling->size].payload);

        ++filling->size;
        if (filling->size >= capacity) filling = nullptr;

        return rv;
    }

    void deallocate(T* p, std::size_t n) {
        assert(n == 1);

        auto chunk = (Chunk<T, capacity>*)(((char*)p) - sizeof(size_t));  // ??? offsetof
        auto bucket = chunk->bucket;

        if (bucket == filling) filling = nullptr;
        --bucket->size;
        if (bucket->size == 0) free(bucket);
    }

    template <typename U, typename... Args>
    void construct(U* p, Args&&... args) const {
        new (p) U(std::forward<Args>(args)...);
    }

    void destroy(T* p) const {
        p->~T();
    }

    template <typename U>
    struct rebind {
        using other = MyAlloc<U, capacity>;
    };
};
