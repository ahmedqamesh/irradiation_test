#include "StackingAction.hh"
#include "Run.hh"
#include "EventAction.hh"
#include "HistoManager.hh"
#include "StackingMessenger.hh"
#include "G4RunManager.hh"
#include "G4Track.hh"
#include "G4Gamma.hh"
#include "DetectorConstruction.hh"
#include "G4Event.hh"
#include "RunAction.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4TrackingManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
StackingAction::StackingAction(DetectorConstruction* det, EventAction* EA)
 : G4UserStackingAction(),fDetector(det),fEventAction(EA),
   fKillSecondary(0),fStackMessenger(0),fPhotoGamma(-1),fComptGamma(-1),
   fPhotoAuger(-1),fComptAuger(-1),fPixeGamma(-1),fPixeAuger(-1),fPhoto(-1),fcompton(-1),fundefined(-1),
   fIDdefined(false)
{
  fStackMessenger = new StackingMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StackingAction::~StackingAction()
{
  delete fStackMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack
StackingAction::ClassifyNewTrack(const G4Track* aTrack)
{
  //keep primary particle
  if (aTrack->GetParentID() == 0) { return fUrgent; }
  if(!fIDdefined) {
    fIDdefined = true;
    //A unique interface named G4VAtomicDeexcitation is available in Geant4 for the simulation of atomic deexcitation using Standard,
    //Low Energy and Very Low Energy electromagnetic processes. Atomic deexcitation includes fluorescence and Auger electron
    //emission induced by photons, electrons and ions (PIXE : Particle‐induced X‐ray emission )
    fPhotoGamma = G4PhysicsModelCatalog::GetIndex("phot_fluo");
    fComptGamma = G4PhysicsModelCatalog::GetIndex("compt_fluo");

    fPhotoAuger = G4PhysicsModelCatalog::GetIndex("phot_auger");
    fComptAuger = G4PhysicsModelCatalog::GetIndex("compt_auger");

    fPixeGamma 	= G4PhysicsModelCatalog::GetIndex("gammaPIXE");

    fPixeAuger	= G4PhysicsModelCatalog::GetIndex("e-PIXE");

    fPhoto	= G4PhysicsModelCatalog::GetIndex("phot");
    fcompton	= G4PhysicsModelCatalog::GetIndex("compt");
    fundefined	= G4PhysicsModelCatalog::GetIndex("undefined");
  }

  //energy spectrum of secondaries
  G4double Secondaryenergy = aTrack->GetKineticEnergy();
  G4double charge = aTrack->GetDefinition()->GetPDGCharge();
  G4int idx = aTrack->GetCreatorModelID();
  G4String Model = aTrack->GetCreatorModelName();
  G4ThreeVector position = aTrack->GetPosition();
  //G4ThreeVector vertex   = aTrack->GetVertexPosition();  

  if (charge != 0.){
	G4AnalysisManager::Instance()->FillH1(51,Secondaryenergy);
	if(idx == fPhoto)		G4AnalysisManager::Instance()->FillH1(52,Secondaryenergy);
	else if(idx == fcompton)	G4AnalysisManager::Instance()->FillH1(53,Secondaryenergy);
	else if(idx == fPhotoAuger || idx == fComptAuger)G4AnalysisManager::Instance()->FillH1(54,Secondaryenergy);
    else if(idx == fPixeAuger)	G4AnalysisManager::Instance()->FillH1(55,Secondaryenergy);
	else if(idx == fundefined)	G4AnalysisManager::Instance()->FillH1(56,Secondaryenergy);
	G4AnalysisManager::Instance()->FillH1(57,position.x()- 1* cm);
	//G4double pos = position.x()- 1* cm;
//if (pos>1.3 *cm)G4cout<<"Model = "<<Model<<"&& Creator process= "<<aTrack->GetCreatorProcess()->GetProcessName()<<"&& TrackPosition = "<<G4BestUnit(pos,"Length")<<G4endl;
  }

  if (aTrack->GetParentID() < 3 && aTrack->GetStep() == 0 && aTrack->GetParticleDefinition()->GetParticleName() == "gamma"){
	G4AnalysisManager::Instance()->FillH1(28, Secondaryenergy);
    if(idx == fPhotoGamma || idx == fComptGamma) {
     	G4AnalysisManager::Instance()->FillH1(29,Secondaryenergy);
  		}
   else if(idx == fPixeGamma) {
	    G4AnalysisManager::Instance()->FillH1(30,Secondaryenergy);
    	}
  }
  //stack or delete secondaries
  G4ClassificationOfNewTrack status = fUrgent;
  if (fKillSecondary) {
    if (aTrack->GetDefinition() == G4Gamma::Gamma()) status = fKill;
    }
 
  return status;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
