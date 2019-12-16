
#ifndef Normalization_h
#define Normalization_h 1
#include "globals.hh"

class DataSet;
class Normalization
{
public:

  //constructor
  Normalization();

  //destructor
  ~Normalization();

  //this method returns a data set equivalent to the one in the file whose 
  //name must be passed as the last argument normalized to the value returned 
  //by Integrate
  //the first and second arguments identifies the energy value in which 
  //Integrate() integrates, the third is the number of buns used in the
  //integration
  const DataSet* Normalize(G4double, G4double, G4int,G4String);

private:
  //this method integrates the function achieved interpolating
  //berween the points of the data file and returns the value of the integral 
  G4double Integrate(G4double, G4double, G4int, DataSet*);

};
 
#endif
