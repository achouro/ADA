//Hamiltonian path problem for rooms arrangements, NP-complete, reduced to SAT then CNF output is fed to SAT-solver

#include <iostream>
#include <vector>
#include <cstdio>
#include <math.h>

using namespace std;

//struct Edge {int from; int to; };

struct HamiltonToSat {
    int num_vertices;
    //vector<Edge> edges;
    int num_clauses;
    vector<vector<bool> > matrix;
    vector<vector<int> > data;
    string clauses_stream;

    HamiltonToSat(int n, int m) : num_clauses(0), num_vertices(n),
		   		  matrix(n, vector<bool>(n,false)),
				  data(n,vector<int>(n)) {

	for(int i=0, count=0; i<n; i++){
		for(int j=0;j<n; j++){
			data[i][j]= ++count;} } }

    void each_vertex_in_path(){
	for(int i=0; i<num_vertices; ++i, ++num_clauses){
	    for(int j=0; j<num_vertices;++j){
		clauses_stream+= to_string(data[i][j])+ " ";}
	    clauses_stream+="0\n";} 
    }

    void each_vertex_in_path_only_once(){
	
	for(int i=0; i<num_vertices;++i){
	    for(int j=0; j<num_vertices;++j){
		for(int k=i+1; k<num_vertices; ++k, ++num_clauses){
		    clauses_stream+= to_string(-data[i][j])+" "+
				     to_string(-data[k][j])+" "+ " 0\n";} } }	

    }

    void each_path_position_occupied(){
        
        for(int i=0; i<num_vertices; ++i, ++num_clauses){
            for(int j=0; j<num_vertices;++j){
                clauses_stream+= to_string(data[j][i])+ " ";}
            clauses_stream+="0\n";} 
	
    }

    void vertices_in_different_positions(){

        for(int i=0; i<num_vertices;++i){
            for(int j=0; j<num_vertices;++j){
                for(int k=i+1; k<num_vertices; ++k, ++num_clauses){
                    clauses_stream+= to_string(-data[i][j])+" "+
                                     to_string(-data[i][k])+" "+ " 0\n";} } }

    }

    void nonadjascent_vertices_nonadjascent_in_path(){

        for(int i=0; i<num_vertices;++i){
            for(int j=0; j<num_vertices;++j){
		if(!matrix[i][j] && j!=i){
                for(int k=0; k<num_vertices-1; ++k, ++num_clauses){
                    clauses_stream+= to_string(-data[i][k])+" "+
                                     to_string(-data[j][k+1])+ " 0\n";} } }    
    } }
   

    void EquisatisfiableSatFormula(const int max_clauses)  {
	
	clauses_stream.reserve(max_clauses*3);
    
	each_vertex_in_path();
    each_vertex_in_path_only_once(); 
	each_path_position_occupied();
	vertices_in_different_positions();
	nonadjascent_vertices_nonadjascent_in_path(); 

	cout<<num_clauses<<" "<<num_vertices*num_vertices<<endl<< clauses_stream.c_str(); 
    }
};

int main() {
    ios::sync_with_stdio(false);
    int n, m;
    
    cin >> n >> m;
    
    HamiltonToSat converter(n, m);

    for(int k=0;k<m;++k){
	    int i,j;
	    cin>>i>>j;
	    converter.matrix[i-1][j-1]=true;
        converter.matrix[j-1][i-1]=true; }
    

    converter.EquisatisfiableSatFormula(12000);

    return 0;
}

