// implementation of the CosmicConstruction class
#include <iostream>

#include "CosmicConstruction.h"
#include "Materials.h"

#include "G4SDManager.hh"
#include "G4UnitsTable.hh"
//#################################
#include "G4LogicalVolumeStore.hh" 
//#################################
#include "SiPMSD.h"

using namespace std;

CosmicConstruction::CosmicConstruction(Event& theEvent) : 
	G4VUserDetectorConstruction(),
	fEvent(theEvent)
{ 

	cout << "...CosmicConstruction..." << endl;
	fCheckOverlaps = fEvent.GetConfig().fCheckOverlaps;

}

CosmicConstruction::~CosmicConstruction() 
	{ }


G4VPhysicalVolume*
CosmicConstruction::CreateDetector()
{

	CreateWorld();
	//CreateGround();
	PlaceDetector(fEvent);
	return physWorld;
}

void
CosmicConstruction::CreateWorld()
{


	solidWorld 	= new G4Box("World", fWorldSizeX/2, fWorldSizeY/2, fWorldSizeZ/2);
	logicWorld = new G4LogicalVolume(solidWorld, Materials().Air, "World");
	physWorld	 =  new G4PVPlacement(nullptr, G4ThreeVector(), "World", logicWorld, 0, false, 0, fCheckOverlaps);

}

void
CosmicConstruction::CreateGround()
{
	solidGround = new G4Box("Ground", fGroundSizeX/2, fGroundSizeY/2, fGroundSizeZ/2);
	G4VisAttributes brown(G4Colour::Brown());
	logicGround = new G4LogicalVolume(solidGround, Materials().StdRock, "Ground");
	logicGround->SetVisAttributes(brown);
	physGround  =  new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -fWorldSizeZ/2 + fGroundSizeZ/2), logicGround, "Ground", logicWorld, false, 0, fCheckOverlaps);
}


void
CosmicConstruction::PlaceDetector(Event& theEvent)
{
	
	// loop in detector vector
	for (auto detIt = theEvent.DetectorRange().begin(); detIt != theEvent.DetectorRange().end(); detIt++) {
		auto& currentDet = detIt->second;
		BuildDetector(logicWorld, currentDet, theEvent, fCheckOverlaps);

	}

}


G4VPhysicalVolume* 
CosmicConstruction::Construct()

{

	if (!physWorld) {
		return CreateDetector();
	}
	return physWorld;

}

//################################################################################################################
//void CosmicConstruction::ConstructSDandField()
//{
    // Crear instancia del SiPM sensitive detector
  //  auto sipmSD = new SiPMSD("SiPMSD");

    // Registrar en el SDManager
    //G4SDManager::GetSDMpointer()->AddNewDetector(sipmSD);

    // Buscar el volumen lógico del SiPM por nombre
   // auto logicSiPM = G4LogicalVolumeStore::GetInstance()->GetVolume("SiPMLogical");

   // if (logicSiPM) {
     //   SetSensitiveDetector(logicSiPM, sipmSD);
     //   G4cout << "✅ SiPM asignado como Sensitive Detector" << G4endl;
   // } else {
      //  G4cerr << "⚠️ [SiPM] No se encontró el volumen lógico 'SiPMLogical'" << G4endl;
  //  }
//}
//################################################################################################################ 
