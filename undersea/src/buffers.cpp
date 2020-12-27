#include "buffers.hpp"

buffers::buffers(int n) : _buffers(n) { glGenBuffers(n, _buffers.data()); }
buffers::buffers(buffers &&b) : _buffers(std::move(b._buffers)) {}
buffers::~buffers() { glDeleteBuffers(_buffers.size(), _buffers.data()); }
GLuint buffers::operator[](int n) { return _buffers[n]; }
