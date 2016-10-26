#ifndef VacancyAnnihilationKernel_H
#define VacancyAnnihilationKernel_H

#include "Kernel.h"

//Forward Declarations
class VacancyAnnihilationKernel;

template<>
InputParameters validParams<VacancyAnnihilationKernel>();

/**
 * This kernel calculates the residual for grain growth during sintering.
 * It calculates the residual of the ith order parameter, and the values of
 * all other order parameters are coupled variables and are stored in vals.
 */
class VacancyAnnihilationKernel : public Kernel
{
public:
  VacancyAnnihilationKernel(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

  std::string _Svgb_name;

private:
  std::vector<const VariableValue *> _vals;
  std::vector<unsigned int> _vals_var;

  const MaterialProperty<Real> & _Svgb;

  unsigned int _ncrys;
  Real _ceq;

  //VariableValue & _c;
};

#endif //VacancyAnnihilationKernel_h
