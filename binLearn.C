//learn bin

void binLearn()
{
    // Create a histogram
   TH1F *hist = new TH1F("hist", "Example Histogram", 100, 0, 100);

   // Fill the histogram with some values
   hist->Fill(10);
   hist->Fill(20);
   hist->Fill(30);
   hist->Fill(40);
   hist->Fill(50);

   // Get the bin number corresponding to a value
   double value = 25;
   int bin = hist->GetXaxis()->FindBin(value);

   // Print the bin number
   std::cout << "Value " << value << " is in bin " << bin << std::endl;

   // Access the bin content
   double binContent = hist->GetBinContent(bin);
   std::cout << "Bin " << bin << " content: " << binContent << std::endl;

   // Cleanup
   delete hist;

   return 0;
}   