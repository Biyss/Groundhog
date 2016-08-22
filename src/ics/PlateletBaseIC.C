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
  params.addParam<Real>("int_width_r", 0.0, "The interfacial width along the radius.  Defaults to sharp interface");
  params.addParam<Real>("int_width_th", 0.0, "The interfacial width along the thickness.  Defaults to sharp interface");
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
    _int_width_r(parameters.get<Real>("int_width_r")),
    _int_width_th(parameters.get<Real>("int_width_th")),
    _norm(parameters.get<RealVectorValue>("normal")),
    _center(_x1, _y1, _z1)
{
  _random.seed(_tid, getParam<unsigned int>("rand_seed"));
  _norm /= _norm.norm();
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

  if (_thick > 0.1) //  Platelet with a finite thickness
    {
      if ((std::abs(dist.contract(_norm)) <= _thick/2) && (dist - dist.contract(_norm) * _norm).norm() <= _radius - _int_width_r/2.0) //Inside platelet
        value = 1;
      else if ((std::abs(dist.contract(_norm)) <= _thick/2) && (dist - dist.contract(_norm) * _norm).norm() >= _radius - _int_width_r/2.0 && (dist - dist.contract(_norm) * _norm).norm() <= _radius + _int_width_r/2.0) //Smooth interface along radius
      {
        Real int_pos = ((dist - dist.contract(_norm) * _norm).norm() - _radius + _int_width_r * 0.5)/_int_width_r;
        value = (1.0 + std::cos(int_pos * libMesh::pi)) / 2.0;
      }
      else if ((dist - dist.contract(_norm) * _norm).norm() <= _radius - _int_width_r/2.0 && (std::abs(dist.contract(_norm)) >= _thick/2) && (std::abs(dist.contract(_norm)) <= _thick/2 + _int_width_th)) //Smooth interface along thickness
      {
        Real int_pos = (std::abs(dist.contract(_norm)) - _thick/2)/_int_width_th;
        value = (1.0 + std::cos(int_pos * libMesh::pi)) / 2.0;
      }
    }
  else //  Platelet with zero thickness, reduces to a single layer
    {
      if (dist.contract(_norm) == 0 && dist.norm() <= _radius - _int_width_r/2.0) //Inside platelet
        value = 1;
      else if (dist.contract(_norm) == 0 && dist.norm() >= _radius - _int_width_r/2.0 && dist.norm() <= _radius + _int_width_r/2.0) //Smooth interface along radius
      {
        Real int_pos = (dist.norm() - _radius + _int_width_r * 0.5)/_int_width_r;
        value = (1.0 + std::cos(int_pos * libMesh::pi)) / 2.0;
      }
    }

  return value;
}
