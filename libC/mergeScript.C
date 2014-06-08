#include <iostream>
#include <fstream>
#include "TChain.h"

using namespace std;

int mergeScript(const string& fileList, const string& outFile ){
  
  if (fileList == ""){
  	cout<<"File list not supplied. Please supply a list of files to be merged. Exiting.."<<endl;
  	return 1;
  }
  
  if (outFile == ""){
  	cout<<"outFile name not supplied. Please supply a name for merged file. Exiting.."<<endl;
  	return 2;
  }
  
  TChain ch1("Events");
  cout<<"Parsing File: "<<fileList.c_str()<<endl;
  ifstream file(fileList.c_str());
  if (!file.good()){
  	cout<<Form("%s is not a good file. exiting..",fileList.c_str())<<endl;
  	return 3;
  }

  string ntupleLocation;

  ///////////////////////////////////////
  //parses list of root files to merge.//
  ///////////////////////////////////////
  while (file){
	ntupleLocation = "";
	file>>ntupleLocation;
	// cout<<ntupleLocation.empty()<<endl;
	if(ntupleLocation.empty()){
	  // cout<<"line is empty"<<endl;
	  continue;
	}else{
	  cout<<Form("Running Command:ch1.Add(\"%s\");",ntupleLocation.c_str())<<endl;
	  ch1.Add(ntupleLocation.c_str());
	}
  }
  

  unsigned int nEntries = ch1.GetEntries();
  
  cout << Form("Merging Sample with %u entries... ",nEntries) << endl;

  ch1.Merge(outFile.c_str(), "fast");

  return 0;
  
}
