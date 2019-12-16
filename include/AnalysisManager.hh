
#ifndef G4PROCESSTESTANALYSIS_HH
#define G4PROCESSTESTANALYSIS_HH

#include "globals.hh"
#include <vector>
#include "G4ThreeVector.hh"
#include "DataSet.hh"
#include "AnalysisMessenger.hh"

class G4Step;
class AnalysisMessenger;

//....oooOO0OOoo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
class AnalysisManager
{
public:
 
  virtual ~AnalysisManager();
  
  void book();
  
  void finish();
  
  //fill histograms with data from XrayFluoSteppingAction
  void analyseStepping(const G4Step* aStep);
  
 //fill histograms with data from XrayFluoEventAction
  void analyseEnergyDep(G4double eDep);
  
 //fill histograms with data from XrayFluoPrimarygeneratorAction
  void analysePrimaryGenerator(G4double energy);
  
  //method to call to create an instance of this class
  static AnalysisManager* getInstance();

  // methods to set the flag for the storage of the space of phases into ntuple
  inline void PhaseSpaceOn(){phaseSpaceFlag = true;}

  inline void PhaseSpaceOff(){phaseSpaceFlag = false;}

  //method to chenge the name of the output file
  void SetOutputFileName(G4String);

  const std::pair<G4double,G4String> GetEmittedParticleEnergyAndType();

  void LoadGunData(G4String, G4bool);

  void SetPhysicFlag(G4bool);

private:
  //private constructor in order to create a singleton
  AnalysisManager();

  G4String outputFileName;

  G4bool phaseSpaceFlag;

  G4bool physicFlag;

  std::vector<G4double>* gunParticleEnergies;
  std::vector<G4String>* gunParticleTypes;

  //Instance for singleton implementation this is the returned 
  static AnalysisManager* instance;
  
  //pointer to the analysis messenger
  AnalysisMessenger* analisysMessenger;

  G4bool dataLoaded;
 
  G4int fParticleEnergyAndTypeIndex;

};
#endif



