// implementation of the CosmicRunAction class

#include "G4Timer.hh"
#include "G4Run.hh"
#include "g4root.hh"
#include "G4AccumulableManager.hh"
 
#include "CosmicRunAction.h"
//#include "histosRun.hh"

//#include <TFile.h>

CosmicRunAction::CosmicRunAction()
 : G4UserRunAction()
{
  G4cout << "...CosmicRunAction..." << G4endl;
}


CosmicRunAction::~CosmicRunAction()
{}


void 
CosmicRunAction::BeginOfRunAction(const G4Run* )
{
  
}


void 
CosmicRunAction::EndOfRunAction(const G4Run* )
{
  
}
