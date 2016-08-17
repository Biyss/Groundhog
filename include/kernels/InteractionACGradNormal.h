/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef INTERACTIONACGRADNORMAL_H
#define INTERACTIONACGRADNORMAL_H

#include "Kernel.h"
#include "RankTwoTensor.h"
#include "KernelGrad.h"
#include "JvarMapInterface.h"

class InteractionACGradNormal;

template<>
InputParameters validParams<InteractionACGradNormal>();

/**
 * Compute the Allen-Cahn gradient term, but has a dependence on plane normal direction
 */
class InteractionACGradNormal : public Kernel
{
public:
  InteractionACGradNormal(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  /// Gradients for coupled variable
  const VariableGradient & _grad_arg;

  /// Mobility
  const MaterialProperty<Real> & _L;
  /// Interfacial parameter
  const MaterialProperty<Real> & _kappa_op;

  /**
   * normal direction of the slip plane.
   */
  RealVectorValue _normal_var;
  RealVectorValue _normal_arg;

  /**
   * Class variables to hold the components of plane normal coming from the input parameters.
   */
  Real _n1;
  Real _n2;
  Real _n3;
};

#endif //INTERACTIONACGRADNORMAL_H
