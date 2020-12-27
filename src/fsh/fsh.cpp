#include "fsh.hpp"
#include "asmjit/x86.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <optional>

token::token(char const *str, unsigned int l) : string(str), length(l) {}
std::size_t token::get_length() { return length; }

const char *token::get_ptr() { return string; }

static std::optional<token_stream> create_token_stream(const char *file) {
  auto fp = std::fopen(file, "r");
  if (fp == nullptr) {
    return {};
  } else {
    return {fp};
  }
}

token_stream::token_stream(std::FILE *fp) : eof(false), file(fp) {}

token_stream::~token_stream() {
  if (file != nullptr) {
    std::fclose(file);
    file = nullptr;
  }
}

token token_stream::next_token() {
  unsigned int length = 0;
  int c;
  for (c = fgetc(file); c != EOF && isspace(c); c = fgetc(file))
    ;
  for (; c != EOF && !isspace(c); c = fgetc(file), length++) {
    current_token[length] = c;
    if (length >= BUFSIZE) {
      throw std::runtime_error("Token too big!");
    }
  }
  return {current_token, length};
}
