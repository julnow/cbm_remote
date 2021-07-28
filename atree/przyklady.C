void przyklady()
{
  TString fileName = "1.analysistree.root";
  TFile* fileIn = TFile::Open(fileName, "read");
  TTree* treeIn = fileIn->Get<TTree>("rTree");

  auto* eve_header = new AnalysisTree::EventHeader();
  auto* sim_tracks = new AnalysisTree::Particles();
  auto* vtx_tracks = new AnalysisTree::TrackDetector();
  auto* sim_vtx_matching = new AnalysisTree::Matching();

  AnalysisTree::Configuration* config = fileIn->Get<AnalysisTree::Configuration>("Configuration");
  const int x_id = config->GetBranchConfig("SimParticles").GetFieldId("x");
  const int y_id = config->GetBranchConfig("SimParticles").GetFieldId("y");
  const int z_id = config->GetBranchConfig("SimParticles").GetFieldId("z");
  const int b_id = config->GetBranchConfig("SimEventHeader").GetFieldId("b");

  treeIn->SetBranchAddress("SimEventHeader", &eve_header);
  treeIn->SetBranchAddress("SimParticles", &sim_tracks);
  treeIn->SetBranchAddress("VtxTracks", &vtx_tracks);
  treeIn->SetBranchAddress(config->GetMatchName("VtxTracks", "SimParticles").c_str(), &sim_vtx_matching);

  int N = 0;

  const int Nevents = treeIn->GetEntries();

   //******* write distribution in histogram and write down in output file
  // TFile* fileOut = TFile::Open("fileOut.root", "recreate");
  // TH1F histo("histo", "", 1000, -3, 3);
  // for(int i=0; i<Nevents; i++)
  // {
  //   treeIn -> GetEntry(i);

    // for(const auto& vtx_track : *(vtx_tracks->GetChannels()) )
    // {
    //   histo.Fill(vtx_track.GetPx());
    // }
  // }

  // histo.Write();
  // fileOut->Close();
//   //***********************************************************************

// //********* calculate N of lambdas in each event and look at correlation with impact parameter
//   for(int i=0; i<Nevents; i++)
//   {
//     treeIn -> GetEntry(i);
//
//     int N_lambdas = 0;
//
//     for(const auto& sim_track : *(sim_tracks->GetChannels()) )
//     {
//       if(sim_track.GetPid() == 3122)
//         N_lambdas++;
//     }
//
//     std::cout << i << "\tN_lambdas = " << N_lambdas <<  "\tb = " << eve_header->GetField<float>(b_id) << "\n";
//
//   }
///******************************************************************************

//   //********* Matching between reconstructed (vtx) and mc-true (sim) tracks
//   TFile* fileOut = TFile::Open("fileOut.root", "recreate");
//   TH2F histo("histo", "", 1000, -3, 3, 1000, -3, 3);
//   for(int i=0; i<Nevents; i++)
//   {
//     treeIn -> GetEntry(i);
//
//     for(const auto& vtx_track : *(vtx_tracks->GetChannels()) )
//     {
//       const float px_vtx = vtx_track.GetPx();
//       const int sim_id = sim_vtx_matching->GetMatch(vtx_track.GetId());
//       if(sim_id<0) continue;
//       const float px_sim = sim_tracks->GetChannel(sim_id).GetPx();
//
//       histo.Fill(px_sim, px_vtx);
//     }
//   }
//
//   histo.Write();
//   fileOut->Close();
//   //******************************************************************************
// ******* write distribution in histogram and write down in output file
///////////////////////     distribution of b
TFile* fileOut1 = TFile::Open("distB.root", "recreate");
TH1F histo1("histo1", "distribution b", 100, 0, 16);
for(int i=0; i<Nevents; i++)
{
  treeIn -> GetEntry(i);

    histo1.Fill(eve_header->GetField<float>(b_id));
}

histo1.Write();
fileOut1->Close();

//////////////////////  
//difference between simulated and reconstructed pz
   TFile* fileOut2 = TFile::Open("sim-rec_pz.root", "recreate");
   TH1F histo2("histo2", "sim-rec pz", 1000, -3, 3);
   for (int i=0; i<Nevents; i++)
   {
     treeIn -> GetEntry(i);

     for(const auto& vtx_track : *(vtx_tracks->GetChannels()) )
     {
       const float px_vtx = vtx_track.GetPx();
       const int sim_id = sim_vtx_matching->GetMatch(vtx_track.GetId());
       if(sim_id<0) continue;
       const float px_sim = sim_tracks->GetChannel(sim_id).GetPx();

       histo2.Fill(px_sim - px_vtx);
     }
   }
   histo2.Write();
   fileOut2->Close();  
}
