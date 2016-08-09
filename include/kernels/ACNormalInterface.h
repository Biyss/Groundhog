/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef ACNORMALINTERFACE_H
#define ACNORMALINTERFACE_H

#include "Kernel.h"

class ACNormalInterface;

template<>
InputParameters validParams<ACNormalInterface>();

/**
 * Compute the Allen-Cahn gradient term, but has a dependence on plane normal direction
 */
class ACNormalInterface : public Kernel
{
public:
  ACNormalInterface(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:

  /// Mobility
  const MaterialProperty<Real> & _L;
  /// Interfacial parameter
  const MaterialProperty<Real> & _kappa_op;

  /**
   * normal direction of the slip plane.
   */
  RealGradient _normal;

  Real _coef;
  /**
   * Class variables to hold the components of plane normal coming from the input parameters.
   */
  Real _n1;
  Real _n2;
  Real _n3;
};

#endif //ACNORMALINTERFACE_H
