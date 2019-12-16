#include "StepMaxMessenger.hh"

#include "StepMax.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StepMaxMessenger::StepMaxMessenger(StepMax* stepM)
:G4UImessenger(),fStepMax(stepM),
 fStepMaxDir(0),    
 fStepMaxCmd(0)
{
  fStepMaxDir = new G4UIdirectory("/testem/stepMax/");
  fStepMaxDir->SetGuidance("histograms control");
   
  fStepMaxCmd = new G4UIcommand("/testem/stepMax/absorber",this);
  fStepMaxCmd->SetGuidance("Set max allowed step length in absorber k");
  //
  G4UIparameter* k = new G4UIparameter("k",'i',false);
  k->SetGuidance("absorber number : from 1 to MaxHisto-1");
  k->SetParameterRange("k>0");
  fStepMaxCmd->SetParameter(k);
  //    
  G4UIparameter* sMax = new G4UIparameter("sMax",'d',false);
  sMax->SetGuidance("stepMax, expressed in choosen unit");
  sMax->SetParameterRange("sMax>0.");
  fStepMaxCmd->SetParameter(sMax);
  //    
  G4UIparameter* unit = new G4UIparameter("unit",'s',false);
  fStepMaxCmd->SetParameter(unit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StepMaxMessenger::~StepMaxMessenger()
{
  delete fStepMaxCmd;
  delete fStepMaxDir;
  G4cout << "StepMaxMessenger deleted" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void StepMaxMessenger::SetNewValue(G4UIcommand* command, G4String newValues)
{ 
  if (command == fStepMaxCmd)
   { G4int k; G4double sMax; 
     G4String unts;
     std::istringstream is(newValues);
     is >> k >> sMax >> unts;
     G4String unit = unts;
     G4double vUnit = G4UIcommand::ValueOf(unit);  
     fStepMax->SetStepMax(k,sMax*vUnit);
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
