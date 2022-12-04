#ifndef COMPARENODES_H
#define COMPARENODES_H

#include <iostream>
#include "route_model.h"

class CompareNodes
{
  public:
      bool operator()(RouteModel::Node*, RouteModel::Node*);

};

#endif