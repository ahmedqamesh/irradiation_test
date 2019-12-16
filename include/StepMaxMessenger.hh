
#ifndef StepMaxMessenger_h
#define StepMaxMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class StepMax;
class G4UIdirectory;
class G4UIcommand;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class StepMaxMessenger: public G4UImessenger
{
  public:
    StepMaxMessenger(StepMax*);
   ~StepMaxMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    StepMax*       fStepMax;
    
    G4UIdirectory* fStepMaxDir;    
    G4UIcommand*   fStepMaxCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
