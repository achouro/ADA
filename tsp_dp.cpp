//School bus trajectory to reach every student once
//Travelling Salesman Problem 
//NP-completed solved by exact algorithm using Dynamic Programming, Backtracking
//Help-Karp Algorithm
//Minimum possible time/distance to traverse all vertices exactly once and return to the initial position

#include <iostream>
#include <algorithm>
#include <vector>
#include <bitset>

using namespace std;

const int INF = 1e9;

struct node{ int dist=INF; int parent=0;};

using Matrix= vector<vector<int> >;
using Subset= vector<int> ;
using Cost= vector<vector<node> > ;
using Tour= pair<int, vector<int> > ;


Matrix read_data() {
    int vertex_count, edge_count;
    std::cin >> vertex_count >> edge_count;
    Matrix graph(vertex_count, vector<int>(vertex_count, INF));
    for (int i = 0; i < edge_count; ++i) {
        int from, to, weight;
        std::cin >> from >> to >> weight;
        --from, --to;
        graph[from][to] = graph[to][from] = weight;
    }
    return graph;
}


struct TSP{

    const int N;
    Cost C;
    Matrix dist;

    TSP(Matrix complete_graph): N{(int) complete_graph.size()}, 
				C{Cost(1<<(N),vector<node>(N))},
				dist{ move(complete_graph)} {
				    
	    for(int i=1; i<N;++i){
	        C[1<<i][i]=node{dist[0][i],0};} }

//1 binary shifted by i bits to the left i.e +2^i -1, 100000(0 i times)

    Subset combinations(const int n){

    Subset combs;
    std::bitset<16> bset=n;

    for(auto k=0u; k<bset.size(); ++k){
        if(bset[k]){
            combs.emplace_back(k+1);}}
    return combs;
    }

    Tour optimal_tour(){

    for(int s=2; s< (1<< (N-1));++s){
	if(s & (s-1)){
	    Subset S= combinations(s);
	    int bits =s*2;

	    for(auto k:S){
//xor inverter returns position from binary encryption
		int prev= bits^(1<<k);	
		node min_node;

		for(auto m:S){
		    if(min_node.dist > C[prev][m].dist +dist[m][k] && m!=k){
			min_node= node{C[prev][m].dist +dist[m][k], m};} } 
		C[bits][k]=min_node; } } }

    return backtrack_optimal();
    }


    Tour backtrack_optimal(){

    node min_node;
    int bits= (1<<N) -2;  //(2^(n) -1)  -2
    
    for(int k=1; k<N;++k){
	if(min_node.dist> C[bits][k].dist +dist[k][0]){
	    min_node= node{C[bits][k].dist +dist[k][0], k}; } }

    if(min_node.dist==INF){ return {-1, {} }; }

    //pair<int, vector<int> > optimal_tour

    Tour optimal_tour;

    optimal_tour.second.reserve(N);
    optimal_tour.first=min_node.dist;
    optimal_tour.second.emplace_back(1);

// start filling optimal_tour with accumulated distance and vertices along path

    for(int i=0; i<N-1; ++i){
        optimal_tour.second.emplace_back(min_node.parent +1); //next node
	min_node.parent= C[bits][min_node.parent].parent; //backtrack to node parent
        bits=bits ^ (1<<optimal_tour.second.back() -1); } //xor invert bits

    return optimal_tour; 
    }


};

void print_answer(const std::pair<int, vector<int> >& answer) {
    std::cout << answer.first << "\n";
    if (answer.first == -1)
        return;
    const vector<int>& path = answer.second;
    for (size_t i = 0; i < path.size(); ++i)
        std::cout << path[i] << " ";
    std::cout << "\n";
}

int main() {
    TSP tsp(read_data());
    print_answer(tsp.optimal_tour());
    return 0;
}
