/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef DISLOCATIONGRADIENTENERGY_H
#define DISLOCATIONGRADIENTENERGY_H

#include "TotalFreeEnergyBase.h"

//Forward Declarations
class DislocationGradientEnergy;

template<>
InputParameters validParams<DislocationGradientEnergy>();

/**
 * Total free energy for PFM dislocation model(both the bulk and gradient parts), where the bulk free energy has been defined in a material and called f_name
 */
class DislocationGradientEnergy : public TotalFreeEnergyBase
{
public:
  DislocationGradientEnergy(const InputParameters & parameters);

protected:
  virtual Real computeValue();

  /// Gradient interface free energy coefficients
  std::vector<const MaterialProperty<Real> *> _kappas;

  /**
   * normal direction of the slip plane.
   */
  RealGradient _normal;
};

#endif //DISLOCATIONGRADIENTENERGY_H
