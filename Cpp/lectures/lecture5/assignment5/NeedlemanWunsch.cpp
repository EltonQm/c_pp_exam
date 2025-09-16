#include "SubstitutionMatrix.hpp"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// Global alignment using Needleman-Wunsch with substitution matrix
class NeedlemanWunsch {
private:
  const SubstitutionMatrix &matrix;
  int gapPenalty;

public:
  NeedlemanWunsch(const SubstitutionMatrix &matrix, int gapPenalty)
      : matrix(matrix), gapPenalty(gapPenalty) {}

  void align(const std::string &seq1, const std::string &seq2) {
    size_t m = seq1.size();
    size_t n = seq2.size();
    std::vector<std::vector<int>> score(m + 1, std::vector<int>(n + 1));
    std::vector<std::vector<char>> traceback(m + 1, std::vector<char>(n + 1));

    // Initialize first row and column
    for (size_t i = 0; i <= m; ++i) {
      score[i][0] = i * gapPenalty;
      traceback[i][0] = 'U'; // Up
    }
    for (size_t j = 0; j <= n; ++j) {
      score[0][j] = j * gapPenalty;
      traceback[0][j] = 'L'; // Left
    }
    traceback[0][0] = '0';

    // Fill the score matrix
    for (size_t i = 1; i <= m; ++i) {
      for (size_t j = 1; j <= n; ++j) {
        int match =
            score[i - 1][j - 1] + matrix.getScore(seq1[i - 1], seq2[j - 1]);
        int deleteGap = score[i - 1][j] + gapPenalty;
        int insertGap = score[i][j - 1] + gapPenalty;

        int maxScore = std::max({match, deleteGap, insertGap});
        score[i][j] = maxScore;

        if (maxScore == match)
          traceback[i][j] = 'D'; // Diagonal
        else if (maxScore == deleteGap)
          traceback[i][j] = 'U'; // Up
        else
          traceback[i][j] = 'L'; // Left
      }
    }

    // Traceback
    std::string aligned1, aligned2;
    size_t i = m, j = n;
    while (i > 0 || j > 0) {
      if (traceback[i][j] == 'D') {
        aligned1 += seq1[i - 1];
        aligned2 += seq2[j - 1];
        --i;
        --j;
      } else if (traceback[i][j] == 'U') {
        aligned1 += seq1[i - 1];
        aligned2 += '-';
        --i;
      } else { // 'L'
        aligned1 += '-';
        aligned2 += seq2[j - 1];
        --j;
      }
    }

    std::reverse(aligned1.begin(), aligned1.end());
    std::reverse(aligned2.begin(), aligned2.end());

    // Output result
    std::cout << "Alignment Score: " << score[m][n] << "\n";
    std::cout << aligned1 << "\n";
    std::cout << aligned2 << "\n";
  }
};
