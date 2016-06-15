/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "DislocationGradientEnergy.h"

template<>
InputParameters validParams<DislocationGradientEnergy>()
{
  InputParameters params = validParams<FunctionMaterialBase>();
  params.addClassDescription("The gradient energy contributions for a dislocation model.");
  params.addRequiredCoupledVar("args", "Arguments of the gradient energy function");
  return params;
}

DislocationGradientEnergy::DislocationGradientEnergy(const InputParameters & parameters) :
    FunctionMaterialBase(parameters),
    _grad_args(coupledGradient("args"))
{
}

Real
DislocationGradientEnergy::computeF()
{
  return _grad_args[_qp] * _grad_args[_qp];
}
