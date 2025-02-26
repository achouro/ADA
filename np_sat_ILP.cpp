//Integer Linear Programming Problem of Budget Allocation with constraints
//NP-complete problem reduced to CNF then fed to SAT-solver
#include <ios>
#include <iostream>
#include <vector>
#include <cmath>
#include <bitset>
#include <algorithm>
using namespace std;

struct ConvertILPToSat {
    vector< vector<int> > A;
    vector<int> b;

    ConvertILPToSat(int n, int m) : A(n, vector<int>(m)), b(n) {}

    bitset<3> combinations;
    string clauses_stream;

    void printEquisatisfiableSatFormula() {

    int count=0;
    for(int i=0; i<A.size();++i){
	const auto &row=A[i];
	int n= count_if(row.begin(),row.cend(), [](const auto a){ return a!=0;});

	for(int j=0, s=pow(2,n); j<s;++j){
	    combinations=j; 
	    int sum=0, comb=0;
	    for(const auto x: row){
		if(x!=0 && combinations[comb++]){ sum += x;} }

	    if(sum>b[i]){
		bool is_clause=false;
		for(int k=0, comb=0;k<row.size();++k){
		    if(row[k]!=0){
			clauses_stream+=combinations[comb]  
					? (to_string(-(k+1))+" ") : (to_string(k+1)+" ");
			++comb;
			is_clause=true; } }
		if(is_clause){
		    clauses_stream +="0 \n";
		    ++count; } } } }

    if(count==0){
	count++;
	clauses_stream+="1 -1 0\n";}

    cout<<count<<" "<<A[0].size()<<endl;
    cout<<clauses_stream.c_str();
    }
};

int main() {
    ios::sync_with_stdio(false);

    int n, m;
    cin >> n >> m;
    ConvertILPToSat converter(n, m);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        cin >> converter.A[i][j];
      }
    }
    for (int i = 0; i < n; i++) {
      cin >> converter.b[i];
    }

    converter.printEquisatisfiableSatFormula();

    return 0;
}