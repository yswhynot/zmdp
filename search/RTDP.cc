/********** tell emacs we use -*- c++ -*- style comments *******************
 $Revision: 1.11 $  $Author: trey $  $Date: 2006-04-06 04:14:11 $
   
 @file    RTDP.cc
 @brief   Implementation of Barto, Bradke, and Singh's RTDP algorithm.

 Copyright (c) 2006, Trey Smith. All rights reserved.

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

/**********************************************************************
  This is my implementation of the RTDP algorithm, based on the paper

    "Learning to Act Using Real-Time Dynamic Programming."
    A. Barto, S. Bradke, and S. Singh.
    Artificial Intelligence 72(1-2): 81-138. 1995.

  Inevitably they could not include all the details of the algorithm in
  their paper, so it is possible that my implementation differs from
  theirs in important ways.  They have not signed off on this
  implementation: use at your own risk.  (And please inform me if you
  find any errors!)

  -Trey Smith, Feb. 2006
 **********************************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

#include <iostream>
#include <fstream>
#include <queue>

#include "zmdpCommonDefs.h"
#include "zmdpCommonTime.h"
#include "MatrixUtils.h"
#include "Pomdp.h"
#include "RTDP.h"

using namespace std;
using namespace sla;
using namespace MatrixUtils;

namespace zmdp {

RTDP::RTDP(AbstractBound* _initUpperBound) :
  RTDPCore(NULL, _initUpperBound)
{}

void RTDP::trialRecurse(MDPNode& cn, int depth)
{
  // check for termination
  if (cn.isTerminal) {
#if USE_DEBUG_PRINT
    printf("trialRecurse: depth=%d ubVal=%g terminal node (terminating)\n",
	   depth, cn.ubVal);
#endif
    return;
  }

  // cached Q values must be up to date for subsequent calls
  int maxUBAction;
  bounds->update(cn, &maxUBAction);

  int simulatedOutcome = bounds->getSimulatedOutcome(cn, maxUBAction);

#if USE_DEBUG_PRINT
  printf("  trialRecurse: depth=%d a=%d o=%d ubVal=%g\n",
	 depth, maxUBAction, simulatedOutcome, cn.ubVal);
  printf("  trialRecurse: s=%s\n", sparseRep(cn.s).c_str());
#endif

  // recurse to successor
  trialRecurse(cn.getNextState(maxUBAction, simulatedOutcome), depth+1);

  bounds->update(cn, NULL);
}

bool RTDP::doTrial(MDPNode& cn)
{
#if USE_DEBUG_PRINT
  printf("-*- doTrial: trial %d\n", (numTrials+1));
#endif

  trialRecurse(cn, 0);
  numTrials++;

  return false;
}

}; // namespace zmdp

/***************************************************************************
 * REVISION HISTORY:
 * $Log: not supported by cvs2svn $
 * Revision 1.10  2006/04/03 21:39:24  trey
 * updated to use IncrementalBounds
 *
 * Revision 1.9  2006/02/27 20:12:37  trey
 * cleaned up meta-information in header
 *
 * Revision 1.8  2006/02/19 18:33:47  trey
 * targetPrecision now stared as a field rather than passed around recursively
 *
 * Revision 1.7  2006/02/14 19:34:43  trey
 * now use targetPrecision properly
 *
 * Revision 1.6  2006/02/13 20:20:33  trey
 * refactored some common code from RTDP and LRTDP
 *
 * Revision 1.5  2006/02/13 19:08:49  trey
 * moved numBackups tracking code for better flexibility
 *
 * Revision 1.4  2006/02/11 22:38:10  trey
 * moved much of the RTDP implementation into RTDPCore, where it can be shared by many RTDP variants
 *
 * Revision 1.3  2006/02/10 20:14:33  trey
 * standardized fields in bounds.plot
 *
 * Revision 1.2  2006/02/10 19:33:32  trey
 * chooseAction() now relies on upper bound as it should (since the lower bound is not even calculated in vanilla RTDP!
 *
 * Revision 1.1  2006/02/09 21:59:04  trey
 * initial check-in
 *
 *
 ***************************************************************************/
