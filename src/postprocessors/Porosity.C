/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "Porosity.h"
#include "SubProblem.h"
#include "MooseMesh.h"

template <>
InputParameters
validParams<Porosity>()
{
  InputParameters params = validParams<ElementAverageValue>();
  return params;
}

Porosity::Porosity(const InputParameters & parameters)
  : ElementAverageValue(parameters)
{
}

Real
Porosity::computeQpIntegral()
{
  if (_u[_qp] > 0.7)
    return 1.0;
  else
    return 0.0;
}
