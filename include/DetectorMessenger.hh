#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1


#include "G4UImessenger.hh"
#include "globals.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAString.hh"
class DetectorConstruction;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorMessenger: public G4UImessenger
{
  public:
    DetectorMessenger(DetectorConstruction* );
   ~DetectorMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

  private:
    DetectorConstruction* Detector;

    G4UIdirectory*             fTestemDir;
    G4UIdirectory*             fDetDir;
    
    G4UIcmdWithADoubleAndUnit* fSizeYZCmd;


    G4UIcmdWithAnInteger*      fNbLayersCmd;

    G4UIcmdWithAnInteger*      fNbAbsorCmd;

    G4UIcommand*               fAbsorCmd;
    G4UIcmdWithAnInteger*      fNbFilterCmd;
    G4UIcommand*               fFilterCmd;
    G4UIcommand* 			   fxposfilterCmd;
    G4UIcommand* 			   fxposAbsCmd;
    G4UIcmdWith3VectorAndUnit* fposfilterCmd;
    G4UIcmdWithADoubleAndUnit* fRotfilterCmd;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

