#include <catch2/catch_test_macros.hpp>
#include <exception>
#include <iostream>
#include <stdexcept>

#include "smalled_ptr.hpp"

TEST_CASE("size of the pointer is equal to raw pointer") {
  //static_assert(sizeof(smalled_ptr<int>) == sizeof(void *),
  //              "grand feature is broken");
}

struct testable_destructor {
  explicit testable_destructor(bool &destructor_called)
      : _destructor_called(destructor_called) {}

  ~testable_destructor() {
    if (_destructor_called) {
      std::cerr << "destructor called flag is already set";
      std::terminate();
    }
    _destructor_called = true;
  }

private:
  bool &_destructor_called;
};

TEST_CASE("testable destructor works") {
  bool destructor_called = false;
  { testable_destructor d{destructor_called}; }
  REQUIRE(destructor_called);
}

TEST_CASE("object held by the pointer is destroyed") {
  bool destructor_called = false;
  {
    smalled_ptr<testable_destructor> p{
        new testable_destructor{destructor_called}};
  }
  REQUIRE(destructor_called);
}

TEST_CASE("object keeps living until second pointer is destroyed") {
  bool destructor_called = false;
  {
    smalled_ptr<testable_destructor> outer_ptr{
        new testable_destructor{destructor_called}};
    { smalled_ptr<testable_destructor> inner_ptr = outer_ptr; }
    REQUIRE(!destructor_called);
  }
  REQUIRE(destructor_called);
}
