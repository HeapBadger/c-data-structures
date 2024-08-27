# Data Structures

## Overview

The data-structures repository provides a collection of essential data structure implementations, including linked lists, stacks, queues, and more. These implementations are organized for easy integration and use in various projects.

## Table of Contents

- [Overview](#overview)
- [Usage](#usage)
  - [Requirements](#requirements)
  - [Setup](#setup)
  - [Operator Guide](#operator-guide)
  - [Testing](#testing)
- [Known Issues/Bugs](#known-issuesbugs)
- [Repository Structure](#repository-structure)

## Usage

### Requirements

- **Hardware**: Any machine capable of running C.
- **Operating System**: Recommended Ubuntu 22.04.
- **Software**: 
  - `git` to clone the repo
  - Dependencies installed via `setup.sh`

### Setup

1. Install `git`:

   ```sh
   sudo apt install git
   ```
1. Clone the repository:

   ```sh
   git clone https://github.com/yourusername/data-structures.git
   cd data-structures/
   ```
1. Install necessary dependencies:

   ```sh
   ./setup.sh
   ```

### Operator Guide

To compile the C files and generate the object files, use:

```sh
make all
```

To remove the generated object files and clean up the build directory, use:

```sh
make clean
```

To use the data structures, write a program that links against the generated object files and utilizes the internal functions.

### Testing

Each data structure includes tests located in the tests/ directory. The tests cover edge cases, performance benchmarks, and comparisons with built-in library functions.

To compile the tests, use:

```sh
make test
```

To remove the generated object files and clean up the build directory, use:

```sh
make clean
```

The testing suite supports various command-line arguments:

```sh
./bin/test_main help
./bin/test_main list
./bin/test_main <specific suite name>
```

- `help`: Displays a usage guide.
- `list`: Lists all available test suite names.
- `<specific suite name>`: Runs a specific test suite.

The Makefile includes additional commands:

```sh
make format
make valgrind
```

- `format`: Formats all C source and header files using clang-format-15 to match Barr C standards.
- `valgrind`: Runs all tests with Valgrind to check for memory leaks.

## Known Issues/Bugs

No known issues.

## Repository Structure

```
data-structures/
│
├── include/
│   ├── linked_list.h
│   ├── doubly_linked_list.h
│   ├── circular_linked_list.h
│   ├── matrix.h
│   ├── stack.h
│   ├── queue.h
│   ├── binary_tree.h
│   ├── binary_heap.h
│   ├── hash_table.h
├── src/
│   ├── linked_list.c
│   ├── doubly_linked_list.c
│   ├── circular_linked_list.c
│   ├── matrix.c
│   ├── stack.c
│   ├── queue.c
│   ├── binary_tree.c
│   ├── binary_heap.c
│   └── hash_table.c
│
├── tests/
│
└── README.md
```
