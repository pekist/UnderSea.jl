#include "fish.hpp"
#include "asmjit/x86.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <optional>

#define DICT_SIZE 1024 * 64
using namespace fish;

dictionary fish::global_dictionary(DICT_SIZE);
uint32_t global_stack[16] = {1};

token::token(char const *str, unsigned int l) : string(str), length(l) {}
std::size_t token::get_length() const { return length; }

const char *token::get_ptr() const { return string; }

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

static bool parse_int(const token &t, int &n) {
  auto l = t.get_length();
  const char *s = t.get_ptr();
  for (auto i = 0; i < l; i++) {
    char c = s[i];
    if (c >= '0' && c <= '9') {
      n += c - '0';
    } else {
      return false;
    }
  }
  return true;
}

dictionary::dictionary(std::size_t sz)
    : size(sz), base(malloc(size)), here((dictionary::entry *)base) {
    if (base == nullptr) {
        throw std::runtime_error("CANNOT INITIALIZE DICTIONARY");
    }
    here->last = nullptr;
}

dictionary::~dictionary() {
  if (base != nullptr) {
    free(base);
  }
}

const dictionary::entry *dictionary::lookup(const token &token) const {
  dictionary::entry *current;
  dictionary::entry *last;
  for (last = here, current = here->last; current != nullptr;
       last = current, current = current->last) {
      std::size_t length =  (uintptr_t)last - (uintptr_t)current->name;
    if (length == token.get_length() &&
        strncmp(current->name, token.get_ptr(), length) == 0) {
      return current;
    }
  }
  return nullptr;
}

void dictionary::add(const char *s, uint32_t len, void (*f)()){
    here->function = f;
    memcpy(here->name, s, len);
    entry *e = (entry *)(&here->name[len]);
    e->last = here;
    here = e;
}

extern "C" void enter_fish(void *sp, void (*f)(void));
asm(R"(
.globl enter_fish
.type enter_fish, @function
enter_fish:
   .cfi_startproc
   pushq   %r12
   movq    %rdi, %r12
   call    *%rsi
   popq    %r12
   ret
   .cfi_endproc
)");

void fish::eval(const char *file) {
  uint32_t stack[32];
  auto token_stream = create_token_stream(file);
  if (!token_stream.has_value()) {
    return;
  }

  for (auto token = token_stream->next_token(); token.get_length() > 0;
       token = token_stream->next_token()) {
    int i = 0;
    auto *entry = global_dictionary.lookup(token);
    if(entry == nullptr){
        std::cerr << "Could not find an entry" << std::endl;
        return;
    }
    enter_fish(global_stack, entry->function);
  }
}
