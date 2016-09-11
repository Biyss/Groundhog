/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "CoefSimpleACInterface.h"

template<>
InputParameters validParams<CoefSimpleACInterface>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Gradient energy for Allen-Cahn Kernel with constant Mobility and Interfacial parameter");
  params.addParam<MaterialPropertyName>("mob_name", "L", "The mobility used with the kernel");
  params.addParam<MaterialPropertyName>("kappa_name", "kappa_op", "The kappa used with the kernel");
  params.addParam<Real>("coef", 0.1, "small value to scale the dislocaiton energy");
  return params;
}

CoefSimpleACInterface::CoefSimpleACInterface(const InputParameters & parameters) :
    Kernel(parameters),
    _L(getMaterialProperty<Real>("mob_name")),
    _kappa(getMaterialProperty<Real>("kappa_name")),
    _coef(getParam<Real>("coef"))
{
}

Real
CoefSimpleACInterface::computeQpResidual()
{
  return _coef * _grad_u[_qp] * _kappa[_qp] * _L[_qp] * _grad_test[_i][_qp];
}

Real
CoefSimpleACInterface::computeQpJacobian()
{
  return _coef * _grad_phi[_j][_qp] * _kappa[_qp] * _L[_qp] * _grad_test[_i][_qp];
}
