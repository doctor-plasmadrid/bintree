# Bintree - Template Binary Search Tree

A comprehensive template-based Binary Search Tree (BST) implementation in C++ with advanced features including multiple traversal orders, STL-compatible iterators, and tree manipulation operations.

## Features

- **BST Operations**: Insert, search, and maintain sorted order with custom comparators
- **Multiple Traversals**: In-order, pre-order, post-order, and level-order (BFS)
- **STL-Compatible Iterators**: Full iterator support for range-based for loops and STL algorithms
- **Tree Manipulation**: Prune, emplace, assign, and replace subtrees
- **Auto-Sorting**: Rebuilds the tree as a balanced BST when unsorted operations occur
- **Duplicate Handling**: Duplicate elements (per the comparator) are silently omitted on insert and during auto-sort
- **Flexible Access**: Get elements by sorted index, raw array index, or tree position (row/column)
- **Memory Safe**: Bounds checking with assertions and exceptions
- **Custom Comparators**: Support for ascending, descending, or custom ordering via lambdas

## Requirements

- C++11 or higher
- A C++ compiler (g++, clang++, or MSVC)
- CMake 3.10+ (optional, for building)
- Doxygen (optional, for documentation)

## Building

### Using g++ directly

```bash
# Compile the test file
g++ -std=c++11 -Wall -Wextra -o tests tests.cpp

# Run the tests
./tests
```

### Using CMake

```bash
# Create build directory
mkdir build
cd build

# Configure
cmake ..

# Build
cmake --build .

# Run tests
./tests
```

### Using Make

```bash
# Compile
make

# Run tests
./tests
```

## Usage Example

```cpp
#include "bintree.h"
#include <iostream>

int main() {
    // Create a tree with default (ascending) order
    Bintree<int> tree;

    // Insert elements
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    tree.insert(1);
    tree.insert(9);

    // Access elements
    std::cout << "Root: " << tree.root() << std::endl;
    std::cout << "Size: " << tree.size() << std::endl;
    std::cout << "Height: " << tree.height() << std::endl;

    // Get elements by sorted index
    std::cout << "First element: " << tree.get(0) << std::endl;
    std::cout << "Last element: " << tree.get(tree.size() - 1) << std::endl;

    // Safe access with bounds checking
    try {
        std::cout << "Element at index 2: " << tree.at(2) << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "Index out of range" << std::endl;
    }

    // Use iterators
    std::cout << "In-order traversal: ";
    for (auto it = tree.ibegin(); it != tree.iend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // Create tree with custom comparator (descending order)
    Bintree<int> desc_tree(std::greater<int>());
    desc_tree.insert(5);
    desc_tree.insert(3);
    desc_tree.insert(7);

    return 0;
}
```

## API Reference

### Core Operations

- `insert(T element)` - Insert element maintaining BST order
- `get(size_t index)` - Get element by sorted index
- `at(size_t index)` - Safe access with bounds checking
- `operator[](size_t index)` - Access by raw array index
- `root()` - Get root element
- `empty()` - Check if tree is empty
- `size()` - Get number of elements
- `height()` - Get tree height

### Tree Manipulation

- `emplace_left(T element)` - Add element to left without order check
- `emplace_right(T element)` - Add element to right without order check
- `prune_left()` - Remove and return left subtree
- `prune_right()` - Remove and return right subtree
- `assign_subtree(Bintree& subtree)` - Assign subtree to first empty leaf
- `replace_subtree(Bintree& subtree)` - Replace subtree with given tree
- `clear()` - Clear all elements

### Access by Position

- `left(size_t row, size_t col)` - Get element by tree position (left child)
- `right(size_t row, size_t col)` - Get element by tree position (right child)

### Iterators

- `begin()/end()` - In-order iterators (STL compatible)
- `rbegin()/rend()` - Reverse in-order iterators
- `ibegin()/iend()` - Explicit in-order iterators
- `pbegin()/pend()` - Pre-order iterators
- `pobegin()/poend()` - Post-order iterators
- `lbegin()/lend()` - Level-order (BFS) iterators

### Operators

- `operator=(const Bintree&)` - Deep copy assignment
- `operator==(const Bintree&)` - Equality comparison
- `operator!=(const Bintree&)` - Inequality comparison
- `operator*()` - Dereference (returns first element)

## Documentation

Generate Doxygen documentation:

```bash
doxygen Doxyfile
```

The generated HTML documentation will be in the `html/` directory.

## Testing

Run the comprehensive test suite:

```bash
./tests
```

The test suite covers:
- Constructors and destructors
- Insertion and sorting
- Copy and assignment operations
- Equality comparisons
- All traversal methods
- Iterator functionality
- Tree manipulation operations
- Custom comparators
- Edge cases and error handling

## License

MIT License - see [LICENSE](LICENSE) file for details.

## Author

Dr. Plasmadrid

## Contributing

Contributions are welcome! Please ensure all tests pass before submitting a pull request.
# bintree
