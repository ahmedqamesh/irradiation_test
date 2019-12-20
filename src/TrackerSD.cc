#include "TrackerSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include "HistoManager.hh"
#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TrackerSD::TrackerSD( const G4String& name,
                            const G4String& hitsCollectionName,
                            G4int nofCells)
 : G4VSensitiveDetector(name),
   fHitsCollection(0),
   fNofCells(nofCells)
{
  collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackerSD::~TrackerSD() 
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackerSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection  = new TrackerHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection ); 

  // Create hits
  // fNofCells for cells + one more for total sums 
  for (G4int i=0; i<fNofCells+1; i++ ) {
    fHitsCollection->insert(new TrackerHit());
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool TrackerSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{  
  // energy deposit
  G4double edep = step->GetTotalEnergyDeposit(); // The energy lost to the medium = Edep by Eloss process+ Elost by secondaries which have NOT been generated because thier E below the cut threshold.

// Non Ionizing Energy Deposit
  G4double NonIonedep = step->GetNonIonizingEnergyDeposit();

//Ionizing Energy Deposit
G4double Ionedep=edep-NonIonedep;
  G4AnalysisManager::Instance()->FillH1(26,Ionedep);
  // step length
  G4double stepLength = 0.;
  if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. ) stepLength = step->GetStepLength();
 G4String thisVolum  = step->GetTrack()->GetNextVolume()->GetName();
 G4String volumePos= step->GetTrack()->GetVolume()->GetName();

 if (volumePos == thisVolum){
 //primary particles
  G4String primary_particleName = step->GetTrack()->GetParticleDefinition()->GetParticleName();
  G4double primary_Energy 	= step->GetTrack()->GetKineticEnergy();
 //G4cout<<   primary_particleName <<">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"<<primary_Energy<<G4endl;
 // Secondary particles
  const std::vector<const G4Track*>*  	secondary = step->GetSecondaryInCurrentStep();
  size_t nbtrk = (*secondary).size();
  for (size_t lp=0; lp<nbtrk; lp++) {
	G4String Secondary_particleName = (*secondary)[lp]->GetDefinition()->GetParticleName();
	G4double Secondary_Energy = (*secondary)[lp]->GetKineticEnergy();
	if(Secondary_particleName == "gamma")G4AnalysisManager::Instance()->FillH1(25,Secondary_Energy);
     }

  //Fill the same Histogram with primary Energy if it is gamma (it will fill the Histogram if Gamma spectrum used as input)
  if(primary_particleName   == "gamma" && primary_Energy > 0.15)G4AnalysisManager::Instance()->FillH1(25,primary_Energy);

  } //if volumePos == thisVolum

  if ( edep==0. && stepLength == 0. ) return false;      

  G4TouchableHistory* touchable = (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());
    
  // Get Detector cell id
  G4int layerNumber = touchable->GetReplicaNumber(1);
  
  // Get hit accounting data for this cell
  TrackerHit* hit = (*fHitsCollection)[layerNumber];
  if ( ! hit ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hit " << layerNumber; 
    G4Exception("TrackerSD::ProcessHits()", "MyCode0004", FatalException, msg);
  }         

  // Get hit for total accounting
  TrackerHit* hitTotal = (*fHitsCollection)[fHitsCollection->entries()-1];
  
  // Add values
  hit->Add(edep, stepLength);
  hitTotal->Add(edep, stepLength); 
      
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackerSD::EndOfEvent(G4HCofThisEvent*)
{
  if ( verboseLevel>1 ) { 
     G4int nofHits = fHitsCollection->entries();
     G4cout
       << G4endl 
       << "-------->Hits Collection: in this event they are " << nofHits 
       << " hits : " << G4endl;
     for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
