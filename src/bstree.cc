// bstree.h
//
// Implements an unbalanced binary tree.
//
// This file is part of treebench.
//
// treebench is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// treebench is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with treebench.  If not, see <https://www.gnu.org/licenses/>.
//
// Copyright (C) 2018 Gregory Hedger
//

#include "bstree.h"
#include "stdio.h"

namespace hedger
{

// Constructor
BSTree::BSTree()
{
  root_ = nullptr;
  size_ = 0;
  maxSize_ = 0;
  nodeTot_ = 0;
}

// Destructor
BSTree::~BSTree()
{
  // TODO: Delete all nodes
  DeleteRecursive(root_);
}

// DeleteRecursive
// Delete the whole substree under and including node.
// Entry: pointer to node
void BSTree::DeleteRecursive(hedger::Node *node)
{
  if (node && node->data) {
    DeleteRecursive(node->left);
    DeleteRecursive(node->right);
    delete (int *) node->data;
    node->data = nullptr;
    delete node;
  }
}

// Add
//
// Adds a node and returns pointer to the node.
// Calls itself recursively.  We keep the depth for data structure  analysis.
//
// Entry: key
//        pointer to depth int
// Exit:  -
Node *BSTree::Add(hedger::S_T key, int *depth)
{
  hedger::Node *node = new hedger::Node(key);

  // Is this the first node in the tree? If so, we are done!
  if (nullptr == root_) {
    root_ = node;
    nodeTot_++;
    *depth = 1;
    return node;
  }

  // Find appropriate parent based on key.
  Node *currentNode = root_;
  Node *candidateParent = nullptr;
  int currentDepth = 0;
  while (currentNode != nullptr) {
    candidateParent = currentNode;
    if (node->key < currentNode->key) {
      currentNode = currentNode->left;
      currentDepth++;
    } else {
      currentNode = currentNode->right;
      currentDepth++;
    }
  }

  // We are keeping the depth record for analysis
  currentDepth++;
  if(depth) {
    *depth = currentDepth;
  }

  // Determine whether to add node at right or left of parent.
  node->parent = candidateParent;
  if (node->key < node->parent->key) {
    node->parent->left = node;
  } else {
    node->parent->right = node;
  }
  node->left = nullptr;
  node->right = nullptr;

  // Change the size of the tree by +1
  ChangeSize(1);

  // Increase the node total
  nodeTot_++;
  return node;
}

// DeleteKey
// Delete hte node associated with the given key.
// Entry: key
// Exit: true == success
bool BSTree::DeleteKey(hedger::S_T key)
{
  bool result = false;              // assume failure
  hedger::Node *node = Find(key);
  if (node) {
    DeleteNode(node, key);
    result = true;
  }
  return result;
}

// DeleteNode
//
// Remove node, realigning descendent nodes.  Called recursively.
//
// Entry: pointer to node
//        key
// Exit:  pointer to node
hedger::Node *BSTree::DeleteNode(hedger::Node *node, hedger::S_T key)
{
  if (node) {
    if (key < node->key) {
      // target key is smaller; it's to the left
      node->left = DeleteNode(node->left, key);
    } else if (key > node->key) {
      // target key is greater; it's to the right
      node->right = DeleteNode(node->right, key);
    } else {
      // Case 0: Zero or single child: update linkage
      if (nullptr == node->left) {
        hedger::Node *successor = node->right;
        if (node->parent->left == node) {
          node->parent->left = successor;
        } else if (node->parent->right == node) {
          node->parent->right = successor;
        }
        delete node;
        return successor;
      } else if (nullptr == node->right) {
        hedger::Node *successor = node->left;
        if (node->parent->left == node) {
          node->parent->left = successor;
        } else if (node->parent->right == node) {
          node->parent->right = successor;
        }
        delete node;
        return successor;
      }

      // Case 2: Two children... need to surgically (recursively)
      // find successor and replace deleted node with that.
      hedger::Node *successor = FindMin(node->right);
      node->key = successor->key;
      // TODO: Consider using a smart pointer for "data"
      delete (int *) node->data;
      node->data = successor->data;
      successor->data = nullptr;
      node->right = DeleteNode(node->right, successor->key);
    }
  }
  return node;
}

// FindMin
// Find the "minimal" node, that is, the bottom-leftmost node.
//
// Entry: pointer righthand child of node to be deleted
// Exit:  pointer to minimal node
hedger::Node *BSTree::FindMin(hedger::Node *node)
{
  hedger::Node *current = node;
  while (current->left != nullptr) {
    current = current->left;
  }
  return current;
}

// MaxDepth
//
// Report the maximum depth of the tree.
// TODO: Implement
//
// Entry:
// Exit:
int BSTree::MaxDepth()
{
  // Traverse the tree beginning from the root and keep a high-water mark of
  // maximum depth, and return it.
  int max_depth = 0;

  if (root_) {
    MaxDepthRecurse(root_, 0, &max_depth);
  }

  return max_depth;
}

// MaxDepthRecurse
// Internal recursion function to find the maximum depth of the tree.
// Entry: pointer to node
//        current depth
//        pointer to max depth
void BSTree::MaxDepthRecurse(hedger::Node *node, int depth, int *max_depth)
{
  // Terminal leaf node?
  if (!node->left && !node->right) {
    // update high water mark
    if (depth > *max_depth) {
      *max_depth = depth;
    }
    printf( "%d\n", depth);
  } else {
    if (node->left) {
      MaxDepthRecurse(node->left, depth + 1, max_depth);
    }
    if (node->right) {
      MaxDepthRecurse(node->right, depth + 1, max_depth);
    }
  }
}

// Find
//
// Find node by key.
//
// Entry: key
// Exit: node
hedger::Node *BSTree::Find(hedger::S_T key)
{
  hedger::Node *node = FindRecurse(key, root_);
  return node;
}

//
// Helper functions
//

// ChangeSize
// Change the size of the tree (# of nodes)
// Entry: size change delta
void BSTree::ChangeSize(int delta)
{
  if(size_ + delta > 0 ) {
    size_ += delta;
  }
}

// Print
// Spit out textual representation of tree.  Called recursively.
// Entry: pointer to top node to print
// Exit:  -
void BSTree::Print(hedger::Node *node)
{
  if (!node) {          // sanity check
    node = root_;
    if (!node) {
      return;
    }
  }

  // Print the relationships of this node
  printf("%p:%d (p:%p l:%p r:%p)\n",
    node,
    node->key,
    node->parent,
    node->left,
    node->right);
  // Recurse
  if (node->left) {
    Print(node->left);
  }
  if (node->right) {
    Print(node->right);
  }
}

// FindRecurse
// Internal recursion function to find node by key.
// Entry: key
//        pointer to node
// Exit:  Node matching, or nullptr if not found / duplicate key
hedger::Node *BSTree::FindRecurse(hedger::S_T key, hedger::Node *node)
{
  if (node) {
    if (node->key == key) {
      return node;
    }
    if (node->key > key) {
      return FindRecurse(key, node->left);
    } else if(node->key < key) {
      return FindRecurse(key, node->right);
    } else {
      // TODO: Equal case - duplicate keys disallowed.
      // For the nonce, we'll return the nullptr since we just don't know.
    }
  }
  return nullptr;
}
} // namespace hedger
