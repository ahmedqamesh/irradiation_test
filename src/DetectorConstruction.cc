#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4Trd.hh"
#include <iomanip>
#include "G4SDManager.hh"
#include "TrackerSD.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4SDChargedFilter.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSTrackLength.hh"
#include "G4VSensitiveDetector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction() :
		G4VUserDetectorConstruction(), fCheckOverlaps(true), fDefaultMaterial(
				0), fSolidWorld(0), fLogicWorld(0), fPhysiWorld(0), fSolidCollInner(
				0), fLogicCollInner(0), fPhysCollInner(0), fSolidCollOuter(0), fLogicCollOuter(
				0), fPhysCollOuter(0), fSolidShield(0), fLogicShield(0), fPhysShield(
				0), fScoringVolume(0), fDetectorMessenger(0) {

	// default parameter values
	fNbOfAbsor = 21;
	fNbOfFilter = 2;
	fNbOfLayers = 1;

	ComputeDetectorParameters();
	//================================== position ============================================================
	// Shield
	fXposShield = 10 * cm;
	//Absorber : at position 9 cm from the world center
	fXposAbs[1] = 9 * cm;

	//collimator 1 : 4 cm away from the absorbers
	fXposinCollimator = 5.0 * cm;

	// Filter 1: 5 cm away from  collimator 1
	fXposFilter[1] = 0.0 * cm;

	//collimator 2 : 1 cm away from the Filter 1
	fXposoutCollimator = -1.0 * cm;

	// Filter 2: 2 cm away from  collimator 2
	fXposFilter[2] = -3.0 * cm;

	//================================== Dimensions ===========================================================
	//YZ dimensions
	fDetectorSizeYZ = 40. * cm;
	fCollimatorYZ = 40.0 * cm;

	fAbsSizeYZ = 40. * cm;
	fFilterYZ[1] = 40. * cm;
	fFilterYZ[2] = 40. * cm;

	//Filter
	fFilterThickness[1] = 0.3 * mm;
	fFilterThickness[2] = 0.15 * mm;

	// collimators
	fCollinradius = 5 * mm;
	fColloutradius = 1.5 * mm;

	fCollimatorinThickness = 2.0 * cm;
	fCollimatoroutThickness = 0.5 * cm;

	//Absorber
	fAbsorThickness[1] = 0.21 * mm;
	fAbsorThickness[2] = 0.78 * mm;
	fAbsorThickness[3] = 0.5 * mm;
	fAbsorThickness[4] = 0.34 * mm;
	fAbsorThickness[5] = 0.00067 * mm;
	fAbsorThickness[6] = 0.01 * mm;
	fAbsorThickness[7] = 0.00067 * mm;
	fAbsorThickness[8] = 0.00022 * mm;
	fAbsorThickness[9] = 0.000175 * mm;

	fAbsorThickness[10] = 0.00022 * mm;
	fAbsorThickness[11] = 0.075 * mm;

	fAbsorThickness[12] = 0.022 * mm;
	fAbsorThickness[13] = 0.000175 * mm;
	fAbsorThickness[14] = 0.00022 * mm;

	fAbsorThickness[15] = 0.0075 * mm;
	fAbsorThickness[16] = 0.00022 * mm;
	fAbsorThickness[17] = 0.005 * mm;
	fAbsorThickness[18] = 0.00022 * mm;
	fAbsorThickness[19] = 0.000175 * mm;
	fAbsorThickness[20] = 0.0218 * mm;
	fAbsorThickness[21] = 0.00031 * mm;

	//Shield
	fShieldThickness = 2 * cm;
	//================================== materials===========================================================
	DefineMaterials();
	SetWorldMaterial("G4_AIR");
	// Absorber
	const G4String Absorber_id[] = { "quartz", "G4_Al", "quartz", "G4_Cu",
			"G4_Cu", "G4_Cu", "G4_Cu", "G4_Cu", "G4_Cu", "G4_Cu", "G4_Cu",
			"G4_Cu", "G4_Cu", "G4_Cu", "G4_Cu", "G4_Cu", "quartz", "quartz",
			"quartz", "quartz", "quartz", "quartz" };

	for (G4int k = 1; k <= fNbOfAbsor; k++) {
		SetAbsorMaterial(k, Absorber_id[k]);
	}

	//Filter
	SetFilterMaterial(1, "G4_Be");
	SetFilterMaterial(2, "G4_Al");
	//collimator
	fCollMaterialOuter = G4NistManager::Instance()->FindOrBuildMaterial(
			"G4_Al");
	fCollMaterialInner = G4NistManager::Instance()->FindOrBuildMaterial(
			"G4_PLEXIGLASS");
	// create commands for interactive definition of the Detectorimeter
	fDetectorMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction() {
	delete fDetectorMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ComputeDetectorParameters() {
	// Compute derived parameters of the detector
	fLayerThickness = 0.;
	for (G4int iAbs = 1; iAbs <= fNbOfAbsor; iAbs++) {
		fLayerThickness += fAbsorThickness[iAbs];
	}
	fWorldSizeX = 25 * cm; //1.2 * fLayerThickness;
	fWorldSizeYZ = 1.2 * fDetectorSizeYZ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct() {
	G4cout << "DetectorConstruction::Construct()" << G4endl;
	// complete the Detector parameters definition
	ComputeDetectorParameters();

	// Cleanup old geometry
	G4GeometryManager::GetInstance()->OpenGeometry();
	G4PhysicalVolumeStore::GetInstance()->Clean();
	G4LogicalVolumeStore::GetInstance()->Clean();
	G4SolidStore::GetInstance()->Clean();

	//VisAttributes
	G4VisAttributes * Black = new G4VisAttributes(G4Colour(0.0, 0.0, 0.0));
	G4VisAttributes * Gray = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));
	G4VisAttributes * yellow = new G4VisAttributes(
			G4Colour(255 / 255., 255 / 255., 51 / 255.));
	G4VisAttributes * red = new G4VisAttributes(
			G4Colour(255 / 255., 0 / 255., 0 / 255.));
	G4VisAttributes * blue = new G4VisAttributes(
			G4Colour(0 / 255., 0 / 255., 255 / 255.));
	G4VisAttributes* collOuterVisAtt = new G4VisAttributes(
			G4Colour(240. / 255., 240. / 255., 222. / 255., 3. / 5.));
	G4VisAttributes* collInnerVisAtt = new G4VisAttributes(
			G4Colour(240. / 255., 240. / 255., 222. / 255., 3. / 5.));

	Black->SetVisibility(true);

	Gray->SetVisibility(true);
	Gray->SetForceSolid(true);

	yellow->SetVisibility(true);
	yellow->SetForceSolid(true);

	red->SetVisibility(true);
	red->SetForceSolid(true);

	blue->SetVisibility(true);
	blue->SetForceSolid(true);

	collOuterVisAtt->SetVisibility(true);
	collOuterVisAtt->SetForceSolid(true);

	collInnerVisAtt->SetVisibility(true);
	collInnerVisAtt->SetForceSolid(true);
	//
	// World (Experimental place)
	//

	fSolidWorld = new G4Box("World", fWorldSizeX / 2, fWorldSizeYZ / 2,
			fWorldSizeYZ / 2);
	fLogicWorld = new G4LogicalVolume(fSolidWorld, fDefaultMaterial, "World");

	fPhysiWorld = new G4PVPlacement(0, G4ThreeVector(), fLogicWorld, "World", 0,
			false, 0, fCheckOverlaps);

	// Absorbers
	G4RotationMatrix rotm2 = G4RotationMatrix();
	rotm2.rotateZ(0 * deg);
	for (G4int k = 1; k <= fNbOfAbsor; k++) {
		G4double xcenter = fXposAbs[1] - 0.5 * fAbsorThickness[k];
		fXposAbs[1] += fAbsorThickness[k];
		fXstartAbs[k] = xcenter - 0.5 * fAbsorThickness[k];
		fXendAbs[k] = xcenter + 0.5 * fAbsorThickness[k];
		fSolidAbsor[k] = new G4Box(fAbsorMaterial[k]->GetName(),
				fAbsorThickness[k] / 2, fDetectorSizeYZ / 2,
				fDetectorSizeYZ / 2);
		fLogicAbsor[k] = new G4LogicalVolume(fSolidAbsor[k], fAbsorMaterial[k],
				fAbsorMaterial[k]->GetName());
		fPhysiAbsor[k] = new G4PVPlacement(0, G4ThreeVector(xcenter, 0., 0.),
				fLogicAbsor[k], fAbsorMaterial[k]->GetName(), fLogicWorld,
				false, k, fCheckOverlaps);
		if (k == 1 || k == 3 || k == 5 || k == 7 || k == 9 || k == 11 || k == 13
				|| k == 15 || k == 17 || k == 19 || k == 21)
			fLogicAbsor[k]->SetVisAttributes(blue);
		if (k == 2)
			fLogicAbsor[k]->SetVisAttributes(yellow);
		if (k == 4 || k == 6 || k == 8 || k == 10 || k == 12 || k == 14
				|| k == 16 || k == 18 || k == 20)
			fLogicAbsor[k]->SetVisAttributes(red);
	}

	//Collimators
	//two cylinders with different material and small hole in the middle
	//
	G4RotationMatrix rotm1 = G4RotationMatrix();
	rotm1.rotateY(90 * deg);
	rotm1.rotateZ(0 * deg);

	G4double xcollimatorincenter = fXposinCollimator
			- 0.5 * fCollimatorinThickness;

	fSolidCollInner = new G4Tubs("CollimatorInner", fCollinradius,
			fCollimatorYZ / 2, fCollimatorinThickness, 0. * deg, 360. * deg);
	fLogicCollInner = new G4LogicalVolume(fSolidCollInner, fCollMaterialInner,
			"CollimatorInner");
	fPhysCollInner = new G4PVPlacement(
			G4Transform3D(rotm1, G4ThreeVector(xcollimatorincenter, 0, 0)),
			fLogicCollInner, "CollimatorInner", fLogicWorld, false, 0,
			fCheckOverlaps);                                //overlaps checking
	fLogicCollInner->SetVisAttributes(collInnerVisAtt);

	G4double xcollimatoroutcenter = fXposoutCollimator
			- 0.5 * fCollimatoroutThickness;
	fSolidCollOuter = new G4Tubs("CollimatorOuter", fColloutradius,
			fCollimatorYZ / 2, fCollimatoroutThickness, 0. * deg, 360. * deg);

	fLogicCollOuter = new G4LogicalVolume(fSolidCollOuter, fCollMaterialOuter,
			"CollimatorOuter");
	fPhysCollOuter = new G4PVPlacement(
			G4Transform3D(rotm1, G4ThreeVector(xcollimatoroutcenter, 0, 0)),
			fLogicCollOuter, "CollimatorOuter", fLogicWorld, false, 0,
			fCheckOverlaps);                                //overlaps checking
	fLogicCollOuter->SetVisAttributes(collOuterVisAtt);
	/*
	// Filters
	G4RotationMatrix rotm3 = G4RotationMatrix();
	rotm3.rotateY(90 * deg);
	rotm3.rotateZ(0 * deg);
	G4double i = 0.0;
	for (G4int k = 1; k <= fNbOfFilter; k++) {
		G4double x2center = fXposFilter[k] + 0.5 * fFilterThickness[k];
		fXstartFilter[k] = x2center - 0.5 * fFilterThickness[k];
		fXendFilter[k] = x2center + 0.5 * fFilterThickness[k];
		if (k != fNbOfFilter) {
			fSolidFilter[k] = new G4Tubs("Filter1", 0. * mm, fFilterYZ[k] / 2,
					fFilterThickness[k] / 2, 0.0 * deg, 360. * deg);
			fLogicFilter[k] = new G4LogicalVolume(fSolidFilter[k],
					fFilterMaterial[k], "Filter1");
			fPhysiFilter[k] = new G4PVPlacement(
					G4Transform3D(rotm3, G4ThreeVector(x2center, 0.0, 0.)),
					fLogicFilter[k], fFilterMaterial[k]->GetName(), fLogicWorld,
					false, k, fCheckOverlaps);
			// Set Filter[1] as scoring volume (Most probably the Al filter)
			fScoringVolume = fLogicFilter[k];

		}
		if (k == fNbOfFilter) { // Filter 2 is a sensitive detector
			fSolidFilter[k] = new G4Tubs("Filter2", 0. * mm, fFilterYZ[k] / 2,
					fFilterThickness[k] / 2, 0.0 * deg, 360. * deg);
			fLogicFilter[k] = new G4LogicalVolume(fSolidFilter[k],
					fFilterMaterial[k], "Filter2");
			//G4cout<<k<<"fXposAbs = "<<x2center<<" & fXstartAbs = "<<fXstartFilter[k]<<" fXendAbs = "<<fXendFilter[k]<<" fFilterThickness = "<<fFilterThickness[k]<<"mm"<<G4endl;
			fPhysiFilter[k] = new G4PVPlacement(
					G4Transform3D(rotm3, G4ThreeVector(x2center, 0.0, 0.)),
					fLogicFilter[k], fFilterMaterial[k]->GetName(), fLogicWorld,
					false, k, fCheckOverlaps);
		}
		fLogicFilter[k]->SetVisAttributes(Gray);
		i += fXposFilter[k];
	}
	SetSizeX(i);

	//	Setup shielding
	fSolidShield = new G4Box("Shield", fShieldThickness / 2,
			fDetectorSizeYZ / 2, fDetectorSizeYZ / 2);
	fShieldMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Pb");
	fLogicShield = new G4LogicalVolume(fSolidShield, fShieldMaterial, "Shield");
	fPhysShield = new G4PVPlacement(0, G4ThreeVector(fXposShield, 0, 0),
			fLogicShield, "Shield", fLogicWorld, false, 0, fCheckOverlaps);
	fLogicShield->SetVisAttributes(Black);
*/
	//PrintDetectorParameters();
	//always return the fPhysical World
	return fPhysiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::PrintDetectorParameters() {
	G4cout << "\n-------------------------------------------------------------"
			<< "\n ---> The Detectorimeter is " << fNbOfLayers << " layers of:";
	for (G4int i = 1; i <= fNbOfAbsor; i++) {
		G4cout << "\n \t" << std::setw(12) << fAbsorMaterial[i]->GetName()
				<< ": " << std::setw(6)
				<< G4BestUnit(fAbsorThickness[i], "Length");
	}
	G4cout
			<< "\n-------------------------------------------------------------\n";

	G4cout << "\n" << fDefaultMaterial << G4endl;
	for (G4int j = 1; j <= fNbOfAbsor; j++)
		G4cout << "\n" << fAbsorMaterial[j] << G4endl;

	G4cout
			<< "\n-------------------------------------------------------------\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetWorldMaterial(const G4String& material) {
	// search the material by its name
	G4Material* pttoMaterial = G4NistManager::Instance()->FindOrBuildMaterial(
			material);
	if (pttoMaterial)
		fDefaultMaterial = pttoMaterial;
	G4RunManager::GetRunManager()->PhysicsHasBeenModified();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetNbOfLayers(G4int ival) {
	// set the number of Layers
	//
	if (ival < 1) {
		G4cout << "\n --->warning from SetfNbOfLayers: " << ival
				<< " must be at least 1. Command refused" << G4endl;
		return;
	}
	fNbOfLayers = ival;
	G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetNbOfAbsor(G4int ival) {
	// set the number of Absorbers
	//
	if (ival < 1 || ival > (kMaxAbsor - 1)) {
		G4cout << "\n ---> warning from SetfNbOfAbsor: " << ival
				<< " must be at least 1 and and most " << kMaxAbsor - 1
				<< ". Command refused" << G4endl;
		return;
	}
	fNbOfAbsor = ival;
	G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetAbsorMaterial(G4int ival,
		const G4String& material) {
	// search the material by its name
	//
	if (ival > kMaxAbsor || ival <= 0) {
		G4cout << "\n --->warning from SetAbsorMaterial: absor number " << ival
				<< " out of range. Command refused" << G4endl;
		return;
	}

	G4Material* pttoMaterial = G4NistManager::Instance()->FindOrBuildMaterial(
			material);
	if (pttoMaterial)
		fAbsorMaterial[ival] = pttoMaterial;
	G4RunManager::GetRunManager()->PhysicsHasBeenModified();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetAbsorThickness(G4int ival, G4double val) {
	// change Absorber thickness
	//
	if (ival > fNbOfAbsor || ival <= 0) {
		G4cout << "\n --->warning from SetAbsorThickness: absor number " << ival
				<< " out of range. Command refused" << G4endl;
		return;
	}
	if (val <= DBL_MIN) {
		G4cout << "\n --->warning from SetAbsorThickness: thickness " << val
				<< " out of range. Command refused" << G4endl;
		return;
	}
	fAbsorThickness[ival] = val;
	G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetDetectorSizeYZ(G4double val) {
	// change the transverse size
	//
	if (val <= DBL_MIN) {
		G4cout << "\n --->warning from SetfDetectorSizeYZ: thickness " << val
				<< " out of range. Command refused" << G4endl;
		return;
	}
	fDetectorSizeYZ = val;
	G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

void DetectorConstruction::ConstructSDandField() {
	if (fFieldMessenger.Get() == 0) {
		// Create global magnetic field messenger.
		// Uniform magnetic field is then created automatically if
		// the field value is not zero.
		G4ThreeVector fieldValue = G4ThreeVector();
		G4GlobalMagFieldMessenger* msg = new G4GlobalMagFieldMessenger(
				fieldValue);
		//msg->SetVerboseLevel(1);
		G4AutoDelete::Register(msg);
		fFieldMessenger.Put(msg);

	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetCollimator(const bool& value) {
	if (!value && fPhysCollInner->GetMotherLogical() == fLogicWorld) {
		fLogicWorld->RemoveDaughter(fPhysCollInner);
		fLogicWorld->RemoveDaughter(fPhysCollOuter);
		fPhysCollInner->SetMotherLogical(0);
		fPhysCollOuter->SetMotherLogical(0);
		G4cout << "DetectorConstruction: deactivate collimator" << G4endl;
	} else if (value && fPhysCollInner->GetMotherLogical() == 0) {
		fLogicWorld->AddDaughter(fPhysCollInner);
		fLogicWorld->AddDaughter(fPhysCollOuter);
		fPhysCollInner->SetMotherLogical(fLogicWorld);
		fPhysCollOuter->SetMotherLogical(fLogicWorld);
		G4cout << "DetectorConstruction: activate collimator" << G4endl;
	}
	G4RunManager::GetRunManager()->GeometryHasBeenModified();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetCollInnerMaterial(G4String materialChoice) {
	G4Material* newMaterial = G4NistManager::Instance()->FindOrBuildMaterial(
			materialChoice);
	if (newMaterial && fCollMaterialInner != newMaterial) {
		fCollMaterialInner = newMaterial;
		if (fLogicCollInner)
			fLogicCollInner->SetMaterial(fCollMaterialInner);
		G4RunManager::GetRunManager()->PhysicsHasBeenModified();
		G4cout << "Inner collimator material set to " << materialChoice
				<< G4endl;
	}
	if (newMaterial == 0) {
		G4ExceptionDescription msg;
		msg << "Material " << materialChoice << " not known";
		G4Exception("DetectorConstruction", "Please select a known material.",
				JustWarning, msg);
	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetCollOuterMaterial(G4String materialChoice) {
	G4Material* newMaterial = G4NistManager::Instance()->FindOrBuildMaterial(
			materialChoice);
	if (newMaterial && fCollMaterialOuter != newMaterial) {
		fCollMaterialOuter = newMaterial;
		if (fLogicCollInner)
			fLogicCollInner->SetMaterial(fCollMaterialOuter);
		G4RunManager::GetRunManager()->PhysicsHasBeenModified();
		G4cout << "Outer collimator material set to " << materialChoice
				<< G4endl;
	}
	if (newMaterial == 0) {
		G4ExceptionDescription msg;
		msg << "Material " << materialChoice << " not known";
		G4Exception("DetectorConstruction", "Please select a known material.",
				JustWarning, msg);
	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetCollInnerRadius(G4double radius) {
	fSolidCollInner->SetInnerRadius(radius);
	G4RunManager::GetRunManager()->GeometryHasBeenModified();
	G4cout << "Inner collimator radius set to " << G4BestUnit(radius, "Length")
			<< G4endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetCollMiddleRadius(G4double radius) {
	fSolidCollInner->SetOuterRadius(radius);
	fSolidCollOuter->SetInnerRadius(radius);
	G4RunManager::GetRunManager()->GeometryHasBeenModified();
	G4cout << "Middle collimator radius set to " << G4BestUnit(radius, "Length")
			<< G4endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetCollOuterRadius(G4double radius) {
	fSolidCollOuter->SetOuterRadius(radius);
	G4RunManager::GetRunManager()->GeometryHasBeenModified();
	G4cout << "Outer collimator radius set to " << G4BestUnit(radius, "Length")
			<< G4endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetCollThickness(G4double val) {
	fSolidCollInner->SetZHalfLength(val / 2.);
	fSolidCollOuter->SetZHalfLength(val / 2.);
	G4RunManager::GetRunManager()->GeometryHasBeenModified();
	G4cout << "Set collimator thickness set to " << G4BestUnit(val, "Length")
			<< G4endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetAbsorberXpos(G4double val) {
	fXposAbs[1] = val;
	ComputeDetectorParameters();
	G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetNbOfFilter(G4int ival) {
	// set the number of Filter
	//
	if (ival < 1 || ival > (MaxFilter - 1)) {
		G4cout << "\n ---> warning from SetfNbOfFilter: " << ival
				<< " must be at least 1 and and most " << MaxFilter - 1
				<< ". Command refused" << G4endl;
		return;
	}
	fNbOfFilter = ival;
	G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetFilterMaterial(G4int ival,
		const G4String& material) {
	// search the material by its name
	//
	if (ival > fNbOfFilter || ival <= 0) {
		G4cout << "\n --->warning from SetFilterMaterial: Filter number "
				<< ival << " out of range. Command refused" << G4endl;
		return;
	}
	G4Material* pttoMaterial = G4NistManager::Instance()->FindOrBuildMaterial(
			material);
	if (pttoMaterial)
		fFilterMaterial[ival] = pttoMaterial;
	G4RunManager::GetRunManager()->PhysicsHasBeenModified();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetFilterThickness(G4int ival, G4double val) {
	// change Filter thickness
	//
	if (ival > fNbOfFilter || ival <= 0) {
		G4cout << "\n --->warning from SetFilterThickness: Filter number "
				<< ival << " out of range. Command refused" << G4endl;
		return;
	}
	if (val <= DBL_MIN) {
		G4cout << "\n --->warning from SetFilterThickness: thickness " << val
				<< " out of range. Command refused" << G4endl;
		return;
	}
	fFilterThickness[ival] = val;
	G4RunManager::GetRunManager()->ReinitializeGeometry();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetFilterXpos(G4int ival, G4double val) {
	fXposFilter[ival] = val;
	ComputeDetectorParameters();
	G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void DetectorConstruction::SetFilterPos(G4ThreeVector pos) {
	G4cout << "Filter 1 is set to position" << pos << G4endl;
	fPhysiFilter[1]->SetTranslation(pos);
	G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void DetectorConstruction::SetFilterRot(G4double phi) {
	// u, v, w are the daughter axes, projected on the mother frame
	G4ThreeVector w = G4ThreeVector(1, 0, 0);
	G4ThreeVector v = G4ThreeVector(0., std::cos(phi), -std::sin(phi));
	G4ThreeVector u = G4ThreeVector(0, std::sin(phi), std::cos(phi));
	G4RotationMatrix* rotm1 = new G4RotationMatrix(u, v, w);
	fPhysiFilter[1]->SetRotation(rotm1);
	G4RunManager::GetRunManager()->GeometryHasBeenModified();
	G4cout << "Filter 1 rotated to phi = " << phi << G4endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetSizeX(G4double value) {
	fFilterSizeX = value;
	G4RunManager::GetRunManager()->GeometryHasBeenModified();

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::DefineMaterials() {
	// This function illustrates the possible ways to define materials using
	// G4 database on G4Elements
	G4NistManager* manager = G4NistManager::Instance();
	manager->SetVerbose(0);
	//
	// define Elements
	//
	G4double z, a;

	G4Element* H = manager->FindOrBuildElement(1);
	G4Element* C = manager->FindOrBuildElement(6);
	G4Element* N = manager->FindOrBuildElement(7);
	G4Element* O = manager->FindOrBuildElement(8);
	G4Element* Si = manager->FindOrBuildElement(14);
	G4Element* Ge = manager->FindOrBuildElement(32);
	G4Element* Sb = manager->FindOrBuildElement(51);
	G4Element* I = manager->FindOrBuildElement(53);
	G4Element* Cs = manager->FindOrBuildElement(55);
	G4Element* Pb = manager->FindOrBuildElement(82);
	G4Element* Bi = manager->FindOrBuildElement(83);

	//
	// define an Element from isotopes, by relative abundance
	//
	G4int iz, n;                       //iz=number of protons  in an isotope;
									   // n=number of nucleons in an isotope;
	G4int ncomponents;
	G4double abundance;

	G4Isotope* U5 = new G4Isotope("U235", iz = 92, n = 235,
			a = 235.01 * g / mole);
	G4Isotope* U8 = new G4Isotope("U238", iz = 92, n = 238,
			a = 238.03 * g / mole);

	G4Element* U = new G4Element("enriched Uranium", "U", ncomponents = 2);
	U->AddIsotope(U5, abundance = 90. * perCent);
	U->AddIsotope(U8, abundance = 10. * perCent);

	//
	// define simple materials
	//
	G4double density;

	new G4Material("liquidH2", z = 1., a = 1.008 * g / mole,
			density = 70.8 * mg / cm3);
	new G4Material("Aluminium", z = 13., a = 26.98 * g / mole,
			density = 2.700 * g / cm3);
	new G4Material("Titanium", z = 22., a = 47.867 * g / mole,
			density = 4.54 * g / cm3);
	new G4Material("Iron", z = 26., a = 55.85 * g / mole,
			density = 7.870 * g / cm3);
	new G4Material("Copper", z = 29., a = 63.55 * g / mole,
			density = 8.960 * g / cm3);
	new G4Material("Tungsten", z = 74., a = 183.85 * g / mole,
			density = 19.30 * g / cm3);
	new G4Material("Gold", z = 79., a = 196.97 * g / mole,
			density = 19.32 * g / cm3);
	new G4Material("Uranium", z = 92., a = 238.03 * g / mole,
			density = 18.95 * g / cm3);

	//
	// define a material from elements.   case 1: chemical molecule
	//
	G4int natoms;

	G4Material* H2O = new G4Material("Water", density = 1.000 * g / cm3,
			ncomponents = 2);
	H2O->AddElement(H, natoms = 2);
	H2O->AddElement(O, natoms = 1);
	H2O->GetIonisation()->SetMeanExcitationEnergy(78.0 * eV);
	H2O->SetChemicalFormula("H_2O");

	G4Material* CH = new G4Material("Polystyrene", density = 1.032 * g / cm3,
			ncomponents = 2);
	CH->AddElement(C, natoms = 1);
	CH->AddElement(H, natoms = 1);

	G4Material* Sci = new G4Material("Scintillator", density = 1.032 * g / cm3,
			ncomponents = 2);
	Sci->AddElement(C, natoms = 9);
	Sci->AddElement(H, natoms = 10);

	Sci->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);

	G4Material* Lct = new G4Material("Lucite", density = 1.185 * g / cm3,
			ncomponents = 3);
	Lct->AddElement(C, 59.97 * perCent);
	Lct->AddElement(H, 8.07 * perCent);
	Lct->AddElement(O, 31.96 * perCent);

	G4Material* Sili = new G4Material("Silicon", density = 2.330 * g / cm3,
			ncomponents = 1);
	Sili->AddElement(Si, natoms = 1);

	G4Material* SiO2 = new G4Material("quartz", density = 2.200 * g / cm3,
			ncomponents = 2);
	SiO2->AddElement(Si, natoms = 1);
	SiO2->AddElement(O, natoms = 2);

	G4Material* G10 = new G4Material("NemaG10", density = 1.700 * g / cm3,
			ncomponents = 4);
	G10->AddElement(Si, natoms = 1);
	G10->AddElement(O, natoms = 2);
	G10->AddElement(C, natoms = 3);
	G10->AddElement(H, natoms = 3);

	G4Material* CsI = new G4Material("CsI", density = 4.534 * g / cm3,
			ncomponents = 2);
	CsI->AddElement(Cs, natoms = 1);
	CsI->AddElement(I, natoms = 1);
	CsI->GetIonisation()->SetMeanExcitationEnergy(553.1 * eV);

	G4Material* BGO = new G4Material("BGO", density = 7.10 * g / cm3,
			ncomponents = 3);
	BGO->AddElement(O, natoms = 12);
	BGO->AddElement(Ge, natoms = 3);
	BGO->AddElement(Bi, natoms = 4);

	//SiNx
	density = 3.1 * g / cm3;
	G4Material* SiNx = new G4Material("SiNx", density, ncomponents = 3);
	SiNx->AddElement(Si, 300);
	SiNx->AddElement(N, 310);
	SiNx->AddElement(H, 6);

	//
	// define gaseous materials using G4 NIST database
	//
	G4double fractionmass;

	G4Material* Air = manager->FindOrBuildMaterial("G4_AIR");
	manager->ConstructNewGasMaterial("Air20", "G4_AIR", 293. * kelvin,
			1. * atmosphere);

	G4Material* lAr = manager->FindOrBuildMaterial("G4_lAr");
	G4Material* lArEm3 = new G4Material("liquidArgon",
			density = 1.390 * g / cm3, ncomponents = 1);
	lArEm3->AddMaterial(lAr, fractionmass = 1.0);

	//
	// define a material from elements and others materials (mixture of mixtures)
	//

	G4Material* Lead = new G4Material("Lead", density = 11.35 * g / cm3,
			ncomponents = 1);
	Lead->AddElement(Pb, fractionmass = 1.0);

	G4Material* LeadSb = new G4Material("LeadSb", density = 11.35 * g / cm3,
			ncomponents = 2);
	LeadSb->AddElement(Sb, fractionmass = 4. * perCent);
	LeadSb->AddElement(Pb, fractionmass = 96. * perCent);

	G4Material* Aerog = new G4Material("Aerogel", density = 0.200 * g / cm3,
			ncomponents = 3);
	Aerog->AddMaterial(SiO2, fractionmass = 62.5 * perCent);
	Aerog->AddMaterial(H2O, fractionmass = 37.4 * perCent);
	Aerog->AddElement(C, fractionmass = 0.1 * perCent);

	//
	// examples of gas in non STP conditions
	//
	G4double temperature, pressure;

	G4Material* CO2 = new G4Material("CarbonicGas", density = 27. * mg / cm3,
			ncomponents = 2, kStateGas, temperature = 325. * kelvin,
			pressure = 50. * atmosphere);
	CO2->AddElement(C, natoms = 1);
	CO2->AddElement(O, natoms = 2);

	G4Material* steam = new G4Material("WaterSteam", density = 1.0 * mg / cm3,
			ncomponents = 1, kStateGas, temperature = 273 * kelvin,
			pressure = 1 * atmosphere);
	steam->AddMaterial(H2O, fractionmass = 1.);

	new G4Material("ArgonGas", z = 18, a = 39.948 * g / mole,
			density = 1.782 * mg / cm3, kStateGas, 273.15 * kelvin,
			1 * atmosphere);
	//
	// examples of vacuum
	//

	density = universe_mean_density;    //from PhysicalConstants.h
	pressure = 3.e-18 * pascal;
	temperature = 2.73 * kelvin;
	new G4Material("Galactic", z = 1., a = 1.008 * g / mole, density, kStateGas,
			temperature, pressure);

	density = 1.e-5 * g / cm3;
	pressure = 2.e-2 * bar;
	temperature = STP_Temperature;         //from PhysicalConstants.h
	G4Material* beam = new G4Material("Beam", density, ncomponents = 1,
			kStateGas, temperature, pressure);
	beam->AddMaterial(Air, fractionmass = 1.);

	//  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}
