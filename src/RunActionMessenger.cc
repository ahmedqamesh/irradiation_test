#include "RunActionMessenger.hh"
#include "RunAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithABool.hh"

#include <sstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunActionMessenger::RunActionMessenger(RunAction* run)
:G4UImessenger(),fRunAction(run),
 fRunDir(0),
 fAccCmd(0),
 fLimCmd(0)
{
  fRunDir = new G4UIdirectory("/testem/run/");
  fRunDir->SetGuidance("run commands");
    
  fAccCmd = new G4UIcommand("/testem/run/acceptance",this);
  fAccCmd->SetGuidance("Check Edep and RMS of energy deposition for given absorber");
  //
  G4UIparameter* AbsNbPrm = new G4UIparameter("AbsorNb",'i',false);
  AbsNbPrm->SetGuidance("absorber number : from 1 to NbOfAbsor");
  AbsNbPrm->SetParameterRange("AbsorNb>0");
  fAccCmd->SetParameter(AbsNbPrm);
  //    
  G4UIparameter* edep = new G4UIparameter("Edep",'d',false);
  edep->SetGuidance("mean energy deposition (MeV)");
  edep->SetParameterRange("Edep>=0.");
  fAccCmd->SetParameter(edep);
  //    
  G4UIparameter* rms = new G4UIparameter("RMS",'d',false);
  rms->SetGuidance("RMS of energy deposition (MeV)");
  rms->SetParameterRange("RMS>=0.");
  fAccCmd->SetParameter(rms);
  //    
  G4UIparameter* lim = new G4UIparameter("nRMS",'d',false);
  lim->SetGuidance("Limit in number of RMS of energy deposition");
  lim->SetParameterRange("Limit>=0.");
  fAccCmd->SetParameter(lim);
  //
  fLimCmd = new G4UIcmdWithABool("/testem/run/limitEdep",this);
  fLimCmd->SetGuidance("remove energy outside acceptance limit");
  fLimCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  loadPhaseSpace=new G4UIcmdWithAString("/testem/run/loadGunData",this);
  loadPhaseSpace->SetGuidance("Load emission from samples form previous runs");
  loadPhaseSpace->SetGuidance("Please enter the filename");
  loadPhaseSpace->SetParameterName("choice",true);
  loadPhaseSpace->AvailableForStates(G4State_Idle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunActionMessenger::~RunActionMessenger()
{
  delete fAccCmd;
  delete fRunDir;    
  delete fLimCmd;
  delete loadPhaseSpace;
  G4cout << "RunActionMessenger deleted" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunActionMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{      
  if( command == fAccCmd )
   { 
     G4int num; 
     G4double edep, rms, lim;
     std::istringstream is(newValue);
     is >> num >> edep >> rms >> lim;
     fRunAction->SetEdepAndRMS(num,edep,rms,lim);
   }
  if( command == loadPhaseSpace )
    { fRunAction->SetFileName(newValue);}

  if( command == fLimCmd )
   { fRunAction->SetApplyLimit(fLimCmd->GetNewBoolValue(newValue));}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
