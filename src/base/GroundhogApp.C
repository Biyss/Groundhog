#include "GroundhogApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

// IC
#include "PlateletBaseIC.h"

// kernels
#include "LaplaceYoung.h"
#include "SimpleACGradNormal.h"
#include "UniformVacancySource.h"
#include "CoefAllenCahn.h"
#include "CoefSimpleACInterface.h"
#include "PrefACBulk.h"
#include "PrefAllenCahn.h"
#include "ACNormalInterface.h"
#include "InteractionACGradNormal.h"
#include "VacancyAnnihilationKernel.h"

// AuxKernels
#include "DislocationTotalFreeEnergy.h"
#include "DislocationGradientEnergy.h"


// Materials
#include "ThreePhaseEigenstrain.h"
#include "ThreePhaseVariableEigenstrain.h"

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
  registerInitialCondition(PlateletBaseIC);

  registerKernel(LaplaceYoung);
  registerKernel(SimpleACGradNormal);
  registerKernel(UniformVacancySource);
  registerKernel(CoefAllenCahn);
  registerKernel(CoefSimpleACInterface);
  registerKernel(PrefACBulk);
  registerKernel(PrefAllenCahn);
  registerKernel(ACNormalInterface);
  registerKernel(InteractionACGradNormal);
  registerKernel(VacancyAnnihilationKernel);

  registerAux(DislocationTotalFreeEnergy);
  registerAux(DislocationGradientEnergy);

  registerMaterial(ThreePhaseEigenstrain);
  registerMaterial(ThreePhaseVariableEigenstrain);
}

// External entry point for dynamic syntax association
extern "C" void GroundhogApp__associateSyntax(Syntax & syntax, ActionFactory & action_factory) { GroundhogApp::associateSyntax(syntax, action_factory); }
void
GroundhogApp::associateSyntax(Syntax & /*syntax*/, ActionFactory & /*action_factory*/)
{
}
