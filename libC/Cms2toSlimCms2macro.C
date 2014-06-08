#include "TFile.h"
#include "TTree.h"
#include <string>
#include <iostream>

int Cms2toSlimCms2macro(const std::string &ifpath, const std::string &ifName){
  
  const std::string ifname = ifpath+"/"+ifName;
  TFile *oldfile = TFile::Open(ifname.c_str());
	if (not oldfile)
	{
		std::cout << "Could not open input file.  Exiting." << std::endl;
		return 1;
	}

	TTree *oldtree = (TTree*)oldfile->Get("Events");
	if (not oldtree)
	{
		std::cout << "Input file does not contain a good tree.  Exiting." << std::endl;
		return 2;
	}

	oldtree->SetBranchStatus("*", 1);
	oldtree->SetBranchStatus("*_*_pfcandsposAtEcalp4_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_pfcandsisMuIso_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_pfcandsecalE_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_pfcandshcalE_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_pfcandsrawEcalE_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_pfcandsrawHcalE_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_pfcandspS1E_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_pfcandspS2E_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_pfcandsdeltaP_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_pfcandsmvaepi_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_pfcandsmvaemu_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_pfcandsmvapimu_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_pfcandsmvanothinggamma_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_pfcandsmvanothingnh_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_pfcandsflag_CMS2*", 0);
	oldtree->SetBranchStatus("*_trackMaker_trksvertexp4_CMS2*", 0);
	oldtree->SetBranchStatus("*_trackMaker_trksouterp4_CMS2*", 0);
	oldtree->SetBranchStatus("*_trackMaker_trksinnerposition_CMS2*", 0);
	oldtree->SetBranchStatus("*_trackMaker_trksouterposition_CMS2*", 0);
	oldtree->SetBranchStatus("*_trackMaker_trkslayer1layer_CMS2*", 0);
	oldtree->SetBranchStatus("*_trackMaker_trkslayer1sizerphi_CMS2*", 0);
	oldtree->SetBranchStatus("*_trackMaker_trkslayer1sizerz_CMS2*", 0);
	oldtree->SetBranchStatus("*_trackMaker_trkslayer1charge_CMS2*", 0);
	oldtree->SetBranchStatus("*_trackMaker_trkslayer1det_CMS2*", 0);
	oldtree->SetBranchStatus("*_trackMaker_trksd0corrPhi_CMS2*", 0);
	oldtree->SetBranchStatus("*_trackMaker_trksnlayers3D_CMS2*", 0);
	oldtree->SetBranchStatus("*_trackMaker_trksnlayersLost_CMS2*", 0);
	oldtree->SetBranchStatus("*_trackMaker_trkslostpixelhits_CMS2*", 0);
	oldtree->SetBranchStatus("*_l1Maker_*_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_jetsmcdr_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_jetsmcidx_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_jetsmcemEnergy_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_jetsmchadEnergy_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_jetsmcinvEnergy_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_jetsmcotherEnergy_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_jetsmcp4_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_jetsmcgpdr_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_jetsmcgpidx_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_jetsmcgpp4_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_jetsmcid_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_jetsmcmotherid_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_jetsmcmotherp4_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_jetsmc3dr_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_jetsmc3idx_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_jetsmc3id_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_trkmcid_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_trkmcmotherid_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_trkmcidx_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_trkmcp4_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_trkmcdr_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_trkmc3id_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_trkmc3motherid_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_trkmc3idx_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_trkmc3motheridx_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_trkmc3dr_CMS2*", 0);
	oldtree->SetBranchStatus("*_jptMaker_*_CMS2*", 0);
	oldtree->SetBranchStatus("*_bTagJPTJetMaker_*_CMS2*", 0);
	oldtree->SetBranchStatus("*_jetMaker_*_CMS2*", 0);
	oldtree->SetBranchStatus("*_bTagMaker_*_CMS2*", 0);
	oldtree->SetBranchStatus("*_trkJetMaker_*_CMS2*", 0);
	oldtree->SetBranchStatus("*_bTagTrkMaker_*_CMS2*", 0);
	oldtree->SetBranchStatus("*_bTagPFJetMaker_pfjetssoftElectronByIP3dBJetTag_CMS2*", 0);
	oldtree->SetBranchStatus("*_bTagPFJetMaker_pfjetssoftElectronByPtBJetTag_CMS2*", 0);
	oldtree->SetBranchStatus("*_bTagPFJetMaker_pfjetssoftMuonByIP3dBJetTag_CMS2*", 0);
	oldtree->SetBranchStatus("*_bTagPFJetMaker_pfjetssoftMuonByPtBJetTag_CMS2*", 0);
	oldtree->SetBranchStatus("*_bTagPFJetMaker_pfjetssoftMuonBJetTag_CMS2*", 0);
	oldtree->SetBranchStatus("*_bTagPFJetMaker_pfjetssoftElectronTag_CMS2*", 0);
	oldtree->SetBranchStatus("*_davertexMaker_*_CMS2*", 0);
	oldtree->SetBranchStatus("*_vertexMakerWithBS_*_CMS2*", 0);
	oldtree->SetBranchStatus("*_secondaryVertexMaker_*_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_gsftrksvertexp4_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_gsftrksouterp4_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_gsftrksinnerposition_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_gsftrksouterposition_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_gsftrkslayer1layer_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_gsftrkslayer1sizerphi_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_gsftrkslayer1sizerz_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_gsftrkslayer1charge_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_gsftrkslayer1det_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_gsftrksd0corrPhi_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_gsftrksnlayers3D_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_gsftrksnlayersLost_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_gsftrkslostpixelhits_CMS2*", 0);
	oldtree->SetBranchStatus("*_trkToVtxAssMaker_*_CMS2*", 0);
	oldtree->SetBranchStatus("*_myTrkJetMetMaker_*_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_convsrefitPairMomp4_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_convsvtxpos_CMS2*", 0);
	oldtree->SetBranchStatus("*_hypDilepVertexMaker_*_CMS2*", 0);
	oldtree->SetBranchStatus("*_hypTrilepMaker_*_CMS2*", 0);
	oldtree->SetBranchStatus("*_hypQuadlepMaker_*_CMS2*", 0);
	oldtree->SetBranchStatus("*_hypDilepMaker_hypotherjetsp4_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypnjets_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypnojets_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypltvalidHits_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypltlostHits_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypltd0_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypltz0_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypltd0corr_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypltz0corr_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypltchi2_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypltndof_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypltd0Err_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypltz0Err_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypltptErr_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypltetaErr_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypltphiErr_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hyplttrkp4_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypllvalidHits_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hyplllostHits_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hyplld0_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypllz0_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hyplld0corr_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypllz0corr_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypllchi2_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypllndof_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hyplld0Err_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypllz0Err_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypllptErr_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hyplletaErr_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypllphiErr_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hyplltrkp4_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypltdPhiunCorrMet_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hyplldPhiunCorrMet_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypltdPhimuCorrMet_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hyplldPhimuCorrMet_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypltdPhitcMet_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hyplldPhitcMet_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypltdPhimetMuonJESCorr_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hyplldPhimetMuonJESCorr_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypdPhinJetunCorrMet_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypdPhinJetmuCorrMet_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypdPhinJettcMet_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypdPhinJetmetMuonJESCorr_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypmt2tcMet_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypmt2muCorrMet_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypmt2metMuonJESCorr_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypjetsidx_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypotherjetsidx_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypjetsp4_CMS2*", 0);
	oldtree->SetBranchStatus("*_*_hypotherjetsp4_CMS2*", 0);
	oldtree->SetBranchStatus("*_electronMaker_elsconvsposp4_CMS2*", 0);
	oldtree->SetBranchStatus("*_pfElectronMaker_pfelsposAtEcalp4_CMS2*", 0);
	oldtree->SetBranchStatus("*_puSummaryInfoMaker_puInfosumpthighpt_CMS2*", 0);
	oldtree->SetBranchStatus("*_puSummaryInfoMaker_puInfozpositions_CMS2*", 0);
	oldtree->SetBranchStatus("*_puSummaryInfoMaker_puInfosumptlowpt_CMS2*", 0);
	oldtree->SetBranchStatus("*_puSummaryInfoMaker_puInfontrkshighpt_CMS2*", 0);
	oldtree->SetBranchStatus("*_puSummaryInfoMaker_puInfontrkslowpt_CMS2*", 0);
	oldtree->SetBranchStatus("*_jetToElAssMaker_jetsclosestElectronDR_CMS2*", 0);
	oldtree->SetBranchStatus("*_jetToMuAssMaker_jetsclosestMuonDR_CMS2*", 0);
	oldtree->SetBranchStatus("*_muonMaker_musgfitouterPosp4_CMS2*", 0);
	oldtree->SetBranchStatus("*_muonMaker_musfitfirsthitp4_CMS2*", 0);
	oldtree->SetBranchStatus("*_muonMaker_musfitdefaultp4_CMS2*", 0);
	oldtree->SetBranchStatus("*_muonMaker_musfitpickyp4_CMS2*", 0);
	oldtree->SetBranchStatus("*_muonMaker_musecalposp4_CMS2*", 0);
	oldtree->SetBranchStatus("*_scMaker_scsvtxp4_CMS2*", 0);

	const std::string ofname = ifName;
	// ofname.erase(ofname.length()-5,5);
	// ofname.append("_slim.root");
	TFile *newfile = TFile::Open(ofname.c_str(),"recreate");
	TTree *newtree = oldtree->CloneTree(0);
	newtree->CopyEntries(oldtree);
	newfile->Write();

	delete oldfile;
	delete newfile;

	return 0;
}
