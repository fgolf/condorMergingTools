
int runAddBranches(const string &mData, const string &inFileName, const string &outFileName, const string &FWLite){
  
  if (FWLite.c_str() == ""){
	gSystem->Load("libMiniFWLite_CMSSW_5_3_2_patch4_V05-03-18.so");
  }else{
	gSystem->Load(FWLite.c_str());
  }

  gROOT->ProcessLine(".L addBranches.C+");
  addBranches(mData, inFileName, outFileName);
  
 return 0;

}
