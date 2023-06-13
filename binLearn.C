//learn bin
#include <iostream>
void binLearn()
{
    // Create a histogram
   TH2F *hist = new TH2F("hist", "Example Histogram", 100, 0, 100, 100, 0, 100);

   // Fill the histogram with some values
   hist->Fill(10,10);
   hist->Fill(20,20);
   hist->Fill(30,30);
   hist->Fill(40,40);
   hist->Fill(50,50);

   // Get the bin number corresponding to a value
   double value = 25;
   int binX = hist->GetXaxis()->FindBin(value);
   int binY = hist->GetYaxis()->FindBin(51);
   //double bincenter = hist->GetXaxis()->GetBinCenter(binx);
   int binxy = hist->GetBinContent(31,31);

   // Print the bin number
   cout << "Value " << value << " is in binX " << binX << endl;
   cout << "Value " << value << " is in binY " << binY << endl;
   cout << "binxy:" << binxy << endl;


   // Access the bin content
   //double binContent = hist->GetBinContent(bin);
   //std::cout << "Bin " << bin << " content: " << binContent << std::endl;

   // Cleanup
   delete hist;

   return 0;
}   