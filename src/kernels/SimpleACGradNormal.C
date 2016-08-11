/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "SimpleACGradNormal.h"

template<>
InputParameters validParams<SimpleACGradNormal>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Compute the Allen-Cahn gradient term, but has a dependence on plane normal direction");
  params.addParam<MaterialPropertyName>("kappa_name", "kappa_op", "The kappa used with the kernel, should be constant value");
  params.addParam<MaterialPropertyName>("mob_name", "L", "The mobility used with the kernel, should be constant value");
  params.addRequiredParam<RealVectorValue>("normal", "the slip plane unit normal vector");
  params.addParam<Real>("coef", 1.0, "Coefficient");
  return params;
}

SimpleACGradNormal::SimpleACGradNormal(const InputParameters & parameters) :
  Kernel(parameters),
    _L(getMaterialProperty<Real>("mob_name")),
    _kappa_op(getMaterialProperty<Real>("kappa_name")),
    _normal(getParam<RealVectorValue>("normal")),
    _coef(getParam<Real>("coef"))
{
  _n1=_normal(0);
  _n2=_normal(1);
  _n3=_normal(2);
}

Real
SimpleACGradNormal::computeQpResidual()
{
  RankTwoTensor _prePsi (_n2*_n2 + _n3*_n3, _n1*_n1 + _n3*_n3, _n1*_n1 + _n2*_n2, -1 * _n2 * _n3, -1 * _n1 * _n3, -1 * _n1 * _n2);

  return _coef * _kappa_op[_qp] * _L[_qp] * _prePsi * _grad_u[_qp] * _grad_test[_i][_qp];
}

Real
SimpleACGradNormal::computeQpJacobian()
{
  RankTwoTensor _prePsi (_n2*_n2 + _n3*_n3, _n1*_n1 + _n3*_n3, _n1*_n1 + _n2*_n2, -1 * _n2 * _n3, -1 * _n1 * _n3, -1 * _n1 * _n2);

  return _coef * _kappa_op[_qp] * _L[_qp] * _prePsi * _grad_phi[_j][_qp] * _grad_test[_i][_qp];
}
