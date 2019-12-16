#ifndef RunAction_h
#define RunAction_h 1
#include "G4UserRunAction.hh"
#include "globals.hh"
#include <map>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class Run; 
class DetectorConstruction;
class PrimaryGeneratorAction;
class RunActionMessenger;
class HistoManager;
class G4Timer;

class G4Run;
class DataSet;
class G4DataVector;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class RunAction : public G4UserRunAction
{
public:

  RunAction(DetectorConstruction*, PrimaryGeneratorAction* prim=0);
 ~RunAction();

  virtual G4Run* GenerateRun();

   
  // Acceptance parameters
  void SetEdepAndRMS(G4int, G4double, G4double, G4double);
  void SetApplyLimit(G4bool val);
  void SetFileName  (G4String val);
  const DataSet* GetSet() const;
  const DataSet* GetGammaSet() const;

  G4DataVector* GetEnergies() const;
  G4DataVector* GetData() const;


public:
  void BeginOfRunAction(const G4Run*);
  void   EndOfRunAction(const G4Run*);
  //returns the sum of the element of data
  G4double GetDataSum() const;
  G4String GetFileName(){return File;}

private:
  void Initialise();
  G4bool isInitialized;

  const DataSet* dataSet;
  const DataSet* dataGammaSet;  //stores the data of the incident gamma spectrum
  G4DataVector* energies; //stores the energy data of the proton and alpha spectra
  G4DataVector* data;//stores the data of the proton and alpha spectra
  void ReadData(G4double,G4String);//read the data for protons and alpha spectra
  G4String File ;
  DetectorConstruction*   fDetector;
  PrimaryGeneratorAction* fPrimary;
  Run*                    fRun;      
  RunActionMessenger*     fRunMessenger;
  HistoManager*           fHistoManager;
  G4Timer*                fTimer;
};


#endif

