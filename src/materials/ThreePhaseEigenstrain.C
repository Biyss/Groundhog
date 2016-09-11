/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "ThreePhaseEigenstrain.h"

template<>
InputParameters validParams<ThreePhaseEigenstrain>()
{
  InputParameters params = validParams<ComputeStressFreeStrainBase>();
  params.addClassDescription("Computes Eigenstrain of three phases");
  params.addParam<MaterialPropertyName>("prefactor_1", 1.0, "Name of material defining the variable dependence");
  params.addParam<MaterialPropertyName>("prefactor_2", 1.0, "Name of material defining the variable dependence");
  params.addParam<MaterialPropertyName>("prefactor_3", 1.0, "Name of material defining the variable dependence");
  params.addRequiredParam<std::vector<Real> >("eigen_base_1", "Vector of values defining the constant base tensor for the Eigenstrain of phase 1");
  params.addRequiredParam<std::vector<Real> >("eigen_base_2", "Vector of values defining the constant base tensor for the Eigenstrain of phase 2");
  params.addRequiredParam<std::vector<Real> >("eigen_base_3", "Vector of values defining the constant base tensor for the Eigenstrain of phase 3");
  return params;
}

ThreePhaseEigenstrain::ThreePhaseEigenstrain(const InputParameters & parameters) :
    ComputeStressFreeStrainBase(parameters),
    _prefactor_1(getMaterialProperty<Real>("prefactor_1")),
    _prefactor_2(getMaterialProperty<Real>("prefactor_2")),
    _prefactor_3(getMaterialProperty<Real>("prefactor_3"))
{
  _eigen_base_tensor_1.fillFromInputVector(getParam<std::vector<Real> >("eigen_base_1"));
  _eigen_base_tensor_2.fillFromInputVector(getParam<std::vector<Real> >("eigen_base_2"));
  _eigen_base_tensor_3.fillFromInputVector(getParam<std::vector<Real> >("eigen_base_3"));
}

void
ThreePhaseEigenstrain::computeQpStressFreeStrain()
{
  // Define Eigenstrain
  _stress_free_strain[_qp] = _eigen_base_tensor_1 * _prefactor_1[_qp] + _eigen_base_tensor_2 * _prefactor_2[_qp] + _eigen_base_tensor_3 * _prefactor_3[_qp];
}
