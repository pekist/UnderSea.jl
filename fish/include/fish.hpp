#pragma once
#include "llvm/ADT/APInt.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <unordered_map>
#include <vector>

namespace fish {
const std::size_t BUFSIZE = 256;

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

  std::string next_token();
};

struct llvm_context {
  llvm::LLVMContext context;
  llvm::IRBuilder<> builder;
  llvm::Module module;

  llvm_context();
};


struct word_entry {
  enum {
    PRIMITIVE = 1 << 0,
    IMMEDIATE = 1 << 1,
  };

  uint8_t flags;
  std::vector<llvm::Type *> inputs;
  std::vector<llvm::Type *> outputs;
  std::string name;
};

class trace {
    std::vector<llvm::Value *> trail;
    std::vector<llvm::Type *> arguments;
public:
    trace();
    ~trace();

    void push_back(llvm::Value *);
    std::size_t trail_length();
    std::vector<llvm::Value *> pull_args(llvm::Type *types);
    std::vector<llvm::Value *> popn(std::size_t n);
    llvm::Value *finalize(llvm_context &c);
};

class token {
public:
  virtual void compile(llvm_context &, trace &trace) = 0;
  virtual void print() = 0;
};

class literal_token : public token {
  int32_t val;

public:
  literal_token(int32_t);
  virtual ~literal_token();
  virtual void compile(llvm_context &, trace &trace);
  virtual void print();
};

class symbol_token : public token {
  const word_entry &entry;
  void compile_primitive(llvm_context &, trace &);
public:
  symbol_token(const word_entry &);
  virtual ~symbol_token();
  virtual void compile(llvm_context &, trace &trace);
  virtual void print();
};

class temporary_token : public token {
  std::string name;

public:
  temporary_token(const std::string &);
  virtual ~temporary_token();
  virtual void compile(llvm_context &, trace &trace);
  virtual void print();
};

class compiler {
  llvm_context ctx;
  std::unordered_map<std::string, word_entry> dictionary;

public:
  compiler();
  ~compiler();
  std::vector<std::unique_ptr<token>> tokenize(token_stream &);
  llvm::Value *compile(std::vector<std::unique_ptr<token>> &tokens);
  word_entry &intern(const std::string &);

  compiler(const compiler &) = delete;
  compiler &operator=(const compiler &) = delete;
  compiler(compiler &&) = delete;
};

typedef void (*fish_fn)();
std::optional<token_stream> create_token_stream(const char *file);

} // namespace fish
