#include "PlateletBaseIC.h"
#include "MooseMesh.h"

template<>
InputParameters validParams<PlateletBaseIC>()
{
  InputParameters params = validParams<InitialCondition>();
  params.addRequiredParam<Real>("x1", "The x coordinate of the circle center");
  params.addRequiredParam<Real>("y1", "The y coordinate of the circle center");
  params.addParam<Real>("z1", 0.0, "The z coordinate of the circle center");
  params.addRequiredParam<Real>("radius", "The radius of a circle");
  params.addRequiredParam<Real>("thickness", "The thickness of the platelet");
  params.addRequiredParam<RealVectorValue>("normal", "Unit normal vector of the platelet");
  params.addParam<unsigned int>("rand_seed", 12345, "Seed value for the random number generator");
  return params;
}

PlateletBaseIC::PlateletBaseIC(const InputParameters & parameters) :
    InitialCondition(parameters),
    _mesh(_fe_problem.mesh()),
    _x1(parameters.get<Real>("x1")),
    _y1(parameters.get<Real>("y1")),
    _z1(parameters.get<Real>("z1")),
    _radius(parameters.get<Real>("radius")),
    _thick(parameters.get<Real>("thickness")),
    _norm(parameters.get<RealVectorValue>("normal")),
    _center(_x1, _y1, _z1)
{
  _random.seed(_tid, getParam<unsigned int>("rand_seed"));
}

void
PlateletBaseIC::initialSetup()
{
}

Real
PlateletBaseIC::value(const Point & p)
{
  Point l_p = p;

  //Compute the distance between the current point and the center
  RealVectorValue dist = _mesh.minPeriodicVector(_var.number(), l_p, _center);

  //Return value
  Real value = 0;//Outside circle

    if ((std::abs(dist.contract(_norm)) <= _thick/2) && (dist - dist.contract(_norm) * _norm).norm() <= _radius) //Inside platelet
      value = 1;

  return value;
}
