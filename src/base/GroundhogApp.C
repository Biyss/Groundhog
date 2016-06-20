#include "GroundhogApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

// kernels
#include "LaplaceYoung.h"

// Materials
#include "DislocationGradientEnergy.h"
#include "TwoPhaseEigenstrain.h"
#include "TwoPhaseVariableEigenstrain.h"

template<>
InputParameters validParams<GroundhogApp>()
{
  InputParameters params = validParams<MooseApp>();

  params.set<bool>("use_legacy_uo_initialization") = false;
  params.set<bool>("use_legacy_uo_aux_computation") = false;
  params.set<bool>("use_legacy_output_syntax") = false;

  return params;
}

GroundhogApp::GroundhogApp(InputParameters parameters) :
    MooseApp(parameters)
{
  Moose::registerObjects(_factory);
  ModulesApp::registerObjects(_factory);
  GroundhogApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  ModulesApp::associateSyntax(_syntax, _action_factory);
  GroundhogApp::associateSyntax(_syntax, _action_factory);
}

GroundhogApp::~GroundhogApp()
{
}

// External entry point for dynamic application loading
extern "C" void GroundhogApp__registerApps() { GroundhogApp::registerApps(); }
void
GroundhogApp::registerApps()
{
  registerApp(GroundhogApp);
}

// External entry point for dynamic object registration
extern "C" void GroundhogApp__registerObjects(Factory & factory) { GroundhogApp::registerObjects(factory); }
void
GroundhogApp::registerObjects(Factory & factory)
{
  registerKernel(LaplaceYoung);

  registerMaterial(DislocationGradientEnergy);
  registerMaterial(TwoPhaseEigenstrain);
  registerMaterial(TwoPhaseVariableEigenstrain);
}

// External entry point for dynamic syntax association
extern "C" void GroundhogApp__associateSyntax(Syntax & syntax, ActionFactory & action_factory) { GroundhogApp::associateSyntax(syntax, action_factory); }
void
GroundhogApp::associateSyntax(Syntax & /*syntax*/, ActionFactory & /*action_factory*/)
{
}
