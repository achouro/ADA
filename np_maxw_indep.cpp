//Maximum Independent set in tree computation for maximum utility assignment of subsets
//NP-complete problem with a special case solution 
//implemented recusively using Dynamic Programming
//Gathering design so that no one is put toether with a parent/boss/superior to maximise a social metric.

#include <iostream>
#include <vector>

struct Vertex {
    int weight;
    std::vector <int> children;
};
typedef std::vector<Vertex> Graph;
typedef std::vector<int> Sum;

Graph ReadTree() {
    int vertices_count;
    std::cin >> vertices_count;

    Graph tree(vertices_count);

    for (int i = 0; i < vertices_count; ++i)
        std::cin >> tree[i].weight;

    for (int i = 1; i < vertices_count; ++i) {
        int from, to, weight;
        std::cin >> from >> to;
        tree[from - 1].children.push_back(to - 1);
        tree[to - 1].children.push_back(from - 1);
    }

    return tree;
}

int  recursive_dp(const Graph &tree,const int vertex,Sum &s, int parent) {
	
    if(s[vertex]==-1){

	if(tree[vertex].children.empty()){
	    s[vertex]=tree[vertex].weight;}

	else{
//nodes can be put together with sister nodes of children of children but no node with its parent node/child node

	    auto m1=tree[vertex].weight;
	    for(auto u: tree[vertex].children){
		if(u==parent)
		    continue;
	    	for(auto w: tree[u].children){
		    if(w!=vertex){
		       m1 += recursive_dp(tree, w, s, u); } } }

	    auto m0=0;
	    for(auto u:tree[vertex].children){
		if(u!=parent){
		    m0+=recursive_dp(tree,u,s,vertex);}}

	    s[vertex]=std::max(m0,m1); } }

    return s[vertex];
}

int MaxWeightIndependentTreeSubset(const Graph &tree) {
    size_t size = tree.size();
    if (size == 0)
        return 0;

    auto s=Sum(size,-1); //vector<int>(size,-1) 
    return recursive_dp(tree, 0,s, -1);
}

int main() {

    Graph tree = ReadTree();
    int weight = MaxWeightIndependentTreeSubset(tree);
    std::cout << weight << std::endl;
    return 0;
}

