// definition of CosmicEventAction class
#ifndef CosmicEventAction_h 
#define CosmicEventAction_h 1

#include "G4UserEventAction.hh"
#include "G4Event.hh"

#include "CosmicRunAction.h"

#include <string>
#include <vector>


class CosmicEventAction : public G4UserEventAction
{
  public:
    CosmicEventAction();
    virtual ~CosmicEventAction();

    virtual void BeginOfEventAction(const G4Event *event);
    virtual void EndOfEventAction(const G4Event *event);

};

#endif

