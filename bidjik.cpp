/*Bidirectional Djikstra for Social Networks*/

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

length INF= numeric_limits<length>::max()/4;

class graph
{
adj_list adj;
djistance dist;
vector<point> coord;
que queue_f, queue_r;
vector<int> visited_f, visited_r, workset;
int t_=0, s_=0;

public:

graph(int n) : adj(adj_list(2,vector<vector<edge> >(n,vector<edge>() ))),
	       dist(2, vector<length>(n,INF)),
	       visited_f(vector<int>(n,0)),
	       visited_r(vector<int>(n,0)){};

void add_edge(int u, int v, int w){
  adj[0][u].push_back(edge(w,v));
  adj[1][v].push_back(edge(w,u));}


void clear_containers(){
  for(int u=-1; !queue_f.empty(); queue_f.pop()){
    u=queue_f.top().second;
    dist[0][u]=dist[1][u]=INF;
    visited_f[u]=visited_r[u]=0;}
  for(int u=-1; !queue_r.empty(); queue_r.pop()){
    u=queue_r.top().second;
    dist[0][u]=dist[1][u]=INF;
    visited_f[u]=visited_r[u]=0;}
  for(auto u:workset){
    dist[0][u]=dist[1][u]=INF;
    visited_f[u]=visited_r[u]=0;}
  workset.clear();}

void relax(que &queue, length d, int u, int v, int side){
  if(dist[side][u]> d + dist[side][v]){
    dist[side][u]= d + dist[side][v];
    queue.push(edge(dist[side][u],u));}}

void process(int u, que &queue, int side){
  queue.pop();
  for(auto &edger: adj[side][u]){
    relax(queue, edger.first /*length d*/,edger.second, u, side);}}

length compute_path(){
  length result= INF;
  for( auto u: workset)
    if(dist[0][u]+dist[1][u]<result)
      result= dist[0][u]+ dist[1][u];
  return result==INF ? -1: result;} //return no path -1 or path length}

length bidirectional_djikstra(int s, int t){
  clear_containers();
  s_=s, t_=t;

  queue_f.push(edge(0,s));
  queue_r.push(edge(0,t));
  dist[0][s]=dist[1][t]=0;

  while(!queue_f.empty() && !queue_r.empty()){
    int u= queue_f.top().second;
    process(u, queue_f, 0);
    visited_f[u]++;
    if(visited_r[u])
      break;
    workset.push_back(u);

    u= queue_r.top().second;
    process(u, queue_r,1);
    visited_r[u]++;
    if(visited_f[u])
      break;
    workset.push_back(u);}

  return compute_path();}

};  

int main()
{
int n,m,s,t,q; 
cin>>n>>m;  //n graph size, m number of edges
graph g(n);  

for(int i=0; i<m; i++){
  int x,y,w; 
  cin>>x>>y>>w;  //pair (x,y), weight/dist w constitute the edge
  g.add_edge(x-1,y-1,w);}

cin>>q;
while(q--){
  cin>>s>>t;  //start and finish 
  cout<<g.bidirectional_djikstra(--s,--t)<<endl;} }  //Bi-Djik ShPA 

