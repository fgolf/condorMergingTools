#include <string>
#include <iostream>
#include <fstream>
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TString.h"

using namespace std;

int addBranches(string mData, string inFileName, string outFileName);

int addCMS2Branches(TString infname, TString outfile, unsigned int events, 
					Float_t xsec, Float_t kfactor,
					Float_t filt_eff, bool SortBasketsByEntry = false);


int addBranches(string mData, string inFileName, string outFileName){
  
  
  cout<<"File Name = "<<mData.c_str()<<endl;
  ifstream file(mData.c_str());
  if (!file.good()){
  	cout<<Form("%s is not a good file. exiting..",mData.c_str())<<endl;
  	return 3;
  }

  unsigned int nEvents = 0;
  float kFactor = 0;
  float filtEff = 0;
  float xSect = 0;


  while(file){

	//This assigns the variables such as cross section etc given by the metaData.txt file
	string varName = "";
	string varValue = "";
	file>>varName>>varValue;

	  if(varName.compare("n:") == 0){
		nEvents = atoi(varValue.c_str());
		cout<<"nEvents = "<<nEvents<<endl;
	  }
	  if(varName.compare("k:") == 0){
		kFactor = (float)atof(varValue.c_str());
		cout<<"kFactor = "<<kFactor<<endl;
	  }
	  if(varName.compare("f:") == 0){
		filtEff = atof(varValue.c_str());
		cout<<"filtEff = "<<filtEff<<endl;
	  }
	  if(varName.compare("x:") == 0){
		xSect = atof(varValue.c_str());
		cout<<"xSect = "<<xSect<<endl;
	  }
	  
  }

  addCMS2Branches(inFileName, outFileName, nEvents, xSect, kFactor, filtEff );

  return 0;

}

int addCMS2Branches(TString infname, TString outfile, unsigned int events, 
		      Float_t xsec, Float_t kfactor,
		      Float_t filt_eff, bool SortBasketsByEntry ) {
  
  cout << "Processing File " << infname << endl;

  TFile *f = TFile::Open(infname.Data(), "READ");
  if (! f || f->IsZombie()) {
    cout << "File does not exist!" << endl;
    return 1;
  }
  
  TTree* t = (TTree*)f->Get("Events");
  if (! t || t->IsZombie()) {
    cout << "Tree does not exist!" << endl;
    return 2;
  }
        
  const bool isdata = (Int_t)t->GetMaximum("int_eventMaker_evtisRealData_CMS2.obj");
  
  //-------------------------------------------------------------
  // Removes all non *_CMS2.* branches
  //-------------------------------------------------------------`
  t->SetBranchStatus("*", 0);
  t->SetBranchStatus("*_CMS2.*", 1);

  // Removes the branches (if they exist) that we want to replace
  //evt_xsec_excl
  TString bName = t->GetAlias("evt_xsec_excl");
  //cout << "evt_xsec_excl " << bName << endl;
  if(bName != "") {
    bName.ReplaceAll(".obj", "*");
    t->SetBranchStatus(bName.Data(), 0); 
  }

  //evt_xsec_incl
  bName = t->GetAlias("evt_xsec_incl");
  //cout << "evt_xsec_incl " << bName << endl;
  if(bName != "") {
    bName.ReplaceAll(".obj", "*");
    t->SetBranchStatus(bName.Data(), 0);   
  }
  
  //evt_kfactor
  bName = t->GetAlias("evt_kfactor");
  //cout << "evt_kfactor " << bName << endl;
  if(bName != "") {
    bName.ReplaceAll(".obj", "*");
    t->SetBranchStatus(bName.Data(), 0); 
  }

  //evt_nEvts
  bName = t->GetAlias("evt_nEvts");
  //cout << "evt_nEvts " << bName << endl;
  if(bName != "") {
    bName.ReplaceAll(".obj", "*");
    t->SetBranchStatus(bName.Data(), 0); 
  }

  //evt_filt_eff
  bName = t->GetAlias("evt_filt_eff");
  //cout << "evt_filt_eff " << bName << endl;
  if(bName != "") {
    bName.ReplaceAll(".obj", "*");
    t->SetBranchStatus(bName.Data(), 0); 
  }

  //evt_scale1fb
  bName = t->GetAlias("evt_scale1fb");
  //cout << "evt_scale1fb " << bName << endl;
  if(bName != "") {
    bName.ReplaceAll(".obj", "*");
    t->SetBranchStatus(bName.Data(), 0); 
  }

  TFile *out = TFile::Open(outfile.Data(), "RECREATE");
  TTree *clone;
  if(SortBasketsByEntry)
    clone = t->CloneTree(-1, "fastSortBasketsByEntry");
  else 
    clone = t->CloneTree(-1, "fast");
   

  //-------------------------------------------------------------

  //Calculate scaling factor and put variables into tree 
  Float_t scale1fb = xsec*kfactor*1000*filt_eff/(Float_t)events;

  if(isdata){

	scale1fb = 1.0;
	cout<< "Data file. scale1fb: " << scale1fb << endl;
	
  }else{
	cout << "scale1fb: " << scale1fb << endl; 
  }
  
  TBranch* b1 = clone->Branch("evtscale1fb", &scale1fb, "evt_scale1fb/F");
  TBranch* b2 = clone->Branch("evtxsecexcl", &xsec, "evt_xsec_excl/F");
  TBranch* b3 = clone->Branch("evtxsecincl", &xsec, "evt_xsec_incl/F");
  TBranch* b4 = clone->Branch("evtkfactor", &kfactor, "evt_kfactor/F");
  TBranch* b5 = clone->Branch("evtnEvts", &events, "evt_nEvts/I");
  TBranch* b6 = clone->Branch("evtfilteff", &filt_eff, "evt_filt_eff/F");
   
  clone->SetAlias("evt_scale1fb",  "evtscale1fb");
  clone->SetAlias("evt_xsec_excl", "evtxsecexcl");
  clone->SetAlias("evt_xsec_incl",  "evtxsecincl");
  clone->SetAlias("evt_kfactor",   "evtkfactor");
  clone->SetAlias("evt_nEvts",     "evtnEvts");
  clone->SetAlias("evt_filt_eff",     "evtfilteff");

  Int_t nentries = t->GetEntries();
  for(Int_t i = 0; i < nentries; i++) {
    b1->Fill();
    b2->Fill();
    b3->Fill();
    b4->Fill();
    b5->Fill();
    b6->Fill();
  }
  //-------------------------------------------------------------

  clone->Write(); 
  out->Close();
  f->Close();
  return 0;
  
}






