/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef PREFACBULK_H
#define PREFACBULK_H

#include "Kernel.h"
#include "RankTwoTensor.h"
#include "DerivativeMaterialInterface.h"
#include "MaterialPropertyInterface.h"

class PrefACBulk;

template<>
InputParameters validParams<PrefACBulk>();

/**
 * Compute the Allen-Cahn interface term with constant Mobility and Interfacial parameter
 */
class PrefACBulk : public DerivativeMaterialInterface<Kernel>
{
public:
  PrefACBulk(const InputParameters & parameters);

protected:
  virtual RealGradient computeCapPsi();
  virtual RealGradient computeCapPhi();
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

  /// Mobility
  const MaterialProperty<Real> & _L;
  /// Bulk free energy material property
  const MaterialProperty<Real> & _F;
  const MaterialProperty<Real> & _dFdEta;

  RealGradient _normal;

  /**
   * Class variables to hold the components of plane normal coming from the input parameters.
   */
  Real _n1;
  Real _n2;
  Real _n3;
};

#endif //PREFACBULK_H
