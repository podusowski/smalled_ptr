#pragma once

#include <cstdint>

struct reference_counter {
  unsigned references;
};

/// Takes advantage of malloc's alignment to store a value on the last bit.
template <class T> struct hidden_value_ptr {
  explicit hidden_value_ptr(T *pointer)
      : _pointer(reinterpret_cast<std::uintptr_t>(pointer)) {}

  T *get() const { return reinterpret_cast<T *>(_pointer & ~1ul); }

  bool flag() const { return (_pointer & 1ul) == 1ul; }

  void flag(bool value) {
    if (value)
      _pointer |= 1ul;
    else
      _pointer &= ~1ul;
  }

private:
  std::uintptr_t _pointer;
};

/// Reference counted smart pointer, similar to `shared_ptr`, but its size
/// equals the size of the raw pointer.
template <class T> struct smalled_ptr {
  smalled_ptr(T *pointer) : _ptr(pointer), _counter(new reference_counter{1}) {}

  smalled_ptr(const smalled_ptr &other)
      : _ptr(other._ptr), _counter(other._counter) {
    _counter->references++;
  }

  ~smalled_ptr() {
    _counter->references--;
    if (!_counter->references) {
      delete _ptr.get();
      delete _counter;
    }
  }

private:
  hidden_value_ptr<T> _ptr;
  reference_counter *_counter;
};
