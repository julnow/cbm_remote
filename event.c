void event(){

  TString fileName = "1.analysistree.root";
  TFile* fileIn = TFile::Open(fileName, "read");
  TTree* treeIn = fileIn->Get<TTree>("rTree");

  auto* eve_header = new AnalysisTree::EventHeader();
  auto* rec_header = new AnalysisTree::EventHeader();
  auto* sim_tracks = new AnalysisTree::Particles();
  auto* vtx_tracks = new AnalysisTree::TrackDetector();
  auto* sim_vtx_matching = new AnalysisTree::Matching();

  AnalysisTree::Configuration* config = fileIn->Get<AnalysisTree::Configuration>("Configuration");
  const int sx = config->GetBranchConfig("SimEventHeader").GetFieldId("vtx_x"); //simulated
  const int sy = config->GetBranchConfig("SimEventHeader").GetFieldId("vtx_y");
  const int sz = config->GetBranchConfig("SimEventHeader").GetFieldId("vtx_z");
  const int spsi = config->GetBranchConfig("SimEventHeader").GetFieldId("psi_RP");
  const int rx = config->GetBranchConfig("RecEventHeader").GetFieldId("vtx_x"); //reconstructed
  const int ry = config->GetBranchConfig("RecEventHeader").GetFieldId("vtx_y");
  const int rz = config->GetBranchConfig("RecEventHeader").GetFieldId("vtx_z");
  const int rchi2 = config->GetBranchConfig("RecEventHeader").GetFieldId("vtx_chi2");
  const int rEpsd = config->GetBranchConfig("RecEventHeader").GetFieldId("Epsd");
  const int sb = config->GetBranchConfig("SimEventHeader").GetFieldId("b");
  const int rm = config->GetBranchConfig("RecEventHeader").GetFieldId("M");

  treeIn->SetBranchAddress("SimEventHeader", &eve_header);
  treeIn->SetBranchAddress("SimParticles", &sim_tracks);
  treeIn->SetBranchAddress("VtxTracks", &vtx_tracks);
  treeIn->SetBranchAddress("RecEventHeader", &rec_header);
  treeIn->SetBranchAddress(config->GetMatchName("VtxTracks", "SimParticles").c_str(), &sim_vtx_matching);

  int N = 0;

  const int Nevents = treeIn->GetEntries();

  //zadania
  TFile* fileOut1 = TFile::Open("event.root", "recreate");
  TH1F hsx("hsx", "sim x", 1000, -3, 3); //simulated
  TH1F hsy("hsy", "sim y", 1000, -3, 3);
  TH1F hsz("hsz", "sim z", 1000, -3, 3);
  TH1F hspsi("hspsi", "sim \psi", 100, 0, 8);
  TH1F hrx("hrx", "rec x", 1000, -3, 3); //reconstructed
  TH1F hry("hry", "rec y", 1000, -3, 3);
  TH1F hrz("hrz", "rec z", 1000, -3, 3);
  TH1F hrchi2("hrchi2", "rec chi2", 100, 0, 3);
  TH1F hEpsd("hEpsd", "rec Epsd", 100, -1, 26);
  TH1F hm("hm", "rec Multiplicity", 100, 0, 16);
  TH1F hb("hb", "rec impact par", 100, 0, 16);
  TH2F hcx("hcx", "corr x", 1000, -.5, .5, 1000, -.5, .5); //correlation
  TH2F hcy("hcy", "corr y", 1000, -.5, .5, 1000, -.5, .5);
  TH2F hcz("hcz", "corr z", 1000, -.5, .5, 1000, -.5, .5);
  TH2F hcbm("hcbm", "corr (b, multiplicty)", 100, 0, 16, 100, 0, 16);
  TH1F hdx("hdx", "difference x", 1000, -.2, .2); //simulated
  TH1F hdy("hdy", "diiference y", 1000, -.2, .2);
  TH1F hdz("hdz", "difference z", 10000, -3, 3);


  for(int i=0; i<Nevents; i++){
    treeIn -> GetEntry(i);
       const float sim_x = eve_header->GetField<float>(sx); //simulated
       hsx.Fill(sim_x);
       const float sim_y = eve_header->GetField<float>(sy);
       hsy.Fill(sim_y);
       const float sim_z = eve_header->GetField<float>(sz);
       hsz.Fill(sim_z);
       const float sim_psi = eve_header->GetField<float>(spsi);
       hspsi.Fill(sim_psi);
       const float rec_x = rec_header->GetField<float>(rx); //reconstructed
       const float rec_y = rec_header->GetField<float>(ry);
       const float rec_z = rec_header->GetField<float>(rz);
       hrx.Fill(rec_x);
       hry.Fill(rec_y);
       hrz.Fill(rec_z);
       hrchi2.Fill(rec_header->GetField<float>(rchi2));
       const float rec_epsd = rec_header->GetField<float>(rEpsd);
       hEpsd.Fill(rec_epsd);
       const float rec_m = rec_header->GetField<float>(rm);
       hm.Fill(rec_m);
       const float sim_b = eve_header->GetField<float>(sb);
       hb.Fill(sim_b);
       hcbm.Fill(sim_b, rec_m);
       hcx.Fill(sim_x, rec_x); //correlations
       hcy.Fill(sim_y, rec_y);
       hcz.Fill(sim_z, rec_z);
       hdx.Fill(sim_x - rec_x); //difference
       hdy.Fill(sim_y - rec_y);
       hdz.Fill(sim_z - rec_z);
  }

   hsx.Write();
   hsy.Write();
   hsz.Write();
   hspsi.Write();
   hrx.Write();
   hry.Write();
   hrz.Write();
   hrchi2.Write();
   hEpsd.Write();
   hm.Write();
   hb.Write();
   hcx.Write();
   hcy.Write();
   hcz.Write();
   hcbm.Write();
   hdx.Write();
   hdy.Write();
   hdz.Write();

  fileOut1->Close();
}
