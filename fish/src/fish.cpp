#include "fish.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iterator>
#include <llvm-10/llvm/ADT/APInt.h>
#include <llvm-10/llvm/IR/BasicBlock.h>
#include <llvm-10/llvm/IR/DerivedTypes.h>
#include <llvm-10/llvm/IR/Type.h>
#include <llvm-10/llvm/IR/Verifier.h>
#include <memory>
#include <optional>

#define DICT_SIZE 1024 * 64
using namespace fish;

static uint32_t global_stack[16] = {};

std::optional<token_stream> fish::create_token_stream(const char *file) {
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

std::string token_stream::next_token() {
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

llvm_context::llvm_context()
    : builder(context), module("fish", context) {

}
compiler::compiler() {}
compiler::~compiler() {}

word_entry &compiler::intern(const std::string &s){
    auto e = dictionary.find(s);
    if(e == dictionary.end()){
        dictionary[s] = {};
        return dictionary[s];
    } else {
        return e->second;
    }
}

bool number(std::string &s, int &n) {
  n = 0;
  int i = 0;
  for (char c : s) {
    if (c < '0' || c > '9') {
      return false;
    }
    n += (c - '0') * (i == 0 ? 1 : i * 10);
    i++;
  }
  return true;
}

trace::trace() {}
trace::~trace() {}
void trace::push_back(llvm::Value *v) { trail.push_back(v); }
std::size_t trace::trail_length() { return trail.size(); }
std::vector<llvm::Value *> trace::pull_args(llvm::Type *types) { return {}; }
std::vector<llvm::Value *> trace::popn(std::size_t n) {
  std::vector<llvm::Value *> values(n);
  for (int i = 0; i < n; i++) {
    values[i] = trail.back();
    trail.pop_back();
  }
  return values;
}

llvm::Value *trace::finalize(llvm_context &c){
    llvm::FunctionType *ft = llvm::FunctionType::get(llvm::Type::getVoidTy(c.context), false);
    llvm::Function *func = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, "main", c.module);
    llvm::BasicBlock *bb = llvm::BasicBlock::Create(c.context, "entry", func);
    c.builder.SetInsertPoint(bb);
    c.builder.CreateRet(trail.back());
    llvm::verifyFunction(*func);
    return func;
}

std::vector<std::unique_ptr<token>> compiler::tokenize(token_stream &stream) {
  std::vector<std::unique_ptr<token>> tokens;
  for (auto token = stream.next_token(); token.length() != 0;
       token = stream.next_token()) {
    int n;
    if (number(token, n)) {
        tokens.push_back(std::make_unique<literal_token>(n));
    } else {
      auto entryp = dictionary.find(token);
      if (entryp != dictionary.end()) {
        const auto &entry = entryp->second;
        tokens.push_back(std::make_unique<symbol_token>(entry));
      } else {
        tokens.push_back(std::make_unique<temporary_token>(token));
      }
    }
  }
  return tokens;
}

llvm::Value *compiler::compile(std::vector<std::unique_ptr<token>> &tokens) {
  std::vector<llvm::Value *> compiles;
  trace tr;
  for (auto &token : tokens) {
    token->compile(ctx, tr);
  }

  return tr.finalize(ctx);
}

literal_token::literal_token(int32_t n) : val(n) {}
literal_token::~literal_token() {}

void literal_token::compile(llvm_context &ctx, trace &trace) {
  trace.push_back(
      llvm::ConstantInt::get(ctx.context, llvm::APInt(32, val, true)));
}
void literal_token::print() {
    std::cout << val << std::endl;
}

symbol_token::symbol_token(const word_entry &x) : entry(x) {}
symbol_token::~symbol_token() {}
void symbol_token::compile_primitive(llvm_context &ctx, trace &trace) {
  auto &name = entry.name;
  if (name == "+") {
    if (trace.trail_length() < 2) {
      throw std::runtime_error("Whoopsies");
    }

    auto args = trace.popn(2);
    trace.push_back(ctx.builder.CreateAdd(args[1], args[0]));
  } else {
    throw std::runtime_error("WHOOOPSIES");
  }
}
void symbol_token::compile(llvm_context &ctx, trace &trace) {
  if(entry.flags & word_entry::PRIMITIVE) {
      compile_primitive(ctx, trace);
  } else {
    llvm::Function *callee = ctx.module.getFunction(entry.name);
    auto arity = callee->arg_size();
    if (arity > trace.trail_length()) {
      // error for now
      throw std::runtime_error("hello");
    }
    auto args = trace.popn(arity);
    trace.push_back(ctx.builder.CreateCall(callee, args));
  }
}
void symbol_token::print() { std::cout << entry.name << std::endl; }

temporary_token::temporary_token(const std::string &s) : name(s) {}
temporary_token::~temporary_token() {}
void temporary_token::compile(llvm_context &ctx, trace &trace) {
  throw std::runtime_error("lol");
}
void temporary_token::print() { std::cout << name << std::endl; }
