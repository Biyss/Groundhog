#ifndef LAPLACEYOUNG_H
#define LAPLACEYOUNG_H

#include "Kernel.h"

class LaplaceYoung;

template<>
InputParameters validParams<LaplaceYoung>();

/**
 * This kernel implements the Laplacian operator:
 * $\nabla u \cdot \nabla \phi_i$
 */
class LaplaceYoung : public Kernel
{
public:
  LaplaceYoung(const InputParameters & parameters);

  virtual ~LaplaceYoung();

protected:
  virtual Real computeQpResidual();

  virtual Real computeQpJacobian();
};


#endif /* LAPLACEYOUNG_H */
