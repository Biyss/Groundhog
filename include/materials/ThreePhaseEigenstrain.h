/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef THREEPHASEEIGENSTRAIN_H
#define THREEPHASEEIGENSTRAIN_H

#include "ComputeStressFreeStrainBase.h"

class ThreePhaseEigenstrain;

template<>
InputParameters validParams<ThreePhaseEigenstrain>();

/**
 * TwoPhaseEigenstrain computes an Eigenstrain based on two parts
 * prefector_1 * eigenstrain_tensor_1 + prefactor_2 * eigenstrain_tensor_2
 */
class ThreePhaseEigenstrain : public ComputeStressFreeStrainBase
{
public:
  ThreePhaseEigenstrain(const InputParameters & parameters);

protected:
  virtual void computeQpStressFreeStrain();

  const MaterialProperty<Real> & _prefactor_1;
  const MaterialProperty<Real> & _prefactor_2;
  const MaterialProperty<Real> & _prefactor_3;

  RankTwoTensor _eigen_base_tensor_1;
  RankTwoTensor _eigen_base_tensor_2;
  RankTwoTensor _eigen_base_tensor_3;
};

#endif