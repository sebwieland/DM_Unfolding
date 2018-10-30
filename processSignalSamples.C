using namespace std;

TH1* processSignal(TString SignalName) {
	cout << "processing " << SignalName << endl;
	// TString ntuples_path = "/nfs/dust/cms/user/swieland/Darkmatter/ntuples_signal_tagging";
	TString ntuples_path = "/nfs/dust/cms/user/swieland/Darkmatter/ntuples_signal_madgraph_tagging";
	TChain* chain = new TChain("MVATree");
	chain->Add(TString(ntuples_path + "/" + SignalName + "/" + SignalName + "*_nominal_Tree.root"));
	cout << "Processing " << chain->GetEntries() << " entries" << endl;
	// Heidelbergs binning
	// double bins[13] = {200., 240., 300., 380., 470., 570., 670., 790., 910., 1040., 1180., 1330., 2000. };
	// Heidelbergs binning modified
	// double bins[13] = {200., 250., 300., 350., 450., 550., 650., 750., 850., 1000., 1150., 1300., 1400 };
	// 2 sigma Binning
	// double bins[14] = {250., 320., 390., 460., 530., 600., 670., 740., 820., 900., 980., 1060., 1160, 1300.};


	// double bins[12] = {200., 300., 400., 500., 600., 700., 800., 900., 1000., 1100, 1200, 1400.};

	std::vector<double> bins = {250, 350, 450, 550, 650, 750, 850, 950, 1050, 1150, 1300};
	// double bins[10] = {250, 350, 450, 550, 650, 750, 850, 950, 1050, 1150};
	// std::vector<double> bins = {21,250, 1100};
	// std::vector<double> bins = {250,300,350,400,450,500,550,600,650,700,750,800,900,1000,1100,1200,1400};
	// 2 sigma
	// std::vector<double> bins = {250,320,390,460,530,600,670,740,820,900,980,1060,1260,1460};

	if (bins.size() == 3) {
		std::vector<double> tmp = bins;
		bins.clear();
		double BinWidth = abs(tmp.at(2) - tmp.at(1)) / tmp.at(0);
		for (int i = 0; i <= tmp.at(0); i++) {
			bins.push_back(tmp.at(1) + i * BinWidth);
		}
	}


	float signalXS;
	TTree *XS_tree = new TTree("XS", "XS");
	XS_tree->ReadFile("Madgraph_Signal_XS.csv", "sample/C:xs/F");

	char sampleVar[100] = "october";
	// (TString) sampleVar="";
	float xsVar = 0;
	XS_tree->SetBranchAddress("sample", &sampleVar);
	XS_tree->SetBranchAddress("xs", &xsVar);
	double entries = XS_tree->GetEntries();
	for (int i = 0; i < entries; i++)
	{
		XS_tree->GetEntry(i);
		if (sampleVar == SignalName) {
			signalXS = xsVar;
			// cout << sampleVar << " has XS: " << signalXS << endl;
		}
	}


	TH1F* h_sig(nullptr);
	h_sig = new TH1F("h_sig", "Signal", bins.size() - 1  , bins.data() );
	h_sig->Sumw2();

	TH1F* h_sig_MuRup(nullptr);
	h_sig_MuRup = new TH1F("h_sig_MuRup", "SignalMuRup", bins.size() - 1  , bins.data() );
	h_sig_MuRup->Sumw2();

	TH1F* h_sig_MuRdown(nullptr);
	h_sig_MuRdown = new TH1F("h_sig_MuRdown", "SignalMuRdown", bins.size() - 1  , bins.data() );
	h_sig_MuRdown->Sumw2();

	TH1F* h_sig_MuFup(nullptr);
	h_sig_MuFup = new TH1F("h_sig_MuFup", "SignalMuFup", bins.size() - 1  , bins.data() );
	h_sig_MuFup->Sumw2();

	TH1F* h_sig_MuFdown(nullptr);
	h_sig_MuFdown = new TH1F("h_sig_MuFdown", "SignalMuFdown", bins.size() - 1  , bins.data() );
	h_sig_MuFdown->Sumw2();

	TTreeReader theReader(chain);
	TTreeReaderValue<Float_t> Gen(theReader, "Gen_Hadr_Recoil_Pt");
	TTreeReaderValue<Float_t> Reco(theReader, "Hadr_Recoil_Pt");

	//triggers
	TTreeReaderValue<Long64_t> Triggered_HLT_PFMET170_X (theReader, "Triggered_HLT_PFMET170_X");
	TTreeReaderValue<Long64_t> Triggered_HLT_PFMETNoMu100_PFMHTNoMu100_IDTight_X (theReader,  "Triggered_HLT_PFMETNoMu100_PFMHTNoMu100_IDTight_X" );
	TTreeReaderValue<Long64_t> Triggered_HLT_PFMETNoMu110_PFMHTNoMu110_IDTight_X (theReader,  "Triggered_HLT_PFMETNoMu110_PFMHTNoMu110_IDTight_X" );
	TTreeReaderValue<Long64_t> Triggered_HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_X (theReader,  "Triggered_HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_X" );
	TTreeReaderValue<Long64_t> Triggered_HLT_PFMETNoMu90_PFMHTNoMu90_IDTight_X (theReader,  "Triggered_HLT_PFMETNoMu90_PFMHTNoMu90_IDTight_X" );

	//bools
	TTreeReaderValue<Long64_t> recoSelected (theReader,  "recoSelected" );
	TTreeReaderValue<Long64_t> GenMonoJetSelection (theReader,  "GenMonoJetSelection" );
	TTreeReaderValue<Long64_t> GenLeptonVetoSelection (theReader,  "GenLeptonVetoSelection" );
	TTreeReaderValue<Long64_t> GenBTagVetoSelection (theReader,  "GenBTagVetoSelection" );
	TTreeReaderValue<Long64_t> GenPhotonVetoSelection (theReader,  "GenPhotonVetoSelection" );
	TTreeReaderValue<Long64_t> GenmonoVselection (theReader,  "GenmonoVselection" );

	//weights
	TTreeReaderValue<Float_t> Weight_XS (theReader, "Weight_XS" );
	TTreeReaderValue<Float_t> Weight_GEN_nom (theReader, "Weight_GEN_nom" );
	TTreeReaderValue<Float_t> Weight_PU (theReader, "Weight_pu69p2" );
	TTreeReaderValue<Float_t> Weight_CSV (theReader, "Weight_CSV" );
	TTreeReaderValue<Float_t> Weight_MuRup(theReader, "Weight_scale_variation_muR_2p0_muF_1p0" );
	TTreeReaderValue<Float_t> Weight_MuRdown(theReader, "Weight_scale_variation_muR_0p5_muF_1p0" );
	TTreeReaderValue<Float_t> Weight_MuFup(theReader, "Weight_scale_variation_muR_1p0_muF_2p0" );
	TTreeReaderValue<Float_t> Weight_MuFdown(theReader, "Weight_scale_variation_muR_1p0_muF_0p5" );

	chain->LoadTree(-1);
	long iEntry = 0;
	long misscounter = 0;
	while (theReader.Next()) {
		if (iEntry % 50000 == 0) cout << "Processing Event Nr.: " << iEntry << endl;

		theReader.SetEntry(iEntry);
		// chain->GetEntry(iEntry);

		double weight_ = (*Weight_XS) * (*Weight_CSV) * (*Weight_PU) * (*Weight_GEN_nom) * 35.91823 * signalXS;
		// double weight_ = (*Weight_XS) * (*Weight_CSV) * (*Weight_PU) * (*Weight_GEN_nom) * 35.91823;
		bool triggered = *Triggered_HLT_PFMET170_X || *Triggered_HLT_PFMETNoMu100_PFMHTNoMu100_IDTight_X || *Triggered_HLT_PFMETNoMu110_PFMHTNoMu110_IDTight_X || *Triggered_HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_X || *Triggered_HLT_PFMETNoMu90_PFMHTNoMu90_IDTight_X;
		bool gensel = *Gen > bins[0]  && *GenMonoJetSelection && *GenLeptonVetoSelection && *GenBTagVetoSelection && *GenPhotonVetoSelection &&  *GenmonoVselection;

		// bool gensel = *GenMonoJetSelection && *GenLeptonVetoSelection && *GenBTagVetoSelection && *GenPhotonVetoSelection && *GenMETSelection && *GenmonoVselection;

		bool miss = gensel && (!*recoSelected || !triggered);
		if (gensel) {
			h_sig->Fill(*Gen, weight_);
			h_sig_MuRup->Fill(*Gen, weight_ * fabs(*Weight_MuRup));
			h_sig_MuRdown->Fill(*Gen, weight_ * fabs(*Weight_MuRdown));
			h_sig_MuFup->Fill(*Gen, weight_ * fabs(*Weight_MuFup));
			h_sig_MuFdown->Fill(*Gen, weight_ * fabs(*Weight_MuFdown));
		}
		iEntry++;
	}
	// h_sig->Draw();
	cout << misscounter << " misses in " << SignalName << endl;
	h_sig->SetName(SignalName);
	h_sig_MuRup->SetName(SignalName + "_Weight_scale_variation_muRUp");
	h_sig_MuRdown->SetName(SignalName + "_Weight_scale_variation_muRDown");
	h_sig_MuFup->SetName(SignalName + "_Weight_scale_variation_muFUp");
	h_sig_MuFdown->SetName(SignalName + "_Weight_scale_variation_muFDown");

	h_sig->Write();
	h_sig_MuRup->Write();
	h_sig_MuRdown->Write();
	h_sig_MuFup->Write();
	h_sig_MuFdown->Write();
	return h_sig;

}



// void processSignalSamples(const char* signalname = "Vector_MonoJ_NLO_Mphi-500_Mchi-200_gSM-0p25_gDM-1p0_13TeV-madgraph", const char* filename = "/nfs/dust/cms/user/swieland/Darkmatter/DM_Unfolding/rootfiles/signals.root") {
void processSignalSamples(const char* signalname = "Axial_MonoJ_NLO_Mphi-1000_Mchi-300_gSM-0p25_gDM-1p0_13TeV-madgraph", const char* filename = "/nfs/dust/cms/user/swieland/Darkmatter/DM_Unfolding/rootfiles/signals.root") {

	// TFile* outputfile = new TFile(filename, "UPDATE");
	TFile* outputfile = new TFile("rootfiles/signals/" + TString(signalname) + ".root", "UPDATE");
	// for (auto& name : Names_madgraph) processSignal(name);
	processSignal(signalname);
	// for (auto& name : Names) processSignal(name);

}
