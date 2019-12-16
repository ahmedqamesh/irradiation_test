#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "DetectorConstruction.hh"
#include "TrackerHit.hh"
class EventActionMessenger;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventAction : public G4UserEventAction
{
  public:  
    EventAction(DetectorConstruction*);
   ~EventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void   EndOfEventAction(const G4Event*);
    
    void SumEnergy(G4int k, G4double de, G4double dl)
        {fEnergyDeposit[k] += de; fTrackLengthCh[k] += dl;};          
    void SetTransmitFlag (G4int flag) 
                           {if (flag > fTransmitFlag) fTransmitFlag = flag;};
    void SetReflectFlag  (G4int flag) 
                           {if (flag > fReflectFlag)   fReflectFlag = flag;};    
  private:  

  // methods
  TrackerHitsCollection* GetHitsCollection(G4int hcID,const G4Event* event) const;
  void PrintEventStatistics(G4double FilterEdep, G4double FilterTrackLength,G4int nhit) const;

    DetectorConstruction* fDetector;
    G4double              fEnergyDeposit[MaxAbsor];
    G4double              fTrackLengthCh[MaxAbsor];

    G4int 		  HCHCID;
    G4int                 HHC1ID;
    G4int 		  verboseLevel;

  public:
    inline void SetVerbose(G4int val) { verboseLevel = val; }
    inline G4int GetVerbose() const { return verboseLevel; }
    G4int    fTransmitFlag,   fReflectFlag;    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
