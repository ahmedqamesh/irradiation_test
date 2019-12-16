

#include "StepMax.hh"
#include "StepMaxMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StepMax::StepMax(const G4String& processName)
 : G4VDiscreteProcess(processName),fMess(0) 
{
  for (G4int k=0; k<MaxAbsor; k++) fStepMax[k] = DBL_MAX;
  fMess = new StepMaxMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StepMax::~StepMax() { delete fMess; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool StepMax::IsApplicable(const G4ParticleDefinition& particle)
{
  return (particle.GetPDGCharge() != 0. && !particle.IsShortLived());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void StepMax::SetStepMax(G4int k,G4double step) {fStepMax[k] = step;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


