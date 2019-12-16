#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "G4DataVector.hh"
#include "G4ThreeVector.hh"
#include <vector>


class G4ParticleGun;
class G4Event;
class DetectorConstruction;
class PrimaryGeneratorMessenger;
class RunAction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction(DetectorConstruction*);    
   ~PrimaryGeneratorAction();

  public:
    void SetDefaultKinematic();
    void SetRndmBeam(G4double val) { fRndmBeam = val;}
    void SetSpectrum (G4String val) { spectrum= val  ;} 
    void ActivatePhaseSpace(G4String);
    void SetRayleighFlag (G4bool);

    virtual 
    void GeneratePrimaries(G4Event*);
    G4ParticleGun* GetParticleGun() {return fParticleGun;};
    
  private:
    G4ParticleGun*         fParticleGun;
    DetectorConstruction*  fDetector;   
    const  RunAction* runAction;//pointer to the run manager (master)
    G4double fRndmBeam;   //lateral random beam extension in fraction sizeYZ/2   
    G4String spectrum;	  //flag  for shooting particles according to certain spectra 
  G4bool phaseSpaceGunFlag;//flag for the use of phase-space created with a previous run
  G4bool rayleighFlag;   // the flag to load  particle coming from Rayleigh scattering

    PrimaryGeneratorMessenger* fGunMessenger; 
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


