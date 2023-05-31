#include <iostream>
#include <fstream>
#include <vector>
#include <TFile.h>
#include <TH1D.h>

int slabHist() {
  //std::ifstream inputFile("fourInalintEnergy.txt");
  std::ifstream inputFile("oneHitperLayerEnergy.txt");
  if (!inputFile.is_open()) {
    std::cout << "Failed to open the input file." << std::endl;
    return 1;
  }

  std::vector<double> numbers;
  double number;
  while (inputFile >> number) {
    numbers.push_back(number);
  }

  inputFile.close();

  // Create a ROOT file to store the histogram
  //TFile outputFile("histogram.root", "RECREATE");
  TFile outputFile("histogram1Hitperlayer.root", "RECREATE");
  // Create a 1D histogram
  TH1D* histogram = new TH1D("histogram", "Histogram", 100, 0, 15);

  // Fill the histogram with the numbers
  for (const auto& number : numbers) {
    histogram->Fill(number);
  }

  // Save the histogram to the ROOT file
  histogram->Write();

  // Clean up and close the ROOT file
  outputFile.Close();

  std::cout << "Histogram saved to histogram.root" << std::endl;

  return 0;
}
