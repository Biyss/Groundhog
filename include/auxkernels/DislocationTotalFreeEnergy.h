/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef DISLOCATIONTOTALFREEENERGY_H
#define DISLOCATIONTOTALFREEENERGY_H

#include "TotalFreeEnergyBase.h"

//Forward Declarations
class DislocationTotalFreeEnergy;

template<>
InputParameters validParams<DislocationTotalFreeEnergy>();

/**
 * Total free energy for PFM dislocation model(both the bulk and gradient parts), where the bulk free energy has been defined in a material and called f_name
 */
class DislocationTotalFreeEnergy : public TotalFreeEnergyBase
{
public:
  DislocationTotalFreeEnergy(const InputParameters & parameters);

protected:
  virtual Real computeValue();

  /// Bulk free energy material property
  const MaterialProperty<Real> & _F;

  /// Gradient interface free energy coefficients
  std::vector<const MaterialProperty<Real> *> _kappas;

  /**
   * normal direction of the slip plane.
   */
  RealGradient _normal;
};

#endif //DISLOCATIONTOTALFREEENERGY_H
