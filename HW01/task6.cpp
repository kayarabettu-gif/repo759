#include <cstdio>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    int N = std::stoi(argv[1]);

    for (int i = 0; i <= N; ++i) {
        if (i > 0) {
            std::printf(" ");
        }
        std::printf("%d", i);
    }
    std::printf("\n");

    for (int i = N; i >= 0; --i) {
        if (i < N) {
            std::cout << " ";
        }
        std::cout << i;
    }
    std::cout << "\n";

    return 0;
}
