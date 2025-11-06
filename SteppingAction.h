// definition of the CosmicSteppingAction class

#ifndef CosmicSteppingAction_h
#define CosmicSteppingAction_h 1
#include <iostream>
#include "G4UserSteppingAction.hh"
#include "G4String.hh"

#include "Event.h"


class CosmicConstruction;
class CosmicEventAction;

/* Stepping action class.
     UserSteppingAction collects info at a step level
*/

class CosmicSteppingAction : public G4UserSteppingAction
{
    public:
        CosmicSteppingAction(const CosmicConstruction* det, CosmicEventAction* event, Event& theEvent);
        virtual ~CosmicSteppingAction();
        virtual void UserSteppingAction(const G4Step* step);

    private:
        const CosmicConstruction* fDetectorConstruction;
        CosmicEventAction* fEventAction;
        std::ofstream* fOutputFile;
        Event& fEvent;

        G4double fSiPMTime;
        G4double fScinTime;
        G4String stepVolume;
        G4String trackVolume;
};

