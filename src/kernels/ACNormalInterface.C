/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "ACNormalInterface.h"

template<>
InputParameters validParams<ACNormalInterface>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Compute the Allen-Cahn gradient term, but has a dependence on plane normal direction");
  params.addParam<MaterialPropertyName>("kappa_name", "kappa_op", "The kappa used with the kernel, should be constant value");
  params.addParam<MaterialPropertyName>("mob_name", "L", "The mobility used with the kernel, should be constant value");
  params.addRequiredParam<RealGradient>("normal", "the slip plane unit normal vector");
  params.addParam<Real>("coef", 1.0, "Coefficient");
  return params;
}

ACNormalInterface::ACNormalInterface(const InputParameters & parameters) :
  Kernel(parameters),
    _L(getMaterialProperty<Real>("mob_name")),
    _kappa_op(getMaterialProperty<Real>("kappa_name")),
    _normal(getParam<RealGradient>("normal")),
    _coef(getParam<Real>("coef"))
{
  _n1=_normal(0);
  _n2=_normal(1);
  _n3=_normal(2);
}

Real
ACNormalInterface::computeQpResidual()
{
  return _coef * _kappa_op[_qp] * _L[_qp] * _normal * _grad_test[_i][_qp];
}

Real
ACNormalInterface::computeQpJacobian()
{
  return 0.0;
}
