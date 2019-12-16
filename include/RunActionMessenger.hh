
#ifndef RunActionMessenger_h
#define RunActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithAString.hh"
class RunAction;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class RunActionMessenger: public G4UImessenger
{
public:
  RunActionMessenger(RunAction*);
  virtual ~RunActionMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);
    
private:
  RunAction*          fRunAction;
  G4UIdirectory*      fRunDir;
  G4UIcommand*        fAccCmd;
  G4UIcmdWithABool*   fLimCmd;
  G4UIcmdWithAString* loadPhaseSpace;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
