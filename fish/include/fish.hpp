#pragma once
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>

namespace fish {
const std::size_t BUFSIZE = 256;

class token {
  const char *string;
  unsigned int length;

public:
  token(char const *str, unsigned int l);
  std::size_t get_length() const;
  const char *get_ptr() const;
};

class token_stream {
  char current_token[BUFSIZE];
  bool eof;
  std::FILE *file;

  bool eat_space();

  token_stream &operator=(const token_stream &) = delete;
  token_stream(const token_stream &) = delete;
  token_stream(token_stream &&) = delete;

public:
  token_stream(std::FILE *file);
  ~token_stream();

  token next_token();
};

class dictionary {
  struct entry {
    entry *last;
    void (*function)(void);
    char name[];
  };

  std::size_t size;
  void *base;
  entry *here;
public:
  dictionary(std::size_t allocation);
  ~dictionary();
  const entry *lookup(const token &c) const;
  void add(const char *,  uint32_t n, void (*)(void));
};

extern dictionary global_dictionary;

void eval(const char *file);
}
