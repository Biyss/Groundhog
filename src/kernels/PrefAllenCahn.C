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
  InputParameters params = PrefAllenCahnBase<Real>::validParams();
  params.addClassDescription("Allen-Cahn Kernel that uses a DerivativeMaterial Free Energy");
  params.addRequiredParam<MaterialPropertyName>("f_name", "Base name of the free energy function F defined in a DerivativeParsedMaterial");
  params.addRequiredParam<RealGradient>("normal", "the slip plane unit normal vector");
  return params;
}

PrefAllenCahn::PrefAllenCahn(const InputParameters & parameters) :
    PrefAllenCahnBase<Real>(parameters),
    _nvar(_coupled_moose_vars.size()),
    _dFdEta(getMaterialPropertyDerivative<Real>("f_name", _var.name())),
    _d2FdEta2(getMaterialPropertyDerivative<Real>("f_name", _var.name(), _var.name())),
    _d2FdEtadarg(_nvar),
    _normal(getParam<RealGradient>("normal"))
{
  // Iterate over all coupled variables
  for (unsigned int i = 0; i < _nvar; ++i)
    _d2FdEtadarg[i] = &getMaterialPropertyDerivative<Real>("f_name", _var.name(), _coupled_moose_vars[i]->name());

    _n1=_normal(0);
    _n2=_normal(1);
    _n3=_normal(2);
}

void
PrefAllenCahn::initialSetup()
{
  PrefAllenCahnBase<Real>::initialSetup();
  validateNonlinearCoupling<Real>("f_name");
  validateDerivativeMaterialPropertyBase<Real>("f_name");
}

Real
PrefAllenCahn::computeDFDOP(PFFunctionType type)
{
  // Gamma is the prefector we introduced: |n x _grad_u|^2/|_grad_u|^2
  Gamma = (_normal.cross(_grad_u[_qp])).norm_sq()/_grad_u[_qp].norm_sq();
  // CapPsi is a vector, which equals to d(|n x _grad_u|^2)/d(_grad_u).
  // and it can be written in the form: matrix times vector (_prePsi * _grad_u)
  RankTwoTensor _prePsi (_n2*_n2 + _n3*_n3, _n1*_n1 + _n3*_n3, _n1*_n1 + _n2*_n2, -1 * _n2 * _n3, -1 * _n1 * _n3, -1 * _n1 * _n2);
  CapPsi = _prePsi * _grad_u[_qp];
  // CapPhi is a vector: CapPsi/|_grad_u|^2 - 2 *|n x (_grad_u|^2/|_grad_u|^4) * _grad_u
  CapPhi = CapPsi/(_grad_u[_qp]*_grad_u[_qp]) - 2 * ((_normal.cross(_grad_u[_qp])).norm_sq()) / ((_grad_u[_qp].norm_sq())*(_grad_u[_qp].norm_sq())) * _grad_u[_qp];

  switch (type)
  {
    case Residual:
      return Gamma * _dFdEta[_qp];

    case Jacobian:
      return Gamma * _d2FdEta2[_qp] * _phi[_j][_qp] + CapPhi * _dFdEta[_qp] * _grad_phi[_j][_qp];
  }

  mooseError("Internal error");
}

// Real
// PrefAllenCahn::computeQpOffDiagJacobian(unsigned int jvar)
// {
//   // get the coupled variable jvar is referring to
//   unsigned int cvar;
//   if (!mapJvarToCvar(jvar, cvar))
//     return 0.0;
//
//     Real Gamma = (_normal.cross(_grad_u[_qp])).norm_sq()/_grad_u[_qp].norm_sq();
//
//   return PrefAllenCahnBase<Real>::computeQpOffDiagJacobian(jvar) +
//          Gamma * _L[_qp] * (*_d2FdEtadarg[cvar])[_qp] * _phi[_j][_qp] * _test[_i][_qp];
// }
