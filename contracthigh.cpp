/*Bidirectional Djikstra with Contraction Hierarchies Preprocessing for large Road Networks*/

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <functional>
#include <cmath>
using namespace std;

using point =pair<long long, long long> ;
using length=long long ;
using djistance=vector<vector<length> >;

using edge=pair<long long, int> ;
using adj_list=vector<vector< vector< edge> > >;
using que=priority_queue<edge, vector<edge>, greater<edge> >;

using importance_node= pair<int,int>;
using importance_queue= priority_queue<importance_node, vector<importance_node>,					greater<importance_node> >;

length INF= numeric_limits<length>::max()/2;

class graph
{
public:
adj_list adj;
djistance dist;
vector<vector<bool> > visited;
que queue_f, queue_r;
vector<int> min_outgoing,max_outgoing, max_incoming, workset;

int t_=0, s_=0;



graph(int n) : adj(adj_list(2,vector<vector<edge> >(n,vector<edge>() ))),
	       dist(2, vector<length>(n,INF)),
	       visited(2,vector<bool>(n,false)),
	       min_outgoing(n,INF),
	       max_outgoing(n,0),
	       max_incoming(n,0) {}

void add_edge(int u, int v, int w){
  adj[0][u].push_back(edge(w,v));
  adj[1][v].push_back(edge(w,u));
  
  if(max_outgoing[u]<w)
    max_outgoing[u]=w;
  if(max_incoming[u]<w)
    max_incoming[u]=w;
  if(min_outgoing[u]>w)
    min_outgoing[u]=w;  }

void reset_containers(){
  for(auto u:workset){
    dist[0][u]=dist[1][u]=INF;
    visited[0][u]=visited[1][u]=0;}
  workset.clear();}

void relax(que &queue, length d, int u, int v, int side){
  if(dist[side][u]> d + dist[side][v]){
    dist[side][u]= d + dist[side][v];
    queue.push(edge(dist[side][u],u));}}

void process(int u, que &queue, int side){
  queue.pop();
  for(auto &edger: adj[side][u]){
    relax(queue, edger.first ,edger.second, u, side);}}
/*
length compute_path(){
  length result= INF;
  for( auto u: workset)
    if(dist[0][u]+dist[1][u]<result)
      result= dist[0][u]+ dist[1][u];
  return result==INF ? -1: result;} //return no path -1 or path length}
*/

length bidirectional_contraction(int s, int t){
  reset_containers();
  s_=s, t_=t;
  length estimate =INF;
  queue_f.push(edge(0,s));
  queue_r.push(edge(0,t));
  dist[0][s]=dist[1][t]=0;

  workset.push_back(s);
  workset.push_back(t);
  bool tobe_processed=true;

  while(!queue_f.empty() || !queue_r.empty()){
    if(!queue_f.empty()){
    int u= queue_f.top().second;
    queue_f.pop();
    tobe_processed=true;

//draw edges from rear adjacency list of u , within them       
/*if dist from front u is greater than combined distance of the drawn node from the front and its cumulative distance make the latter the distance from the front for u plus one*/

    for(auto pred:adj[1][u]){    
      if(dist[0][pred.second]+ pred.first <dist[0][u]){   
        dist[0][u]=dist[0][pred.second] +pred.first +1;
        tobe_processed=false;
        break;}}

    if(tobe_processed &&dist[0][u]<=estimate)
      process(u,queue_f, 0);

    visited[0][u]=true;
    
    if(visited[1][u] &&dist[0][u]+dist[1][u]<estimate)
      estimate=dist[0][u]+dist[1][u];}

    if(!queue_r.empty()){
    int u= queue_r.top().second;
    queue_r.pop();
    tobe_processed=true;

    for(auto pred:adj[0][u]){
      if(dist[1][pred.second]+ pred.first <dist[1][u]){
        dist[1][u]=dist[1][pred.second] +pred.first +1;
        tobe_processed=false;
        break;}}

    if(tobe_processed &&dist[1][u]<=estimate)
      process(u,queue_r, 1);

    visited[1][u]=true;

    if(visited[0][u] &&dist[0][u]+dist[1][u]<estimate)
      estimate=dist[0][u]+dist[1][u];}

  return estimate==INF ? -1: estimate;}}

};  

struct contraction_hierarchies
{
struct shortcut{int from; int to; length dist; 
		shortcut(int f, int t, length d):from(f), to(t), dist(d){} };

bool adding_shortcuts=false;
int contract_node_importance=0;
int n;

vector<int> rank, node_level, settled_vertices;
vector<shortcut> shortcuts;

graph &g;
importance_queue imp_que;
vector<length> dist;

contraction_hierarchies(graph &graphe) : 
  g(graphe), n(graphe.adj[0].size()), dist(vector<length>(n,INF)),
  rank(n,INF), node_level(n,0) {}

void update_neighbours_level(int v){
  auto &outgoing_edges=g.adj[0][v];
  auto &incoming_edges=g.adj[1][v];
  //int current_level= node_level +1;
  
  for( auto out_edge: outgoing_edges){
    if(node_level[out_edge.second]<node_level[v]+1)
      node_level[out_edge.second]=node_level[v]+1;}

  for( auto in_edge: incoming_edges){
    if(node_level[in_edge.second]<node_level[v]+1)
      node_level[in_edge.second]=node_level[v]+1;} }

int contracted_and_level(int v){
  int num=0, level=0;
  auto &outgoing_edges=g.adj[0][v];
  auto &incoming_edges=g.adj[1][v];

  for(auto out_edge: outgoing_edges){
    if(rank[out_edge.second]!=INF){
      ++num;
      if(node_level[out_edge.second]>level){
        level=node_level[out_edge.second]; }}}

  for(auto in_edge: incoming_edges){
    if(rank[in_edge.second]!=INF){
      ++num;
      if(node_level[in_edge.second]>level){
        level=node_level[in_edge.second]; }}}

  return num + level +1;} // number of neighbouring edges contracted/to contract plus node level 

void witness_search(int s, int v, int lim){
  que queue;
  queue.push(edge(0,s));
  settled_vertices.push_back(s);

  dist[s]=0;
  int hops=5;

  while(hops-- && !queue.empty()){
    int u=queue.top().second;
    queue.pop();

    if(lim<dist[u]) break;

    auto &outgoing_edges=g.adj[0][u];
    for(auto out_edge: outgoing_edges){

      if(rank[out_edge.second]<rank[v] || out_edge.second==v) continue;

      if(dist[out_edge.second]> dist[u]+ out_edge.first){
        dist[out_edge.second]=dist[u]+out_edge.first;
	queue.push(edge(dist[out_edge.second], out_edge.second)); 
	settled_vertices.push_back(out_edge.second); } } } }

void contract_node(int v){
shortcuts.clear();
int added_shortcuts=0;
int shortcuts_cover=0;

auto &outgoing_edges=g.adj[0][v];
auto &incoming_edges=g.adj[1][v];

for(auto in_edge : incoming_edges){
  if (rank[in_edge.second]<rank[v] || outgoing_edges.empty()) continue;
  
  witness_search(in_edge.second, v, g.max_incoming[v]+g.max_outgoing[v]);

  bool oneplus_shortcut=false;
  
  for(auto out_edge: outgoing_edges){ 
    if(rank[out_edge.second]<rank[v]) continue;
    bool shortcut_needed=true;
    
    for(auto in_out_edge: g.adj[1][out_edge.second]){
      if(rank[in_out_edge.second]<rank[v] || in_out_edge.second==v) continue;
      
      if(in_edge.first+ out_edge.first >= dist[in_out_edge.second]+ in_out_edge.first){
        shortcut_needed=false;
	break; } }

    if(shortcut_needed){
      ++added_shortcuts;
      oneplus_shortcut=true;
      if(adding_shortcuts){
	shortcuts.__emplace_back(shortcut(in_edge.second,out_edge.second, in_edge.first +out_edge.first)); }}

    if(oneplus_shortcut) ++shortcuts_cover;

    for(auto settled: settled_vertices) dist[settled]=INF; 
    settled_vertices.clear();}

    contract_node_importance= (added_shortcuts - outgoing_edges.size()- incoming_edges.size() + shortcuts_cover + contracted_and_level(v)); }} 

void remove_edges(){
for(int i=0;i<g.adj[0].size();++i){
  for(int j=0;j<g.adj[0].size(); ++j){
    if(rank[i]>rank[g.adj[0][i][j].second]){
      g.adj[0][i].erase(g.adj[0][i].begin() + j--);}}

  for(int j=0;j<g.adj[1].size(); ++j){
    if(rank[i]>rank[g.adj[1][i][j].second]){
      g.adj[0][i].erase(g.adj[1][i].begin() + j--);}} } }

void initialise_node_queue(){
for(int v=0; v<g.adj[0].size();++v){
  contract_node(v);
  imp_que.push(importance_node(contract_node_importance,v));}}

void preprocess_graph(){
initialise_node_queue();
adding_shortcuts=true;

for(int i=0; imp_que.size(); i++){
  int u=imp_que.top().second;
  imp_que.pop();

  contract_node(u);

  if(imp_que.empty() || contract_node_importance<=imp_que.top().first){

    for(auto &shortcut : shortcuts){
      g.add_edge(shortcut.from,shortcut.to, shortcut.dist);}
    
    update_neighbours_level(u);
    rank[u]=i;}

  else{
    imp_que.push(importance_node(contract_node_importance,u));}}

  remove_edges();} };
    
    
    


int main()
{
int n,m,s,t,q; 
cin>>n>>m;  //n graph size, m number of edges
graph g(n);  

for(int i=0; i<m; i++){
  int x,y,w; 
  cin>>x>>y>>w;  //pair (x,y), weight/dist w constitute the edge
  g.add_edge(x-1,y-1,w);}

contraction_hierarchies ch(g);
ch.preprocess_graph();

cout<< "Graph preprocessing complete"<<endl;

cin>>q;
while(q--){
  cin>>s>>t;  //start and finish 
  cout<<g.bidirectional_contraction(--s,--t)<<endl;} 

return 0;
} 


