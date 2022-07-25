#pragma once

struct reference_counter {
  unsigned references;
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
      delete _ptr;
      delete _counter;
    }
  }

private:
  T *_ptr;
  reference_counter *_counter;
};
