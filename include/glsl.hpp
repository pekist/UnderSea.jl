#pragma once

#include <glm/glm.hpp>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace glsl {
template <typename T> class shader_node;
template <typename T> class variable;
template <typename T> class op_assign;
template <typename A, typename B> class op_next;

struct vec4 {
  typedef glm::vec4 glm_type;
  typedef float type[4];
};


template <typename T> class shader_node {
public:
  shader_node() {}
  virtual ~shader_node() {}
  virtual void compile(std::ostream &out) const = 0;

protected:
};

template <typename T> class variable : public shader_node<T> {
public:
  variable(const std::string &name) : _name(name) {}
  virtual ~variable() {}
  op_assign<T> assign(const shader_node<T> &node) const {
    return op_assign<T>(*this, node);
  }

  bool operator==(variable<T> v) { return v._name == _name; }

  virtual void compile(std::ostream &out) const {
      out << _name;
  }

private:
  std::string _name;
};

template <typename T> class op_assign : public shader_node<void> {
public:
  op_assign(const variable<T> &dst, const shader_node<T> &src)
      : _dst(dst), _src(src) {}
  virtual void compile(std::ostream &out) const {
    _dst.compile(out);
    out << " = ";
    _src.compile(out);
    out << ";" << std::endl;
  }

private:
  const variable<T> &_dst;
  const shader_node<T> &_src;
};

template <typename A, typename B> class op_next : public shader_node<B> {
public:
  op_next(const shader_node<A> &, const shader_node<B> &);
};

class program {
public:
    program(const shader_node<void> &node);
    void compile(std::ostream &out) const;
private:
    const shader_node<void> &_node;
};


class shader {
public:
  shader();
  template <typename A> variable<A> in(std::string &&x) {
      _inputs.emplace_back(std::move(x));
    return variable<A>(_inputs.back()._name);
  }

  program main(const shader_node<void> &m) const;

private:
  struct variable_info {
    std::string _name;
    variable_info(std::string &&name);
  };

  std::vector<variable_info> _inputs;
};

variable<vec4> gl_position("gl_Position");
}; // namespace glsl
