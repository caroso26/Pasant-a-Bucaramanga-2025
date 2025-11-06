// definition of the CosmicRunAction class

#ifndef CosmicRunAction_h
#define CosmicRunAction_h 1

#include "globals.hh"
#include "G4UserRunAction.hh"
#include "g4root.hh"

#include <fstream>
#include <iostream>
#include <string>


class G4Run;


class CosmicRunAction : public G4UserRunAction
{
  public:
    CosmicRunAction();
    virtual ~CosmicRunAction();

    virtual void BeginOfRunAction(const G4Run* aRun);
    virtual void EndOfRunAction(const G4Run* aRun);

    std::ofstream* outFile;


  private:
};
#endif 
