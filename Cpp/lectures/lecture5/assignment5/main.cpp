#include "NeedlemanWunsch.cpp"
#include "SubstitutionMatrix.hpp"

int main() {
  SubstitutionMatrix blosum;
  blosum.loadFromFile("BLOSUM62.txt");

  std::string seq1 = "PLEASANTLY";
  std::string seq2 = "MEANLY";

  NeedlemanWunsch nw(blosum, -5); // Typical gap penalty for BLOSUM62 is -5
  nw.align(seq1, seq2);

  return 0;
}
