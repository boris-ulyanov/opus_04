
#include "myalloc.h"

#include <map>

struct hard {
    int fa;
    int fi;
    hard(int fa, int fi) : fa(fa), fi(fi){};
};

std::ostream& operator<<(std::ostream& os, const hard& h) {
    os << "hard(" << h.fa << ":" << h.fi << ")";
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
        map_default.emplace(std::make_pair(i, hard{factorial[i], fibonacci[i]}));

    // 2 - map + myalloc
    std::map<int, hard, std::less<int>, MyAlloc<std::pair<const int, hard>, capacity_step>> map_myalloc;
    for (int i = 0; i < N; ++i)
        map_myalloc.emplace(std::make_pair(i, hard{factorial[i], fibonacci[i]}));

    for (const auto& p : map_myalloc) std::cout << p.first << " " << p.second << std::endl;

    // 3 - my container + default allocator

    // 3 - my container + myalloc
    // dump

    return 0;
}
