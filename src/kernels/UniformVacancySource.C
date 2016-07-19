#include "UniformVacancySource.h"

template<>
InputParameters validParams<UniformVacancySource>()
{
  InputParameters params = validParams<Kernel>();
  params.addParam<Real>("Vg", 0.5 ,"Maximum increase in vacancy concentration");
  return params;
}

UniformVacancySource::UniformVacancySource(const InputParameters & parameters) :
    Kernel(parameters),
    _Vg(getParam<Real>("Vg"))
{
}

Real
UniformVacancySource::computeQpResidual()
{
    return -1 * _Vg * _test[_i][_qp];
}
