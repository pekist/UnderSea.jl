#pragma once
#include <cctype>
#include <cstdlib>
#include <cstdio>

const std::size_t BUFSIZE = 256;

class token {
  const char *string;
  unsigned int length;

public:
  token(char const *str, unsigned int l);
  std::size_t get_length();
  const char *get_ptr();
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
  bool has_next();
};

class code_block {
  void *buffer;

public:
  code_block(void *b, std::size_t remaining);
};

class dictionary {
  struct entry {
    entry *previous;
    unsigned int length;
    char string[];
  };

  entry *head;
  entry *tail;
  std::size_t buffer_size;

public:
  dictionary(std::size_t buffer);
  ~dictionary();
  const entry *follow(token &tok) const;
  code_block add(token &token);
};
