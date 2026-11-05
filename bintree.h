/**
 * @file bintree.h
 * @brief Template-based Binary Search Tree implementation
 *
 * @author Dr. Plasmadrid
 * @copyright MIT License
 */

#ifndef BINTREE_H
#define BINTREE_H

#include <utility>
#include <vector>
#include <iterator>
#include <functional>
#include <queue>

using namespace std;

/**
 * @brief A template-based Binary Search Tree (BST) implementation
 *
 * @tparam T The type of elements stored in the tree
 *
 * This class provides a complete binary tree implementation with:
 * - BST insertion with custom comparators
 * - Multiple traversal orders (in-order, pre-order, post-order, level-order)
 * - STL-compatible iterators
 * - Tree manipulation operations (prune, emplace, assign, replace)
 * - Auto-sorting when tree becomes unsorted
 */
template <typename T>

class Bintree {
    private:
        T parent; ///< Root element of the tree
        vector<pair<T,T>> leaves; ///< Vector storing pairs of elements (left and right children)
        bool parent_initialized; ///< Flag indicating if root is initialized
        vector<pair<bool,bool>> leaves_initialized; ///< Flags indicating which elements are initialized
        function<bool(T,T)> compare; ///< Comparator function for BST ordering
        bool is_sorted; ///< Flag indicating if tree maintains BST order

        /**
         * @brief Calculate row and column from index
         * @param index The element index
         * @return Pair of (row, column)
         */
        pair<size_t,size_t> calc_pos(size_t index) const;

        /**
         * @brief Calculate index from row and column
         * @param row The tree level
         * @param col The position in the level
         * @return The calculated index
         */
        size_t calc_index(size_t row, size_t col) const;

        /**
         * @brief Get left child value by index
         * @param index The parent index
         * @return The left child value
         */
        T left(size_t index);

        /**
         * @brief Get right child value by index
         * @param index The parent index
         * @return The right child value
         */
        T right(size_t index);

        /**
         * @brief Prune subtree at given index and return it
         * @param index The index to prune from
         * @return The pruned subtree as a new tree
         */
        Bintree prune_subtree(size_t index);

        /**
         * @brief Get node value at index
         * @param index The node index
         * @return The node value
         */
        T get_node_value(size_t index) const;

        /**
         * @brief Check if node at index is initialized
         * @param index The node index
         * @return True if initialized, false otherwise
         */
        bool is_node_initialized(size_t index) const;

        /**
         * @brief Copy subtree from source to destination
         * @param source_index Source node index
         * @param dest Destination tree
         * @param dest_index Destination node index
         */
        void copy_subtree(size_t source_index, Bintree& dest, size_t dest_index);

        /**
         * @brief Remove subtree at index
         * @param index The node index to remove
         */
        void remove_subtree(size_t index);

        /**
         * @brief Sort the tree by rebuilding it as a balanced BST
         */
        void sort_tree();

        /**
         * @brief Assign a value to the node at the given index
         * @param index The linear node index
         * @param value The value to assign
         */
        void assign_node(size_t index, const T& value);

        /**
         * @brief Recursively build a balanced BST from a sorted range
         * @param elements Sorted element vector
         * @param lo Start of range (inclusive)
         * @param hi End of range (exclusive)
         * @param index Linear index of the node receiving the median
         */
        void build_balanced(const vector<T>& elements, size_t lo, size_t hi, size_t index);

    protected:
        /**
         * @brief In-order traversal (left, root, right)
         * @param index Starting node index
         * @param result Vector to store traversal result
         */
        void in_order_traversal(size_t index, vector<T>& result) const;

        /**
         * @brief Pre-order traversal (root, left, right)
         * @param index Starting node index
         * @param result Vector to store traversal result
         */
        void pre_order_traversal(size_t index, vector<T>& result) const;

        /**
         * @brief Post-order traversal (left, right, root)
         * @param index Starting node index
         * @param result Vector to store traversal result
         */
        void post_order_traversal(size_t index, vector<T>& result) const;

        /**
         * @brief Level-order traversal (BFS by level)
         * @param result Vector to store traversal result
         */
        void level_order_traversal(vector<T>& result) const;

    public:
        /**
         * @brief Default constructor
         */
        Bintree() : parent_initialized(false), compare(less<T>()), is_sorted(true) {}

        /**
         * @brief Copy constructor (deep copy)
         * @param other The tree to copy from
         */
        Bintree(const Bintree& other)
            : parent(other.parent), leaves(other.leaves),
              parent_initialized(other.parent_initialized),
              leaves_initialized(other.leaves_initialized),
              compare(other.compare), is_sorted(other.is_sorted) {}

        /**
         * @brief Constructor with custom comparator
         * @param comp The comparator function
         */
        Bintree(function<bool(T,T)> comp) : parent_initialized(false), compare(comp), is_sorted(true) {}

        /**
         * @brief Destructor
         */
        ~Bintree() {
            clear();
        }

        /**
         * @brief Assignment operator (deep copy)
         * @param other The tree to assign from
         * @return Reference to this tree
         */
        Bintree& operator=(const Bintree& other);

        /**
         * @brief Equality operator
         * @param other The tree to compare with
         * @return True if trees are identical, false otherwise
         */
        bool operator==(const Bintree& other) const;

        /**
         * @brief Inequality operator
         * @param other The tree to compare with
         * @return True if trees are different, false otherwise
         */
        bool operator!=(const Bintree& other) const;

        /**
         * @brief Get the root element
         * @return The root value
         */
        T root() const;

        /**
         * @brief Check if tree is empty
         * @return True if empty, false otherwise
         */
        bool empty() const;

        /**
         * @brief Insert element maintaining BST order
         * @param element The element to insert
         */
        void insert(T element);

        /**
         * @brief Get tree height (number of levels)
         * @return The height of the tree
         */
        size_t height() const;

        /**
         * @brief Get total number of elements
         * @return The size of the tree
         */
        size_t size() const;

        /**
         * @brief Get element by sorted index (in-order)
         * @param index The sorted index
         * @return The element at that index
         */
        T get(size_t index) const;

        /**
         * @brief Get element by sorted index with bounds checking
         * @param index The sorted index
         * @return The element at that index
         * @throws std::out_of_range if index is out of bounds
         */
        T at(size_t index) const;

        /**
         * @brief Access element by raw array index
         * @param index The raw index
         * @return The element at that index
         */
        T operator[](size_t index) const;

        /**
         * @brief Get left child by row and column
         * @param row The tree level
         * @param col The position in the level
         * @return The left child value
         */
        T left(size_t row, size_t col) const;

        /**
         * @brief Get right child by row and column
         * @param row The tree level
         * @param col The position in the level
         * @return The right child value
         */
        T right(size_t row, size_t col) const;

        /**
         * @brief Prune left subtree and return it
         * @return The pruned left subtree
         */
        Bintree prune_left();

        /**
         * @brief Prune right subtree and return it
         * @return The pruned right subtree
         */
        Bintree prune_right();

        /**
         * @brief Emplace element to left (no order check)
         * @param element The element to add
         */
        void emplace_left(T element);

        /**
         * @brief Emplace element to right (no order check)
         * @param element The element to add
         */
        void emplace_right(T element);

        /**
         * @brief Assign subtree to first empty leaf
         * @param subtree The subtree to assign (will be destroyed)
         */
        void assign_subtree(Bintree& subtree);

        /**
         * @brief Replace subtree with given tree
         * @param subtree The replacement tree (will be destroyed)
         */
        void replace_subtree(Bintree& subtree);

        /**
         * @brief Clear all elements
         */
        void clear();

        /**
         * @brief Dereference operator (returns first element)
         * @return The first element in sorted order
         */
        T operator*() const {
            return get(0);
        }

        /**
         * @brief In-order iterator class
         */
        class InOrderIterator : public std::iterator<std::bidirectional_iterator_tag, T> {
            private:
                const Bintree* tree;
                vector<T> elements;
                size_t current_index;

            public:
                InOrderIterator(const Bintree* t, size_t idx) : tree(t), current_index(idx) {
                    if(tree){
                        tree->in_order_traversal(0, elements);
                    }
                }

                InOrderIterator& operator++() {
                    ++current_index;
                    return *this;
                }

                InOrderIterator operator++(int) {
                    InOrderIterator temp = *this;
                    ++current_index;
                    return temp;
                }

                InOrderIterator& operator--() {
                    if(current_index > 0){
                        --current_index;
                    }
                    return *this;
                }

                InOrderIterator operator--(int) {
                    InOrderIterator temp = *this;
                    if(current_index > 0){
                        --current_index;
                    }
                    return temp;
                }

                T& operator*() {
                    return elements[current_index];
                }

                T* operator->() {
                    return &elements[current_index];
                }

                bool operator==(const InOrderIterator& other) const {
                    return current_index == other.current_index;
                }

                bool operator!=(const InOrderIterator& other) const {
                    return current_index != other.current_index;
                }
        };

        /**
         * @brief Pre-order iterator class
         */
        class PreOrderIterator : public std::iterator<std::bidirectional_iterator_tag, T> {
            private:
                const Bintree* tree;
                vector<T> elements;
                size_t current_index;

            public:
                PreOrderIterator(const Bintree* t, size_t idx) : tree(t), current_index(idx) {
                    if(tree){
                        tree->pre_order_traversal(0, elements);
                    }
                }

                PreOrderIterator& operator++() {
                    ++current_index;
                    return *this;
                }

                PreOrderIterator operator++(int) {
                    PreOrderIterator temp = *this;
                    ++current_index;
                    return temp;
                }

                PreOrderIterator& operator--() {
                    if(current_index > 0){
                        --current_index;
                    }
                    return *this;
                }

                PreOrderIterator operator--(int) {
                    PreOrderIterator temp = *this;
                    if(current_index > 0){
                        --current_index;
                    }
                    return temp;
                }

                T& operator*() {
                    return elements[current_index];
                }

                T* operator->() {
                    return &elements[current_index];
                }

                bool operator==(const PreOrderIterator& other) const {
                    return current_index == other.current_index;
                }

                bool operator!=(const PreOrderIterator& other) const {
                    return current_index != other.current_index;
                }
        };

        /**
         * @brief Post-order iterator class
         */
        class PostOrderIterator : public std::iterator<std::bidirectional_iterator_tag, T> {
            private:
                const Bintree* tree;
                vector<T> elements;
                size_t current_index;

            public:
                PostOrderIterator(const Bintree* t, size_t idx) : tree(t), current_index(idx) {
                    if(tree){
                        tree->post_order_traversal(0, elements);
                    }
                }

                PostOrderIterator& operator++() {
                    ++current_index;
                    return *this;
                }

                PostOrderIterator operator++(int) {
                    PostOrderIterator temp = *this;
                    ++current_index;
                    return temp;
                }

                PostOrderIterator& operator--() {
                    if(current_index > 0){
                        --current_index;
                    }
                    return *this;
                }

                PostOrderIterator operator--(int) {
                    PostOrderIterator temp = *this;
                    if(current_index > 0){
                        --current_index;
                    }
                    return temp;
                }

                T& operator*() {
                    return elements[current_index];
                }

                T* operator->() {
                    return &elements[current_index];
                }

                bool operator==(const PostOrderIterator& other) const {
                    return current_index == other.current_index;
                }

                bool operator!=(const PostOrderIterator& other) const {
                    return current_index != other.current_index;
                }
        };

        /**
         * @brief Level-order iterator class
         */
        class LevelOrderIterator : public std::iterator<std::bidirectional_iterator_tag, T> {
            private:
                const Bintree* tree;
                vector<T> elements;
                size_t current_index;

            public:
                LevelOrderIterator(const Bintree* t, size_t idx) : tree(t), current_index(idx) {
                    if(tree){
                        tree->level_order_traversal(elements);
                    }
                }

                LevelOrderIterator& operator++() {
                    ++current_index;
                    return *this;
                }

                LevelOrderIterator operator++(int) {
                    LevelOrderIterator temp = *this;
                    ++current_index;
                    return temp;
                }

                LevelOrderIterator& operator--() {
                    if(current_index > 0){
                        --current_index;
                    }
                    return *this;
                }

                LevelOrderIterator operator--(int) {
                    LevelOrderIterator temp = *this;
                    if(current_index > 0){
                        --current_index;
                    }
                    return temp;
                }

                T& operator*() {
                    return elements[current_index];
                }

                T* operator->() {
                    return &elements[current_index];
                }

                bool operator==(const LevelOrderIterator& other) const {
                    return current_index == other.current_index;
                }

                bool operator!=(const LevelOrderIterator& other) const {
                    return current_index != other.current_index;
                }
        };

        /**
         * @brief Reverse iterator class
         */
        class ReverseIterator : public std::iterator<std::bidirectional_iterator_tag, T> {
            private:
                const Bintree* tree;
                vector<T> elements;
                size_t current_index;

            public:
                ReverseIterator(const Bintree* t, size_t idx) : tree(t), current_index(idx) {
                    if(tree){
                        tree->in_order_traversal(0, elements);
                    }
                }

                ReverseIterator& operator++() {
                    --current_index;
                    return *this;
                }

                ReverseIterator operator++(int) {
                    ReverseIterator temp = *this;
                    --current_index;
                    return temp;
                }

                ReverseIterator& operator--() {
                    ++current_index;
                    return *this;
                }

                ReverseIterator operator--(int) {
                    ReverseIterator temp = *this;
                    ++current_index;
                    return temp;
                }

                T& operator*() {
                    return elements[current_index];
                }

                T* operator->() {
                    return &elements[current_index];
                }

                bool operator==(const ReverseIterator& other) const {
                    return current_index == other.current_index;
                }

                bool operator!=(const ReverseIterator& other) const {
                    return current_index != other.current_index;
                }
        };

        /**
         * @brief Get iterator to beginning (in-order)
         * @return InOrderIterator to first element
         */
        InOrderIterator begin() const {
            return InOrderIterator(this, 0);
        }

        /**
         * @brief Get iterator to end (in-order)
         * @return InOrderIterator past last element
         */
        InOrderIterator end() const {
            return InOrderIterator(this, size());
        }

        /**
         * @brief Get reverse iterator to beginning
         * @return ReverseIterator to last element
         */
        ReverseIterator rbegin() const {
            return ReverseIterator(this, size() > 0 ? size() - 1 : 0);
        }

        /**
         * @brief Get reverse iterator to end
         * @return ReverseIterator before first element
         */
        ReverseIterator rend() const {
            return ReverseIterator(this, (size_t)-1);
        }

        /**
         * @brief Get in-order iterator to beginning
         * @return InOrderIterator to first element
         */
        InOrderIterator ibegin() const { return begin(); }

        /**
         * @brief Get in-order iterator to end
         * @return InOrderIterator past last element
         */
        InOrderIterator iend() const { return end(); }

        /**
         * @brief Get pre-order iterator to beginning
         * @return PreOrderIterator to first element
         */
        PreOrderIterator pbegin() const {
            return PreOrderIterator(this, 0);
        }

        /**
         * @brief Get pre-order iterator to end
         * @return PreOrderIterator past last element
         */
        PreOrderIterator pend() const {
            return PreOrderIterator(this, size());
        }

        /**
         * @brief Get post-order iterator to beginning
         * @return PostOrderIterator to first element
         */
        PostOrderIterator pobegin() const {
            return PostOrderIterator(this, 0);
        }

        /**
         * @brief Get post-order iterator to end
         * @return PostOrderIterator past last element
         */
        PostOrderIterator poend() const {
            return PostOrderIterator(this, size());
        }

        /**
         * @brief Get level-order iterator to beginning
         * @return LevelOrderIterator to first element
         */
        LevelOrderIterator lbegin() const {
            return LevelOrderIterator(this, 0);
        }

        /**
         * @brief Get level-order iterator to end
         * @return LevelOrderIterator past last element
         */
        LevelOrderIterator lend() const {
            return LevelOrderIterator(this, size());
        }
};

#include "bintree.tpp"

#endif
