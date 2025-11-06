// implementation of the CosmicEventAction class

#include "CosmicEventAction.h"

#include "G4Track.hh"

CosmicEventAction::CosmicEventAction() : G4UserEventAction()
{
  G4cout << "...CosmicEventAction..." << G4endl;

}

CosmicEventAction::~CosmicEventAction()
{

}

void
CosmicEventAction::BeginOfEventAction(const G4Event*)
{

}

void 
CosmicEventAction::EndOfEventAction(const G4Event* )
{
  
}

