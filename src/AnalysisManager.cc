#include "g4root.hh"
#include "G4VProcess.hh"
#include "AnalysisManager.hh"
#include "G4Step.hh"
#include "DetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Proton.hh"
#include "G4SystemOfUnits.hh"

AnalysisManager* AnalysisManager::instance = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

namespace { 
  //Mutex to acquire access to singleton instance check/creation
  G4Mutex instanceMutex = G4MUTEX_INITIALIZER;
  //Mutex to acquire accss to histograms creation/access
  //It is also used to control all operations related to histos 
  //File writing and check analysis
  G4Mutex dataManipulationMutex = G4MUTEX_INITIALIZER;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

AnalysisManager::AnalysisManager()
  :outputFileName("xray"), phaseSpaceFlag(false), physicFlag (false),
   gunParticleEnergies(0), gunParticleTypes(0)
{
  dataLoaded = false;
  fParticleEnergyAndTypeIndex = 0;

  //creating the messenger
  analisysMessenger = new AnalysisMessenger(this);
  
  //Instantiate the analysis manager
  G4AnalysisManager::Instance();

  G4cout << "AnalysisManager created" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

AnalysisManager::~AnalysisManager() 
{
  if ( gunParticleEnergies ) delete gunParticleEnergies;
  gunParticleEnergies = 0;
  if ( gunParticleTypes ) delete gunParticleTypes;
  gunParticleTypes = 0;
  delete instance;
  instance = 0;
  
  delete G4AnalysisManager::Instance();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

AnalysisManager* AnalysisManager::getInstance()

{
  G4AutoLock l(&instanceMutex);
  if (instance == 0) {instance = new AnalysisManager;}
  return instance;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void AnalysisManager::book()
{
  G4AutoLock l(&dataManipulationMutex);
  // Get analysis manager
  G4AnalysisManager* man = G4AnalysisManager::Instance();
  // Open an output file
  man->OpenFile(outputFileName);
  man->SetVerboseLevel(1);
  man->SetFirstHistoId(1);
  man->SetFirstNtupleId(1);

  G4cout << "Open output file: " << outputFileName << G4endl;

  if (phaseSpaceFlag) 
    {   
      // Book output Tuple (ID = 1)
      man->CreateNtuple("101","OutputNTuple");
      man->CreateNtupleIColumn("particle");      
      man->CreateNtupleDColumn("energies");
      man->CreateNtupleDColumn("momentumTheta");
      man->CreateNtupleDColumn("momentumPhi");
      man->CreateNtupleIColumn("processes");
      man->CreateNtupleIColumn("material");
      man->CreateNtupleIColumn("origin");
      man->CreateNtupleDColumn("depth");
      man->FinishNtuple();
      G4cout << "Created ntuple for phase space" << G4endl;
    }
  else {
    // Book histograms
    man->CreateH1("h1","Energy Deposit", 500,0.,10.); //20eV def.
    man->CreateH1("h2","Gamma born in the sample", 100,0.,10.);
    man->CreateH1("h3","Electrons  born in the sample", 100,0.,10.);
    man->CreateH1("h4","Gammas leaving the sample", 300,0.,10.);
    man->CreateH1("h5","Electrons leaving the sample ",200000 ,0.,10.0); // .05 eV def.
    man->CreateH1("h6","Gammas reaching the detector", 100,0.,10.);
    man->CreateH1("h7","Spectrum of the incident particles", 100,0.,10.);
    man->CreateH1("h8","Protons reaching the detector", 100,0.,10.);
    man->CreateH1("h9","Protons leaving the sample", 100,0.,10.);
    G4cout << "Created histos" << G4endl;
  }
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void AnalysisManager::LoadGunData(G4String fileName, G4bool raileighFlag) 
{
  G4AutoLock l(&dataManipulationMutex);

  //Do not allow more than one thread to trigger the file reading
  if (dataLoaded)
    return;

  // Get analysis reader manager
  G4AnalysisReader* analysisReader = G4AnalysisReader::Instance();
  analysisReader->SetVerboseLevel(1);

  //This is for testing purposes
  G4int ntupleId = analysisReader->GetNtuple("101",fileName);
  G4cout << "Got ntupleId: " << ntupleId << G4endl;

  gunParticleEnergies = new std::vector<G4double>;
  gunParticleTypes = new std::vector<G4String>;

  G4int particleID, processID;
  G4double energy;
  analysisReader->SetNtupleIColumn("processes", processID);
  analysisReader->SetNtupleDColumn("energies", energy);
  analysisReader->SetNtupleIColumn("particles", particleID);

  while (analysisReader->GetNtupleRow() ) 
    {
      if (raileighFlag ^ (!raileighFlag && (processID == 1 || 
					    processID == 2)) )  
	{
	  gunParticleEnergies->push_back(energy*MeV);
	  if (particleID == 1)
	    gunParticleTypes->push_back("gamma");
	  else if (particleID == 0)
	    gunParticleTypes->push_back("e-");
	}
    }

  G4cout << "Maximum mumber of events: "<< gunParticleEnergies->size() <<G4endl;  

  dataLoaded= true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

const std::pair<G4double,G4String> AnalysisManager::GetEmittedParticleEnergyAndType() 
{
  G4AutoLock l(&dataManipulationMutex);
  std::pair<G4double,G4String> result;
  
  if(fParticleEnergyAndTypeIndex < (G4int) gunParticleEnergies->size())
    {
      G4double energy = gunParticleEnergies->at(fParticleEnergyAndTypeIndex);
      G4String name = gunParticleTypes->at(fParticleEnergyAndTypeIndex);
      result.first = energy;
      result.second = name;
    }

  fParticleEnergyAndTypeIndex++;
  return result;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void AnalysisManager::SetPhysicFlag(G4bool val)
{
  physicFlag = val;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void AnalysisManager::finish()
{ 
  G4AutoLock l(&dataManipulationMutex);
  G4cout << "Going to save histograms" << G4endl;
  // Save histograms
  G4AnalysisManager* man = G4AnalysisManager::Instance();
  man->Write();
  man->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void AnalysisManager::SetOutputFileName(G4String newName)
{
  
  outputFileName = newName;
}


