#pragma once

/// Reference counted smart pointer, similar to `shared_ptr`, but its size
/// equals the size of the raw pointer.
template <class T> struct smalled_ptr {
  smalled_ptr(T *pointer) : _ptr(pointer) {}
  smalled_ptr(const smalled_ptr &other) : _ptr(other._ptr) {}
  ~smalled_ptr() { delete _ptr; }

private:
  T *_ptr;
};
