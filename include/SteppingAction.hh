#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class DetectorConstruction;
class EventAction;
class G4LogicalVolume;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction(DetectorConstruction*, EventAction*);
   ~SteppingAction();

    virtual void UserSteppingAction(const G4Step*);
    virtual void StepInfo(const G4Step*);
    G4double BirksAttenuation(const G4Step*);
    
  private:
    DetectorConstruction* fDetector;
    EventAction*          fEventAct;
  G4double           fXstartFilter[21], fXendFilter[21]; 

    G4LogicalVolume* fScoringVolume;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
