
#include "EmAcceptance.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EmAcceptance::EmAcceptance()
 : fIsAccepted(false)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EmAcceptance::~EmAcceptance()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EmAcceptance::BeginOfAcceptance(const G4String& title, G4int stat)
{
  G4cout << "\n<<<<<ACCEPTANCE>>>>> " << stat << " events for " << title 
         << G4endl;
  fIsAccepted = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EmAcceptance::EndOfAcceptance()
{
  G4String resume = "IS ACCEPTED";
  if (!fIsAccepted) resume = "IS NOT ACCEPTED";
  G4cout << "<<<<<END>>>>>   " << resume << G4endl;
  G4cout << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EmAcceptance::EmAcceptanceGauss(const G4String& title, G4int stat,
                                           G4double avr, G4double avr0,
                                           G4double rms, G4double limit)
{
  G4double x = std::sqrt((G4double)stat);
  G4double dde = avr - avr0;
  G4double de = dde*x/rms;

  G4cout << title << ": " << avr << "  del"<< title << "= " << dde
         << " nrms= " << de << G4endl;

  if (std::fabs(de) > limit) fIsAccepted = false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
