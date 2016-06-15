/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef DISLOCATIONGRADIENTENERGY_H
#define DISLOCATIONGRADIENTENERGY_H

#include "FunctionMaterialBase.h"
#include "MooseEnum.h"

// Forward Declaration
class DislocationGradientEnergy;

template<>
InputParameters validParams<DislocationGradientEnergy>();

/**
 * Material class to compute the gradient energy for dislocation model
 */
class DislocationGradientEnergy : public FunctionMaterialBase
{
public:
  DislocationGradientEnergy(const InputParameters & parameters);

protected:
  virtual Real computeF();
  // virtual Real computeDF(unsigned int i_var);

  // coupled variable gradient
  const VariableGradient & _grad_args;

  // /// Variables for second order derivatives
  // const VariableSecond & _second_args;
};

#endif //ELASTICENERGYMATERIAL_H
