#ifndef CosmicTrackingAction_h
#define CosmicTrackingAction_h 1

#include <G4UserTrackingAction.hh>

class G4Track;

class CosmicTrackingAction : public G4UserTrackingAction {
	public:
		CosmicTrackingAction() { }
		~CosmicTrackingAction() { }

		virtual void PreUserTrackingAction(const G4Track*  track) override;
    virtual void PostUserTrackingAction(const G4Track* track) override;

    bool GetFirstStepInVolume() const { return firstStepInVolume; }
    void SetFirstStepInVolume(const bool b) { firstStepInVolume = b; }


  private:

  	bool firstStepInVolume = true;

};

#endif

