#ifndef GROUNDHOGAPP_H
#define GROUNDHOGAPP_H

#include "MooseApp.h"

class GroundhogApp;

template<>
InputParameters validParams<GroundhogApp>();

class GroundhogApp : public MooseApp
{
public:
  GroundhogApp(InputParameters parameters);
  virtual ~GroundhogApp();

  static void registerApps();
  static void registerObjects(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
};

#endif /* GROUNDHOGAPP_H */
