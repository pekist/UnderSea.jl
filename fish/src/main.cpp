#include "fish.hpp"

#include <iostream>
#include <llvm-10/llvm/Support/raw_ostream.h>

int main(int argc, char **argv) {
    fish::compiler compiler;
    auto &add = compiler.intern("+");
    add.name = "+";
    add.flags = fish::word_entry::PRIMITIVE;

    auto stream = fish::create_token_stream("../test.fish");
    auto tokens = compiler.tokenize(*stream);
    auto *value = compiler.compile(tokens);
    value->print(llvm::errs());
}
