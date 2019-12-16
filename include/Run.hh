//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file electromagnetic/TestEm11/include/Run.hh
/// \brief Definition of the Run class
//
// $Id: Run.hh 71375 2013-06-14 07:39:33Z maire $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef Run_h
#define Run_h 1

#include "DetectorConstruction.hh"

#include "G4Run.hh"
#include "globals.hh"
#include <map>
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

class DetectorConstruction;
class G4ParticleDefinition;
class G4Track;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  			
class Run : public G4Run
{
  public:
    Run(DetectorConstruction*);
   ~Run();

  public:

    void SetPrimary(G4ParticleDefinition* particle, G4double energy);
      
    void FillPerEvent(G4int,G4double,G4double);
    
    void SumEnergyFlow (G4int plane, G4double Eflow);
    void SumLateralEleak(G4int cell, G4double Eflow);
    void AddChargedStep();
    void AddNeutralStep();
    void AddSecondaryTrack(const G4Track*);
    void SetEdepAndRMS(G4int, G4double, G4double, G4double);
    void SetApplyLimit(G4bool);


   void AddMscProjTheta (G4double theta)
              {if (std::abs(theta) <= fMscThetaCentral) { fMscEntryCentral++;
                 fMscProjecTheta += theta;  fMscProjecTheta2 += theta*theta;}
              };

   void CountParticles (G4ParticleDefinition* part)
              {     if (part == G4Gamma::Gamma())       fNbGamma++ ;
               else if (part == G4Electron::Electron()) fNbElect++ ;
               else if (part == G4Positron::Positron()) fNbPosit++ ; };

   void CountTransmit (G4int flag)
              {     if (flag == 1)  fTransmit[0]++;
               else if (flag == 2) {fTransmit[0]++; fTransmit[1]++; }};

   void CountReflect (G4int flag)
              {     if (flag == 1)  fReflect[0]++;
               else if (flag == 2) {fReflect[0]++; fReflect[1]++; }};
    

         
   G4double ComputeMscHighland();



    virtual void Merge(const G4Run*);
    void EndOfRun();
     
  private:
    DetectorConstruction*  fDetector;
    G4ParticleDefinition*  fParticle;
    G4double  fEkin;
                           
    G4double fSumEAbs [MaxAbsor], fSum2EAbs [MaxAbsor]; 
    G4double fSumLAbs [MaxAbsor], fSum2LAbs [MaxAbsor];
    
    std::vector<G4double> fEnergyFlow;
    std::vector<G4double> fLateralEleak;
    std::vector<G4double> fEnergyDeposit[MaxAbsor];
    
    G4double fChargedStep;
    G4double fNeutralStep;

    G4int  fN_gamma;
    G4int  fN_elec;
    G4int  fN_pos;

    
   G4int    fNbGamma, fNbElect, fNbPosit;
   G4int    fTransmit[2],   fReflect[2];
   G4double fMscProjecTheta, fMscProjecTheta2;
   G4int    fMscEntryCentral;
   G4double fMscThetaCentral;


    G4double fEdeptrue [MaxAbsor];
    G4double fRmstrue  [MaxAbsor];
    G4double fLimittrue[MaxAbsor];        
    G4bool fApplyLimit;
  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

