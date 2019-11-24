
#include "myalloc.h"

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

    for (int i = 0; i < N; ++i) {
        std::cout << i << " -> " << factorial[i] << "\t" << fibonacci[i] << std::endl;
    }

    return 0;
}
