
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
    int factorial[N];
    int fibonacci[N];

    // ? не придумал как сгенерить в compile time
    static_assert(N <= 12, "int overflow");
    for (int i = 0; i < N; ++i) {
        factorial[i] = (i < 2) ? 1 : factorial[i - 1] * i;
        fibonacci[i] = (i < 2) ? i : fibonacci[i - 1] + fibonacci[i - 2];
    }

    // 1 - map + default allocator
    std::map<int, hard> map_default;
    for (int i = 0; i < N; ++i)
        map_default.emplace(std::make_pair(i, hard{factorial[i], fibonacci[i]}));

    // 2 - map + my allocator
    // dump

    // 3 - my container + default allocator

    // 3 - my container + my allocator
    // dump

    return 0;
}
