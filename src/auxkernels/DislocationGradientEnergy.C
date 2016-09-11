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
  InputParameters params = validParams<TotalFreeEnergyBase>();
  params.addClassDescription("Total free energy (both the bulk and gradient parts), where the bulk free energy has been defined in a material");
  params.addParam< std::vector<MaterialPropertyName> >("kappa_names", std::vector<MaterialPropertyName>(), "Vector of kappa names corresponding to each variable name in interfacial_vars in the same order.");
  params.addRequiredParam<RealGradient>("normal", "the slip plane unit normal vector");
  return params;
}

DislocationGradientEnergy::DislocationGradientEnergy(const InputParameters & parameters) :
    TotalFreeEnergyBase(parameters),
    _kappas(_nkappas),
    _normal(getParam<RealGradient>("normal"))
{
  //Error check to ensure size of interfacial_vars is the same as kappa_names
  if (_nvars != _nkappas)
    mooseError("Size of interfacial_vars is not equal to the size of kappa_names in TotalFreeEnergy");

  // Assign kappa values
  for (unsigned int i = 0; i < _nkappas; ++i)
    _kappas[i] = &getMaterialPropertyByName<Real>(_kappa_names[i]);
}

Real
DislocationGradientEnergy::computeValue()
{
  Real total_grad_energy = 0.0;

  // Calculate interfacial energy of each variable
  for (unsigned int i = 0; i < _nvars; ++i)
  {
    RealGradient crossprod = (_normal.cross((*_grad_vars[i])[_qp]));
    total_grad_energy += (*_kappas[i])[_qp] / 2.0 * crossprod.norm_sq();
  }

  return total_grad_energy;
}
