#include "fsh.hpp"

#include <cstdio>
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

dictionary::dictionary(std::size_t buffer)
    : head((entry *)std::malloc(buffer)), tail(head), buffer_size(buffer) {
  tail->previous = nullptr;
  tail->length = 0;
}
dictionary::~dictionary() {
    if(head != nullptr) {
        free(head);
    }
}

code_block add(token &token) {
  unsigned int length = token.get_length();
  tail->length = length;
  memcpy(tail->string, token.get_ptr(), length);
  return {&tail->string[length],
          buffer_size -
              ((std::size_t)(&(tail->string[length])) - (std::size_t)head)};
}

const entry *dictionary::follow(token &tok) const {
  const entry *tracking = tail->previous;
  while (tracking != nullptr && tracking->length == tok.get_length() &&
         std::strncmp(tok.get_ptr(), tracking->string, tok.get_length())) {
    tracking = tracking->previous;
  }
  return tracking;
}
