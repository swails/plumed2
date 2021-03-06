/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   Copyright (c) 2013,2014 The plumed team
   (see the PEOPLE file at the root of the distribution for a list of names)

   See http://www.plumed-code.org for more information.

   This file is part of plumed, version 2.

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
#include "MultiColvar.h"
#include "tools/Torsion.h"
#include "core/ActionRegister.h"

#include <string>
#include <cmath>

using namespace std;

namespace PLMD{
namespace multicolvar{

//+PLUMEDOC MCOLVAR TORSIONS
/*
Calculate whether or not a set of torsional angles are within a particular range.

\par Examples

The following provides an example of the input for the torsions command

\verbatim
TORSIONS ...
ATOMS1=168,170,172,188
ATOMS2=170,172,188,190
ATOMS3=188,190,192,230 
LABEL=ab
... TORSIONS
PRINT ARG=ab.* FILE=colvar STRIDE=10
\endverbatim

Writing out the atoms involved in all the torsions in this way can be rather tedious. Thankfully if you are working with protein you
can avoid this by using the \ref MOLINFO command.  PLUMED uses the pdb file that you provide to this command to learn 
about the topology of the protein molecule.  This means that you can specify torsion angles using the following syntax:

\verbatim
MOLINFO MOLTYPE=protein STRUCTURE=myprotein.pdb
TORSIONS ...
ATOMS1=@phi-3
ATOMS2=@psi-3
ATOMS3=@phi-4
LABEL=ab
... TORSIONS 
PRINT ARG=ab FILE=colvar STRIDE=10
\endverbatim

Here, \@phi-3 tells plumed that you would like to calculate the \f$\phi\f$ angle in the third residue of the protein.  
Similarly \@psi-4 tells plumed that you want to calculate the \f$\psi\f$ angle of the 4th residue of the protein.


*/
//+ENDPLUMEDOC

class Torsions : public MultiColvar {
public:
  static void registerKeywords( Keywords& keys );
  Torsions(const ActionOptions&);
  virtual double compute();
  bool isPeriodic(){ return true; }
  void retrieveDomain( std::string& min, std::string& max ){ min="-pi"; max="pi"; }
  Vector getCentralAtom();  
};

PLUMED_REGISTER_ACTION(Torsions,"TORSIONS")

void Torsions::registerKeywords( Keywords& keys ){
  MultiColvar::registerKeywords( keys );
  keys.use("ATOMS"); keys.use("BETWEEN"); keys.use("HISTOGRAM");
}

Torsions::Torsions(const ActionOptions&ao):
PLUMED_MULTICOLVAR_INIT(ao)
{
  // Read in the atoms
  int natoms=4; readAtoms( natoms );
  // Read in the vessels
  readVesselKeywords();  
  // And check everything has been read in correctly
  checkRead();
}

double Torsions::compute(){
  Vector d0,d1,d2;
  d0=getSeparation(getPosition(1),getPosition(0));
  d1=getSeparation(getPosition(2),getPosition(1));
  d2=getSeparation(getPosition(3),getPosition(2));

  Vector dd0,dd1,dd2; PLMD::Torsion t;
  double value  = t.compute(d0,d1,d2,dd0,dd1,dd2);

  addAtomsDerivatives(0,dd0);
  addAtomsDerivatives(1,dd1-dd0);
  addAtomsDerivatives(2,dd2-dd1);
  addAtomsDerivatives(3,-dd2);

  addBoxDerivatives  (-(extProduct(d0,dd0)+extProduct(d1,dd1)+extProduct(d2,dd2)));

  return value;
}

Vector Torsions::getCentralAtom(){
   addCentralAtomDerivatives( 1, 0.5*Tensor::identity() );
   addCentralAtomDerivatives( 2, 0.5*Tensor::identity() );
   return 0.5*( getPosition(1) + getPosition(2) );
}

}
}
