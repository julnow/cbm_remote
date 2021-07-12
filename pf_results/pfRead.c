void pfRead(){

  TString fileName = "pftree.root";
  TFile* fileIn = TFile::Open(fileName, "read");
  TTree* treeIn = fileIn->Get<TTree>("aTree");

  auto* can_header = new AnalysisTree::Particles();
  auto* sim_header = new AnalysisTree::Particles();
  auto* can2sim_matching = new AnalysisTree::Matching();

  AnalysisTree::Configuration* config = fileIn->Get<AnalysisTree::Configuration>("Configuration");
  //Candidates
  const int canpx = config->GetBranchConfig("Candidates").GetFieldId("px");
  const int canpy = config->GetBranchConfig("Candidates").GetFieldId("py");
  const int canpz = config->GetBranchConfig("Candidates").GetFieldId("pz");
  const int canpt = config->GetBranchConfig("Candidates").GetFieldId("pT");
  const int canp = config->GetBranchConfig("Candidates").GetFieldId("p");
  //simulated
  const int simpx = config->GetBranchConfig("Simulated").GetFieldId("px");
  const int simpy = config->GetBranchConfig("Simulated").GetFieldId("py");
  const int simpz = config->GetBranchConfig("Simulated").GetFieldId("pz");
  const int simpt = config->GetBranchConfig("Simulated").GetFieldId("pT");
  const int simp = config->GetBranchConfig("Simulated").GetFieldId("p");

  treeIn->SetBranchAddress("Candidates", &can_header);
  treeIn->SetBranchAddress("Simulated", &sim_header);
  treeIn->SetBranchAddress(config->GetMatchName("Candidates", "Simulated").c_str(), &can2sim_matching);

  int N = 0;

  const int Nevents = treeIn->GetEntries();

  //Candidates

  TFile* fileOut1 = TFile::Open("pfread.root", "recreate");
  TH1F hcanpx("hcanpx", "candidate px;px [GeV/c];dN/dpx", 1000, -3, 3);
  TH1F hcanpy("hcanpy", "candidate py;py [GeV/c];dN/dpy", 1000, -3, 3);
  TH1F hcanpz("hcanpz", "candidate pz;pz [GeV/c];dN/dpz", 1000, 0, 14);
  TH1F hcanpt("hcanpt", "candidate pt;pt [GeV/c];dN/dpt", 1000, -1, 3);
  TH1F hcanp("hcanp", "candidate p;p [GeV/c];dN/dp", 1000, 0, 14);
  //difference
  TH1F hdifpx("hdifpx", "difference px;px [GeV/c];dN/dpx", 1000, -.4, .4);
  TH1F hdifpy("hdifpy", "difference py;py [GeV/c];dN/dpy", 1000, -.4, .4);
  TH1F hdifpz("hdifpz", "difference pz;pz [GeV/c];dN/dpz", 1000, -.8, .8);
  TH1F hdifpt("hdifpt", "difference pt;pt [GeV/c];dN/dpt", 1000, -.4, .4);
  TH1F hdifp("hdifp", "difference p;p [GeV/c];dN/dp", 1000, -.8, .8);
  //correlations
  TH2F hcorpx("hcorpx", "correlations px; candidate; simulated", 1000, -3, 3,  1000, -3, 3);
  TH2F hcorpy("hcorpy", "correlations py; candidate; simulated", 1000, -3, 3,  1000, -3, 3);
  TH2F hcorpz("hcorpz", "correlations pz; candidate; simulated", 1000, 0, 14,  1000, 0, 14);
  TH2F hcorpt("hcorpt", "correlations pt; candidate; simulated", 1000, -1, 3,  1000, -1, 3);
  TH2F hcorp("hcorp", "correlations p; candidate; simulated", 1000, 0, 14,  1000, 0, 14);

  for(int i=0; i<Nevents; i++){
    treeIn -> GetEntry(i);

    for (const auto& can_head : *can_header){
      //Candidates
      const float can_px = can_head.GetField<float>(canpx);
      const float can_py = can_head.GetField<float>(canpy);
      const float can_pz = can_head.GetField<float>(canpz);
      const float can_pt = can_head.GetField<float>(canpt);
      const float can_p = can_head.GetField<float>(canp);
      hcanpx.Fill(can_px);
      hcanpy.Fill(can_py);
      hcanpz.Fill(can_pz);
      hcanpt.Fill(can_pt);
      hcanp.Fill(can_p);
      //simulated
      const int sim_id = can2sim_matching->GetMatch(can_head.GetId());
      if(sim_id<0) continue;
      const float sim_px = sim_header->GetChannel(sim_id).GetField<float>(simpx);
      const float sim_py = sim_header->GetChannel(sim_id).GetField<float>(simpy);
      const float sim_pz = sim_header->GetChannel(sim_id).GetField<float>(simpz);
      const float sim_pt = sim_header->GetChannel(sim_id).GetField<float>(simpt);
      const float sim_p = sim_header->GetChannel(sim_id).GetField<float>(simp);
      //differences
      hdifpx.Fill(can_px - sim_px);
      hdifpy.Fill(can_py - sim_py);
      hdifpz.Fill(can_pz - sim_pz);
      hdifpt.Fill(can_pt - sim_pt);
      hdifp.Fill(can_p - sim_p);
      //correlations
      hcorpx.Fill(can_px, sim_px);
      hcorpy.Fill(can_py, sim_py);
      hcorpz.Fill(can_pz, sim_pz);
      hcorpt.Fill(can_pt, sim_pt);
      hcorp.Fill(can_p, sim_p);
    }
  }

  hcanpx.Write();
  hcanpy.Write();
  hcanpz.Write();
  hcanpt.Write();
  hcanp.Write();
  hdifpx.Write();
  hdifpy.Write();
  hdifpz.Write();
  hdifpt.Write();
  hdifp.Write();
  hcorpx.Write();
  hcorpy.Write();
  hcorpz.Write();
  hcorpt.Write();
  hcorp.Write();

  fileOut1->Close();
}
