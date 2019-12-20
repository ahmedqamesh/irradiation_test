#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Cache.hh"
#include "G4Tubs.hh"
#include "G4RotationMatrix.hh"

class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class DetectorMessenger;
class G4GlobalMagFieldMessenger;
class G4MultiFunctionalDetector;

const G4int kMaxAbsor = 22;                        // 0 + 9
const G4int MaxFilter = 10;                       // 0 + 9

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction: public G4VUserDetectorConstruction {
public:

	DetectorConstruction();
	~DetectorConstruction();

public:
	virtual G4VPhysicalVolume* Construct();
	virtual void ConstructSDandField();

	void SetNbOfAbsor(G4int);
	void SetAbsorMaterial(G4int, const G4String&);
	void SetAbsorThickness(G4int, G4double);
	void SetAbsorberXpos(G4double);

	//Filter
	void SetFilterPos(G4ThreeVector pos);
	void SetFilterRot(G4double phi);
	void SetFilterXpos(G4int, G4double);
	void SetNbOfFilter(G4int);
	void SetFilterMaterial(G4int, const G4String&);
	void SetFilterThickness(G4int, G4double);

	//World
	void SetWorldMaterial(const G4String&);
	void SetDetectorSizeYZ(G4double);
	void SetNbOfLayers(G4int);

	// collimator
	void SetCollimator(const bool&);
	void SetCollInnerMaterial(G4String);
	void SetCollOuterMaterial(G4String);
	void SetCollInnerRadius(G4double);
	void SetCollMiddleRadius(G4double);
	void SetCollOuterRadius(G4double);
	void SetCollThickness(G4double);
	void SetCollPos(G4ThreeVector pos);

public:
	void PrintDetectorParameters();
	void SetSizeX(G4double);
	G4double GetWorldSizeX() {
		return fWorldSizeX;
	}
	;
	G4double GetWorldSizeYZ() {
		return fWorldSizeYZ;
	}
	;
	G4double GetDetectorSizeYZ() {
		return fDetectorSizeYZ;
	}
	;
	G4int GetNbOfLayers() {
		return fNbOfLayers;
	}
	;

	G4int GetNbOfAbsor() {
		return fNbOfAbsor;
	}
	;
	G4Material* GetAbsorMaterial(G4int i) {
		return fAbsorMaterial[i];
	}
	;
	G4double GetAbsorThickness(G4int i) {
		return fAbsorThickness[i];
	}
	;

	G4double GetLayerThickness() {
		return fLayerThickness;
	}
	;

	G4double GetAbsorberXpos(G4int i) {
		return fXposAbs[i];
	}
	;

	G4double GetxstartAbs(G4int i) {
		return fXstartAbs[i];
	}
	;
	G4double GetxendAbs(G4int i) {
		return fXendAbs[i];
	}
	;

	G4double GetFilterSizeX() {
		return fFilterSizeX;
	}
	;

	G4double GetFilterXpos(G4int i) {
		return fXposFilter[i];
	}
	;
	G4double GetxstartFilter(G4int i) {
		return fXstartFilter[i];
	}
	;
	G4double GetxendFilter(G4int i) {
		return fXendFilter[i];
	}
	;

	G4int GetNbOfFilter() {
		return fNbOfFilter;
	}
	;
	G4Material* GetFilterMaterial(G4int i) {
		return fFilterMaterial[i];
	}
	;
	G4double GetFilterThickness(G4int i) {
		return fFilterThickness[i];
	}
	;

	const G4VPhysicalVolume* GetphysiWorld() {
		return fPhysiWorld;
	}
	;

	const G4Material* GetWorldMaterial() {
		return fDefaultMaterial;
	}
	;
	const G4VPhysicalVolume* GetAbs(G4int i) {
		return fPhysiAbsor[i];
	}
	;
	const G4VPhysicalVolume* GetFilter(G4int i) {
		return fPhysiFilter[i];
	}
	;

	G4LogicalVolume* GetScoringVolume() const {
		return fScoringVolume;
	}

private:
	// Define Absorbers
	G4bool fCheckOverlaps;
	G4int fNbOfAbsor;
	G4Material* fAbsorMaterial[kMaxAbsor];
	G4double fAbsorThickness[kMaxAbsor];

	// Define Filter
	G4int fNbOfFilter;
	G4Material* fFilterMaterial[MaxFilter];
	G4double fFilterThickness[MaxFilter];
	G4double fFilterYZ[MaxFilter];

	// Define Collimators
	G4double fCollinradius;
	G4double fColloutradius;

	// Define Layers
	G4int fNbOfLayers;
	G4double fLayerThickness;

	G4double fXposAbs[kMaxAbsor];
	G4double fXposFilter[MaxFilter];
	G4double fXstartFilter[kMaxAbsor], fXendFilter[kMaxAbsor];
	G4double fXstartAbs[kMaxAbsor], fXendAbs[kMaxAbsor];

	G4double fDetectorSizeYZ;
	G4double fAbsSizeYZ;

	G4double AbsorSeparation;
	G4double fDetectorThickness;
	G4double fXposShield;
	G4double fXposoutCollimator;
	G4double fXposinCollimator;
	G4double fCollimatorYZ;

	G4double fCollimatoroutThickness;
	G4double fCollimatorinThickness;
	G4double fShieldThickness;

	G4double fFilterSizeX;
	G4Material* fDefaultMaterial;
	G4double fWorldSizeYZ;
	G4double fWorldSizeX;

	G4Box* fSolidWorld;
	G4LogicalVolume* fLogicWorld;
	G4VPhysicalVolume* fPhysiWorld;

	G4Tubs* fSolidFilter[MaxFilter];
	G4LogicalVolume* fLogicFilter[MaxFilter];
	G4VPhysicalVolume* fPhysiFilter[MaxFilter];

	G4Box* fSolidAbsor[kMaxAbsor];
	G4LogicalVolume* fLogicAbsor[kMaxAbsor];
	G4VPhysicalVolume* fPhysiAbsor[kMaxAbsor];

	G4Material* fCollMaterialInner;
	G4Tubs* fSolidCollInner;
	G4LogicalVolume* fLogicCollInner;
	G4VPhysicalVolume* fPhysCollInner;

	G4Material* fCollMaterialOuter;
	G4Tubs* fSolidCollOuter;
	G4LogicalVolume* fLogicCollOuter;
	G4VPhysicalVolume* fPhysCollOuter;

	G4Material* fShieldMaterial;
	G4Box* fSolidShield;
	G4LogicalVolume* fLogicShield;
	G4VPhysicalVolume* fPhysShield;

	G4LogicalVolume* fScoringVolume;

	DetectorMessenger* fDetectorMessenger;
	G4Cache<G4GlobalMagFieldMessenger*> fFieldMessenger;

private:
	void DefineMaterials();
	void ComputeDetectorParameters();

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

