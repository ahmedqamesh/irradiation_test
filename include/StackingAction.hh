#ifndef StackingAction_h
#define StackingAction_h 1

#include "G4UserStackingAction.hh"
#include "globals.hh"

class EventAction;
class StackingMessenger;
class DetectorConstruction;
class G4LogicalVolume;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class StackingAction : public G4UserStackingAction
{
  public:
    StackingAction(DetectorConstruction*, EventAction*);
   ~StackingAction();   
    void SetKillStatus(G4int value) { fKillSecondary = value;};
     
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);
    
  private:
    DetectorConstruction* fDetector;
    EventAction*        fEventAction;    

    G4int               fKillSecondary;
    StackingMessenger*  fStackMessenger;

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

