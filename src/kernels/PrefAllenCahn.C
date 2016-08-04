/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "PrefAllenCahn.h"

template<>
InputParameters validParams<PrefAllenCahn>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Allen-Cahn Kernel that uses a DerivativeMaterial Free Energy");
  params.addParam<MaterialPropertyName>("mob_name", "L", "The mobility used with the kernel");
  params.addRequiredParam<MaterialPropertyName>("f_name", "Base name of the free energy function F defined in a DerivativeParsedMaterial");
  params.addRequiredParam<RealGradient>("normal", "the slip plane unit normal vector");
  return params;
}

PrefAllenCahn::PrefAllenCahn(const InputParameters & parameters) :
    DerivativeMaterialInterface<Kernel>(parameters),
    _L(getMaterialProperty<Real>("mob_name")),
    _dFdEta(getMaterialPropertyDerivative<Real>("f_name", _var.name())),
    _d2FdEta2(getMaterialPropertyDerivative<Real>("f_name", _var.name(), _var.name())),
    _normal(getParam<RealGradient>("normal"))
{
    _n1=_normal(0);
    _n2=_normal(1);
    _n3=_normal(2);
}

Real
PrefAllenCahn::computeQpResidual()
{
  if(_grad_u[_qp].norm() <= 1e-2)
    Real Gamma = 1;
  else if(_grad_u[_qp].norm() > 1e-2)
    // Gamma is the prefector we introduced: |n x _grad_u|^2/|_grad_u|^2
    Real Gamma = (_normal.cross(_grad_u[_qp])).norm_sq()/_grad_u[_qp].norm_sq();

  return _L[_qp] * Gamma * _dFdEta[_qp] * _test[_i][_qp];
}

Real
PrefAllenCahn::computeQpJacobian()
{
  Real value = 0.0;
  
  if(_grad_u[_qp].norm() <= 1e-2)
    value = _L[_qp] * _d2FdEta2[_qp] * _phi[_j][_qp] * _test[_i][_qp];
  else if(_grad_u[_qp].norm() > 1e-2)
  {
    // Gamma is the prefector we introduced: |n x _grad_u|^2/|_grad_u|^2
    Real Gamma = (_normal.cross(_grad_u[_qp])).norm_sq()/_grad_u[_qp].norm_sq();
    // CapPsi is a vector, which equals to d(|n x _grad_u|^2)/d(_grad_u).
    // and it can be written in the form: matrix times vector (_prePsi * _grad_u)
    RankTwoTensor _prePsi (_n2*_n2 + _n3*_n3, _n1*_n1 + _n3*_n3, _n1*_n1 + _n2*_n2, -1 * _n2 * _n3, -1 * _n1 * _n3, -1 * _n1 * _n2);
    RealGradient CapPsi = _prePsi * _grad_u[_qp];
    // CapPhi is a vector: CapPsi/|_grad_u|^2 - 2 *|n x (_grad_u|^2/|_grad_u|^4) * _grad_u
    RealGradient CapPhi = CapPsi/(_grad_u[_qp]*_grad_u[_qp]) - 2 * ((_normal.cross(_grad_u[_qp])).norm_sq()) / ((_grad_u[_qp].norm_sq())*(_grad_u[_qp].norm_sq())) * _grad_u[_qp];
    value = _L[_qp] * (Gamma * _d2FdEta2[_qp] * _phi[_j][_qp] + CapPhi * _dFdEta[_qp] * _grad_phi[_j][_qp]) * _test[_i][_qp];
  }

  return value;
}
