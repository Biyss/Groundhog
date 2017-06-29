/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef POROSITY_H
#define POROSITY_H

#include "ElementAverageValue.h"

/**
 *
 */
class Porosity : public ElementAverageValue
{
public:
  Porosity(const InputParameters & parameters);

protected:
  virtual Real computeQpIntegral();
};

#endif // POROSITY_H
