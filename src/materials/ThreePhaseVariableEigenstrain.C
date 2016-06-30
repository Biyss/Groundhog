/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "ThreePhaseVariableEigenstrain.h"

template<>
InputParameters validParams<ThreePhaseVariableEigenstrain>()
{
  InputParameters params = validParams<ThreePhaseEigenstrain>();
  params.addClassDescription("Computes an Eigenstrain and its derivatives that is a function of multiple variables, where the prefactor is defined in a derivative material");
  params.addRequiredCoupledVar("args", "variable dependencies for the prefactor");
  return params;
}

ThreePhaseVariableEigenstrain::ThreePhaseVariableEigenstrain(const InputParameters & parameters) :
    DerivativeMaterialInterface<ThreePhaseEigenstrain>(parameters),
    _num_args(coupledComponents("args")),
    _dprefactor_1(_num_args),
    _d2prefactor_1(_num_args),
    _dprefactor_2(_num_args),
    _d2prefactor_2(_num_args),
    _dprefactor_3(_num_args),
    _d2prefactor_3(_num_args),
    _delastic_strain(_num_args),
    _d2elastic_strain(_num_args)
{
  // fetch prerequisite derivatives and build elastic_strain derivatives and cross-derivatives
  for (unsigned int i = 0; i < _num_args; ++i)
  {
    const VariableName & iname = getVar("args", i)->name();
    _dprefactor_1[i] = &getMaterialPropertyDerivative<Real>("prefactor_1", iname);
    _dprefactor_2[i] = &getMaterialPropertyDerivative<Real>("prefactor_2", iname);
    _dprefactor_3[i] = &getMaterialPropertyDerivative<Real>("prefactor_3", iname);
    _delastic_strain[i] = &declarePropertyDerivative<RankTwoTensor>(_base_name + "elastic_strain", iname);

    _d2prefactor_1[i].resize(_num_args);
    _d2prefactor_2[i].resize(_num_args);
    _d2prefactor_3[i].resize(_num_args);
    _d2elastic_strain[i].resize(_num_args);

    for (unsigned int j = i; j < _num_args; ++j)
    {
      const VariableName & jname = getVar("args", j)->name();
      _d2prefactor_1[i][j] = &getMaterialPropertyDerivative<Real>("prefactor_1", iname, jname);
      _d2prefactor_2[i][j] = &getMaterialPropertyDerivative<Real>("prefactor_2", iname, jname);
      _d2prefactor_3[i][j] = &getMaterialPropertyDerivative<Real>("prefactor_3", iname, jname);
      _d2elastic_strain[i][j] = &declarePropertyDerivative<RankTwoTensor>(_base_name + "elastic_strain", iname, jname);
    }
  }
}

void
ThreePhaseVariableEigenstrain::computeQpStressFreeStrain()
{
  ThreePhaseEigenstrain::computeQpStressFreeStrain();

  //Define derivatives of the elastic strain
  for (unsigned int i = 0; i < _num_args; ++i)
  {
    (*_delastic_strain[i])[_qp] = - _eigen_base_tensor_1 * (*_dprefactor_1[i])[_qp] - _eigen_base_tensor_2 * (*_dprefactor_2[i])[_qp] - _eigen_base_tensor_3 * (*_dprefactor_3[i])[_qp];
    for (unsigned int j = i; j < _num_args; ++j)
    (*_d2elastic_strain[i][j])[_qp] = - _eigen_base_tensor_1 * (*_d2prefactor_1[i][j])[_qp] - _eigen_base_tensor_2 * (*_d2prefactor_2[i][j])[_qp] - _eigen_base_tensor_3 * (*_d2prefactor_3[i][j])[_qp];
  }
}
