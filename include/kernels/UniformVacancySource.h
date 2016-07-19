#ifndef UNIFORMVACANCYSOURCE_H
#define UNIFORMVACANCYSOURCE_H

#include "Kernel.h"
#include "MooseRandom.h"

//Forward Declarations
class UniformVacancySource;

template<>
InputParameters validParams<UniformVacancySource>();

class UniformVacancySource : public Kernel
{
public:
  UniformVacancySource(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();

private:

  Real _Vg;
};


#endif //UNIFORMVACANCYSOURCE_H
