/********** tell emacs we use -*- c++ -*- style comments *******************
 $Revision: 1.1 $  $Author: trey $  $Date: 2006-04-05 21:43:20 $
   
 @file    SawtoothUpperBound.h
 @brief   No brief

 Copyright (c) 2002-2005, Trey Smith
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are
 met:

 * The software may not be sold or incorporated into a commercial
   product without specific prior written permission.
 * The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 ***************************************************************************/

#ifndef INCSawtoothUpperBound_h
#define INCSawtoothUpperBound_h

#include <list>

#include "MatrixUtils.h"
#include "AbstractBound.h"
#include "Pomdp.h"

#define PRUNE_EPS (1e-10)

namespace zmdp {

struct BVPair {
  belief_vector b;
  double v;
  bool disabled;

  BVPair(void) : disabled(false) {}
  BVPair(const belief_vector& _b, double _v) {
    copyFrom(_b, _v);
  }

  // override standard copy ctor and = op to get the right behavior
  void copyFrom(const belief_vector& _b, double _v);
  BVPair(const BVPair& rhs) { copyFrom(rhs.b, rhs.v); }
  BVPair& operator=(const BVPair& rhs) { copyFrom(rhs.b, rhs.v); return *this; }
};

class SawtoothUpperBound : public AbstractBound {
public:
  const Pomdp* pomdp;
  int numStates;
  int lastPruneNumPts;
  std::list< BVPair > pts;
  sla::dvector cornerPts;

  SawtoothUpperBound(const MDP* _pomdp);

  // override standard copy ctor and = op to get the right behavior
  void copyFrom(const SawtoothUpperBound& rhs);
  SawtoothUpperBound(const SawtoothUpperBound& rhs) { copyFrom(rhs); }
  SawtoothUpperBound& operator=(const SawtoothUpperBound& rhs) {
    copyFrom(rhs); return *this;
  }

  // performs any computation necessary to initialize the bound
  void initialize(double targetPrecision);

  // returns the bound value at state s
  double getValue(const belief_vector& b) const;

  void maybePrune(void);
  void prune(void);
  int whichCornerPoint(const belief_vector& b) const;
  void addPoint(const belief_vector& b, double val);
  void printToStream(std::ostream& out) const;
};

struct SawtoothWithQBound {
  SawtoothUpperBound V;
#if DO_UB_CACHED_Q
  std::vector<SawtoothUpperBound> Q;
#endif
};

}; // namespace zmdp

#endif // INCBoundFunction_h

/***************************************************************************
 * REVISION HISTORY:
 * $Log: not supported by cvs2svn $
 * Revision 1.16  2006/02/01 01:09:38  trey
 * renamed pomdp namespace -> zmdp
 *
 * Revision 1.15  2005/11/03 17:48:33  trey
 * removed MATRIX_NAMESPACE macro
 *
 * Revision 1.14  2005/10/28 03:52:15  trey
 * simplified license
 *
 * Revision 1.13  2005/10/28 02:55:36  trey
 * added copyright header
 *
 * Revision 1.12  2005/10/27 22:03:40  trey
 * cleaned out some cruft
 *
 * Revision 1.11  2005/10/21 20:20:09  trey
 * added namespace zmdp
 *
 * Revision 1.10  2005/03/28 18:14:35  trey
 * renamed updateAsSafety to useSafetyUpdate
 *
 * Revision 1.9  2005/03/25 21:43:26  trey
 * added updateAsSafety flag in BoundFunction and AlphaList, made some FocusedPomdp functions take a bound as an argument
 *
 * Revision 1.8  2005/03/11 19:24:35  trey
 * switched from hash_map to list representation
 *
 * Revision 1.7  2005/02/08 23:55:47  trey
 * updated to work when alpha_vector = cvector
 *
 * Revision 1.6  2005/02/06 16:41:30  trey
 * removed broken upperBoundInternal2 and added #if USE_CPLEX around upperBoundInternal1
 *
 * Revision 1.5  2005/02/04 21:03:40  trey
 * added upperBoundInternal3 and timing code for lp optimization
 *
 * Revision 1.4  2005/01/28 03:26:24  trey
 * removed spurious argument from resize() call
 *
 * Revision 1.3  2005/01/17 19:34:30  trey
 * added some test code for comparing upper bound mechanisms
 *
 * Revision 1.2  2004/11/24 20:51:32  trey
 * changed #include ValueFunction to #include AlphaList
 *
 * Revision 1.1.1.1  2004/11/09 16:18:56  trey
 * imported hsvi into new repository
 *
 * Revision 1.8  2003/09/17 20:54:24  trey
 * seeing good performance on tagAvoid (but some mods since run started...)
 *
 * Revision 1.7  2003/09/16 00:57:01  trey
 * lots of performance tuning, mostly fixed rising upper bound problem
 *
 * Revision 1.6  2003/09/11 01:46:41  trey
 * completed conversion to compressed matrices
 *
 * Revision 1.5  2003/07/18 16:07:59  trey
 * finished implementing Q caching
 *
 * Revision 1.4  2003/07/17 22:19:52  trey
 * fixed problems with timing early in the solution run, added upperBoundQ
 *
 * Revision 1.3  2003/07/17 18:50:40  trey
 * did prep for caching upper bound Q values
 *
 * Revision 1.2  2003/07/16 16:06:41  trey
 * alpha vectors now tagged with action, tweaked interface to LP solver
 *
 * Revision 1.1  2003/06/26 15:41:19  trey
 * C++ version of pomdp solver functional
 *
 *
 ***************************************************************************/