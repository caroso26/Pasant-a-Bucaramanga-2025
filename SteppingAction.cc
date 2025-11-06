// implementation of the CosmicSteppingAction class
#include "CosmicSimulator.h"
#include "CosmicSteppingAction.h"
#include "CosmicConstruction.h"
#include "CosmicEventAction.h"
#include "CosmicTrackingAction.h"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"

#include "G4OpticalPhoton.hh"

#include "SimData.h"
#include "Detector.h"

CosmicSteppingAction::CosmicSteppingAction(const CosmicConstruction* det, CosmicEventAction* G4event, Event& theEvent)
	: G4UserSteppingAction(),
		fDetectorConstruction(det),
		fEventAction(G4event),
    fEvent(theEvent)
{
}

CosmicSteppingAction::~CosmicSteppingAction()
{
}

void
CosmicSteppingAction::UserSteppingAction(const G4Step* step)
{
  

}
