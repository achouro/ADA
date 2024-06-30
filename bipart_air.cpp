#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

using std::vector;
using std::cin;
using std::cout;

class MaxMatching {
 public:
  void Solve() {
    vector<vector<bool> > adj_matrix = ReadData();
    vector<int> matching = FindMatching(adj_matrix);
    WriteResponse(matching);
  }

 private:
  vector<vector<bool> > ReadData() {
    int num_left, num_right;
    cin >> num_left >> num_right;
    vector<vector<bool> > adj_matrix(num_left, vector<bool>(num_right));
    for (int i = 0; i < num_left; ++i)
      for (int j = 0; j < num_right; ++j) {
        int bit;
        cin >> bit;
        adj_matrix[i][j] = (bit == 1);
      }
    return adj_matrix;
  }

  void WriteResponse(const vector<int>& matching) {
    for (int i = 0; i < matching.size(); ++i) {
      if (i > 0)
        cout << " ";
      if (matching[i] == -1)
        cout << "-1";
      else
        cout << (matching[i] + 1);
    }
    cout << "\n";
  }

  vector<int> FindMatching(const vector<vector<bool> >& adj_matrix) {
    // Replace this code with an algorithm that finds the maximum
    // matching correctly in all cases.


    int num_flights = adj_matrix.size();
    int num_crew = adj_matrix[0].size();

    vector<int> matching_flight(num_flights, -1);
    vector<bool> busy_crew(num_crew, false);

    for (int i = 0; i < num_flights; ++i)
      for (int j = 0; j < num_crew; ++j)
        if (matching_flight[i] == -1 && !busy_crew[j] && adj_matrix[i][j]) {
          matching_flight[i] = j;
          busy_crew[j] = true;
        }
    return matching_flight;
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  MaxMatching max_matching;
  max_matching.Solve();
  return 0;
}

