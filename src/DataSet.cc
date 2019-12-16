
#include "DataSet.hh"
#include <fstream>
#include "G4VDataSetAlgorithm.hh"

DataSet::DataSet(G4int /*Z*/,
			 G4DataVector* points, 
			 G4DataVector* values,
			 const G4VDataSetAlgorithm* interpolation,
			 G4double unitE, G4double unitData)
  :energies(points), data(values), algorithm(interpolation)
{
  numberOfBins = energies->size();
  unit1 = unitE;
  unit2 = unitData;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

DataSet:: DataSet(G4int /*Z*/, 
			  const G4String& dataFile,
			  const G4VDataSetAlgorithm* interpolation,
			  G4double unitE, G4double unitData)
  : algorithm(interpolation)
{
  energies = new G4DataVector;
  data = new G4DataVector;
  unit1 = unitE;
  unit2 = unitData;  
  LoadData(dataFile);
  numberOfBins = energies->size();
//PrintData();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

// Destructor
DataSet::~DataSet()
{ 
  delete energies;
  delete data;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double DataSet::FindValue(G4double e, G4int) const
{
  G4double value;
  G4double e0 = (*energies)[0];
  // Protections
  size_t bin = FindBinLocation(e);
  if (bin == numberOfBins)
    {
     
      value = (*data)[bin];
    }
  else if (e <= e0)
    {
    
      value = (*data)[0];
    }
  else
    {
      value = algorithm->Calculate(e,bin,*energies,*data);
    }
  return value;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4int DataSet::FindBinLocation(G4double energy) const
{
  // Protection against call outside allowed range
  G4double e0 = (*energies)[0];
  if (energy < e0)
    {
    
      energy = e0;
    }

  size_t lowerBound = 0;
  size_t upperBound = numberOfBins - 1;
  
  // Binary search
  while (lowerBound <= upperBound) 
    {
      size_t midBin = (lowerBound + upperBound)/2;
      if ( energy < (*energies)[midBin] ) upperBound = midBin-1;
      else lowerBound = midBin+1;
  }
  return upperBound;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4bool DataSet::LoadData(const G4String& fileName)
{
  // Build the complete string identifying the file with the data set
  G4String dirFile = "";
  
  char* path;

  path = getenv("XRAYDATA");
  if (!path)
    path = getenv("PWD");

  G4cout << path << G4endl;
  G4cout << fileName << G4endl;


  G4String pathString(path);
  pathString += "\0";
  dirFile = pathString + "/" + fileName + ".dat";
  std::ifstream file(dirFile);
  std::filebuf* lsdp = file.rdbuf();
  
  if (! (lsdp->is_open()) )
	{
	  G4ExceptionDescription execp;
	  execp << "DataSet - data file: " + dirFile + " not found"<<G4endl;
	  G4Exception("DataSet::LoadData()","xray",
	  FatalException, execp);
	}
  G4double a = 0;
  G4int k = 1;

  do
    {
      file >> a;
      G4int nColumns = 2;
      // The file is organized into two columns:
      // 1st column is the energy
      // 2nd column is the corresponding value
      // The file terminates with the pattern: -1   -1
      //                                       -2   -2
      if (a == -1 || a == -2)
	{
	 
	}
      else
	{
	  if (k%nColumns != 0)
	    {	
	      G4double e = a * unit1;
	      energies->push_back(e);
	    
	      k++;

	    }
	  else if (k%nColumns == 0)
	    {
	      G4double value = a * unit2;
	      data->push_back(value);
	     
	      k = 1;
	    }
	}
      
    } while (a != -2); // end of file
  
  file.close();
  return true;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void DataSet::PrintData() const
{
  size_t size = numberOfBins;
  for (size_t i=0; i<size; i++)
    {
      G4double e = (*energies)[i]  / unit1;
      G4double sigma = (*data)[i] / unit2 ;
      G4cout << "Point: "
	     << e
	     << " - Data value : "
	     << sigma 
	     << G4endl; 
    }
}



