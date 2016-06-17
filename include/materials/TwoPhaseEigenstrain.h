/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef TWOPHASEEIGENSTRAIN_H
#define TWOPHASEEIGENSTRAIN_H

#include "ComputeStressFreeStrainBase.h"

class TwoPhaseEigenstrain;

template<>
InputParameters validParams<TwoPhaseEigenstrain>();

/**
 * TwoPhaseEigenstrain computes an Eigenstrain based on two parts
 * prefector_1 * eigenstrain_tensor_1 + prefactor_2 * eigenstrain_tensor_2
 */
class TwoPhaseEigenstrain : public ComputeStressFreeStrainBase
{
public:
  TwoPhaseEigenstrain(const InputParameters & parameters);

protected:
  virtual void computeQpStressFreeStrain();

  const MaterialProperty<Real> & _prefactor_1;
  const MaterialProperty<Real> & _prefactor_2;

  RankTwoTensor _eigen_base_tensor_1;
  RankTwoTensor _eigen_base_tensor_2;
};

#endif
