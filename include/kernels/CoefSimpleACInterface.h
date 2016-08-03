/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef COEFSIMPLEACINTERFACE_H
#define COEFSIMPLEACINTERFACE_H

#include "Kernel.h"

class CoefSimpleACInterface;

template<>
InputParameters validParams<CoefSimpleACInterface>();

/**
 * Compute the Allen-Cahn interface term with constant Mobility and Interfacial parameter
 */
class CoefSimpleACInterface : public Kernel
{
public:
  CoefSimpleACInterface(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

  /// Mobility
  const MaterialProperty<Real> & _L;
  /// Interfacial parameter
  const MaterialProperty<Real> & _kappa;

  Real _coef;
};

#endif //COEFSIMPLEACINTERFACE_H
