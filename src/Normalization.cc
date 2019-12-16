
#include "Normalization.hh"
#include "DataSet.hh"
#include "G4SystemOfUnits.hh"
#include "G4VDataSetAlgorithm.hh"
#include "G4LogLogInterpolation.hh"

Normalization::Normalization()

{ }

Normalization::~Normalization()

{ }

const DataSet* Normalization::Normalize(G4double minIntExtreme, G4double maxIntExtreme, G4int nBins, G4String fileName)
{
 
 G4VDataSetAlgorithm* interpolation = new G4LogLogInterpolation();

 DataSet* dataSet = new DataSet(1,fileName,interpolation,keV,1);
 G4double integral = Integrate(minIntExtreme, maxIntExtreme, nBins, dataSet);
 
 G4VDataSetAlgorithm* interpolation2 = new G4LogLogInterpolation();
 
 DataSet* finalDataSet = new DataSet(1,fileName,interpolation2,keV,1/(integral/keV));


 return finalDataSet;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double Normalization::Integrate(G4double minIntExtreme, G4double maxIntExtreme, G4int nBins, DataSet* dataSet)
{
 G4double partialHeight = 0;
 G4int id = 0;
 G4double lenghtOfBins = (maxIntExtreme - minIntExtreme)/nBins;
 
 for (G4int i = 0; i<nBins; i++)
   {
     G4double midPoint = minIntExtreme + i*lenghtOfBins+lenghtOfBins/2;
    
     G4double heightOfRectangle = dataSet->FindValue(midPoint,id);
    
     partialHeight += heightOfRectangle;
   
   }

 G4double integral = lenghtOfBins * partialHeight;
 
 delete dataSet;
 dataSet = 0;
 return integral;
}




