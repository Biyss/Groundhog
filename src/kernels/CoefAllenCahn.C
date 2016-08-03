/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "CoefAllenCahn.h"

template<>
InputParameters validParams<CoefAllenCahn>()
{
  InputParameters params = ACBulk<Real>::validParams();
  params.addClassDescription("Allen-Cahn Kernel that uses a DerivativeMaterial Free Energy");
  params.addRequiredParam<MaterialPropertyName>("f_name", "Base name of the free energy function F defined in a DerivativeParsedMaterial");
  params.addParam<Real>("coef", 0.1, "small value to scale the dislocaiton energy");
  return params;
}

CoefAllenCahn::CoefAllenCahn(const InputParameters & parameters) :
    ACBulk<Real>(parameters),
    _nvar(_coupled_moose_vars.size()),
    _dFdEta(getMaterialPropertyDerivative<Real>("f_name", _var.name())),
    _d2FdEta2(getMaterialPropertyDerivative<Real>("f_name", _var.name(), _var.name())),
    _d2FdEtadarg(_nvar),
    _coef(getParam<Real>("coef"))
{
  // Iterate over all coupled variables
  for (unsigned int i = 0; i < _nvar; ++i)
    _d2FdEtadarg[i] = &getMaterialPropertyDerivative<Real>("f_name", _var.name(), _coupled_moose_vars[i]->name());
}

void
CoefAllenCahn::initialSetup()
{
  ACBulk<Real>::initialSetup();
  validateNonlinearCoupling<Real>("f_name");
  validateDerivativeMaterialPropertyBase<Real>("f_name");
}

Real
CoefAllenCahn::computeDFDOP(PFFunctionType type)
{
  switch (type)
  {
    case Residual:
      return _coef * _dFdEta[_qp];

    case Jacobian:
      return _coef * _d2FdEta2[_qp] * _phi[_j][_qp];
  }

  mooseError("Internal error");
}

Real
CoefAllenCahn::computeQpOffDiagJacobian(unsigned int jvar)
{
  // get the coupled variable jvar is referring to
  unsigned int cvar;
  if (!mapJvarToCvar(jvar, cvar))
    return 0.0;

  return ACBulk<Real>::computeQpOffDiagJacobian(jvar) +
         _coef * _L[_qp] * (*_d2FdEtadarg[cvar])[_qp] * _phi[_j][_qp] * _test[_i][_qp];
}
