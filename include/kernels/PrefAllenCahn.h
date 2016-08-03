/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef PREFALLENCAHN_H
#define PREFALLENCAHN_H

#include "PrefAllenCahnBase.h"
#include "RankTwoTensor.h"

//Forward Declarations
class PrefAllenCahn;

template<>
InputParameters validParams<PrefAllenCahn>();

/**
 * AllenCahn uses the Free Energy function and derivatives
 * provided by a DerivativeParsedMaterial to computer the
 * residual for the bulk part of the Allen-Cahn equation.
 */
class PrefAllenCahn : public PrefAllenCahnBase<Real>
{
public:
  PrefAllenCahn(const InputParameters & parameters);

  virtual void initialSetup();

protected:
  virtual Real computeDFDOP(PFFunctionType type);
  // virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  const unsigned int _nvar;
  const MaterialProperty<Real> & _dFdEta;
  const MaterialProperty<Real> & _d2FdEta2;

  std::vector<const MaterialProperty<Real> *> _d2FdEtadarg;

  RealGradient _normal;

  /**
   * Class variables to hold the components of plane normal coming from the input parameters.
   */
  Real _n1;
  Real _n2;
  Real _n3;

  // Gamma is the prefector we introduced: |n x _grad_u|^2/|_grad_u|^2
  Real Gamma;
  // CapPsi is a vector, which equals to d(|n x _grad_u|^2)/d(_grad_u).
  // and it can be written in the form: matrix times vector (_prePsi * _grad_u)
  RealGradient CapPsi;
  // CapPhi is a vector: CapPsi/|_grad_u|^2 - 2 *|n x (_grad_u|^2/|_grad_u|^4) * _grad_u
  RealGradient CapPhi;
};

#endif //PREFALLENCAHN_H
