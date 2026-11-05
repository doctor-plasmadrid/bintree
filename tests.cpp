#include <iostream>
#include <cassert>
#include <stdexcept>
#include "bintree.h"

using namespace std;

void test_default_constructor() {
    cout << "Testing default constructor..." << endl;
    Bintree<int> tree;
    assert(tree.empty());
    assert(tree.size() == 0);
    assert(tree.height() == 0);
    cout << "  PASSED" << endl;
}

void test_insert() {
    cout << "Testing insert..." << endl;
    Bintree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    tree.insert(1);
    tree.insert(9);

    assert(!tree.empty());
    assert(tree.size() == 5);
    assert(tree.root() == 5);
    assert(tree.get(0) == 1);  // Smallest
    assert(tree.get(4) == 9);  // Largest
    cout << "  PASSED" << endl;
}

void test_copy_constructor() {
    cout << "Testing copy constructor..." << endl;
    Bintree<int> tree1;
    tree1.insert(5);
    tree1.insert(3);
    tree1.insert(7);

    Bintree<int> tree2(tree1);
    assert(tree2.size() == tree1.size());
    assert(tree2.root() == tree1.root());
    assert(tree2 == tree1);
    cout << "  PASSED" << endl;
}

void test_assignment_operator() {
    cout << "Testing assignment operator..." << endl;
    Bintree<int> tree1, tree2;
    tree1.insert(5);
    tree1.insert(3);
    tree1.insert(7);

    tree2 = tree1;
    assert(tree2.size() == tree1.size());
    assert(tree2 == tree1);
    cout << "  PASSED" << endl;
}

void test_equality_operators() {
    cout << "Testing equality operators..." << endl;
    Bintree<int> tree1, tree2, tree3;
    tree1.insert(5);
    tree1.insert(3);
    tree1.insert(7);

    tree2.insert(5);
    tree2.insert(3);
    tree2.insert(7);

    tree3.insert(5);
    tree3.insert(3);

    assert(tree1 == tree2);
    assert(tree1 != tree3);
    cout << "  PASSED" << endl;
}

void test_root() {
    cout << "Testing root..." << endl;
    Bintree<int> tree;
    tree.insert(5);
    assert(tree.root() == 5);
    cout << "  PASSED" << endl;
}

void test_empty() {
    cout << "Testing empty..." << endl;
    Bintree<int> tree;
    assert(tree.empty());
    tree.insert(5);
    assert(!tree.empty());
    cout << "  PASSED" << endl;
}

void test_height() {
    cout << "Testing height..." << endl;
    Bintree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    tree.insert(1);
    assert(tree.height() == 3);
    cout << "  PASSED" << endl;
}

void test_size() {
    cout << "Testing size..." << endl;
    Bintree<int> tree;
    assert(tree.size() == 0);
    tree.insert(5);
    assert(tree.size() == 1);
    tree.insert(3);
    tree.insert(7);
    assert(tree.size() == 3);
    cout << "  PASSED" << endl;
}

void test_get() {
    cout << "Testing get..." << endl;
    Bintree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    tree.insert(1);
    tree.insert(9);

    assert(tree.get(0) == 1);
    assert(tree.get(1) == 3);
    assert(tree.get(2) == 5);
    assert(tree.get(3) == 7);
    assert(tree.get(4) == 9);
    cout << "  PASSED" << endl;
}

void test_at() {
    cout << "Testing at..." << endl;
    Bintree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);

    assert(tree.at(0) == 3);
    assert(tree.at(1) == 5);
    assert(tree.at(2) == 7);

    bool exception_thrown = false;
    try {
        tree.at(10);
    } catch (const out_of_range&) {
        exception_thrown = true;
    }
    assert(exception_thrown);
    cout << "  PASSED" << endl;
}

void test_bracket_operator() {
    cout << "Testing bracket operator..." << endl;
    Bintree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);

    // Access by raw index
    assert(tree[0] == 5);  // Parent
    cout << "  PASSED" << endl;
}

void test_emplace_left() {
    cout << "Testing emplace_left..." << endl;
    Bintree<int> tree;
    tree.emplace_left(5);
    tree.emplace_left(3);

    assert(tree.size() == 2);
    assert(tree.root() == 5);
    cout << "  PASSED" << endl;
}

void test_emplace_right() {
    cout << "Testing emplace_right..." << endl;
    Bintree<int> tree;
    tree.emplace_right(5);
    tree.emplace_right(7);

    assert(tree.size() == 2);
    assert(tree.root() == 5);
    cout << "  PASSED" << endl;
}

void test_prune_left() {
    cout << "Testing prune_left..." << endl;
    Bintree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);

    Bintree<int> pruned = tree.prune_left();
    assert(tree.size() == 2);  // parent + right child
    assert(pruned.size() == 1);  // left subtree
    cout << "  PASSED" << endl;
}

void test_prune_right() {
    cout << "Testing prune_right..." << endl;
    Bintree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);

    Bintree<int> pruned = tree.prune_right();
    assert(tree.size() == 2);  // parent + left child
    assert(pruned.size() == 1);  // right subtree
    cout << "  PASSED" << endl;
}

void test_clear() {
    cout << "Testing clear..." << endl;
    Bintree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);

    tree.clear();
    assert(tree.empty());
    assert(tree.size() == 0);
    cout << "  PASSED" << endl;
}

void test_custom_comparator() {
    cout << "Testing custom comparator (greater)..." << endl;
    std::function<bool(int,int)> comp = std::greater<int>();
    Bintree<int> tree(comp);
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);

    assert(tree.get(0) == 7);  // Should be descending
    assert(tree.get(1) == 5);
    assert(tree.get(2) == 3);
    cout << "  PASSED" << endl;
}

void test_iterators() {
    cout << "Testing iterators..." << endl;
    Bintree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    tree.insert(1);
    tree.insert(9);

    // Test in-order iterator
    int expected[] = {1, 3, 5, 7, 9};
    int i = 0;
    for(auto it = tree.ibegin(); it != tree.iend(); ++it, ++i) {
        assert(*it == expected[i]);
    }

    // Test reverse iterator
    i = 4;
    for(auto it = tree.rbegin(); it != tree.rend(); ++it, --i) {
        assert(*it == expected[i]);
    }

    // Test pre-order iterator
    auto pit = tree.pbegin();
    assert(*pit == 5);  // Root first

    // Test post-order iterator
    auto poit = tree.pobegin();
    assert(*poit == 1);  // Leftmost leaf first

    // Test level-order iterator
    auto lit = tree.lbegin();
    assert(*lit == 5);  // Root first (BFS)

    cout << "  PASSED" << endl;
}

void test_dereference_operator() {
    cout << "Testing dereference operator..." << endl;
    Bintree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);

    assert(*tree == 3);  // First element in sorted order
    cout << "  PASSED" << endl;
}

void test_left_right_by_position() {
    cout << "Testing left/right by row and column..." << endl;
    Bintree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);

    // Access root (row 0, col 0)
    assert(tree.left(0, 0) == 5);
    assert(tree.right(0, 0) == 5);

    // Access left child (row 1, col 0)
    assert(tree.left(1, 0) == 3);

    // Access right child (row 1, col 1)
    assert(tree.right(1, 1) == 7);
    cout << "  PASSED" << endl;
}

void test_auto_sort() {
    cout << "Testing auto-sort on unsorted operations..." << endl;
    Bintree<int> tree;
    tree.emplace_left(10);
    tree.emplace_left(5);
    tree.emplace_right(15);

    // Tree should be unsorted now
    // Insert should auto-sort
    tree.insert(7);
    assert(tree.get(0) == 5);
    assert(tree.get(1) == 7);
    assert(tree.get(2) == 10);
    assert(tree.get(3) == 15);
    cout << "  PASSED" << endl;
}

int main() {
    cout << "=== Running Bintree Tests ===" << endl << endl;

    test_default_constructor();
    test_insert();
    test_copy_constructor();
    test_assignment_operator();
    test_equality_operators();
    test_root();
    test_empty();
    test_height();
    test_size();
    test_get();
    test_at();
    test_bracket_operator();
    test_emplace_left();
    test_emplace_right();
    test_prune_left();
    test_prune_right();
    test_clear();
    test_custom_comparator();
    test_iterators();
    test_dereference_operator();
    test_left_right_by_position();
    test_auto_sort();

    cout << endl << "=== All Tests Passed ===" << endl;
    return 0;
}
