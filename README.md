# 🧠C Data Structures

This repository provides a collection of fundamental **data structure implementations in C**, designed for easy integration and reuse in various projects. It includes commonly used structures such as linked lists, stacks, queues, trees, heaps, and hash tables.


## 🧭 Table of Contents

- [Usage](#usage)
  - [Requirements](#requirements)
  - [Setup](#setup)
  - [Building and Cleaning](#building-and-cleaning)
  - [Testing](#testing)
- [Repository Structure](#repository-structure)
- [Known Issues/Bugs](#known-issuesbugs)

## 🚀 Usage

### 📝 Requirements

- **Hardware**: Any machine capable of running C.
- **Operating System**: Recommended Ubuntu 22.04 or similar Linux distro.
- **Software**: 
  - `git` to clone the repository
  - Build tools: `gcc`, `make`
  - `clang-format-15` for formatting (optional, for code style)
  - `valgrind` for memory leak detection (optional)

### ⚙️ Setup

1. Clone the repository:

   ```sh
   git clone https://github.com/yourusername/c-data-structures.git
   cd c-data-structures/
   ```
1. Run the setup script to install dependencies (if applicable):

   ```sh
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
- Run tests under Valgrind to check memory usage:
```sh
make valgrind
```

## 📁 Repository Structure

```
c-data-structures/
│
├── include/
│   ├── linked_list.h
│   ├── doubly_linked_list.h
│   ├── matrix.h            
│   ├── stack.h
│   ├── queue.h
│   ├── binary_tree.h
│   ├── binary_heap.h
│   ├── hash_table.h
│   ├── deque.h               
│   ├── trie.h               
│   ├── disjoint_set.h          
│   ├── skip_list.h           
│   ├── bloom_filter.h 
│
├── src/
│   ├── linked_list.c
│   ├── doubly_linked_list.c
│   ├── matrix.c
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
│   ├── test_linked_list.c
│   ├── test_stack.c
│   ├── test_queue.c
│   ├── test_binary_tree.c
│   ├── test_binary_heap.c
│   ├── test_hash_table.c
│   ├── test_deque.c    
│   ├── test_trie.c
│   ├── test_disjoint_set.c
│   ├── test_skip_list.c
│   ├── test_bloom_filter.c
│
└── README.md
```

## 🪲 Known Issues/Bugs

No known issues.
