# uniptr_t - C++ Utility Class
`uniptr_t` stands for univerasl pointer. It's a small C++ class wrapping any pointer type. It removes the need to cast any pointer to any other pointer type, or to cast a numerical to a pointer type and vice versa.

The purpose of this library is to make working with pointers a lot a lot easier, but you still must be understand how memory and pointers work. Be careful: `uniptr_t` is not always the best type to use when working with pointers or memory addresses.

## Features
- Automatic pointer casting
- Utility functions
- All operators are overloaded
- Platform independent
- Works on C++14 and newer versions (I haven't tested it on C++11 or older versions)
- 1 file include
- Doesn't affect performance of your code at all

## Showcase
You can assign all `uniptr_t` objects to any pointer type or to a numerical.
```cpp
// assign to a void pointer
void* a_void_pointer = nullptr;
uniptr_t p1 = a_void_pointer;

// assign to any other pointer
int* any_pointer_type = nullptr;
uniptr_t p2 = any_pointer_type;

// assign to nullptr
uniptr_t p3 = nullptr;

// assign to 0 without any casting
uniptr_t p4 = 0;

// assign to any other numerical without casting
uniptr_t p5 = 0x18401;
```

Of course you can also do this vice versa - automatically cast `uniptr_t` to any pointer or numerical:
```cpp
uniptr_t p1 = 0xDEADBEEF;

// assign to a void pointer
void* a_void_pointer = p1;

// assign to any other pointer
int* any_pointer_type = p1;

// assign to any numerical
int numerical1 = p1;
uintptr_t numerical2 = p1;
// and even to floats or doubles!
float floating1 = p1;
double floating2 = p1;
```

All operators are overloaded and work with any pointer type or numerical:
```cpp
int numerical1 = 10;
uintptr_t numerical2 = 20;
float floating1 = 10.f;
double floating2 = 49.0;
void* void_ptr = (void*)0x100;
int* any_ptr = (int*)0x81;

uniptr_t my_ptr = 0;
my_ptr += numerical1;
my_ptr = my_ptr - floating1;
my_ptr /= any_ptr;
my_ptr++;
--my_ptr;
// etc.
```

`uniptr_t` has some utility functions:
```cpp
uniptr_t my_ptr = nullptr;

my_ptr.set<int>(10); // same as *(int*)my_ptr = 10;

int my_num = my_ptr.get<int>(); // same as int my_num = *(int*)my_ptr;

// if you want to make sure the pointer is casted properly, you can use .as() or .as_ptr()
uintptr_t numerical = my_ptr.as<uintptr_t>();
int* void_ptr = my_ptr.as_ptr<int>(); // here you only need to specify the type of the pointer without *

// get the size of void* (same as sizeof(void*))
size_t pointer_size = my_ptr.size(); // you can even do uniptr_t::size() since it's a static function

// .relative_addr() and .to_relative_addr() are hard to explain but
// as the name suggests, I use them to caluclate relative addresses
// that an assembly instruction might be holding
// if you don't understand, don't worry lol

// and finally we have some windows specific functions
// .is_valid() checks if the memory address uniptr_t is pointing to is valid (checks if it's commited memory)
// .is_readable() checks if the memory address uniptr_t is pointing to is readable
// .is_writeable() checks if the memory address uniptr_t is pointing to is writeable
// .region_size() if the function succeeds, the return value is non-zero and holds the number of bytes in memory of the
// region uniptr_t is pointing to (in other words how big the region is)
```
