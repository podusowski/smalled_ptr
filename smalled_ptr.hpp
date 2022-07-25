#pragma once

#include <cstdint>
#include <stdexcept>

template <class T> struct reference_counter {
  unsigned references;
  T *pointer;
};

/// Takes advantage of malloc's alignment to store a value on the last bit.
struct hidden_value_ptr {
  hidden_value_ptr(void *pointer)
      : _pointer(reinterpret_cast<std::uintptr_t>(pointer)) {}

  void *get() const { return reinterpret_cast<void *>(_pointer & ~1ul); }

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
  smalled_ptr(T *pointer) : _ptr(pointer) {}

  smalled_ptr duplicate() {
    if (!_ptr.flag()) // Unique.
    {
      // Set the pointer to be not unique and...
      auto *counter =
          new reference_counter<T>{2, reinterpret_cast<T *>(_ptr.get())};
      _ptr = counter;
      _ptr.flag(true);
      // ...return a copy of it.
      return *this;
    }
    throw std::runtime_error("not implemented");
  }

  ~smalled_ptr() {
    if (!_ptr.flag()) {
      delete reinterpret_cast<T *>(_ptr.get());
    } else {
      auto *counter = reinterpret_cast<reference_counter<T> *>(_ptr.get());
      counter->references--;
      if (counter->references == 0) {
        delete counter->pointer;
      }
    }
  }

private:
  smalled_ptr(const smalled_ptr &) = default;

  hidden_value_ptr _ptr;
};
