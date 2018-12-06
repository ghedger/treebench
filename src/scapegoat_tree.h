// scapegoat.h
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

#ifndef SCAPEGOAT_H_
#define SCAPEGOAT_H_

#include "btree.h"

namespace hedger
{

class ScapegoatTree : public hedger::BTree
{
  public:
    ScapegoatTree();
    virtual ~ScapegoatTree();
    hedger::Node *Add(hedger::S_T key);

  private:
    static int const Log32(int q);
    hedger::Node *FindScapegoat(hedger::Node *node);
    bool IsBalancedAtNode(hedger::Node *node);
    int SizeOfSubtree(hedger::Node *node);
    int PackIntoArray(hedger::Node *node, hedger::Node *rebuildArray[], int i);
    void Rebalance(hedger::Node *node);
    hedger::Node *BuildBalanced(hedger::Node **rebuildArray, int i, int nodeTot);
};
} // namespace hedger
#endif // #ifndef SCAPEGOAT_H_

