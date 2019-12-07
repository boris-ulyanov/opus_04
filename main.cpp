
#include "myalloc.h"
#include "mylist.h"

#include <map>

struct hard {
    int fa;
    int fi;

    hard(int fa, int fi) : fa(fa), fi(fi) {}

    hard(const hard& rgt) = delete;
    hard(hard&& rgt) = delete;
};

std::ostream& operator<<(std::ostream& os, const hard& h) {
    os << h.fa << " " << h.fi;
    return os;
}

int main() {
    const int N = 10;
    const size_t capacity_step = 10;

    // ? не придумал как сгенерить в compile time
    int factorial[N];
    int fibonacci[N];
    static_assert(N <= 12, "int overflow");
    for (int i = 0; i < N; ++i) {
        factorial[i] = (i < 2) ? 1 : factorial[i - 1] * i;
        fibonacci[i] = (i < 2) ? i : fibonacci[i - 1] + fibonacci[i - 2];
    }

    // 1 - map + default allocator
    std::map<int, hard> map_default;
    for (int i = 0; i < N; ++i)
        map_default.emplace(std::piecewise_construct,
                            std::forward_as_tuple(i),
                            std::forward_as_tuple(factorial[i], fibonacci[i]));

    // 2 - map + myalloc + dump
    std::map<int, hard, std::less<>, MyAlloc<std::pair<const int, hard>, capacity_step>>
        map_myalloc;
    for (int i = 0; i < N; ++i)
        map_myalloc.emplace(std::piecewise_construct,
                            std::forward_as_tuple(i),
                            std::forward_as_tuple(factorial[i], fibonacci[i]));

    for (const auto& p : map_myalloc)
        std::cout << p.first << " " << p.second << std::endl;

    // 3 - my container + default allocator
    MyList<hard> mylist_default;
    for (int i = 0; i < N; ++i)
        mylist_default.emplace_back(factorial[i], fibonacci[i]);

    // 4 - my container + myalloc + dump
    MyList<hard, MyAlloc<hard, capacity_step>> mylist_myalloc;
    for (int i = 0; i < N; ++i)
        mylist_myalloc.emplace_back(factorial[i], fibonacci[i]);

    for (const auto& p : mylist_myalloc)
        std::cout << p << std::endl;

    return 0;
}
