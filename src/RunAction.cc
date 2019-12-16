#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunActionMessenger.hh"
#include "HistoManager.hh"
#include "Run.hh"
#include "G4Timer.hh"
#include "G4RunManager.hh"
#include "Randomize.hh"
#include "DataSet.hh"
#include "Normalization.hh"
#include "G4SystemOfUnits.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* prim)
:G4UserRunAction(),isInitialized(false),dataSet(0), dataGammaSet(0),fDetector(det), fPrimary(prim), fRun(0),
   fRunMessenger(0),fHistoManager(0),fTimer(0)
{
  fRunMessenger = new RunActionMessenger(this);
  fHistoManager = new HistoManager();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  delete fRunMessenger;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void RunAction::Initialise()
{
  //Only the master is initialized and keeps the data
  //(or in sequential mode)
  if (!IsMaster())
    return;

  Normalization normalization;
  energies = new G4DataVector;
  data = new G4DataVector;
  //File = "File1";
  File = GetFileName();
  ReadData(keV,File);

  G4double minGamma = 0.*keV;
  G4double maxGamma = 60. *keV;
  G4int nBinsGamma = 200;

  dataGammaSet = normalization.Normalize(minGamma, maxGamma, nBinsGamma,File);
  isInitialized = true;
  G4cout << "RunAction initialized" << G4endl;  
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Run* RunAction::GenerateRun()
{ 
  fRun = new Run(fDetector); 
  return fRun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{
  // keep run condition
  if ( fPrimary ) { 

    G4ParticleDefinition* particle = fPrimary->GetParticleGun()->GetParticleDefinition();
    G4double energy = fPrimary->GetParticleGun()->GetParticleEnergy();
    fRun->SetPrimary(particle, energy);
}
       
  //histograms
  //
  G4AnalysisManager* analysis = G4AnalysisManager::Instance();
  if (analysis->IsActive()) analysis->OpenFile();

  // save Rndm status and open the timer
  if (isMaster) {
    G4Random::showEngineStatus();
    fTimer = new G4Timer();
    fTimer->Start();
      G4cout << "### Run " << fRun->GetRunID() << " starts (master)." << G4endl;
      if (!isInitialized)
	Initialise();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run*)
{
  // compute and print statistic 
  if (isMaster) {
    fTimer->Stop();
    if(!((G4RunManager::GetRunManager()->GetRunManagerType() ==
          G4RunManager::sequentialRM))) {
      G4cout << "\n" << "Total number of events:  "  
             << fRun->GetNumberOfEvent() << G4endl;
      G4cout << "Master thread time:  "  << *fTimer << G4endl;
    }
    delete fTimer;
    fRun->EndOfRun();
  }
  //save histograms
  G4AnalysisManager* analysis = G4AnalysisManager::Instance();   
  if (analysis->IsActive()) {   
    analysis->Write();
    analysis->CloseFile();
  }    

  // show Rndm status
  if (isMaster)  G4Random::showEngineStatus();   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::SetEdepAndRMS(G4int i,G4double edep,G4double rms,G4double lim)
{
  if (fRun) fRun->SetEdepAndRMS(i, edep, rms, lim);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::SetApplyLimit(G4bool val)        
{
  if (fRun) fRun->SetApplyLimit(val);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
const DataSet* RunAction::GetSet() const
{
  return  dataSet;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
const DataSet* RunAction::GetGammaSet() const
{
  return  dataGammaSet;

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4DataVector* RunAction::GetEnergies() const
{
  return energies;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4DataVector* RunAction::GetData() const
{
  return data;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double RunAction::GetDataSum() const
{
 
  G4double sum = 0;
  for (size_t i = 0; i < data->size(); i++)
    {
      sum+=(*data)[i];
    }
  return sum;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void RunAction::ReadData(G4double unitE, G4String fileName)
{
  G4cout << "Reading data...";
  std::ostringstream ost;
  
  ost << fileName <<".dat";
  
  G4String name = ost.str();
  char* path;
  
  if (!(getenv("XRAYDATA"))) { 
    
    path = getenv("PWD");    
  }
  
  else {    
    path = getenv("XRAYDATA");
  }
  
  
  G4String pathString(path);
  name = pathString + "/" + name;
  
  
  std::ifstream file(name);
  std::filebuf* lsdp = file.rdbuf();
  
  if (! (lsdp->is_open()) )
    {
      G4ExceptionDescription execp;
      execp <<  "RunAction - data file: " + name + " not found";
      G4Exception("RunAction::ReadData()","example",
	  FatalException, execp);
    }
  G4double a = 0;
  G4int k = 1;
  
  do
    {
      file >> a;
      G4int nColumns = 2;
      // The file is organized into two columns:
      // 1st column is the energy
      // 2nd column is the corresponding value
      // The file terminates with the pattern: -1   -1
      //                                       -2   -2
      if (a == -1 || a == -2)
	{
	  
	}
      else
	{
	  if (k%nColumns != 0)
	    {	
	      G4double e = a * unitE;
	      
	      energies->push_back(e);
	      
	      k++;
	      
	    }
	  else if (k%nColumns == 0)
	    {
	      G4double value = a;
	      data->push_back(value);
	      
	      k = 1;
	    }
	}
      
    } while (a != -2); // end of file
  
  file.close();
  G4cout << "Reading Data is done" << G4endl;
}

void RunAction::SetFileName(G4String file)
{
	if (file) File = file;
	else
		File = "File1";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
