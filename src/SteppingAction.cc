#include "SteppingAction.hh"

#include "DetectorConstruction.hh"
#include "Run.hh"
#include "EventAction.hh"
#include "HistoManager.hh"
#include "G4Positron.hh"
#include "G4RunManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(DetectorConstruction* det, EventAction* evt)
:G4UserSteppingAction(),fDetector(det),fEventAct(evt),fScoringVolume(0) 
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  //track informations
  const G4StepPoint* prePoint = aStep->GetPreStepPoint();   
  const G4StepPoint* endPoint = aStep->GetPostStepPoint();
  const G4ParticleDefinition* particle = aStep->GetTrack()->GetDefinition(); 

  //if World, return
  G4VPhysicalVolume* volume = prePoint->GetTouchableHandle()->GetVolume();    
  //if sum of absorbers do not fill exactly a layer: check material, not volume.
  G4Material* mat = volume->GetLogicalVolume()->GetMaterial();
  if (mat == fDetector->GetWorldMaterial()) return; 
 
  //here we are in an absorber. Locate it
  //
  G4int absorNum  = prePoint->GetTouchableHandle()->GetCopyNumber(0);
  G4int layerNum  = prePoint->GetTouchableHandle()->GetCopyNumber(1);
  
  //get Run
  Run* run = static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
                       
  // collect energy deposit taking into account track weight
  G4double edep = aStep->GetTotalEnergyDeposit()*aStep->GetTrack()->GetWeight();
  // collect step length of charged particles
  G4double stepl = 0.;
  if (particle->GetPDGCharge() != 0.) {

    stepl = aStep->GetStepLength();
    run->AddChargedStep();
  } else { run->AddNeutralStep(); }
  
    //G4cout << "Nabs= " << absorNum << "   edep(keV)= " << edep << G4endl;
  
  // sum up per event
  fEventAct->SumEnergy(absorNum,edep,stepl);
  //longitudinal profile of edep per absorber
  if (edep>0.&& absorNum<=9) G4AnalysisManager::Instance()->FillH1(MaxAbsor+absorNum, G4double(layerNum+1), edep);


  //energy flow
  // unique identificator of layer+absorber
  G4int Idnow = (fDetector->GetNbOfAbsor())*layerNum + absorNum;
  G4int plane;
  //
  //leaving the absorber ?
  if (endPoint->GetStepStatus() == fGeomBoundary) {
    G4ThreeVector position  = endPoint->GetPosition();
    G4ThreeVector direction = endPoint->GetMomentumDirection();
    G4double sizeYZ = 0.5*fDetector->GetCalorSizeYZ();       
    G4double Eflow = endPoint->GetKineticEnergy();
    if (particle == G4Positron::Positron()) Eflow += 2*electron_mass_c2;
   if ((std::abs(position.y()) >= sizeYZ) || (std::abs(position.z()) >= sizeYZ))
                                  run->SumLateralEleak(Idnow, Eflow);
    else if (direction.x() >= 0.) run->SumEnergyFlow(plane=Idnow+1, Eflow);
    else                          run->SumEnergyFlow(plane=Idnow,  -Eflow);    
  }   


  if (!fScoringVolume) { 
    const DetectorConstruction* detectorConstruction = static_cast<const DetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detectorConstruction->GetScoringVolume();   
  }
 // get volume of the current step
  G4LogicalVolume* Absvolume  = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
 // G4double x1 = prePoint->GetPosition().x();
  //G4double x2 = endPoint->GetPosition().x();  
  //G4double x  = x1 + G4UniformRand()*(x2-x1) + 0.5*(fDetector->GetFilterSizeX());

// check if we are in scoring volume
 if (Absvolume == fScoringVolume){ //To print the names just do G4cout<<Absvolume->GetName()<<fScoringVolume->GetName()<<G4endl;

	 //Get the Secondary particles
  const std::vector<const G4Track*>*  	secondary = aStep->GetSecondaryInCurrentStep();
  size_t nbtrk = (*secondary).size();
  for (size_t lp=0; lp<nbtrk; lp++) {
	G4String Secondary_particleName = (*secondary)[lp]->GetDefinition()->GetParticleName();
   	G4double Secondary_Energy = (*secondary)[lp]->GetKineticEnergy();
	if(Secondary_particleName == "gamma")G4AnalysisManager::Instance()->FillH1(24,Secondary_Energy);	        					
	   }

//Information about primaries 
  G4double primary_Energy = aStep->GetTrack()->GetDynamicParticle()->GetKineticEnergy();
  G4String primary_particleName = aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName();
  if(primary_particleName  == "gamma") {
	  G4AnalysisManager::Instance()->FillH1(24,primary_Energy);

  }


 } // if Absvolume == fScoringVolume

//StepInfo(aStep);

////  example of Birk attenuation
///G4double destep   = aStep->GetTotalEnergyDeposit();
///G4double response = BirksAttenuation(aStep);
///G4cout << " Destep: " << destep/keV << " keV"
///       << " response after Birks: " << response/keV << " keV" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void SteppingAction::StepInfo(const G4Step* theStep)
{ // This Method is prepared for further studies
// Step is limited by conv, msc, Transportation, annihil, eBrem, eIoni
  G4cout << "Step is limited by "
   << theStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()
   << G4endl;
  G4cout << "Processes involved to the step" << G4endl;
  G4StepStatus stepStatus = fpSteppingManager->GetfStepStatus();

  if(stepStatus==fAtRestDoItProc)
  {
    G4ProcessVector* procAtRest = fpSteppingManager->GetfAtRestDoItVector();
    G4SelectedAtRestDoItVector* selProcAtRest
     = fpSteppingManager->GetfSelectedAtRestDoItVector();
    size_t MAXofAtRestLoops = fpSteppingManager->GetMAXofAtRestLoops();
    for(size_t i1=0;i1<MAXofAtRestLoops;i1++)
    {
      if((*selProcAtRest)[MAXofAtRestLoops-i1-1]==2)
      { G4cout << "  At rest : " << (*procAtRest)[i1]->GetProcessName() << " (forced)" << G4endl; }
      else if((*selProcAtRest)[MAXofAtRestLoops-i1-1]==1)
      { G4cout << "  At rest : " << (*procAtRest)[i1]->GetProcessName() << G4endl; }
    }
  }

  if(stepStatus!=fExclusivelyForcedProc && stepStatus!=fAtRestDoItProc)
  {
    G4ProcessVector* procAlong = fpSteppingManager->GetfAlongStepDoItVector();
    size_t MAXofAlongStepLoops = fpSteppingManager->GetMAXofAlongStepLoops();
    for(size_t i2=0;i2<MAXofAlongStepLoops;i2++)
    {
      if((*procAlong)[i2]!=0)
      G4cout << "  Along step : " << (*procAlong)[i2]->GetProcessName() << G4endl;
    }
  }

  if(stepStatus!=fAtRestDoItProc)
  {
    G4ProcessVector* procPost = fpSteppingManager->GetfPostStepDoItVector();
    G4SelectedPostStepDoItVector* selProcPost
     = fpSteppingManager->GetfSelectedPostStepDoItVector();
    size_t MAXofPostStepLoops = fpSteppingManager->GetMAXofPostStepLoops();
    for(size_t i3=0;i3<MAXofPostStepLoops;i3++)
    {
      if((*selProcPost)[MAXofPostStepLoops-i3-1]==2)
      { G4cout << "  Post step : " << (*procPost)[i3]->GetProcessName() << " (forced)" << G4endl; }
      else if((*selProcPost)[MAXofPostStepLoops-i3-1]==1)
      { G4cout << "  Post step : " << (*procPost)[i3]->GetProcessName() << G4endl; }
    }
  }

  G4int nSecAtRest = fpSteppingManager->GetfN2ndariesAtRestDoIt();
  G4int nSecAlong  = fpSteppingManager->GetfN2ndariesAlongStepDoIt();
  G4int nSecPost   = fpSteppingManager->GetfN2ndariesPostStepDoIt();
  G4int nSecTotal  = nSecAtRest+nSecAlong+nSecPost;
  G4TrackVector* secVec = fpSteppingManager->GetfSecondary();

  if(nSecTotal>0)
  {
    G4cout << "  :----- List of 2ndaries - " << std::setw(3) << nSecTotal
           << " (Rest=" << std::setw(2) << nSecAtRest
           << ",Along=" << std::setw(2) << nSecAlong
           << ",Post="  << std::setw(2) << nSecPost << ")" << G4endl;

    for(size_t lp1=(*secVec).size()-nSecTotal; lp1<(*secVec).size(); lp1++)
    {
      G4cout << "    : "
             << G4BestUnit((*secVec)[lp1]->GetPosition(), "Length") << " "
             << std::setw( 9) << G4BestUnit((*secVec)[lp1]->GetKineticEnergy() , "Energy") << " "
             << std::setw(18) << (*secVec)[lp1]->GetDefinition()->GetParticleName()
             << " generated by " << (*secVec)[lp1]->GetCreatorProcess()->GetProcessName() << G4endl;
    }
  }

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double SteppingAction::BirksAttenuation(const G4Step* aStep)
{
 //Example of Birk attenuation law in organic scintillators.
 //adapted from Geant3 PHYS337. See MIN 80 (1970) 239-244
 G4Material* material = aStep->GetTrack()->GetMaterial();
 G4double birk1       = material->GetIonisation()->GetBirksConstant();
 G4double destep      = aStep->GetTotalEnergyDeposit();
 G4double stepl       = aStep->GetStepLength();  
 G4double charge      = aStep->GetTrack()->GetDefinition()->GetPDGCharge();
 G4double response = destep;
 if (birk1*destep*stepl*charge != 0.)
   {
     response = destep/(1. + birk1*destep/stepl);
   }
 return response;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

