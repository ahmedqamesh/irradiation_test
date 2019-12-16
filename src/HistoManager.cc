#include "HistoManager.hh"
#include "G4UnitsTable.hh"
#include "DetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
  : fFileName("Xray")
{
  Book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book()
{
  // Create or get analysis manager
  // The choice of analysis technology is done via selection of a namespace
  // in HistoManager.hh
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetFileName(fFileName);
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetActivation(true);   // enable inactivation of histograms

  // Define histograms start values
  
  const G4String id[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
                         "10","11","12","13","14","15","16","17","18","19",
                         "20","21","22","23","24","25","26","27","28","29",
                         "30","31","32","33","34","35","36","37","38","39",
                         "40","41","42","43","44","45","46","47","48","49",
			 "50","51","52","53","54","55","56","57"};
  G4String title;

  // Default values (to be reset via /analysis/h1/set command)
  G4int nbins = 100;
  G4double vmin = 0.;
  G4double vmax = 100.;
  
	G4int ExtraHist=35; // Number of Extra Histograms+1
  for (G4int k=0; k<=MaxHisto+ExtraHist; k++) {
    if (k >=1 && k<=9) title = "Edep in absorber " + id[k];									   //[EventAction.cc]
    if (k > MaxAbsor) title = "Edep longitudinal profile (keV/event) in absorber "+ id[k-MaxAbsor]; //[SteppingAction.cc]
    if (k == 21) title = "energy flow (keV/event)"; 										   //[Run.cc]
    if (k == 22) title = "lateral energy leak (keV/event)";							   		   //[Run.cc]
    if (k == 23) title = "Photon Edep From the spectrum"; 				// Fill primary particleEnergy in PrimaryGeneratorAction.cc //Dont trust

    //Histograms for scoring area
    if (k == 24) title = "Photon Edep the selected Absorber";  					// Secondary_Energy and primary_Energy  in [SteppingAction.cc] (in scoring absorber) (Same as 32)
    if (k == 25) title = "Photon Energy deposited in the Selected Absorber";  	// Secondary_Energy and primary_Energy in [TrackerSD.cc] 		(in Sensitive Detector)

	if (k == 26) title = "Ionizing Energy Deposit from step in Selected Absorber";// step->GetNonIonizingEnergyDeposit()  in TrackerSD.cc      (in Sensitive Detector)
    if (k == 27) title = "Total Energy Deposit  in Selected Absorber"; 			// Hit->GetEdep() [EventAction.cc] with GetSDMpointer (same as Histo1 and 2 , 26)

    if (k == 28) title = "Secondary neutral particles"; 								// secondary neutral particles [StackingAction.cc]
    if (k == 29) title = "Secondary neutral particles due to PhotoGamma or ComptGamma";	// secondary neutral by fPhotoGamma or fComptGamma  [StackingAction.cc]
    if (k == 30) title = "Secondary neutral particles due to fPixeGamma"; 				// secondary neutral by fPixeGamma	[StackingAction.cc]

    if (k == 31) title = "(transmit, charged) : kinetic energy at exit";
    if (k == 32) title =  "(transmit, neutral) : kinetic energy at exit";
    if (k == 33) title =  "(reflect, charged) : kinetic energy at exit";
    if (k == 34) title = "(reflect , neutral) : kinetic energy at exit";

    if (k == 35) title = "Secondary charged particles in all layers";
    if (k == 36) title = "Secondary reflected charged particles in all layers ";
    if (k == 37) title = "Secondary transmitted charged particles in all layers";

    if (k == 38) title = "Secondary absorbed charged particles in all layers";
    if (k == 39) title = "Track length of Secondary transmitted e-";
    if (k == 40) title = "Vertex of Secondary e- ";
    if (k == 41) title = "Energy of phot e- at creation ";
    if (k == 42) title = "Energy of Compton e- at creation";
    if (k == 43) title = "Energy of photAuger or ComptAuger of e- at creation";
    if (k == 44) title ="Energy of PIXEAuger e- at creation";

    if (k == 45) title ="(transmit, charged) : projected angle at exit";
    if (k == 46) title ="(reflect , charged) : projected angle at exit";

    if (k == 47) title ="(transmit, charged) : projected position at exit";
    if (k == 48) title ="(transmit, charged) : radius at exit";
    if (k == 49) title ="x_vertex of charged secondaries (all)";
    if (k == 50) title ="x_vertex of charged secondaries (not absorbed)";

    if (k == 51) title ="Secondary charged particles";
    if (k == 52) title ="Energy of phot e- at creation";
    if (k == 53) title ="Energy of Compton e- at creation";
    if (k == 54) title ="Energy of photAuger or ComptAuger of e- at creation";					
    if (k == 55) title ="Energy of PIXEAuger e- at creation"; 
    if (k == 56) title ="Energy of  e- From undefined processes";
    if (k == 57) title ="Track Length of Secondary e-"; 			//stepl = aTrack->GetPosition();  StackingAction.cc 

    G4int ih = analysisManager->CreateH1(id[k], title, nbins, vmin, vmax);
    analysisManager->SetH1Activation(ih, false);
	}

}
