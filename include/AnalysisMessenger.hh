
#ifndef AnalysisMessenger_h
#define AnalysisMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithAString.hh"
#include "AnalysisManager.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"

class AnalysisManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


class AnalysisMessenger: public G4UImessenger

{
public:
  AnalysisMessenger(AnalysisManager*);
  ~AnalysisMessenger();
  
  void SetNewValue(G4UIcommand*, G4String);
  
private:

  //pointer to AnalysisManager
  AnalysisManager* Analysis;
  G4UIdirectory* AnalysisDir;
  G4UIcmdWithAString* outputFileCommand; 
  G4UIcmdWithABool* physicFlagCmd;

};
#endif



