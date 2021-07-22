THStack *stack(const char *hName){
  //getting signal histogram
  TH1F *sign;
  signFile.GetObject(hName,sign);
  sign->SetDirectory(0);
  //getting background histogram
  TH1F *bckgr;
  signFile.GetObject(hName,bckgr);
  sign->SetDirectory(0);
}


//compares background and signal for topological parameters of kaon reconstruction
void top_selection(char *signFileName, const char *bckgrFilename){

  TFile signFile(signFileName);
  TFile bckgrFile(bckgrFilename);
  //Candidates
  TH1F *hcanpx;
  //amybe a void function could do it?
  pfReadFile.GetObject("hcanpx",hcanpx);
  hcanpx->SetDirectory(0);

  pfReadFile.Close( );
}
