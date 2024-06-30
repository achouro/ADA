//Ford-Fulkerson algorithm for Max-Flow to evacuate a city

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

public:

    explicit FlowGraph(size_t n): graph(n) {}

    void add_edge(int from, int to, int capacity) {
        Edge forward_edge = {from, to, capacity, 0};
        Edge backward_edge = {to, from, 0, 0};
        graph[from].push_back(edges.size());
        edges.push_back(forward_edge);
        graph[to].push_back(edges.size());
        edges.push_back(backward_edge);   }

    size_t size() const { return graph.size(); }

    const vector<size_t>& get_ids(int from) const { return graph[from];}

    const Edge& get_edge(size_t id) const { return edges[id]; }

    void add_flow(size_t id, int flow) {
        edges[id].flow += flow;
        edges[id ^ 1].flow -= flow; }
};

FlowGraph read_data() {
    int vertex_count, edge_count;
    std::cin >> vertex_count >> edge_count;
    FlowGraph graph(vertex_count);
    for (int i = 0; i < edge_count; ++i) {
        int u, v, capacity;
        std::cin >> u >> v >> capacity;
        graph.add_edge(u - 1, v - 1, capacity);
    }
    return graph;
}

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

//u=pred[graph.get_edge(pred[to]).from]
//pred[to]=pred[graph.edges[pred[to]].from]

//compute residual graph, find path in residualg, 
	for(int u= pred[to]; u!=-1; u=pred[graph.get_edge(u).from]){
//sweep through for min capacity in residual
	    min_flow= min(min_flow, 
			  graph.get_edge(u).capacity- graph.get_edge(u).flow);} 
//set flow to min capacity-flow/in res, add flow
        for(int u= pred[to]; u!=-1; u=pred[graph.get_edge(u).from]){
	    graph.add_flow(u, min_flow);} 
//increment total flow
        flow+=min_flow;}
      } while(pred[to]!=-1);
    return flow;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    FlowGraph graph = read_data();

    std::cout << max_flow(graph, 0, graph.size() - 1) << "\n";
    return 0;
}
