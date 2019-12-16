
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

     const G4int MaxAbsor = 22;                        // 0 + 9  
     const G4int MaxFilter = 10;                       // 0 + 9

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  
   DetectorConstruction();
  ~DetectorConstruction();

public:
  
  void SetNbOfAbsor     (G4int);      
  void SetAbsorMaterial (G4int,const G4String&);     
  void SetAbsorThickness(G4int,G4double);
  void SetFilterPos(G4ThreeVector pos);
  void SetFilterRot(G4double phi);
  void SetAbsorberXpos(G4double);
  void SetFilterXpos(G4int, G4double);

  void SetNbOfFilter     (G4int);      
  void SetFilterMaterial (G4int,const G4String&);     
  void SetFilterThickness(G4int,G4double);

  void SetWorldMaterial (const G4String&);
  void SetCalorSizeYZ   (G4double);          
  void SetNbOfLayers    (G4int);   

	// collimator
	void SetCollimator(const bool&);
	void SetCollInnerMaterial(G4String);
	void SetCollOuterMaterial(G4String);
	void SetCollInnerRadius(G4double);
	void SetCollMiddleRadius(G4double);
	void SetCollOuterRadius(G4double);
	void SetCollThickness(G4double);
	void SetCollPos(G4ThreeVector pos);

  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();
     
public:
  void PrintCalorParameters(); 
  void SetSizeX    (G4double);                  
  G4double GetWorldSizeX()           {return fWorldSizeX;}; 
  G4double GetWorldSizeYZ()          {return fWorldSizeYZ;};
     
  G4double GetCalorThickness()       {return fCalorThickness;}; 
  G4double GetCalorSizeYZ()          {return fCalorSizeYZ;};
  G4int GetNbOfLayers()              {return fNbOfLayers;};
     
  G4int       GetNbOfAbsor()             {return fNbOfAbsor;}; 
  G4Material* GetAbsorMaterial(G4int i)  {return fAbsorMaterial[i];};
  G4double    GetAbsorThickness(G4int i) {return fAbsorThickness[i];};      

  G4double    GetAbsorberXpos(G4int i)      {return fXposAbs[i];};

  G4double    GetxstartAbs(G4int i)         {return fXstartAbs[i];};
  G4double    GetxendAbs(G4int i)           {return fXendAbs[i];};

  G4double    GetFilterSizeX()    	  {return fFilterSizeX;};

  G4double    GetFilterXpos(G4int i)      {return fXposFilter[i];};
  G4double    GetxstartFilter(G4int i)         {return fXstartFilter[i];};
  G4double    GetxendFilter(G4int i)           {return fXendFilter[i];};


  G4int       GetNbOfFilter()             {return fNbOfFilter;}; 
  G4Material* GetFilterMaterial(G4int i)  {return fFilterMaterial[i];};
  G4double    GetFilterThickness(G4int i) {return fFilterThickness[i];}; 

  const G4VPhysicalVolume* GetphysiWorld()        {return fPhysiWorld;};

  const G4Material*        GetWorldMaterial()     {return fDefaultMaterial;};
  const G4VPhysicalVolume* GetAbs(G4int i)   	{return fPhysiAbsor[i];};
  const G4VPhysicalVolume* GetFilter(G4int i)   {return fPhysiFilter[i];};



 G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }
private:
 G4bool fCheckOverlaps; // option to activate checking of volumes overlaps
  G4int              fNbOfAbsor;
  G4Material*        fAbsorMaterial [MaxAbsor];
  G4double           fAbsorThickness[MaxAbsor];

  G4int              fNbOfFilter;
  G4Material*        fFilterMaterial [MaxFilter];
  G4double	     fFilterThickness[MaxFilter];
  G4double	     fFilterYZ[MaxFilter];
  G4double fCollinradius;
  G4double fColloutradius;
  
  G4int              fNbOfLayers;
  G4double           fLayerThickness;
  
  G4double           fXposAbs[MaxAbsor];
  G4double           fXposFilter[MaxFilter];
  G4double           fXstartFilter[MaxAbsor], fXendFilter[MaxAbsor];
  G4double           fXstartAbs[MaxAbsor], fXendAbs[MaxAbsor];

  G4double           fCalorSizeYZ;
  G4double  	     fAbsSizeYZ;
  G4double           AbsorSeparation;
  G4double			 fCalorThickness;
  G4double 			 fXposoutCollimator;
  G4double 			 fXposinCollimator;
  G4double			 fCollimatorYZ;

  G4double           fCollimatoroutThickness;
  G4double           fCollimatorinThickness;
  G4double 	     	  Filtergap;

  G4double           fFilterSizeX;
  G4Material*        fDefaultMaterial;
  G4double           fWorldSizeYZ;
  G4double           fWorldSizeX;

  G4Box*             fSolidWorld;
  G4LogicalVolume*   fLogicWorld;
  G4VPhysicalVolume* fPhysiWorld;

  G4Box*             fSolidCalor;
  G4LogicalVolume*   fLogicCalor;
  G4VPhysicalVolume* fPhysiCalor;

  G4Box*             fSolidLayer;
  G4LogicalVolume*   fLogicLayer;
  G4VPhysicalVolume* fPhysiLayer;

  G4Box*             fSolidAbsor[MaxAbsor];
  G4LogicalVolume*   fLogicAbsor[MaxAbsor];
  G4VPhysicalVolume* fPhysiAbsor[MaxAbsor];	

  
  G4Tubs*             fSolidFilter[MaxFilter];
  G4LogicalVolume*   fLogicFilter[MaxFilter];
  G4VPhysicalVolume* fPhysiFilter[MaxFilter] ;
  G4LogicalVolume*  fScoringVolume;



	G4Material* fCollMaterialInner;
	G4Material* fCollMaterialOuter;
	G4Tubs* fSolidCollOuter;
	G4Tubs* fSolidCollInner;

	G4LogicalVolume* fLogicCollInner;
	G4LogicalVolume* fLogicCollOuter;
	G4VPhysicalVolume* fPhysCollInner;
	G4VPhysicalVolume* fPhysCollOuter;


  DetectorMessenger* fDetectorMessenger;
  G4Cache<G4GlobalMagFieldMessenger*> fFieldMessenger;
  
 
private:
  void DefineMaterials();
  void ComputeCalorParameters();
  virtual G4VPhysicalVolume* ConstructCalorimeter();
  

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

