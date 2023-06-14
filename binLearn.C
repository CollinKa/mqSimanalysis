//learn bin
//In TH2, the (0,0) is at the lower left corner.

#include <iostream>
void binLearn()
{
    // Create a histogram
   TH2F *hist = new TH2F("hist", "Example Histogram", 100, -10, 10, 100, -10, 10);

   // Fill the histogram with some values
   hist->Fill(1,1);
   hist->Fill(2,2);
   hist->Fill(3,3);
   hist->Fill(4,4);
   hist->Fill(5,5);

   // Get the bin number corresponding to a value

   //find the center position
   double Xvalue = 0;
   double Yvalue = 0;
   int binX = hist->GetXaxis()->FindBin(Xvalue);
   int binY = hist->GetYaxis()->FindBin(Yvalue);
   //double bincenter = hist->GetXaxis()->GetBinCenter(binx);
   int binxy = hist->GetBinContent(0,0);

   // Print the bin number
   cout << "Value " << Xvalue << " is in binX " << binX << endl;
   cout << "Value " << Yvalue << " is in binY " << binY << endl;
   cout << "binxy:" << binxy << endl;
   
   //create a rectangle center at (0,0)
   //the length is L=2a, and the width is W=2b.
   // 
   double a = 5;
   double b = 4;

   //upper bound
   int Lp = hist->GetXaxis()->FindBin(a);
   int Wp = hist->GetYaxis()->FindBin(b);
   //lower bound
   int Lpp = hist->GetXaxis()->FindBin(-a);
   int Wpp = hist->GetYaxis()->FindBin(-b);


   int Tbinxy = 0;
   for (int x = Lpp; x < Lp; x++)
   {
     for (int y= Wpp; y < Wp; y++)
     {
        binxy = hist->GetBinContent(x,y);
        Tbinxy = Tbinxy + binxy;
     }
   }

   cout << Tbinxy << endl;


   // Access the bin content
   //double binContent = hist->GetBinContent(bin);
   //std::cout << "Bin " << bin << " content: " << binContent << std::endl;

   // Cleanup
   delete hist;

   return 0;
}   