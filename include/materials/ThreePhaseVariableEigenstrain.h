/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef THREEPHASEVARIABLEEIGENSTRAIN_H
#define THREEPHASEVARIABLEEIGENSTRAIN_H

#include "ThreePhaseEigenstrain.h"
#include "DerivativeMaterialInterface.h"

class ThreePhaseVariableEigenstrain;

template<>
InputParameters validParams<ThreePhaseVariableEigenstrain>();

/**
 * TwoPhaseVariableEigenstrain computes an Eigenstrain based on two parts
 * prefector_1 * eigenstrain_tensor_1 + prefactor_2 * eigenstrain_tensor_2
 */
class ThreePhaseVariableEigenstrain : public DerivativeMaterialInterface<ThreePhaseEigenstrain>
{
public:
  ThreePhaseVariableEigenstrain(const InputParameters & parameters);

protected:
  virtual void computeQpStressFreeStrain();

  /// number of variables the prefactor depends on
  const unsigned int _num_args;

  /// first derivatives of the prefactor1 w.r.t. to the args
  std::vector<const MaterialProperty<Real> *> _dprefactor_1;
  /// second derivatives of the prefactor1 w.r.t. to the args
  std::vector<std::vector<const MaterialProperty<Real> *> > _d2prefactor_1;

  /// first derivatives of the prefactor2 w.r.t. to the args
  std::vector<const MaterialProperty<Real> *> _dprefactor_2;
  /// second derivatives of the prefactor2 w.r.t. to the args
  std::vector<std::vector<const MaterialProperty<Real> *> > _d2prefactor_2;

  /// first derivatives of the prefactor2 w.r.t. to the args
  std::vector<const MaterialProperty<Real> *> _dprefactor_3;
  /// second derivatives of the prefactor2 w.r.t. to the args
  std::vector<std::vector<const MaterialProperty<Real> *> > _d2prefactor_3;

  /// first derivatives of the elastic strain w.r.t. to the args
  std::vector<MaterialProperty<RankTwoTensor> *> _delastic_strain;
  /// second derivatives of the elastic strain w.r.t. to the args
  std::vector<std::vector<MaterialProperty<RankTwoTensor> *> > _d2elastic_strain;

};

#endif //THREEPHASEVARIABLEEIGENSTRAIN_H
