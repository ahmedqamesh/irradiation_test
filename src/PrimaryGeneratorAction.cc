#include "PrimaryGeneratorAction.hh"

#include "PrimaryGeneratorMessenger.hh"
#include "DetectorConstruction.hh"
#include "HistoManager.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include "G4Gamma.hh"
#include "G4MTRunManager.hh"
#include "DataSet.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4DataVector.hh"
#include "RunAction.hh"
#include "AnalysisManager.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* det)
:G4VUserPrimaryGeneratorAction(),
 fParticleGun(0),
 fDetector(det),   
 fRndmBeam(0.),
 spectrum("off"),
 phaseSpaceGunFlag(false),
 rayleighFlag(true),
 fGunMessenger(0)

{  runAction = 0;
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);
  SetDefaultKinematic();
  
  //create a messenger for this class
  fGunMessenger = new PrimaryGeneratorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
  delete fGunMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::SetDefaultKinematic()
{
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
 //G4ParticleDefinition* particle = G4Gamma::Definition(); //Another way of defining particles
  G4ParticleDefinition* particle= particleTable->FindParticle(particleName="e-");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
  fParticleGun->SetParticleEnergy(50.*keV);
  G4double position = -0.5*(fDetector->GetWorldSizeX());
  fParticleGun->SetParticlePosition(G4ThreeVector(position,0.*cm,0.*cm));
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction::ActivatePhaseSpace(G4String fileName) {

  // load phase-space
  phaseSpaceGunFlag = true;

  // reads the data stored on disk form previous runs 
  // and get these data to data members

  AnalysisManager* analysis =  AnalysisManager::getInstance();
  analysis->LoadGunData(fileName, rayleighFlag);
   G4double position = -0.5*(fDetector->GetWorldSizeX());
  fParticleGun->SetParticlePosition(G4ThreeVector(position,0.*cm,0.*cm));

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void PrimaryGeneratorAction::SetRayleighFlag (G4bool value)
{
  rayleighFlag = value; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //retrieve runAction, if not done
  if (!runAction)
    {
      //Sequential runaction
      if (G4RunManager::GetRunManager()->GetRunManagerType() == 
	  G4RunManager::sequentialRM)
	runAction = static_cast<const RunAction*>
	  (G4RunManager::GetRunManager()->GetUserRunAction());  
      else //MT master runaction
	runAction = static_cast<const RunAction*>
	  (G4MTRunManager::GetMasterRunManager()->GetUserRunAction());  
      if (!runAction)
	G4cout << "Something wrong here!" << G4endl;
    }

  //this function is called at the begining of event
  //
  //randomize the beam, if requested.
  if (fRndmBeam > 0.) 
    {
      G4ThreeVector oldPosition = fParticleGun->GetParticlePosition();    
      G4double rbeam = 0.5*(fDetector->GetCalorSizeYZ())*fRndmBeam;
      G4double x0 = oldPosition.x();
      G4double y0 = oldPosition.y() + (2*G4UniformRand()-1.)*rbeam;
      G4double z0 = oldPosition.z() + (2*G4UniformRand()-1.)*rbeam;
      fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
      fParticleGun->GeneratePrimaryVertex(anEvent);
      fParticleGun->SetParticlePosition(oldPosition);      
    }
  //shoot particles according to a certain spectrum
  if (spectrum =="on")
    {
      G4String particle =  fParticleGun->GetParticleDefinition()->GetParticleName();
     if(particle == "proton"|| particle == "alpha")
	{
	  G4DataVector* energies =  runAction->GetEnergies();
	  G4DataVector* data =  runAction->GetData();
	 
	  G4double sum = runAction->GetDataSum();
	  G4double partSum = 0;
	  G4int j = 0;
	  G4double random= sum*G4UniformRand();
	  while (partSum<random)
	    {
	      partSum += (*data)[j];
	      j++;
	    }
	  fParticleGun->SetParticleEnergy((*energies)[j]);
	}

     else if (particle == "gamma")
	{
	  const DataSet* dataSet = runAction->GetGammaSet();
	  G4int i = 0;
	  G4int id = 0;
	  G4double minEnergy = 0. * keV;
	  G4double particleEnergy= 0.;
	  G4double maxEnergy = 50. * keV;
	  G4double energyRange = maxEnergy - minEnergy;
	   while ( i == 0)
	    {
	      G4double random = G4UniformRand();
	      G4double randomNum = G4UniformRand(); //*5.0E6;
	      particleEnergy = (random*energyRange) + minEnergy;
	      if ((dataSet->FindValue(particleEnergy,id)) > randomNum)
	      {
	    	  i = 1;
	      }
	    }
	   fParticleGun->SetParticleEnergy(particleEnergy);
	   G4AnalysisManager::Instance()->FillH1(23,particleEnergy);
	}
    }

 // using prevoiously genereated emissions from sample.....

  if (phaseSpaceGunFlag){
    G4double position = -0.5*(fDetector->GetWorldSizeX());
    //fParticleGun->SetParticleMomentumDirection(position);
    fParticleGun->SetParticlePosition(G4ThreeVector(position,0.*cm,0.*cm));   


    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
   
    const std::pair<G4double,G4String> kine = AnalysisManager::getInstance()->GetEmittedParticleEnergyAndType();

    G4double energy = kine.first;
    G4ParticleDefinition* particle = particleTable->FindParticle(kine.second);

    fParticleGun->SetParticleEnergy(energy);
    fParticleGun->SetParticleDefinition(particle);

  }

  else  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

