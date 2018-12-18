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

#ifndef BTREE_H_
#define BTREE_H_

#include "algo.h"

namespace hedger
{
struct Node
{
  Node(hedger::S_T newKey) {
    key = newKey;
    left = right = parent = nullptr;
    data = nullptr;
  }
  ~Node() {};

  hedger::Node *      left;     // left leg
  hedger::Node *      right;    // right leg
  hedger::Node *      parent;   // parent (could be axed)
  hedger::S_T         key;      // key
  void *              data;     // payload / "satellite" data
};


class BSTree
{
 public:
  BSTree();
  virtual ~BSTree();

  Node *Add(hedger::S_T key, int *depth = NULL);
  hedger::Node *DeleteNode(hedger::Node *node, hedger::S_T key);
  bool DeleteKey(hedger::S_T key);
  hedger::Node *Find(hedger::S_T key);
  void Print(hedger::Node *node = nullptr);
  int MaxDepth();

 protected:
  hedger::Node* FindMin(hedger::Node *node);
  void MaxDepthRecurse(hedger::Node *node, int depth, int *maxDepth);
  void DeleteRecursive(hedger::Node *node);
  void ChangeSize(int);
  Node *FindRecurse(hedger::S_T key, hedger::Node *node);

  Node *  root_;
  Node *  size_;
  Node *  maxSize_;
  int     nodeTot_;
};
} // namespace hedger
#endif // #ifndef BTREE_H_

