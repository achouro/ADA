///GSM network frequency assignement equivalent to a 3-colouring problem 
//NP-complete solvable by converting it to SAT then using SAT-solvers
//Hexagonal cells need to be coloured by three different colours ensuring heterogeneity of all connected/adjascent cells.
 
#include <ios>
#include <iostream>
#include <vector>

using namespace std;

struct Edge { int from; int to; };

auto K=3; //number of different colors/frequencies

struct NetworkToSat {

    int num_vertices;
    vector<Edge> edges;

    NetworkToSat(int n, int m) :num_vertices(n),edges(m){}

//    static auto K=3; //number of different colors/frequencies

    void EquisatisfiableSatFormula() const{

	//C number of clauses, V number of variables

	const int C= edges.size()*K + num_vertices;
	const int V= num_vertices*K;

	cout<<C<<" "<<V<<endl;

	for(int j=0, count=1; j<num_vertices; ++j, count+=K){
  		cout<<count<<" "<<count+1<<" "<<count+2<<" 0"<<endl;}

	for(const Edge &e : edges){

        	cout <<-((e.from-1)*K +1)<<" "<<-((e.to-1)*K +1)<<" 0"<<endl;
            cout <<-((e.from-1)*K +2)<<" "<<-((e.to-1)*K +2)<<" 0"<<endl;
            cout <<-((e.from-1)*K +3)<<" "<<-((e.to-1)*K +3)<<" 0"<<endl;} }
};

int main() {
    ios::sync_with_stdio(false);

    int n, m;
    cin >> n >> m;
    NetworkToSat converter(n, m);
    for (int i = 0; i < m; ++i) {
        cin >> converter.edges[i].from >> converter.edges[i].to;
    }

    converter.EquisatisfiableSatFormula();

    return 0;
}