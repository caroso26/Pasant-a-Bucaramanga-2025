/* 
	
	A Meiga application for simulating Water-Cherenkov Detectors

	author: alvaro taboada
	date: 9 Feb 2022

 */

// Headers of this particular application
#include "CosmicSimulator.h"
#include "CosmicConstruction.h"
#include "G4MPrimaryGeneratorAction.h"
#include "CosmicStackingAction.h"
#include "CosmicEventAction.h"
#include "CosmicRunAction.h"
#include "CosmicTrackingAction.h"
#include "CosmicSteppingAction.h"

// Geant4 headers
#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "Randomize.hh"

// Framework libraries
#include "ConfigManager.h"
#include "CorsikaUtilities.h"
#include "Event.h"
#include "SimData.h"
#include "OptDeviceSimData.h"
#include "Detector.h"
#include "OptDevice.h"
#include "G4MPhysicsList.h"
#include "DataWriter.h"

using namespace std;

Particle CosmicSimulator::currentParticle;
CosmicSimulator* fCosmicSimulator;
string fCfgFile;

CosmicSimulator::CosmicSimulator()
{

}

namespace 
{
	void ProgramUsage() 
	{
		cerr << " Program Usage: " << endl;
		cerr << " ./CosmicSimulator [ -c ConfigFile.json ] " << endl;
	}

}


int main(int argc, char** argv) 
{

	if (argc < 3) {
		ProgramUsage();
		throw invalid_argument("[ERROR] CosmicSimulator::main: A configuration file is needed!");
	}

	for (int i=1; i<argc; i=i+2) {
		string sarg(argv[i]);
		if (sarg == "-c")
			fCfgFile = argv[i+1];
	}

	// for program time calculation
	time_t start, end;
	time(&start);

	fCosmicSimulator = new CosmicSimulator();
	// Create Event object
	Event theEvent;
	fCosmicSimulator->Initialize(theEvent, fCfgFile);
	fCosmicSimulator->RunSimulation(theEvent);
	/*************************************************
		
		Geant4 simulation ended here!
		What happens nWCDt is up to you =)

	**************************************************/
	fCosmicSimulator->WriteEventInfo(theEvent);
	time(&end);

	// Calculating total time taken by the program.
	double time_taken = double(end - start);
	cout << "[INFO] CosmicSimulator: Time taken by program is : " << fixed
			 << time_taken << setprecision(5);
	cout << " sec " << endl;
	
	return 0;

}

void
CosmicSimulator::Initialize(Event& theEvent, string cfgFile)
{

	cout << "[INFO] CosmicSimulator::Initialize" << endl;
	cout << "[INFO] CosmicSimulator::Initialize: Reading configuration file " << cfgFile << endl;

	// Fill Event object from configuration file
	// Read Simulation configuration
	theEvent = ConfigManager::ReadConfigurationFile(cfgFile);
	// get simulation simulation settings
	const Event::Config &cfg = theEvent.GetConfig();
	ConfigManager::PrintConfig(cfg);
	// Read Detector Configuration
	ConfigManager::ReadDetectorList(cfg.fDetectorList, theEvent);
	
}                 


bool
CosmicSimulator::RunSimulation(Event& theEvent)
{

	cout << "[INFO] CosmicSimulator::RunSimulation" << endl;
	
	SimData& simData = theEvent.GetSimData();
	const Event::Config &cfg = theEvent.GetConfig();
	const unsigned int numberOfParticles = simData.GetTotalNumberOfParticles();
	cout << "[INFO] CosmicSimulator::RunSimulation: Number of particles to be simulated = " << numberOfParticles << endl;
	
	if (!numberOfParticles) {
		cerr << "[ERROR] CosmicSimulator::RunSimulation: No Particles in the Event! WCDiting." << endl;
		return false;
	}
	
	/***************

		Geant4 Setup    

	*****************/

	G4long myseed = time(NULL);
	G4Random::setTheEngine(new CLHEP::RanecuEngine);
	G4Random::setTheSeed(myseed);

	G4VisManager* fVisManager = nullptr;
	
	// construct the default run manager
	auto fRunManager = G4RunManagerFactory::CreateRunManager();

	// set mandatory initialization classes
	auto fDetConstruction = new CosmicConstruction(theEvent);
	fRunManager->SetUserInitialization(fDetConstruction);
	
	fRunManager->SetUserInitialization(new G4MPhysicsList(fPhysicsName));

	G4MPrimaryGeneratorAction *fPrimaryGenerator = new G4MPrimaryGeneratorAction(theEvent);
	fRunManager->SetUserAction(fPrimaryGenerator);
	
	CosmicStackingAction *fStackingAction = new CosmicStackingAction(theEvent);
	fRunManager->SetUserAction(fStackingAction);

	CosmicRunAction *fRunAction = new CosmicRunAction();
	fRunManager->SetUserAction(fRunAction);
	
	CosmicEventAction *fEventAction = new CosmicEventAction();
	fRunManager->SetUserAction(fEventAction);

	fRunManager->SetUserAction(new CosmicTrackingAction());

	CosmicSteppingAction *fSteppingAction = new CosmicSteppingAction(fDetConstruction, fEventAction, theEvent);
	fRunManager->SetUserAction(fSteppingAction);
	
	// initialize G4 kernel
	fRunManager->Initialize();

	// initialize visualization
	if ((cfg.fGeoVis || cfg.fTrajVis) && !fVisManager)
		fVisManager = new G4VisExecutive;

	// get the pointer to the UI manager and set verbosities
	G4UImanager* fUImanager = G4UImanager::GetUIpointer();
	switch (fVerbosity) {
		case 1:
			fUImanager->ApplyCommand("/run/verbose 1");
			fUImanager->ApplyCommand("/event/verbose 0");
			fUImanager->ApplyCommand("/tracking/verbose 0");
			break;
		case 2:
			fUImanager->ApplyCommand("/run/verbose 1");
			fUImanager->ApplyCommand("/event/verbose 1");
			fUImanager->ApplyCommand("/tracking/verbose 0");
			break;
		case 3:
			fUImanager->ApplyCommand("/run/verbose 1");
			fUImanager->ApplyCommand("/event/verbose 1");
			fUImanager->ApplyCommand("/tracking/verbose 1");
			break;
		default:
			fUImanager->ApplyCommand("/run/verbose 0");
			fUImanager->ApplyCommand("/event/verbose 0");
			fUImanager->ApplyCommand("/tracking/verbose 0");
		}
	
	if (cfg.fGeoVis || cfg.fTrajVis) {
		fVisManager->Initialize();
		fUImanager->ApplyCommand(("/vis/open " + fRenderFile).c_str());
		fUImanager->ApplyCommand("/vis/scene/create");
		fUImanager->ApplyCommand("/vis/sceneHandler/attach");
		fUImanager->ApplyCommand("/vis/scene/add/volume");
		fUImanager->ApplyCommand("/vis/scene/add/axes 0 0 0 1 m");
		fUImanager->ApplyCommand("/vis/viewer/set/viewpointThetaPhi 0. 0.");
		fUImanager->ApplyCommand("/vis/viewer/set/targetPoint 0 0 0");
		fUImanager->ApplyCommand("/vis/viewer/zoom 1");
		fUImanager->ApplyCommand("/vis/viewero/set/style/wireframe");
		fUImanager->ApplyCommand("/vis/drawVolume");
		fUImanager->ApplyCommand("/vis/scene/notifyHandlers");
		fUImanager->ApplyCommand("/vis/viewer/update");

	}

	if (cfg.fTrajVis) {
			fUImanager->ApplyCommand("/tracking/storeTrajectory 1");
			fUImanager->ApplyCommand("/vis/scene/add/trajectories");
	}
	

	// loop over particle vector
	for (auto it = simData.GetParticleVector().begin(); it != simData.GetParticleVector().end(); ++it) {
		CosmicSimulator::currentParticle = *it;
		simData.SetCurrentParticle(*it);
		// Run simulation
		fRunManager->BeamOn(1);
	}


	delete fVisManager;
	delete fRunManager;

	cout << "[INFO] CosmicSimulator::RunSimulation: Geant4 Simulation ended successfully. " << endl;

	return true;

}


void
CosmicSimulator::WriteEventInfo(Event& theEvent)
{
	cout << "[INFO] CosmicSimulator::WriteEventInfo" << endl;

	DataWriter::FileWriter(theEvent);

	return;

}
