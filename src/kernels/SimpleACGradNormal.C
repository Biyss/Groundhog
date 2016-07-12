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
  return params;
}

SimpleACGradNormal::SimpleACGradNormal(const InputParameters & parameters) :
  Kernel(parameters),
    _L(getMaterialProperty<Real>("mob_name")),
    _kappa_op(getMaterialProperty<Real>("kappa_name")),
    _normal(getParam<RealVectorValue>("normal"))
{
  _n1=_normal(0);
  _n2=_normal(1);
  _n3=_normal(2);
}

Real
SimpleACGradNormal::computeQpResidual()
{
  Real Sum = 0.0;
  Sum = (_n3*(_n3*_grad_u[_qp](0)-_n1*_grad_u[_qp](2)) - _n2*(_n1*_grad_u[_qp](1)-_n2*_grad_u[_qp](0))) * _grad_test[_i][_qp](0)
      + (_n1*(_n1*_grad_u[_qp](1)-_n2*_grad_u[_qp](0)) - _n3*(_n2*_grad_u[_qp](2)-_n3*_grad_u[_qp](1))) * _grad_test[_i][_qp](1)
      + (_n2*(_n2*_grad_u[_qp](2)-_n3*_grad_u[_qp](1)) - _n1*(_n3*_grad_u[_qp](0)-_n1*_grad_u[_qp](2))) * _grad_test[_i][_qp](2);

  return _kappa_op[_qp] * _L[_qp] * Sum;
}

Real
SimpleACGradNormal::computeQpJacobian()
{
  Real Sum = 0.0;
  Sum = (_n3*(_n3*_grad_phi[_j][_qp](0)-_n1*_grad_phi[_j][_qp](2)) - _n2*(_n1*_grad_phi[_j][_qp](1)-_n2*_grad_phi[_j][_qp](0))) * _grad_test[_i][_qp](0)
      + (_n1*(_n1*_grad_phi[_j][_qp](1)-_n2*_grad_phi[_j][_qp](0)) - _n3*(_n2*_grad_phi[_j][_qp](2)-_n3*_grad_phi[_j][_qp](1))) * _grad_test[_i][_qp](1)
      + (_n2*(_n2*_grad_phi[_j][_qp](2)-_n3*_grad_phi[_j][_qp](1)) - _n1*(_n3*_grad_phi[_j][_qp](0)-_n1*_grad_phi[_j][_qp](2))) * _grad_test[_i][_qp](2);

  return _kappa_op[_qp] * _L[_qp] * Sum;
}
