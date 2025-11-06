#include "CosmicTrackingAction.h"
#include "CosmicSimulator.h"

#include <G4Track.hh>
#include <G4TrackingManager.hh>
#include <G4TrackStatus.hh>
#include <G4OpticalPhoton.hh>

#include "Particle.h"
#include "MyTrackInformation.h"

void CosmicTrackingAction::PreUserTrackingAction(const G4Track* track)
{
    // Solo guardamos info en fotones ópticos
    if (track->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) {
        G4String parentName = "unknown";

        // ¿Es un fotón secundario?
        if (track->GetParentID() > 0) {
            // Recuperamos el nombre de la partícula que lo creó
            const G4VProcess* creator = track->GetCreatorProcess();
            if (creator) {
                parentName = creator->GetProcessName();  
                // Esto te dice el proceso físico que creó el fotón (ej: Scintillation, Cerenkov).
            }
        } else {
            parentName = "primary";
        }

        // Guardamos el nombre del proceso/partícula madre en la info del track
        track->SetUserInformation(new MyTrackInformation(parentName));
    }
}

void CosmicTrackingAction::PostUserTrackingAction(const G4Track*)
{
    // No necesitamos hacer nada al final del tracking
}
