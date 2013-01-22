/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   Copyright (c) 2012 The plumed team
   (see the PEOPLE file at the root of the distribution for a list of names)

   See http://www.plumed-code.org for more information.

   This file is part of plumed, version 2.0.

   plumed is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   plumed is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with plumed.  If not, see <http://www.gnu.org/licenses/>.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
#include "core/ActionRegister.h"
#include "core/ActionPilot.h"
#include "core/ActionSet.h"
#include "core/PlumedMain.h"

using namespace std;

namespace PLMD{
namespace generic {

//+PLUMEDOC GENERIC DEBUG
/*
  Set some debug options

\par syntax
\verbatim
DEBUG [STRIDE=s] [NOVIRIAL] [logActivity]
\endverbatim
The NOVIRIAL flag switches off (for the entire simulation) the contribution
of virial computed in plumed. The logActivity keyword writes in the log
the list of which objects are active and which are inactive
as a sequence of + (active) and - (inactive). Logging is done with stride s.
*/
//+ENDPLUMEDOC
class Debug:
  public ActionPilot
{
  bool logActivity;
  bool logRequestedAtoms;
  bool novirial;
public:
  Debug(const ActionOptions&ao);
/// Register all the relevant keywords for the action  
  static void registerKeywords( Keywords& keys );
  void calculate(){};
  void apply();
};

PLUMED_REGISTER_ACTION(Debug,"DEBUG")

void Debug::registerKeywords( Keywords& keys ){
  Action::registerKeywords( keys );
  ActionPilot::registerKeywords(keys);
  keys.add("compulsory","STRIDE","1","the frequency with which this action is to be performed");
  keys.addFlag("logActivity",false,"write in the log which actions are inactive and which are inactive");
  keys.addFlag("logRequestedAtoms",false,"write in the log which atoms have been requested at a given time");
  keys.addFlag("NOVIRIAL",false,"switch off the virial contribution for the entirity of the simulation");
}

Debug::Debug(const ActionOptions&ao):
Action(ao),
ActionPilot(ao),
logActivity(false),
logRequestedAtoms(false),
novirial(false){
  parseFlag("logActivity",logActivity);
  if(logActivity) log.printf("  logging activity\n");
  parseFlag("logRequestedAtoms",logRequestedAtoms);
  if(logRequestedAtoms) log.printf("  logging requested atoms\n");
  parseFlag("NOVIRIAL",novirial);
  if(novirial) log.printf("  Switching off virial contribution\n");
  if(novirial) plumed.novirial=true;
  checkRead();
}

void Debug::apply(){
  if(logActivity){
    const ActionSet&actionSet(plumed.getActionSet());
    int a=0;
    for(ActionSet::const_iterator p=actionSet.begin();p!=actionSet.end();++p){
      if(dynamic_cast<Debug*>(*p))continue;
      if((*p)->isActive()) a++;
    };
    if(a>0){
      log.printf("activity at step %i: ",getStep());
      for(ActionSet::const_iterator p=actionSet.begin();p!=actionSet.end();++p){
        if(dynamic_cast<Debug*>(*p))continue;
        if((*p)->isActive()) log.printf("+");
        else                 log.printf("-");
      };
      log.printf("\n");
    };
  };
  if(logRequestedAtoms){
    log.printf("requested atoms at step %i: ",getStep());
    int* l;
    int n;
    plumed.cmd("createFullList",&n);
    plumed.cmd("getFullList",&l);
    for(int i=0;i<n;i++) log.printf(" %d",l[i]);
    log.printf("\n");
    plumed.cmd("clearFullList");
  }

}

}
}
