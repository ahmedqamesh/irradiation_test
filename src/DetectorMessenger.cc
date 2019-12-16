#include "DetectorMessenger.hh"
#include <sstream>
#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction * Det)
:G4UImessenger(),Detector(Det),
 fTestemDir(0),
 fDetDir(0),
 fSizeYZCmd(0),
 fNbLayersCmd(0),
 fNbAbsorCmd(0),
 fAbsorCmd(0),
 fNbFilterCmd(0),
 fFilterCmd(0),
fxposfilterCmd(0),
fposfilterCmd(0),
fRotfilterCmd(0)
{ 
  fTestemDir = new G4UIdirectory("/testem/");
  fTestemDir->SetGuidance("UI commands specific to this example");
  
  fDetDir = new G4UIdirectory("/testem/det/");
  fDetDir->SetGuidance("detector construction commands");

  fSizeYZCmd = new G4UIcmdWithADoubleAndUnit("/testem/det/setSizeYZ",this);
  fSizeYZCmd->SetGuidance("Set tranverse size of the calorimeter");
  fSizeYZCmd->SetParameterName("Size",false);
  fSizeYZCmd->SetRange("Size>0.");
  fSizeYZCmd->SetUnitCategory("Length");
  fSizeYZCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fSizeYZCmd->SetToBeBroadcasted(false);
    
  fNbLayersCmd = new G4UIcmdWithAnInteger("/testem/det/setNbOfLayers",this);
  fNbLayersCmd->SetGuidance("Set number of layers.");
  fNbLayersCmd->SetParameterName("NbLayers",false);
  fNbLayersCmd->SetRange("NbLayers>0");
  fNbLayersCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fNbLayersCmd->SetToBeBroadcasted(false);
    
  fNbAbsorCmd = new G4UIcmdWithAnInteger("/testem/det/setNbOfAbsor",this);
  fNbAbsorCmd->SetGuidance("Set number of Absorbers.");
  fNbAbsorCmd->SetParameterName("NbAbsor",false);
  fNbAbsorCmd->SetRange("NbAbsor>0");
  fNbAbsorCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fNbAbsorCmd->SetToBeBroadcasted(false);
     
  fAbsorCmd = new G4UIcommand("/testem/det/setAbsor",this);
  fAbsorCmd->SetGuidance("Set the absor nb, the material, the thickness.");
  fAbsorCmd->SetGuidance("  absor number : from 1 to NbOfAbsor");
  fAbsorCmd->SetGuidance("  material name");
  fAbsorCmd->SetGuidance("  thickness (with unit) : t>0."); 
  fAbsorCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fAbsorCmd->SetToBeBroadcasted(false);


  fNbFilterCmd = new G4UIcmdWithAnInteger("/testem/det/setNbOfFilter",this);
  fNbFilterCmd->SetGuidance("Set number of Filters.");
  fNbFilterCmd->SetParameterName("NbFilter",false);
  fNbFilterCmd->SetRange("NbFilter>0");
  fNbFilterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fNbFilterCmd->SetToBeBroadcasted(false);
     
  fFilterCmd = new G4UIcommand("/testem/det/setFilter",this);
  fFilterCmd->SetGuidance("Set the Filter nb, the material, the thickness.");
  fFilterCmd->SetGuidance("  Filter number : from 1 to NbOfFilter");
  fFilterCmd->SetGuidance("  material name");
  fFilterCmd->SetGuidance("  thickness (with unit) : t>0."); 
  fFilterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fFilterCmd->SetToBeBroadcasted(false);


  fxposfilterCmd = new G4UIcommand("/testem/det/setXposFilter",this);
  fxposfilterCmd->SetGuidance("Set position. of the Filter x y z");
  fxposfilterCmd->SetGuidance("  Filter number : from 1 to NbOfFilter");
  fxposfilterCmd->SetGuidance("position (with unit) : <= world range");
  fxposfilterCmd->AvailableForStates(G4State_Idle);
  fxposfilterCmd->SetToBeBroadcasted(false);


  fposfilterCmd = new G4UIcmdWith3VectorAndUnit("/testem/det/setfilter/pos", this);
  fposfilterCmd->SetGuidance("Set pos. of the Filter x y z");
  fposfilterCmd->AvailableForStates(G4State_Idle);
  fposfilterCmd->SetToBeBroadcasted(false);


  fxposAbsCmd = new G4UIcommand("/testem/det/setXposAbs",this);
  fxposAbsCmd->SetGuidance("Set x position of the first Abs");
  fxposAbsCmd->SetGuidance("position (with unit) : <= world size");
  fxposAbsCmd->SetGuidance("  (with unit) : t>0.");
  fxposAbsCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fxposAbsCmd->SetToBeBroadcasted(false);

  fRotfilterCmd = new G4UIcmdWithADoubleAndUnit("/testem/det/setfilter/rot", this);
  fRotfilterCmd->SetGuidance("Set Rotation of the Filter");
  fRotfilterCmd->SetUnitCategory("Angle");
  fRotfilterCmd->AvailableForStates(G4State_Idle);
  fRotfilterCmd->SetToBeBroadcasted(false);

  G4UIparameter* AbsNbPrm = new G4UIparameter("AbsorNb",'i',false);
  AbsNbPrm->SetGuidance("absor number : from 1 to NbOfAbsor");
  AbsNbPrm->SetParameterRange("AbsorNb>0");

  //
  G4UIparameter* MatPrm = new G4UIparameter("material",'s',false);
  MatPrm->SetGuidance("material name");

  //    
  G4UIparameter* ThickPrm = new G4UIparameter("thickness",'d',false);
  ThickPrm->SetGuidance("thickness of absorber");
  ThickPrm->SetParameterRange("thickness>0.");

  //
  G4UIparameter* unitPrm = new G4UIparameter("unit",'e',false);
  unitPrm->SetGuidance("unit of thickness");
  G4String unitList = G4UIcommand::UnitsList(G4UIcommand::CategoryOf("mm"));
  unitPrm->SetParameterCandidates(unitList);

 //
  G4UIparameter* FilterNbPrm = new G4UIparameter("FilterNb",'i',false);
  FilterNbPrm->SetGuidance("Filter number : from 1 to NbOfFilter");
  FilterNbPrm->SetParameterRange("FilterNb>0");

  //
  G4UIparameter* posPrm = new G4UIparameter("position",'d',false);
  posPrm->SetGuidance("position of Material");

  //
  fAbsorCmd->SetParameter(AbsNbPrm);
  fAbsorCmd->SetParameter(MatPrm);
  fAbsorCmd->SetParameter(ThickPrm);
  fAbsorCmd->SetParameter(unitPrm);


  //part for Filters
  fFilterCmd->SetParameter(FilterNbPrm);
  fFilterCmd->SetParameter(MatPrm);
  fFilterCmd->SetParameter(ThickPrm);
  fFilterCmd->SetParameter(unitPrm);

  fxposfilterCmd->SetParameter(FilterNbPrm);
  fxposfilterCmd->SetParameter(posPrm);
  fxposfilterCmd->SetParameter(unitPrm);

  fxposAbsCmd->SetParameter(posPrm);
  fxposAbsCmd->SetParameter(unitPrm);



}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
  delete fSizeYZCmd;
  delete fNbLayersCmd;
  delete fNbAbsorCmd;
  delete fAbsorCmd;
  delete fNbFilterCmd;
  delete fFilterCmd;
  delete fDetDir;  
  delete fTestemDir;
  delete fxposfilterCmd;
  delete fxposAbsCmd;
  delete fposfilterCmd;
  delete fRotfilterCmd;
  G4cout << "Detector Messenger deleted" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if( command == fSizeYZCmd )
   { Detector->SetCalorSizeYZ(fSizeYZCmd->GetNewDoubleValue(newValue));}

  if( command == fxposfilterCmd )

   {	G4int num3;
   	   	G4double tick3;
   	   	G4String unt3;
	   std::istringstream is(newValue);
   	   is >> num3 >> tick3 >> unt3;
   	   tick3 *= G4UIcommand::ValueOf(unt3);
	   Detector->SetFilterXpos(num3, tick3);

   }
  if( command == fposfilterCmd ){
	  Detector-> SetFilterPos(fposfilterCmd->GetNew3VectorValue(newValue));
  }
  if (command ==  fRotfilterCmd)
		Detector->SetFilterRot(fRotfilterCmd->GetNewDoubleValue(newValue));
  if( command == fxposAbsCmd )
   {
   	   G4double tick4;
   	   	G4String unt4;
	   std::istringstream is(newValue);
   	   is >> tick4 >> unt4;
   	   tick4 *= G4UIcommand::ValueOf(unt4);
	  Detector->SetAbsorberXpos(tick4);
   }

  if( command == fNbLayersCmd )
   { Detector->SetNbOfLayers(fNbLayersCmd->GetNewIntValue(newValue));}

  if( command == fNbAbsorCmd )
   { Detector->SetNbOfAbsor(fNbAbsorCmd->GetNewIntValue(newValue));}

  if (command == fAbsorCmd)
   {
     G4int num; G4double tick;
     G4String unt, mat;
     std::istringstream is(newValue);
     is >> num >> mat >> tick >> unt;
     G4String material=mat;
     tick *= G4UIcommand::ValueOf(unt);
     Detector->SetAbsorMaterial(num,material);
     Detector->SetAbsorThickness(num,tick);
   }

 if( command == fNbFilterCmd )
   { Detector->SetNbOfFilter(fNbFilterCmd->GetNewIntValue(newValue));}
   
  if (command == fFilterCmd)
   {
     G4int num2;
     G4double tick2;
     G4String unt2, mat2;
     std::istringstream is(newValue);
     is >> num2 >> mat2 >> tick2 >> unt2;
     G4String material2=mat2;
     tick2 *= G4UIcommand::ValueOf(unt2);
     Detector->SetFilterMaterial(num2,material2);
     Detector->SetFilterThickness(num2,tick2);
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
