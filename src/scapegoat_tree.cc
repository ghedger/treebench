// scapegoat.cc
//
// Implements a scapegoat tree derived from BTree.
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

#include <math.h>

#include "scapegoat_tree.h"

namespace hedger
{
// Constructor
ScapegoatTree::ScapegoatTree() : BTree::BTree()
{
}

// Destructor
ScapegoatTree::~ScapegoatTree()
{
  // TODO: delete all nodes to eliminate memory leaks
}

// log32
//
// Gets the log-base 3/2 of tree
//
// (source: https://www.sanfoundry.com/cpp-program-implement-scapegoat-tree/)
int const ScapegoatTree::Log32(int q)
{
  double const log23 = 2.4663034623764317;
  return (int) ceil(log23 * log(q));
}

// add
//
// Add a node to the tree structure.
//
// Entry: key of new node
// Exit:  pointer to new node
hedger::Node *ScapegoatTree::Add(hedger::S_T key)
{
  // Use BTree's regular unbalanced insertion
  int depth;
  hedger::Node *node = BTree::Add(key, &depth);

  // Is it time to rebalance?
  int q = Log32(nodeTot_);
  if (depth > q) {
    // Walk up tree starting at our node.
    hedger::Node *walk = node->parent;
    if (walk) {
      while (3 * SizeOfSubtree(walk) <= 2 * SizeOfSubtree(walk->parent)) {
        walk = walk->parent;
      }
      if (walk->parent) {
        Rebalance(walk->parent);
      }
    }
  }

  return node;
}

// sizeOfSubtree
//
// Returns the number of nodes underneath a given parent node.
//
// Entry: pointer to parent node
// Exit:  node total
int ScapegoatTree::SizeOfSubtree(hedger::Node *node)
{
  if (!node) {
    return 0;
  }
  return SizeOfSubtree(node->left) + SizeOfSubtree(node->right) + 1;
}

// packIntoArray
//
// Put nodes into a flat array for rebuilding purposes.  We
// recursively call ourselves traversing the tree from the reroot
// base.
//
// Entry:  root node
//        rebuild array of node pointers
//        current array index
int ScapegoatTree::PackIntoArray(hedger::Node *node, hedger::Node *rebuildArray[], int i)
{
    if (!node) {
        return i;
    }
    i = PackIntoArray(node->left, rebuildArray, i);
    rebuildArray[i++] = node;     // node pointer
    return PackIntoArray(node->right, rebuildArray, i);
}

// rebalance
//
// Flatten the tree and rebuild it from the designated root node.
//
// Entry: root node of subtree to rebuild
// Exit:
void ScapegoatTree::Rebalance(hedger::Node *node)
{
  // Allocate temporary array for new flattened tree.
  // This array holds pointers to nodes.
  int nodeTot = SizeOfSubtree(node);
  hedger::Node *parent = node->parent;
  hedger::Node **rebuildArray = new hedger::Node* [nodeTot];
  PackIntoArray(node, rebuildArray, 0);

  // Recursively rebuild of array from flattened tree
  if (!parent) {
    hedger::Node *root = BuildBalanced(rebuildArray, 0, nodeTot);
    root->parent = nullptr;
  } else if (parent->right == node) {
    parent->right = BuildBalanced(rebuildArray, 0, nodeTot);
    parent->right->parent = parent;
  } else {
    parent->left = BuildBalanced(rebuildArray, 0, nodeTot);
    parent->left->parent = parent;
  }
}

// buildBalanced
//
// rebalance the tree from a flat array
//
// Entry: pointer to array of node pointers
//        start index in array
//        total number of nodes in the array
// Exit:  Node at midpoint of array
hedger::Node *ScapegoatTree::BuildBalanced(hedger::Node **rebuildArray, int i, int nodeTot)
{
  if (!nodeTot) {
    return NULL;
  }

  int m = nodeTot / 2;
  rebuildArray[i + m]->left = BuildBalanced(rebuildArray, i, m);
  if (rebuildArray[i + m]->left != nullptr) {
    rebuildArray[i + m]->left->parent = rebuildArray[i + m];
  }
  rebuildArray[i + m]->right = BuildBalanced(rebuildArray, i + m + 1, nodeTot - m - 1);

  if (rebuildArray[i + m]->right != nullptr) {
    rebuildArray[i + m]->right->parent = rebuildArray[i + m];
  }

  return rebuildArray[i + m];
}
} // namespace hedger
