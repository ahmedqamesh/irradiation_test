
#ifndef TrackerHit_h
#define TrackerHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

/// Calorimeter hit class
///
/// It defines data members to store the the energy deposit and track lengths
/// of charged particles in a selected volume:
/// - fEdep, fTrackLength

class TrackerHit : public G4VHit
{
  public:
    TrackerHit();
    TrackerHit(const TrackerHit&);
    virtual ~TrackerHit();

    // operators
    const TrackerHit& operator=(const TrackerHit&);
    G4int operator==(const TrackerHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw() {}
    virtual void Print();

    // methods to handle data
    void Add(G4double de, G4double dl);

    // get methods
    G4double GetEdep() const;
    G4double GetTrackLength() const;
      
  private:
    G4double fEdep;        ///< Energy deposit in the sensitive volume
    G4double fTrackLength; ///< Track length in the  sensitive volume
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<TrackerHit> TrackerHitsCollection;

extern G4ThreadLocal G4Allocator<TrackerHit>* TrackerHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* TrackerHit::operator new(size_t)
{
  if(!TrackerHitAllocator)
    TrackerHitAllocator = new G4Allocator<TrackerHit>;
  void *hit;
  hit = (void *) TrackerHitAllocator->MallocSingle();
  return hit;
}

inline void TrackerHit::operator delete(void *hit)
{
  if(!TrackerHitAllocator)
    TrackerHitAllocator = new G4Allocator<TrackerHit>;
  TrackerHitAllocator->FreeSingle((TrackerHit*) hit);
}

inline void TrackerHit::Add(G4double de, G4double dl) {
  fEdep += de; 
  fTrackLength += dl;
}

inline G4double TrackerHit::GetEdep() const { 
  return fEdep; 
}

inline G4double TrackerHit::GetTrackLength() const { 
  return fTrackLength; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
