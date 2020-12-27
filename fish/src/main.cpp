#include "fish.hpp"

#include <iostream>


void raw_print() {
    uint32_t t;
    asm volatile (
        "mov 0(%%r12), %0\n"
        "sub $4, %%r12\n"
        : "=r" (t)
    );
    std::cout << t << std::endl;
}

int main(int argc, char **argv) {
    if(argc < 2) {
        std::cout << "Need to provide file" << std::endl;
        return 1;
    }

    fish::global_dictionary.add("print", 5, raw_print);
    fish::eval(argv[1]);
    return 0;
}
