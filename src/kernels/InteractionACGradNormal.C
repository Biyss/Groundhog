/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "InteractionACGradNormal.h"

template<>
InputParameters validParams<InteractionACGradNormal>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Compute the Allen-Cahn gradient term, but has a dependence on plane normal direction");
  params.addParam<MaterialPropertyName>("kappa_name", "kappa_op", "The kappa used with the kernel, should be constant value");
  params.addParam<MaterialPropertyName>("mob_name", "L", "The mobility used with the kernel, should be constant value");
  params.addRequiredParam<RealVectorValue>("normal_var", "the slip plane unit normal vector");
  params.addCoupledVar("arg", "coupled argument");
  params.addParam<RealVectorValue>("normal_arg", "the slip plane unit normal vector");
  return params;
}

InteractionACGradNormal::InteractionACGradNormal(const InputParameters & parameters) :
  Kernel(parameters),
  _grad_arg(coupledGradient("arg")),
  _L(getMaterialProperty<Real>("mob_name")),
  _kappa_op(getMaterialProperty<Real>("kappa_name")),
  _normal_var(getParam<RealVectorValue>("normal_var")),
  _normal_arg(getParam<RealVectorValue>("normal_arg"))
{
  _n1=_normal_var(0);
  _n2=_normal_var(1);
  _n3=_normal_var(2);
}

Real
InteractionACGradNormal::computeQpResidual()
{
  RankTwoTensor _preN (0, -_n3, _n2, _n3, 0, -_n1, -_n2, _n1, 0);
  RealVectorValue _arg_cross = _normal_arg.cross(_grad_arg[_qp]);

  return _kappa_op[_qp] * _L[_qp] * _preN * _arg_cross * _grad_test[_i][_qp];
}

Real
InteractionACGradNormal::computeQpJacobian()
{
  return 0.0;
}
