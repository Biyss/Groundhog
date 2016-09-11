/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef COEFALLENCAHN_H
#define COEFALLENCAHN_H

#include "ACBulk.h"

//Forward Declarations
class CoefAllenCahn;

template<>
InputParameters validParams<CoefAllenCahn>();

/**
 * AllenCahn uses the Free Energy function and derivatives
 * provided by a DerivativeParsedMaterial to computer the
 * residual for the bulk part of the Allen-Cahn equation.
 */
class CoefAllenCahn : public ACBulk<Real>
{
public:
  CoefAllenCahn(const InputParameters & parameters);

  virtual void initialSetup();

protected:
  virtual Real computeDFDOP(PFFunctionType type);
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  const unsigned int _nvar;
  const MaterialProperty<Real> & _dFdEta;
  const MaterialProperty<Real> & _d2FdEta2;

  std::vector<const MaterialProperty<Real> *> _d2FdEtadarg;

  Real _coef;
};

#endif //COEFALLENCAHN_H
