// algo.h
//
// Parent class for algorithms requiring analysis.
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

#ifndef ALGO_H_
#define ALGO_H_

#include <cstddef>

namespace hedger {

typedef int S_T;

// Algo is an ancestor class for any algorithm, and is to be used
// for maintaining relevant statistics on the algorithm (run time, mean, std deviation, etc)
class Algo
{
 public:
  Algo() {};
  virtual ~Algo() {};
  virtual int Test(hedger::S_T *t, std::size_t size) = 0;
};
}

#endif // ALGO_H_
