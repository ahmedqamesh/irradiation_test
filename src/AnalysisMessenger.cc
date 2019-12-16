#include "AnalysisMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

AnalysisMessenger::AnalysisMessenger(AnalysisManager* analysisManager)
  :Analysis(analysisManager)
  
{ 
  AnalysisDir = new G4UIdirectory("/analysis/");
  AnalysisDir->SetGuidance("analysis control.");
  
  outputFileCommand = new G4UIcmdWithAString("/analysis/outputFile",this);
  outputFileCommand->SetGuidance("specify the name of the output file");
  outputFileCommand->SetParameterName("choice",true);
  outputFileCommand->SetDefaultValue("File.root");
  outputFileCommand->AvailableForStates(G4State_Idle);
    

  physicFlagCmd = new G4UIcmdWithABool("/analysis/setPhysicProduction",this);
  physicFlagCmd->SetGuidance("Select if data stored in the Phase-Space must contain physical data or particles exiting the sample");
  physicFlagCmd->SetGuidance("To be used before and togheter with /gun/loadGunData");
  physicFlagCmd->SetParameterName("Physyc Flag",true);
  physicFlagCmd->SetDefaultValue(false);
  physicFlagCmd->AvailableForStates(G4State_Idle);

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

AnalysisMessenger::~AnalysisMessenger()
{
  
  delete AnalysisDir; 
 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void AnalysisMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 

  if(command == outputFileCommand)
    {      
      Analysis->SetOutputFileName(newValue);
    }
  
  if( command == physicFlagCmd )
    { 
      G4bool newPhysFlag = physicFlagCmd->GetNewBoolValue(newValue);
      Analysis->SetPhysicFlag(newPhysFlag);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....















