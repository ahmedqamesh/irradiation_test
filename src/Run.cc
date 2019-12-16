#include "Run.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "HistoManager.hh"
#include "EmAcceptance.hh"

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Track.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......21 21 

Run::Run(DetectorConstruction* det)
: G4Run(),
  fDetector(det), 
  fParticle(0), fEkin(0.),
  fChargedStep(0), fNeutralStep(0),
  fN_gamma(0), fN_elec(0), fN_pos(0),
  fApplyLimit(false)
{
  //initialize cumulative quantities
  // 
  for (G4int k=0; k<MaxAbsor; k++) {
    fSumEAbs[k] = fSum2EAbs[k]  = fSumLAbs[k] = fSum2LAbs[k] = 0.;
    fEnergyDeposit[k].clear();
    fEdeptrue[k] = fRmstrue[k] = 1.;
    fLimittrue[k] = DBL_MAX;  
  }

  //initialize Eflow
  //
  G4int nbPlanes = (fDetector->GetNbOfLayers())*(fDetector->GetNbOfAbsor()) + 2;
  fEnergyFlow.resize(nbPlanes);
  fLateralEleak.resize(nbPlanes);
  for (G4int k=0; k<nbPlanes; k++) {fEnergyFlow[k] = fLateralEleak[k] = 0.; }  
  //intialize Transmit and reflect for the first absorber
  fTransmit[0] = fTransmit[1] = fReflect[0] = fReflect[1] = 0;
  fMscProjecTheta = fMscProjecTheta2 = 0.;
  fMscThetaCentral = 0.;
  fMscEntryCentral = 0;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Run::~Run()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::SetPrimary(G4ParticleDefinition* particle, G4double energy)
{ 
  fParticle = particle;
  fEkin = energy;
  //compute theta0
  fMscThetaCentral = 3*ComputeMscHighland();


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::FillPerEvent(G4int kAbs, G4double EAbs, G4double LAbs)
{
  //accumulate statistic with restriction
  //
  if(fApplyLimit) fEnergyDeposit[kAbs].push_back(EAbs);
  fSumEAbs[kAbs]  += EAbs;  fSum2EAbs[kAbs]  += EAbs*EAbs;
  fSumLAbs[kAbs]  += LAbs;  fSum2LAbs[kAbs]  += LAbs*LAbs;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::SumEnergyFlow(G4int plane, G4double Eflow)
{
  fEnergyFlow[plane] += Eflow;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
                                            
void Run::SumLateralEleak(G4int cell, G4double Eflow)
{
  fLateralEleak[cell] += Eflow;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
                                            
void Run::AddChargedStep() 
{
  fChargedStep += 1.0; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    
void Run::AddNeutralStep() 
{
  fNeutralStep += 1.0; 
}
    
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::AddSecondaryTrack(const G4Track* track)
{
  const G4ParticleDefinition* d = track->GetDefinition();
  if(d == G4Gamma::Gamma()) { ++fN_gamma; }
  else if (d == G4Electron::Electron()) { ++fN_elec; }
  else if (d == G4Positron::Positron()) { ++fN_pos; }
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::Merge(const G4Run* run)
{
  const Run* localRun = static_cast<const Run*>(run);

  // pass information about primary particle
  fParticle = localRun->fParticle;
  fEkin     = localRun->fEkin;

  // accumulate sums
  //
  for (G4int k=0; k<MaxAbsor; k++) {
    fSumEAbs[k]  += localRun->fSumEAbs[k]; 
    fSum2EAbs[k] += localRun->fSum2EAbs[k]; 
    fSumLAbs[k]  += localRun->fSumLAbs[k]; 
    fSum2LAbs[k] += localRun->fSum2LAbs[k];

  }
   
  G4int nbPlanes = (fDetector->GetNbOfLayers())*(fDetector->GetNbOfAbsor()) + 2;
  for (G4int k=0; k<nbPlanes; k++) {
    fEnergyFlow[k]   += localRun->fEnergyFlow[k];
    fLateralEleak[k] += localRun->fLateralEleak[k];
  }
  
       
  fChargedStep += localRun->fChargedStep;  
  fNeutralStep += localRun->fNeutralStep;
  
  fN_gamma += localRun->fN_gamma;    
  fN_elec  += localRun->fN_elec;
  fN_pos   += localRun->fN_pos;
     
  fApplyLimit = localRun->fApplyLimit;
  
  for (G4int k=1; k<=MaxAbsor; k++) {
    fEdeptrue[k]  = localRun->fEdeptrue[k]; 
    fRmstrue[k]   = localRun->fRmstrue[k]; 
    fLimittrue[k] = localRun->fLimittrue[k]; 
   if (k==MaxAbsor){
 	   fMscThetaCentral = localRun->fMscThetaCentral;
  	   fMscProjecTheta  += localRun->fMscProjecTheta;
  	   fMscProjecTheta2 += localRun->fMscProjecTheta2;
 	   fMscEntryCentral += localRun->fMscEntryCentral;

  	   fTransmit[0] += localRun->fTransmit[0];  
  	   fTransmit[1] += localRun->fTransmit[1];
	
  	   fReflect[0]  += localRun->fReflect[0];
  	   fReflect[1]  += localRun->fReflect[1];
	}
}


  G4Run::Merge(run); 
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::EndOfRun()
{
  G4int nEvt = numberOfEvent;
  G4double  norm = G4double(nEvt);
  if(norm > 0) norm = 1./norm;
  G4double qnorm = std::sqrt(norm);

  fChargedStep *= norm;
  fNeutralStep *= norm;

  //compute and print statistic
  //
  G4double beamEnergy = fEkin;
  G4double sqbeam = std::sqrt(beamEnergy/GeV);

  G4double MeanEAbs,MeanEAbs2,rmsEAbs,resolution,rmsres;
  G4double MeanLAbs,MeanLAbs2,rmsLAbs;

  std::ios::fmtflags mode = G4cout.flags();
  G4int  prec = G4cout.precision(2);
  G4cout << "\n------------------------------------------------------------\n";
  G4cout << std::setw(14) << "material"
         << std::setw(17) << "Edep       RMS"
         << std::setw(33) << "sqrt(E0(GeV))*rmsE/Emean"
         << std::setw(23) << "total tracklen \n \n";

  for (G4int k=1; k<=fDetector->GetNbOfAbsor(); k++)
    {
      MeanEAbs  = fSumEAbs[k]*norm;
      MeanEAbs2 = fSum2EAbs[k]*norm;
      rmsEAbs  = std::sqrt(std::abs(MeanEAbs2 - MeanEAbs*MeanEAbs));
      //G4cout << "k= " << k << "  RMS= " <<  rmsEAbs 
      //     << "  fApplyLimit: " << fApplyLimit << G4endl;
      if(fApplyLimit) {
        G4int    nn    = 0;
        G4double sume  = 0.0;
        G4double sume2 = 0.0;
        // compute trancated means  
        G4double lim   = rmsEAbs * 2.5;
        for(G4int i=0; i<=nEvt; i++) {
          G4double e = (fEnergyDeposit[k])[i];
          if(std::abs(e - MeanEAbs) < lim) {
            sume  += e;
            sume2 += e*e;
            nn++;
          }
        }
        G4double norm1 = G4double(nn);
        if(norm1 > 0.0) norm1 = 1.0/norm1;
        MeanEAbs  = sume*norm1;
        MeanEAbs2 = sume2*norm1;
        rmsEAbs  = std::sqrt(std::abs(MeanEAbs2 - MeanEAbs*MeanEAbs));
      }

      resolution= 100.*sqbeam*rmsEAbs/MeanEAbs;
      rmsres    = resolution*qnorm;

      // Save mean and RMS
      fSumEAbs[k] = MeanEAbs;
      fSum2EAbs[k] = rmsEAbs;

      MeanLAbs  = fSumLAbs[k]*norm;
      MeanLAbs2 = fSum2LAbs[k]*norm;
      rmsLAbs  = std::sqrt(std::abs(MeanLAbs2 - MeanLAbs*MeanLAbs));

      //print
      G4cout
       << std::setw(14) << fDetector->GetAbsorMaterial(k)->GetName() << ": "
       << std::setprecision(5)
       << std::setw(6) << G4BestUnit(MeanEAbs,"Energy") << " :  "
       << std::setprecision(4)
       << std::setw(5) << G4BestUnit( rmsEAbs,"Energy")  
       << std::setw(10) << resolution  << " +- " 
       << std::setw(5) << rmsres << " %"
       << std::setprecision(3)
       << std::setw(10) << G4BestUnit(MeanLAbs,"Length")  << " +- "
       << std::setw(4) << G4BestUnit( rmsLAbs,"Length")
       << G4endl;
    }

  G4cout << "\n------------------------------------------------------------\n";

  G4cout << " Beam particle " 
         << fParticle->GetParticleName()
         << "  E = " << G4BestUnit(beamEnergy,"Energy") << G4endl;
  G4cout << " Mean number of gamma       " << (G4double)fN_gamma*norm << G4endl;
  G4cout << " Mean number of e-          " << (G4double)fN_elec*norm << G4endl;
  G4cout << " Mean number of e+          " << (G4double)fN_pos*norm << G4endl;
  G4cout << std::setprecision(6)
         << " Mean number of charged steps  " << fChargedStep << G4endl;
  G4cout << " Mean number of neutral steps  " << fNeutralStep << G4endl;
  G4cout << "------------------------------------------------------------\n";
  
  //Energy flow
  //
  G4AnalysisManager* analysis = G4AnalysisManager::Instance();
  G4int Idmax = (fDetector->GetNbOfLayers())*(fDetector->GetNbOfAbsor());

  for (G4int Id=1; Id<=Idmax+1; Id++) {
    analysis->FillH1(21, (G4double)Id, fEnergyFlow[Id]);
    analysis->FillH1(22, (G4double)Id, fLateralEleak[Id]);

  }

  //Energy deposit from energy flow balance
  //
  G4double EdepTot[MaxAbsor];
  for (G4int k=0; k<MaxAbsor; k++) EdepTot[k] = 0.;
  G4int nbOfAbsor = fDetector->GetNbOfAbsor();
  for (G4int Id=1; Id<=Idmax; Id++) {
   G4int iAbsor = Id%nbOfAbsor; if (iAbsor==0) iAbsor = nbOfAbsor; 
   EdepTot[iAbsor] += (fEnergyFlow[Id] - fEnergyFlow[Id+1] - fLateralEleak[Id]);
  }
  // Acceptance
  EmAcceptance acc;
  G4bool isStarted = false;
  for (G4int j=1; j<=fDetector->GetNbOfAbsor(); j++) {
    if (fLimittrue[j] < DBL_MAX) {
      if (!isStarted) {
        acc.BeginOfAcceptance("Sampling Calorimeter",nEvt);
        isStarted = true;
      }
      MeanEAbs = fSumEAbs[j];
      rmsEAbs  = fSum2EAbs[j];
      G4String mat = fDetector->GetAbsorMaterial(j)->GetName();
      acc.EmAcceptanceGauss("Edep"+mat, nEvt, MeanEAbs,
                             fEdeptrue[j], fRmstrue[j], fLimittrue[j]);
      acc.EmAcceptanceGauss("Erms"+mat, nEvt, rmsEAbs,
                             fRmstrue[j], fRmstrue[j], 2.0*fLimittrue[j]);
    }
  }
  if(isStarted) acc.EndOfAcceptance();

  for(G4int j=1; j<=fDetector->GetNbOfAbsor(); j++){
		if (j==fDetector->GetNbOfAbsor()){
// Info for transmit and reflect 
  		G4double transmit[2];
  		transmit[0] = 100.*fTransmit[0]/nEvt;
  		transmit[1] = 100.*fTransmit[1]/nEvt;
  		G4double reflect[2];
  		reflect[0] = 100.*fReflect[0]/nEvt;
  		reflect[1] = 100.*fReflect[1]/nEvt;
 		G4double rmsMsc = 0., tailMsc = 0.;
  	if (fMscEntryCentral > 0) {
    		fMscProjecTheta /= fMscEntryCentral; fMscProjecTheta2 /= fMscEntryCentral;
    		rmsMsc = fMscProjecTheta2 - fMscProjecTheta*fMscProjecTheta;
    		if (rmsMsc > 0.)rmsMsc = std::sqrt(rmsMsc); 
   		if(fTransmit[1] > 0.0)tailMsc = 100.- (100.*fMscEntryCentral)/(2*fTransmit[1]);
  				}	
  G4cout << "\n ======================== run summary ======================\n";
  G4cout << "\n Number of events with the primary particle transmitted = "<< transmit[1] << " %" << G4endl;

  G4cout << " Number of events with at least  1 particle transmitted "
         << "(same charge as primary) = " << transmit[0] << " %" << G4endl;

  G4cout << "\n Number of events with the primary particle reflected = "
         << reflect[1] << " %" << G4endl;

  G4cout << " Number of events with at least  1 particle reflected "
         << "(same charge as primary) = " << reflect[0] << " %" << G4endl;

  // compute width of the Gaussian central part of the MultipleScattering
  //
  G4cout << "\n MultipleScattering:" 
         << "\n  rms proj angle of transmit primary particle = "
         << rmsMsc/mrad << " mrad (central part only)" << G4endl;

  G4cout << "  computed theta0 (Highland formula)          = "
         << ComputeMscHighland()/mrad << " mrad" << G4endl;
           
  G4cout << "  central part defined as +- "
         << fMscThetaCentral/mrad << " mrad; " 
         << "  Tail ratio = " << tailMsc << " %" << G4endl;
	}
 }
	
  //normalize histograms
  //
  for (G4int ih = MaxAbsor+1; ih < MaxHisto; ih++) {
    analysis->ScaleH1(ih,norm/MeV);
  }
  
  G4cout.setf(mode,std::ios::floatfield);
  G4cout.precision(prec);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::SetEdepAndRMS(G4int i, G4double edep, G4double rms, G4double lim)
{
  if (i>=0 && i<MaxAbsor) {
    fEdeptrue [i] = edep;
    fRmstrue  [i] = rms;
    fLimittrue[i] = lim;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::SetApplyLimit(G4bool val)
{
  fApplyLimit = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4double Run::ComputeMscHighland()
{
  //compute the width of the Gaussian central part of the MultipleScattering
  //projected angular distribution.
  //Eur. Phys. Jour. C15 (2000) page 166, formule 23.9

  G4double t = (fDetector->GetAbsorThickness(1)) /(fDetector->GetAbsorMaterial(1)->GetRadlen());
  if (t < DBL_MIN) return 0.;

  G4double T = fEkin;
  G4double M = fParticle->GetPDGMass();
  G4double z = std::abs(fParticle->GetPDGCharge()/eplus);

  G4double bpc = T*(T+2*M)/(T+M);
  G4double teta0 = 13.6*MeV*z*std::sqrt(t)*(1.+0.038*std::log(t))/bpc;
  return teta0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

