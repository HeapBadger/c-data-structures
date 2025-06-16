# 🧠C Data Structures

![Build Status](https://github.com/HeapBadger/c-data-structures/actions/workflows/ci.yml/badge.svg)

This repository provides a collection of fundamental **data structure implementations in C**, designed for easy integration and reuse in various projects. It includes commonly used structures such as linked lists, stacks, queues, trees, heaps, and hash tables.


## 🧭 Table of Contents

- [Usage](#-usage)
- [Using as a Static Library](#-using-as-a-static-library)
- [Repository Structure](#-repository-structure)
- [Known Issues](#-known-issues)

## 🚀 Usage

### 📝 Requirements

- **Hardware**: Any machine capable of running C.
- **Operating System**: Recommended Ubuntu 22.04 or similar Linux distro.
- **Software**: 
  - `git` to clone the repository
  - Build tools installed from the `setup.sh`

### ⚙️ Setup

1. Clone the repository:

   ```sh
   git clone https://github.com/HeapBadger/c-data-structures.git
   cd c-data-structures/
   ```
1. Run the setup script to install dependencies:

   ```sh
   chmod +x setup.sh
   ./setup.sh
   ```

### 🛠️ Building and Cleaning

- To compile all data structure implementations:
```sh
make all
```
- To clean up build files:
```sh
make clean
```

### 🧪 Testing

All data structures include tests in the tests/ directory covering edge cases and correctness.

Run tests with:

```sh
./bin/test_main help
./bin/test_main list
./bin/test_main <specific suite name>
```

- `help`: Displays a usage guide.
- `list`: Lists all available test suite names.
- `<specific suite name>`: Runs a specific test suite.

Additional Makefile commands:
- Format all code:
```sh
make format
```
- Run all tests under Valgrind to check memory usage:
```sh
make valgrind
```

## 🧩 Using as a Static Library

You can build and use the data structures as a **static library** (`libcds.a`) in your own C projects.

### 🔧 Step 1: Build the Library

```
make lib
```

This generates:

```
lib/libcds.a     # Static library archive
include/*.h      # Public headers
```

### 📦 Step 2: Use in Your Project

1. **Include headers** in your source files:

```
#include "linked_list.h"
#include "stack.h"
// etc.
```

2. **Link against the library** when compiling:

```
gcc your_app.c -I./include -L./lib -lcds -o your_app
```

### 🛠 Example Directory Structure

```
your-project/
├── main.c
├── Makefile
└── c-data-structures/    # Submodule or copy of this repo
    ├── include/
    ├── lib/
    └── libcds.a
```

## 📁 Repository Structure

```
c-data-structures/
│
├── include/
│   ├── ...
│
├── src/
│   ├── ✅ linked_list.c // maybe return and make data generic
│   ├── ✅ doubly_linked_list.c
│   ├── ✅ array.c
│   ├── ✅ matrix.c
│   ├── stack.c
│   ├── queue.c
│   ├── binary_tree.c
│   ├── binary_heap.c
│   ├── hash_table.c
│   ├── deque.c          
│   ├── trie.c        
│   ├── disjoint_set.c    
│   ├── skip_list.c           
│   ├── bloom_filter.c       
│
├── tests/
│   ├── ...
│
└── Makefile
└── README.md
└── setup.sh
```

## 🪲 Known Issues

No known issues.
