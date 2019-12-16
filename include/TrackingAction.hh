
#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

class DetectorConstruction;
class EventAction;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class TrackingAction : public G4UserTrackingAction {

  public:  
    TrackingAction(DetectorConstruction*,EventAction*);
   ~TrackingAction() {};
   
    virtual void  PreUserTrackingAction(const G4Track*);   
    virtual void PostUserTrackingAction(const G4Track*);
    
  private:
    DetectorConstruction* fDetector;
    EventAction*          fEventAction;

   G4double fXstartAbs[11], fXendAbs[11];
    G4double fXstartFilter[22],fXendFilter[22];
    G4double fPrimaryCharge;
    G4int               fPhotoGamma;
    G4int               fComptGamma;
    G4int               fPhotoAuger;
    G4int               fComptAuger;
    G4int               fPixeGamma;
    G4int               fPixeAuger;
    G4int               fPhoto;
    G4int               fcompton;
    G4int               fundefined;
    G4bool              fIDdefined;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
