/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "PrefACBulk.h"

template<>
InputParameters validParams<PrefACBulk>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Bulk energy for Allen-Cahn Kernel with constant Mobility and prefector");
  params.addParam<MaterialPropertyName>("mob_name", "L", "The mobility used with the kernel");
  params.addParam<MaterialPropertyName>("f_name", "F"," Base name of the free energy function");
  params.addRequiredParam<RealGradient>("normal", "the slip plane unit normal vector");
  return params;
}

PrefACBulk::PrefACBulk(const InputParameters & parameters) :
    DerivativeMaterialInterface<Kernel>(parameters),
    _L(getMaterialProperty<Real>("mob_name")),
    _F(getMaterialProperty<Real>("f_name")),
    _dFdEta(getMaterialPropertyDerivative<Real>("f_name", _var.name())),
    _normal(getParam<RealGradient>("normal"))
{
  _n1=_normal(0);
  _n2=_normal(1);
  _n3=_normal(2);
}

RealGradient
PrefACBulk::computeCapPsi()
{
  // CapPsi is a vector, which equals to d(|n x _grad_u|^2)/d(_grad_u).
  // and it can be written in the form: matrix times vector (_prePsi * _grad_u)
  RankTwoTensor _prePsi (_n2*_n2 + _n3*_n3, _n1*_n1 + _n3*_n3, _n1*_n1 + _n2*_n2, -1 * _n2 * _n3, -1 * _n1 * _n3, -1 * _n1 * _n2);
  RealGradient CapPsi = _prePsi * _grad_u[_qp];

  return CapPsi;
}

RealGradient
PrefACBulk::computeCapPhi()
{
  RealGradient normalcross = _normal.cross(_grad_u[_qp]);
  // CapPhi is a vector: CapPsi/|_grad_u|^2 - 2 *|n x (_grad_u|^2/|_grad_u|^4) * _grad_u
  RealGradient CapPhi = PrefACBulk::computeCapPsi()/_grad_u[_qp].norm_sq() - 2 * normalcross.norm_sq() * _grad_u[_qp] / (_grad_u[_qp].norm_sq()*_grad_u[_qp].norm_sq());

  return CapPhi;
}

Real
PrefACBulk::computeQpResidual()
{
  Real value = 0.0;

  if(_grad_u[_qp].norm() <= 1e-4)
    value = 0;
  else if(_grad_u[_qp].norm() > 1e-4)
    value = _L[_qp] * PrefACBulk::computeCapPhi() * _F[_qp] * _grad_test[_i][_qp];

  return value;
}

Real
PrefACBulk::computeQpJacobian()
{
  Real value = 0.0;

  if(_grad_u[_qp].norm() <= 1e-4)
    value = 0;
  else if(_grad_u[_qp].norm() > 1e-4)
  {
    RealGradient normalcross = _normal.cross(_grad_u[_qp]);
    RankTwoTensor _prePsi (_n2*_n2 + _n3*_n3, _n1*_n1 + _n3*_n3, _n1*_n1 + _n2*_n2, -1 * _n2 * _n3, -1 * _n1 * _n3, -1 * _n1 * _n2);
    // part_1 is the derivative of free energy equation F respect to u[_qp]
    Real part_1 = _L[_qp] * PrefACBulk::computeCapPhi() * _dFdEta[_qp] * _phi[_j][_qp] * _grad_test[_i][_qp];
    // part_2 is the derivative of CapPhi respect to u[_qp], use the chain rule:
    // d(CapPhi)/d(u[_qp]) = d(CapPhi)/d(_grad_u[_qp]) * _grad_phi[_qp]
    // d(CapPhi)/d(_grad_u[_qp]) = (CapPsi * _grad_u[_qp] - 3 * |n x _grad_u|^2)/|_grad_u[_qp]|^4
    Real part_2_1_1 = _L[_qp] * _F[_qp] * (_prePsi / _grad_u[_qp].norm_sq()) * _grad_phi[_j][_qp] * _grad_test[_i][_qp];
    Real part_2_1_2 = _L[_qp] * _F[_qp] * (-2 * _prePsi * _grad_u[_qp] *_grad_u[_qp] / (_grad_u[_qp].norm_sq() * _grad_u[_qp].norm_sq())) * _grad_phi[_j][_qp] * _grad_test[_i][_qp];
    Real part_2_2 = _L[_qp] * _F[_qp] *  (-2 * (PrefACBulk::computeCapPsi()*_grad_u[_qp]-3*normalcross.norm_sq())/(_grad_u[_qp].norm_sq()*_grad_u[_qp].norm_sq())) * _grad_phi[_j][_qp] * _grad_test[_i][_qp];
    value = part_1 + part_2_1_1 + part_2_1_2 + part_2_2;
  }
  return value;
}
