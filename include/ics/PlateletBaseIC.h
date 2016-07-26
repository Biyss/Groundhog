#ifndef PLATELETBASEIC_H
#define PLATELETBASEIC_H

#include "Kernel.h"
#include "InitialCondition.h"
#include "MooseRandom.h"

// System includes
#include <string>

// Forward Declarations
class PlateletBaseIC;

template<>
InputParameters validParams<PlateletBaseIC>();

/**
 * SmoothcircleBaseIC is the base class for all initial conditions that create circles. The circles can have sharp interfaces or a finite interface width. Note that all children must resize _radii and _centers.
 */
class PlateletBaseIC : public InitialCondition
{
public:
  PlateletBaseIC(const InputParameters & parameters);

  virtual Real value(const Point & p);

  virtual void initialSetup();

protected:

  MooseMesh & _mesh;

  Real _x1;
  Real _y1;
  Real _z1;
  Real _radius;
  Real _thick;
  Real _int_width_r;
  Real _int_width_th;
  RealVectorValue _norm;
  Point _center;

  MooseRandom _random;
};

#endif //PLATELETBASEIC_H
