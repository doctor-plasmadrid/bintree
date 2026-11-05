/**
 * @file bintree.tpp
 * @brief Implementation of template methods for the Bintree class template.
 *
 * @author Dr. Plasmadrid
 * @copyright MIT License
 */

#ifndef BINTREE_TPP
#define BINTREE_TPP

#include <utility>
#include <vector>
#include <cassert>
#include <algorithm>
#include "bintree.h"

/**
 * @brief Calculates row and column coordinates in the tree from a linear index.
 * @tparam T The element type stored in the tree.
 * @param index Linear index of the node.
 * @return std::pair<size_t, size_t> Pair representing (row, column).
 */
template <typename T>
pair<size_t,size_t> Bintree<T>::calc_pos(size_t index) const {
    if(index == 0){
        return make_pair(0, 0);
    }
    size_t row = 0;
    size_t elements_so_far = 1; // Root node count
    while(index >= elements_so_far){
        row++;
        elements_so_far += (1 << row); // 2^row
    }
    size_t start_index = (1 << row) - 1;
    size_t col = index - start_index;
    return make_pair(row, col);
}

/**
 * @brief Calculates the linear index corresponding to a given row and column.
 * @tparam T The element type stored in the tree.
 * @param row Level in the binary tree (0 for root).
 * @param col Position within the level.
 * @return size_t Computed linear index.
 */
template <typename T>
size_t Bintree<T>::calc_index(size_t row, size_t col) const {
    if(row == 0){
        return 0;
    }
    size_t start_index = (1 << row) - 1; // 2^row - 1
    return start_index + col;
}

/**
 * @brief Retrieves the value at a specified linear index.
 * @tparam T The element type stored in the tree.
 * @param index The linear index of the node.
 * @return T The node value.
 */
template <typename T>
T Bintree<T>::left(size_t index){
    if(index == 0){
        assert(parent_initialized && "Parent not initialized");
        return parent;
    } else {
        size_t leaf_pos = index - 1;
        size_t parent_index = (index - 1) / 2;
        if(index == 2 * parent_index + 1){
            // Left child of its parent
            assert(leaf_pos < leaves_initialized.size() && leaves_initialized[leaf_pos].first && "Element not initialized");
            return leaves[leaf_pos].first;
        } else {
            // Right child of its parent
            assert(leaf_pos < leaves_initialized.size() && leaves_initialized[leaf_pos].second && "Element not initialized");
            return leaves[leaf_pos].second;
        }
    }
}

/**
 * @brief Retrieves the value at a specified linear index (equivalent to left(index)).
 * @tparam T The element type stored in the tree.
 * @param index The linear index of the node.
 * @return T The node value.
 */
template <typename T>
T Bintree<T>::right(size_t index){
    return left(index);
}

/**
 * @brief Helper function to retrieve the value of a node at a given index.
 * @tparam T The element type stored in the tree.
 * @param index The linear node index.
 * @return T The node value.
 */
template <typename T>
T Bintree<T>::get_node_value(size_t index) const {
    if(index == 0){
        return parent;
    } else {
        size_t leaf_pos = index - 1;
        size_t parent_index = (index - 1) / 2;
        if(index == 2 * parent_index + 1){
            return leaves[leaf_pos].first;
        } else {
            return leaves[leaf_pos].second;
        }
    }
}

/**
 * @brief Checks whether the node at a given index is initialized.
 * @tparam T The element type stored in the tree.
 * @param index The linear node index.
 * @return bool True if the node is initialized, false otherwise.
 */
template <typename T>
bool Bintree<T>::is_node_initialized(size_t index) const {
    if(index == 0){
        return parent_initialized;
    } else {
        size_t leaf_pos = index - 1;
        size_t parent_index = (index - 1) / 2;
        if(index == 2 * parent_index + 1){
            return leaf_pos < leaves_initialized.size() && leaves_initialized[leaf_pos].first;
        } else {
            return leaf_pos < leaves_initialized.size() && leaves_initialized[leaf_pos].second;
        }
    }
}

/**
 * @brief Recursively copies a subtree into a destination tree.
 * @tparam T The element type stored in the tree.
 * @param source_index The starting index in the source tree.
 * @param dest Reference to the destination tree.
 * @param dest_index The starting index in the destination tree.
 */
template <typename T>
void Bintree<T>::copy_subtree(size_t source_index, Bintree<T>& dest, size_t dest_index) {
    if(!is_node_initialized(source_index)){
        return;
    }

    // Copy node value
    if(dest_index == 0){
        dest.parent = get_node_value(source_index);
        dest.parent_initialized = true;
    } else {
        size_t dest_leaf_pos = dest_index - 1;
        size_t dest_parent_index = (dest_index - 1) / 2;
        if(dest_leaf_pos >= dest.leaves.size()){
            dest.leaves.resize(dest_leaf_pos + 1);
            dest.leaves_initialized.resize(dest_leaf_pos + 1);
        }
        if(dest_index == 2 * dest_parent_index + 1){
            dest.leaves[dest_leaf_pos].first = get_node_value(source_index);
            dest.leaves_initialized[dest_leaf_pos].first = true;
        } else {
            dest.leaves[dest_leaf_pos].second = get_node_value(source_index);
            dest.leaves_initialized[dest_leaf_pos].second = true;
        }
    }

    // Recursively copy child nodes
    size_t source_left = 2 * source_index + 1;
    size_t source_right = 2 * source_index + 2;
    size_t dest_left = 2 * dest_index + 1;
    size_t dest_right = 2 * dest_index + 2;

    copy_subtree(source_left, dest, dest_left);
    copy_subtree(source_right, dest, dest_right);
}

/**
 * @brief Prunes a subtree at the given index and returns it as a new tree.
 * @tparam T The element type stored in the tree.
 * @param index Root index of the subtree to prune.
 * @return Bintree<T> The extracted subtree.
 */
template <typename T>
Bintree<T> Bintree<T>::prune_subtree(size_t index){
    Bintree<T> result;
    if(!is_node_initialized(index)){
        return result;
    }

    // Copy subtree into the result tree
    copy_subtree(index, result, 0);

    // Remove subtree from the original tree
    remove_subtree(index);

    result.is_sorted = is_sorted;
    return result;
}

/**
 * @brief Recursively removes a subtree starting at the specified index.
 * @tparam T The element type stored in the tree.
 * @param index Root index of the subtree to delete.
 */
template <typename T>
void Bintree<T>::remove_subtree(size_t index){
    if(!is_node_initialized(index)){
        return;
    }

    is_sorted = false;

    size_t left_child_index = 2 * index + 1;
    size_t right_child_index = 2 * index + 2;

    // Recursively remove left subtree
    remove_subtree(left_child_index);

    // Recursively remove right subtree
    remove_subtree(right_child_index);

    // Mark current node as uninitialized
    if(index == 0){
        parent_initialized = false;
    } else {
        size_t leaf_pos = index - 1;
        size_t parent_index = (index - 1) / 2;
        if(index == 2 * parent_index + 1){
            leaves_initialized[leaf_pos].first = false;
        } else {
            leaves_initialized[leaf_pos].second = false;
        }
    }
}

/**
 * @brief Prunes the entire left branch of the root and returns it.
 * @tparam T The element type stored in the tree.
 * @return Bintree<T> The left subtree.
 */
template <typename T>
Bintree<T> Bintree<T>::prune_left(){
    Bintree<T> result;
    if(parent_initialized && is_node_initialized(1)){
        result = prune_subtree(1);
    }
    return result;
}

/**
 * @brief Prunes the entire right branch of the root and returns it.
 * @tparam T The element type stored in the tree.
 * @return Bintree<T> The right subtree.
 */
template <typename T>
Bintree<T> Bintree<T>::prune_right(){
    Bintree<T> result;
    if(parent_initialized && is_node_initialized(2)){
        result = prune_subtree(2);
    }
    return result;
}

/**
 * @brief Inserts an element to the left without enforcing BST order.
 * @tparam T The element type stored in the tree.
 * @param element The element to insert.
 */
template <typename T>
void Bintree<T>::emplace_left(T element){
    is_sorted = false;
    if(!parent_initialized){
        parent = element;
        parent_initialized = true;
        return;
    }

    if(leaves.size() < 1){
        leaves.resize(1);
        leaves_initialized.resize(1);
    }

    if(!leaves_initialized[0].first){
        leaves[0].first = element;
        leaves_initialized[0].first = true;
    }
}

/**
 * @brief Inserts an element to the right without enforcing BST order.
 * @tparam T The element type stored in the tree.
 * @param element The element to insert.
 */
template <typename T>
void Bintree<T>::emplace_right(T element){
    is_sorted = false;
    if(!parent_initialized){
        parent = element;
        parent_initialized = true;
        return;
    }
    if(leaves.size() < 2){
        leaves.resize(2);
        leaves_initialized.resize(2);
    }
    if(!leaves_initialized[1].second){
        leaves[1].second = element;
        leaves_initialized[1].second = true;
    }
}

/**
 * @brief Assigns a subtree into the first available empty leaf, clearing the source tree.
 * @tparam T The element type stored in the tree.
 * @param subtree The subtree to assign (cleared after assignment).
 */
template <typename T>
void Bintree<T>::assign_subtree(Bintree& subtree){
    is_sorted = false;
    if(!subtree.parent_initialized){
        return;
    }

    if(!parent_initialized){
        // This tree is empty; transfer everything
        *this = subtree;
        subtree.clear();
        return;
    }

    // Attempt to assign to the left child of the root
    if(leaves.size() < 1){
        leaves.resize(1);
        leaves_initialized.resize(1);
    }

    if(!leaves_initialized[0].first){
        leaves[0].first = subtree.parent;
        leaves_initialized[0].first = true;
        if(subtree.leaves.size() > 0){
            leaves[0].second = subtree.leaves[0].second;
            leaves_initialized[0].second = subtree.leaves_initialized[0].second;
        }
        subtree.clear();
    } else if(!leaves_initialized[0].second){
        leaves[0].second = subtree.parent;
        leaves_initialized[0].second = true;
        subtree.clear();
    }
}

/**
 * @brief Replaces the left subtree with the provided subtree, clearing the source tree.
 * @tparam T The element type stored in the tree.
 * @param subtree The replacement tree (cleared after replacement).
 */
template <typename T>
void Bintree<T>::replace_subtree(Bintree& subtree){
    is_sorted = false;
    if(!subtree.parent_initialized){
        return;
    }

    if(!parent_initialized){
        // This tree is empty; transfer everything
        *this = subtree;
        subtree.clear();
        return;
    }

    // Replace the left subtree of the root
    if(leaves.size() > 0 && leaves_initialized[0].first){
        remove_subtree(1);
    }

    // Insert the new subtree as left child
    if(leaves.size() < 1){
        leaves.resize(1);
        leaves_initialized.resize(1);
    }

    leaves[0].first = subtree.parent;
    leaves_initialized[0].first = true;

    if(subtree.leaves.size() > 0){
        leaves[0].second = subtree.leaves[0].second;
        leaves_initialized[0].second = subtree.leaves_initialized[0].second;
    }

    subtree.clear();
}

/**
 * @brief Rebuilds the tree as a valid Binary Search Tree.
 * @tparam T The element type stored in the tree.
 */
template <typename T>
void Bintree<T>::sort_tree(){
    if(is_sorted || !parent_initialized){
        return;
    }

    // Extract all elements using in-order traversal
    vector<T> elements;
    in_order_traversal(0, elements);

    // Sort the elements and silently drop duplicates
    sort(elements.begin(), elements.end(),
         [this](const T& a, const T& b){ return compare(a, b); });
    vector<T> unique_elements;
    for(const auto& elem : elements){
        if(unique_elements.empty() ||
           compare(elem, unique_elements.back()) ||
           compare(unique_elements.back(), elem)){
            unique_elements.push_back(elem);
        }
    }

    // Clear the tree
    clear();

    // Rebuild as a balanced BST (median root, recursive halves)
    build_balanced(unique_elements, 0, unique_elements.size(), 0);

    is_sorted = true;
}

/**
 * @brief Assigns a value to the node at the given linear index.
 * @tparam T The element type stored in the tree.
 * @param index The linear node index.
 * @param value The value to assign.
 */
template <typename T>
void Bintree<T>::assign_node(size_t index, const T& value){
    if(index == 0){
        parent = value;
        parent_initialized = true;
        return;
    }

    size_t leaf_pos = index - 1;
    if(leaf_pos >= leaves.size()){
        leaves.resize(leaf_pos + 1);
        leaves_initialized.resize(leaf_pos + 1);
    }

    size_t parent_index = (index - 1) / 2;
    if(index == 2 * parent_index + 1){
        leaves[leaf_pos].first = value;
        leaves_initialized[leaf_pos].first = true;
    } else {
        leaves[leaf_pos].second = value;
        leaves_initialized[leaf_pos].second = true;
    }
}

/**
 * @brief Recursively builds a balanced BST from a sorted element range.
 * @tparam T The element type stored in the tree.
 * @param elements Sorted vector of unique elements.
 * @param lo Start of the range (inclusive).
 * @param hi End of the range (exclusive).
 * @param index Linear index of the node that receives the range median.
 */
template <typename T>
void Bintree<T>::build_balanced(const vector<T>& elements, size_t lo, size_t hi, size_t index){
    if(lo >= hi){
        return;
    }

    size_t mid = lo + (hi - lo) / 2;
    assign_node(index, elements[mid]);
    build_balanced(elements, lo, mid, 2 * index + 1);
    build_balanced(elements, mid + 1, hi, 2 * index + 2);
}

/**
 * @brief Clears all elements from the tree and resets state.
 * @tparam T The element type stored in the tree.
 */
template <typename T>
void Bintree<T>::clear(){
    parent_initialized = false;
    leaves.clear();
    leaves_initialized.clear();
    is_sorted = true;
}

/**
 * @brief Retrieves the root element of the tree.
 * @tparam T The element type stored in the tree.
 * @return T Root element value.
 */
template <typename T>
T Bintree<T>::root() const {
    assert(parent_initialized && "Tree is empty");
    return parent;
}

/**
 * @brief Checks if the tree contains no elements.
 * @tparam T The element type stored in the tree.
 * @return bool True if empty, false otherwise.
 */
template <typename T>
bool Bintree<T>::empty() const {
    if(!parent_initialized){
        return true;
    }
    return false;
}

/**
 * @brief Copy assignment operator (deep copy).
 * @tparam T The element type stored in the tree.
 * @param other The tree instance to copy from.
 * @return Bintree<T>& Reference to this instance.
 */
template <typename T>
Bintree<T>& Bintree<T>::operator=(const Bintree<T>& other){
    if(this != &other){
        parent = other.parent;
        leaves = other.leaves;
        parent_initialized = other.parent_initialized;
        leaves_initialized = other.leaves_initialized;
        compare = other.compare;
        is_sorted = other.is_sorted;
    }
    return *this;
}

/**
 * @brief Equality comparison operator.
 * @tparam T The element type stored in the tree.
 * @param other The tree instance to compare against.
 * @return bool True if both trees are structurally and element-wise identical, false otherwise.
 */
template <typename T>
bool Bintree<T>::operator==(const Bintree<T>& other) const {
    if(parent_initialized != other.parent_initialized){
        return false;
    }
    if(parent_initialized && parent != other.parent){
        return false;
    }
    if(leaves.size() != other.leaves.size()){
        return false;
    }
    if(leaves_initialized.size() != other.leaves_initialized.size()){
        return false;
    }
    for(size_t i = 0; i < leaves.size(); i++){
        if(leaves_initialized[i].first != other.leaves_initialized[i].first){
            return false;
        }
        if(leaves_initialized[i].second != other.leaves_initialized[i].second){
            return false;
        }
        if(leaves_initialized[i].first && leaves[i].first != other.leaves[i].first){
            return false;
        }
        if(leaves_initialized[i].second && leaves[i].second != other.leaves[i].second){
            return false;
        }
    }
    return true;
}

/**
 * @brief Inequality comparison operator.
 * @tparam T The element type stored in the tree.
 * @param other The tree instance to compare against.
 * @return bool True if trees differ, false otherwise.
 */
template <typename T>
bool Bintree<T>::operator!=(const Bintree<T>& other) const {
    return !(*this == other);
}

/**
 * @brief Inserts an element into the tree while preserving BST order.
 * @tparam T The element type stored in the tree.
 * @param element The element value to insert.
 */
template <typename T>
void Bintree<T>::insert(T element){
    // Sort tree first if unsorted
    if(!is_sorted){
        sort_tree();
    }

    // If tree is empty, assign as root (parent)
    if(!parent_initialized){
        parent = element;
        parent_initialized = true;
        is_sorted = true;
        return;
    }

    // BST insertion: traverse from root using comparator
    size_t current_index = 0;

    while(true){
        T current_value = left(current_index);
        size_t left_child_index = 2 * current_index + 1;
        size_t right_child_index = 2 * current_index + 2;

        if(compare(element, current_value)){
            // Traverse left
            if(left_child_index == 0){
                // Left child of parent (index 1)
                if(leaves.size() < 1){
                    leaves.resize(1);
                    leaves_initialized.resize(1);
                }
                if(leaves_initialized[0].first){
                    current_index = left_child_index;
                } else {
                    leaves[0].first = element;
                    leaves_initialized[0].first = true;
                    return;
                }
            } else {
                size_t leaf_pos = left_child_index - 1;
                if(leaf_pos >= leaves.size()){
                    leaves.resize(leaf_pos + 1);
                    leaves_initialized.resize(leaf_pos + 1);
                }
                if(leaves_initialized[leaf_pos].first){
                    current_index = left_child_index;
                } else {
                    leaves[leaf_pos].first = element;
                    leaves_initialized[leaf_pos].first = true;
                    return;
                }
            }
        } else if(compare(current_value, element)){
            // Traverse right
            if(right_child_index == 0){
                // Right child of parent (index 2)
                if(leaves.size() < 1){
                    leaves.resize(1);
                    leaves_initialized.resize(1);
                }
                if(leaves_initialized[0].second){
                    current_index = right_child_index;
                } else {
                    leaves[0].second = element;
                    leaves_initialized[0].second = true;
                    return;
                }
            } else {
                size_t leaf_pos = right_child_index - 1;
                if(leaf_pos >= leaves.size()){
                    leaves.resize(leaf_pos + 1);
                    leaves_initialized.resize(leaf_pos + 1);
                }
                if(leaves_initialized[leaf_pos].second){
                    current_index = right_child_index;
                } else {
                    leaves[leaf_pos].second = element;
                    leaves_initialized[leaf_pos].second = true;
                    return;
                }
            }
        } else {
            // Element is equivalent to current_value; silently omit duplicate
            return;
        }
    }
}

/**
 * @brief Computes the height of the tree (number of levels).
 * @tparam T The element type stored in the tree.
 * @return size_t Tree height (0 if empty).
 */
template <typename T>
size_t Bintree<T>::height() const {
    if(!parent_initialized){
        return 0;
    }
    size_t max_level = 0;
    for(size_t i = 0; i < leaves_initialized.size(); i++){
        if(leaves_initialized[i].first || leaves_initialized[i].second){
            pair<size_t,size_t> pos = calc_pos(i + 1);
            if(pos.first > max_level){
                max_level = pos.first;
            }
        }
    }
    return max_level + 1;
}

/**
 * @brief Calculates the total number of initialized elements in the tree.
 * @tparam T The element type stored in the tree.
 * @return size_t Element count.
 */
template <typename T>
size_t Bintree<T>::size() const {
    size_t count = 0;
    if(parent_initialized){
        count = 1;
    }
    for(size_t i = 0; i < leaves_initialized.size(); i++){
        if(leaves_initialized[i].first){
            count++;
        }
        if(leaves_initialized[i].second){
            count++;
        }
    }
    return count;
}

/**
 * @brief Performs an in-order traversal (left, root, right) starting at the given index.
 * @tparam T The element type stored in the tree.
 * @param index Starting linear index.
 * @param result Vector where visited element values are appended.
 */
template <typename T>
void Bintree<T>::in_order_traversal(size_t index, vector<T>& result) const {
    if(index == 0 && !parent_initialized){
        return;
    }

    size_t left_child_index = 2 * index + 1;
    size_t right_child_index = 2 * index + 2;

    // Traverse left subtree
    if(left_child_index == 0){
        if(leaves_initialized.size() > 0 && leaves_initialized[0].first){
            in_order_traversal(left_child_index, result);
        }
    } else {
        size_t leaf_pos = left_child_index - 1;
        if(leaf_pos < leaves_initialized.size() && leaves_initialized[leaf_pos].first){
            in_order_traversal(left_child_index, result);
        }
    }

    // Visit current node
    if(index == 0){
        result.push_back(parent);
    } else {
        size_t leaf_pos = index - 1;
        size_t parent_index = (index - 1) / 2;
        if(index == 2 * parent_index + 1){
            // Left child of its parent
            if(leaf_pos < leaves_initialized.size() && leaves_initialized[leaf_pos].first){
                result.push_back(leaves[leaf_pos].first);
            }
        } else {
            // Right child of its parent
            if(leaf_pos < leaves_initialized.size() && leaves_initialized[leaf_pos].second){
                result.push_back(leaves[leaf_pos].second);
            }
        }
    }

    // Traverse right subtree
    if(right_child_index == 0){
        if(leaves_initialized.size() > 0 && leaves_initialized[0].second){
            in_order_traversal(right_child_index, result);
        }
    } else {
        size_t leaf_pos = right_child_index - 1;
        if(leaf_pos < leaves_initialized.size() && leaves_initialized[leaf_pos].second){
            in_order_traversal(right_child_index, result);
        }
    }
}

/**
 * @brief Performs a pre-order traversal (root, left, right) starting at the given index.
 * @tparam T The element type stored in the tree.
 * @param index Starting linear index.
 * @param result Vector where visited element values are appended.
 */
template <typename T>
void Bintree<T>::pre_order_traversal(size_t index, vector<T>& result) const {
    if(index == 0 && !parent_initialized){
        return;
    }

    size_t left_child_index = 2 * index + 1;
    size_t right_child_index = 2 * index + 2;

    // Visit current node first
    if(index == 0){
        result.push_back(parent);
    } else {
        size_t leaf_pos = index - 1;
        size_t parent_index = (index - 1) / 2;
        if(index == 2 * parent_index + 1){
            // Left child of its parent
            if(leaf_pos < leaves_initialized.size() && leaves_initialized[leaf_pos].first){
                result.push_back(leaves[leaf_pos].first);
            }
        } else {
            // Right child of its parent
            if(leaf_pos < leaves_initialized.size() && leaves_initialized[leaf_pos].second){
                result.push_back(leaves[leaf_pos].second);
            }
        }
    }

    // Traverse left subtree
    if(left_child_index == 0){
        if(leaves_initialized.size() > 0 && leaves_initialized[0].first){
            pre_order_traversal(left_child_index, result);
        }
    } else {
        size_t leaf_pos = left_child_index - 1;
        if(leaf_pos < leaves_initialized.size() && leaves_initialized[leaf_pos].first){
            pre_order_traversal(left_child_index, result);
        }
    }

    // Traverse right subtree
    if(right_child_index == 0){
        if(leaves_initialized.size() > 0 && leaves_initialized[0].second){
            pre_order_traversal(right_child_index, result);
        }
    } else {
        size_t leaf_pos = right_child_index - 1;
        if(leaf_pos < leaves_initialized.size() && leaves_initialized[leaf_pos].second){
            pre_order_traversal(right_child_index, result);
        }
    }
}

/**
 * @brief Performs a post-order traversal (left, right, root) starting at the given index.
 * @tparam T The element type stored in the tree.
 * @param index Starting linear index.
 * @param result Vector where visited element values are appended.
 */
template <typename T>
void Bintree<T>::post_order_traversal(size_t index, vector<T>& result) const {
    if(index == 0 && !parent_initialized){
        return;
    }

    size_t left_child_index = 2 * index + 1;
    size_t right_child_index = 2 * index + 2;

    // Traverse left subtree
    if(left_child_index == 0){
        if(leaves_initialized.size() > 0 && leaves_initialized[0].first){
            post_order_traversal(left_child_index, result);
        }
    } else {
        size_t leaf_pos = left_child_index - 1;
        if(leaf_pos < leaves_initialized.size() && leaves_initialized[leaf_pos].first){
            post_order_traversal(left_child_index, result);
        }
    }

    // Traverse right subtree
    if(right_child_index == 0){
        if(leaves_initialized.size() > 0 && leaves_initialized[0].second){
            post_order_traversal(right_child_index, result);
        }
    } else {
        size_t leaf_pos = right_child_index - 1;
        if(leaf_pos < leaves_initialized.size() && leaves_initialized[leaf_pos].second){
            post_order_traversal(right_child_index, result);
        }
    }

    // Visit current node last
    if(index == 0){
        result.push_back(parent);
    } else {
        size_t leaf_pos = index - 1;
        size_t parent_index = (index - 1) / 2;
        if(index == 2 * parent_index + 1){
            // Left child of its parent
            if(leaf_pos < leaves_initialized.size() && leaves_initialized[leaf_pos].first){
                result.push_back(leaves[leaf_pos].first);
            }
        } else {
            // Right child of its parent
            if(leaf_pos < leaves_initialized.size() && leaves_initialized[leaf_pos].second){
                result.push_back(leaves[leaf_pos].second);
            }
        }
    }
}

/**
 * @brief Performs a level-order traversal (breadth-first search) across the tree.
 * @tparam T The element type stored in the tree.
 * @param result Vector where visited element values are appended.
 */
template <typename T>
void Bintree<T>::level_order_traversal(vector<T>& result) const {
    if(!parent_initialized){
        return;
    }

    queue<size_t> q;
    q.push(0);

    while(!q.empty()){
        size_t current_index = q.front();
        q.pop();

        // Visit current node
        if(current_index == 0){
            result.push_back(parent);
        } else {
            size_t leaf_pos = current_index - 1;
            size_t parent_index = (current_index - 1) / 2;
            if(current_index == 2 * parent_index + 1){
                // Left child of its parent
                if(leaf_pos < leaves_initialized.size() && leaves_initialized[leaf_pos].first){
                    result.push_back(leaves[leaf_pos].first);
                }
            } else {
                // Right child of its parent
                if(leaf_pos < leaves_initialized.size() && leaves_initialized[leaf_pos].second){
                    result.push_back(leaves[leaf_pos].second);
                }
            }
        }

        // Enqueue child nodes
        size_t left_child_index = 2 * current_index + 1;
        size_t right_child_index = 2 * current_index + 2;

        if(left_child_index == 0){
            if(leaves_initialized.size() > 0 && leaves_initialized[0].first){
                q.push(left_child_index);
            }
        } else {
            size_t leaf_pos = left_child_index - 1;
            if(leaf_pos < leaves_initialized.size() && leaves_initialized[leaf_pos].first){
                q.push(left_child_index);
            }
        }

        if(right_child_index == 0){
            if(leaves_initialized.size() > 0 && leaves_initialized[0].second){
                q.push(right_child_index);
            }
        } else {
            size_t leaf_pos = right_child_index - 1;
            if(leaf_pos < leaves_initialized.size() && leaves_initialized[leaf_pos].second){
                q.push(right_child_index);
            }
        }
    }
}

/**
 * @brief Retrieves an element by its sorted index using in-order traversal.
 * @tparam T The element type stored in the tree.
 * @param index The 0-based sorted position.
 * @return T The element at that index.
 */
template <typename T>
T Bintree<T>::get(size_t index) const {
    const_cast<Bintree*>(this)->sort_tree();
    vector<T> elements;
    in_order_traversal(0, elements);
    assert(index < elements.size() && "Index out of bounds");
    return elements[index];
}

/**
 * @brief Retrieves an element by sorted index with boundary checking.
 * @tparam T The element type stored in the tree.
 * @param index The 0-based sorted position.
 * @return T The element at that index.
 * @throws std::out_of_range If the index is out of bounds.
 */
template <typename T>
T Bintree<T>::at(size_t index) const {
    const_cast<Bintree*>(this)->sort_tree();
    vector<T> elements;
    in_order_traversal(0, elements);
    if(index >= elements.size()){
        throw out_of_range("Index out of bounds");
    }
    return elements[index];
}

/**
 * @brief Accesses an element by its raw tree array index.
 * @tparam T The element type stored in the tree.
 * @param index The linear index in the tree representation.
 * @return T The element value.
 */
template <typename T>
T Bintree<T>::operator[](size_t index) const {
    if(index == 0){
        assert(parent_initialized && "Parent not initialized");
        return parent;
    } else {
        size_t leaf_pos = index - 1;
        assert(leaf_pos < leaves_initialized.size() && "Index out of bounds");
        size_t parent_index = (index - 1) / 2;
        if(index == 2 * parent_index + 1){
            // Left child
            assert(leaves_initialized[leaf_pos].first && "Element not initialized");
            return leaves[leaf_pos].first;
        } else {
            // Right child
            assert(leaves_initialized[leaf_pos].second && "Element not initialized");
            return leaves[leaf_pos].second;
        }
    }
}

/**
 * @brief Retrieves the left child element at a specified level and column.
 * @tparam T The element type stored in the tree.
 * @param row Level in the tree.
 * @param col Position within the level.
 * @return T The left child value.
 */
template <typename T>
T Bintree<T>::left(size_t row, size_t col) const {
    size_t index = calc_index(row, col);
    if(index == 0){
        assert(parent_initialized && "Parent not initialized");
        return parent;
    } else {
        size_t leaf_pos = index - 1;
        assert(leaf_pos < leaves_initialized.size() && leaves_initialized[leaf_pos].first && "Left element not initialized");
        return leaves[leaf_pos].first;
    }
}

/**
 * @brief Retrieves the right child element at a specified level and column.
 * @tparam T The element type stored in the tree.
 * @param row Level in the tree.
 * @param col Position within the level.
 * @return T The right child value.
 */
template <typename T>
T Bintree<T>::right(size_t row, size_t col) const {
    size_t index = calc_index(row, col);
    if(index == 0){
        assert(parent_initialized && "Parent not initialized");
        return parent;
    } else {
        size_t leaf_pos = index - 1;
        assert(leaf_pos < leaves_initialized.size() && leaves_initialized[leaf_pos].second && "Right element not initialized");
        return leaves[leaf_pos].second;
    }
}

#endif