/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   Copyright (c) 2012-2014 The plumed team
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
#ifndef __PLUMED_reference_RMSDBase_h
#define __PLUMED_reference_RMSDBase_h

#include <vector>
#include <string>
#include "SingleDomainRMSD.h"

namespace PLMD {

class Pbc;

class RMSDBase : public SingleDomainRMSD {
// this is to avoid warnings:
  using SingleDomainRMSD::calc;
public:
  RMSDBase( const ReferenceConfigurationOptions& ro );
  double calc( const std::vector<Vector>& pos, const Pbc& pbc, const bool& squared );
  double calculate( const std::vector<Vector>& pos, const bool& squared );
  virtual double calc( const std::vector<Vector>& pos, const bool& squared )=0;
};

}
#endif
