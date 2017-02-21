# Module Loader

## What is it?

Module Loader is a type- and inheritance-based dependency resolution framework for C++14 with provided shared library sourcing.

Let's break that down:

### Type- and inheritance-based dependency resolution

When you write a class you probably have some dependencies: Other objects of other types.  In the interest of testability, reusability, customizability, and all that other good stuff they taught you about in school you don't depend on an object of some concrete type, rather you depend on an implementation of some abstract concept: You depend on an abstract base class (what other less cultured languages call "interfaces").

When you (or someone else) wishes to consume your class you choose the actual implementation of this dependency at that time.  But this gets tedious and if you want user-facing customizability requires some kind of scheme for mapping the user's input into the selection of a type.

"Type- and inheritance-based dependency resolution" means that Module Loader resolves dependencies (your need of some concrete realization of an abstract base class) based on the type you need and the types all candidate objects inherit from.

### Shared library sourcing

Rather than requiring you to painstakingly choose a bunch objects and put them up for Module Loader to resolve Module Loader comes out of the box with functionality to obtain potential objects from shared libraries (DLLs, SOs, et cetera).  You give Module Loader a bunch of directories and it scans them, loads all the shared libraries it finds into memory, and invokes a function therein.  This function invocation allows those shared libraries to register objects they provide and types they require with Module Loader.  Module Loader then does the hard work of figuring out how to create all those  objects based on the resulting dependency graph.

### Framework

Module Loader is a framework.  At the core it's the concept that you depend on types, and other people provide types, and somewhere in the middle there should be something more flexible and powerful than creating a bunch of objects and calling a bunch of constructors in `main` to get all that figured out.

Module Loader is Tinder for constructors.  Except you don't swipe left, right, or up, you just call `module_loader::dag_resolver::resolve`.  No awkward attempts to strike up a conversation, just graph theory.

But if you want to replace some or all of how Module Loader works you can go for it.  Module Loader would be kind of insincere as a love song for dependency injection if you couldn't inject dependencies into it to change how it worked.

## Requirements

### Windows/Linux

Other operating systems might work, I haven't tried!

### GCC

Tested on:

```
g++.exe (x86_64-posix-seh-rev1, Built by MinGW-W64 project) 6.3.0
Copyright (C) 2016 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

on Windows and on:

```
g++-6 (Ubuntu 6.2.0-3ubuntu11~14.04) 6.2.0 20160901
Copyright (C) 2016 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

on Linux.

Doesn't support other compilers because they don't support [`std::tr2::bases`](https://gcc.gnu.org/onlinedocs/gcc-4.7.2/libstdc++/api/a00907.html).  Once we get static reflection I'll use that but until then everyone who doesn't use GCC is out of luck.

### Boost >=1.61.0

Depends on [Boost.DLL](http://www.boost.org/doc/libs/1_61_0/doc/html/boost_dll.html), [Boost.Filesystem](http://www.boost.org/doc/libs/1_61_0/libs/filesystem/doc/index.htm), and [Boost.Iterator](http://www.boost.org/doc/libs/1_61_0/libs/iterator/doc/index.html).

Do not ask if I will make a version that doesn't depend on Boost.  It's #CurrentYear for goodness' sake just install Boost.

## Building

Assuming that CMake can find your install of Boost and Doxygen, and that your system's default compiler is a high enough version of GCC:

```
git clone https://github.com/RobertLeahy/Module-Loader.git
cd Module-Loader
mkdir build
cd build
cmake ..
cmake --build .
ctest
cmake --build . --target doc
```

## Example

See the `example` directory.
