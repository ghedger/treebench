// btree.h
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

#include "btree.h"
#include "stdio.h"

namespace hedger
{

// Constructor
BTree::BTree()
{
  root_ = nullptr;
  size_ = 0;
  maxSize_ = 0;
  nodeTot_ = 0;
}

BTree::~BTree()
{
  // TODO: Delete all nodes
  DeleteRecursive(root_);
}

void BTree::DeleteRecursive(hedger::Node *node)
{
  if (node && node->data) {
    DeleteRecursive(node->left);
    DeleteRecursive(node->right);
    delete (int *) node->data;
    node->data = nullptr;
    delete node;
  }
}

// add
//
// Adds a node and returns pointer to the node.
// Calls itself recursively.
//
// Entry: key
// Exit:  -
Node *BTree::Add(hedger::S_T key, int *depth)
{
  hedger::Node *node = new hedger::Node(key);

  // Is this the first node in the tree? If so, we are done!
  if (nullptr == root_) {
    root_ = node;
    nodeTot_++;
    *depth = 1;
    return node;
  }

  // Find and set appropriate parent
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
  currentDepth++;
  if(depth) {
    *depth = currentDepth;
  }

  node->parent = candidateParent;
  if (node->key < node->parent->key) {
    node->parent->left = node;
  } else {
    node->parent->right = node;
  }
  node->left = nullptr;
  node->right = nullptr;
  ChangeSize(1);

  nodeTot_++;
  return node;
}

bool BTree::DeleteKey(hedger::S_T key)
{
  bool result = false;              // assume failure
  hedger::Node *node = Find(key);
  if (node) {
    DeleteNode(node, key);
    result = true;
  }
  return result;
}

// deleteNode
//
// remove node accounting for children
//
// Entry:
// Exit:
hedger::Node *BTree::DeleteNode(hedger::Node *node, hedger::S_T key)
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

// removeFindMin
//
// Entry: pointer righthand child of node to be deleted
//
hedger::Node *BTree::FindMin(hedger::Node *node)
{
  hedger::Node *current = node;
  while (current->left != nullptr) {
    current = current->left;
  }
  return current;
}

// maxDepth
//
// Report the maximum depth of the tree.
// TODO: Implement
//
// Entry:
// Exit:
int BTree::MaxDepth()
{
  // Traverse the tree beginning from the root and keep a high-water mark of
  // maximum depth, and return it.
  int maxDepth = 0;

  if (root_) {
    MaxDepthRecurse(root_, 0, &maxDepth);
  }

  return maxDepth;
}

void BTree::MaxDepthRecurse(hedger::Node *node, int depth, int *maxDepth)
{
  // Terminal leaf node?
  if (!node->left && !node->right) {
    // update high water mark
    if (depth > *maxDepth) {
      *maxDepth = depth;
    }
    printf( "%d\n", depth);
  } else {
    if (node->left) {
      MaxDepthRecurse(node->left, depth + 1, maxDepth);
    }
    if (node->right) {
      MaxDepthRecurse(node->right, depth + 1, maxDepth);
    }
  }
}



// find
//
// Find node by key.
//
// Entry: key
// Exit: node
hedger::Node *BTree::Find(hedger::S_T key)
{
  hedger::Node *node = FindRecurse(key, root_);
  return node;
}

//
// Helper functions
//

void BTree::ChangeSize(int delta)
{
  if(size_ + delta > 0 ) {
    size_ += delta;
  }
}

// print
//
// Spit out textual representation of tree
//
// Entry: -
// Exit:  -
void BTree::Print(hedger::Node *node)
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

hedger::Node *BTree::FindRecurse(hedger::S_T key, hedger::Node *node)
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
      // TODO: Equal case - duplicate keys disallowed
    }
  }
  return nullptr;
}
} // namespace hedger
