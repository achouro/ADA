//Edmons-Karp max-flow algorithm on Bipartite graph for airline crew assignment

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
using namespace std;

class FlowGraph {

public:
    struct Edge { int from, to, capacity, flow; };

private:

    vector<Edge> edges;

    vector<vector<size_t> > graph;
 
    const size_t flights;
public:

    explicit FlowGraph(size_t n, size_t m, size_t f): graph(n), flights(f){
			edges.reserve(m*2);}

    void add_edge(int from, int to, int capacity) {
        Edge forward_edge = {from, to, capacity, 0};
        Edge backward_edge = {to, from, 0, 0};
        graph[from].push_back(edges.size());
        edges.push_back(forward_edge);
        graph[to].push_back(edges.size());
        edges.push_back(backward_edge);   }

    size_t size() const { return graph.size(); }

    size_t get_flights() const { return flights; }

    const vector<size_t>& get_ids(int from) const { return graph[from];}

    const Edge& get_edge(size_t id) const { return edges[id]; }

//to get forward id+1, to get backward, id^1 works for both
 
    void add_flow(size_t id, int flow) {
        edges[id].flow += flow;
        edges[id ^ 1].flow -= flow; }
};

FlowGraph read_data() {
//n vertex count flights count,
//m edge count crew members
    int n,m;
    std::cin >> n >> m;

//Bipartite graph with 1 source, n flights,m crew and 1 sink

    FlowGraph graph(n+m+2 ,n+m+2, n );

//stored as graph with n+m+2 nodes and n flights

//source to left edges
    for(int i=0; i<n; ++i) { graph.add_edge(0, i+1, 1);}

//left to right edges
    for(int i=1; i<=n;i++){
	for(int j=0; j<m; j++){
		int bit;
		std::cin>>bit;
		if(bit==1){
		    graph.add_edge(i, n+ j+1, 1);} } }

//right to sink
    for(int i=n+1; i<=n+m; i++){ graph.add_edge(i, n+m+1,1);}

    return graph;} 

void breadth_fs( FlowGraph & graph, int s, int t, vector<int>& pred){
//    pred.resize(graph.size());
//    for(int i=0; i<graph.size(); i++)
	
//    pred[s]=0;

    queue<int> Q;
    Q.push(s);

    fill(pred.begin(), pred.end(),-1);

    while(!Q.empty()){
	int u=Q.front();
	Q.pop();
//iterate through adj list of u
	for(auto id: graph.get_ids(u)){ //edges defined below
	    const FlowGraph::Edge &edgar= graph.get_edge(id);
		//unexplored, valid flow 
	    if(pred[edgar.to]==-1 && edgar.capacity>edgar.flow && edgar.to!=s){
		pred[edgar.to]= id; // edge to set to node id 
                Q.push(edgar.to); } } } } //node added to queue to be searched

//Ford-Fulkerson

int max_flow(FlowGraph& graph, int from, int to) {
    int flow = 0;
    vector<int> pred(graph.size());

    do{
	breadth_fs(graph, from, to, pred);
	if(pred[to]!=-1){

	int min_flow=numeric_limits<int>::max();

//pred[to]=pred[graph.edges[pred[to]].from] backward sweep

//compute residual graph, find path in residualg, 
//sweep through for min capacity in residual
//set flow to min capacity-flow/in res, add flow
//increment total flow

	for(int u= pred[to]; u!=-1; u=pred[graph.get_edge(u).from]){
	    min_flow= min(min_flow, 
			  graph.get_edge(u).capacity- graph.get_edge(u).flow);} 


        for(int u= pred[to]; u!=-1; u=pred[graph.get_edge(u).from]){
	    graph.add_flow(u, min_flow);} 

//increment total flow

        flow+=min_flow;}
      } while(pred[to]!=-1);
    return flow;
}

void job_assignment(FlowGraph &graph, const size_t flights){

    for(int i=0; i<flights; ++i){ //i<n    
	int job=-1;

	for(auto id:graph.get_ids(i+1)){ //iterate over left side

	    const FlowGraph::Edge &edgar = graph.get_edge(id);

	    if(edgar.flow==1){
		job= edgar.to- flights; 
		break;} }

	    cout<<job<<' '; }

	cout<<endl;}


int main() {
    std::ios_base::sync_with_stdio(false);
    FlowGraph graph = read_data();

    max_flow(graph, 0, graph.size() - 1);

    job_assignment(graph, graph.get_flights());

    return 0;
}

