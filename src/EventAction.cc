#include "EventAction.hh"
//#include "EventActionMessenger.hh"
#include "Run.hh"
#include "HistoManager.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VHitsCollection.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include "TrackerHit.hh"
//#include "TrackerSD.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(DetectorConstruction* det)
:G4UserEventAction(),fDetector(det),fTransmitFlag(0), fReflectFlag(0)
{ 
 verboseLevel = 1;
 HHC1ID  = -1;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{       
  //initialize EnergyDeposit per event
  //
  for (G4int k=0; k<MaxAbsor; k++) {
    fEnergyDeposit[k] = fTrackLengthCh[k] = 0.0;   
  }
 fTransmitFlag   = fReflectFlag    = 0; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TrackerHitsCollection* EventAction::GetHitsCollection(G4int hcID,const G4Event* event) const
{
  TrackerHitsCollection* hitsCollection = static_cast<TrackerHitsCollection*>(event->GetHCofThisEvent()->GetHC(hcID));
  
  if ( ! hitsCollection ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID; 
    G4Exception("EventAction::GetHitsCollection()",
      "MyCode0003", FatalException, msg);
  }         

  return hitsCollection;
}   


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
  //get Run
  Run* run = static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());     
  //G4PrimaryParticle* primary = event->GetPrimaryVertex(0)->GetPrimary(0);        
  for (G4int k=1; k<=fDetector->GetNbOfAbsor(); k++) {
     run->FillPerEvent(k,fEnergyDeposit[k],fTrackLengthCh[k]);
     if (fEnergyDeposit[k] > 0. && k<=9)G4AnalysisManager::Instance()->FillH1(k, fEnergyDeposit[k]);  // fEnergyDeposit is equivalent to primary->GetKineticEnergy()
   else continue;
  }

if (HHC1ID<0){HHC1ID = G4SDManager::GetSDMpointer()->GetCollectionID("hitsCollection");}

 TrackerHitsCollection* HHC1 = GetHitsCollection(HHC1ID, event);
 TrackerHit* Hit = (*HHC1)[HHC1->entries()-1]; 
  // fill histograms
  G4AnalysisManager::Instance()->FillH1(27, Hit->GetEdep());
 run->CountTransmit(fTransmitFlag);
 run->CountReflect (fReflectFlag);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::PrintEventStatistics(G4double FilterEdep, G4double FilterTrackLength, G4int nhit) const
{
   // print event statistics
  //G4int eventID = event->GetEventID();
  G4cout
     << "    Filter has:" 
     << std::setw(7) << nhit << " hits." 
     << "       Total energy: " 
     << std::setw(7) << G4BestUnit(FilterEdep, "Energy")
     << "       Total track length: " 
     << std::setw(7) << G4BestUnit(FilterTrackLength, "Length")
     << G4endl;

}
