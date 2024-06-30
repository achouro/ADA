#include <cmath>
#include <iostream>
#include <vector>
#include <iomanip>
#include <cstdio>
#include <bitset>
#include <cmath>
#include <algorithm>
#include <limits>
#include <numeric>
using namespace std;


const long double EPS = 1e-3;
const long double INF = 1.0e+9;

using namespace std;

typedef vector<long double> Column;
typedef vector<long double> Row;
typedef vector<Row> Matrix;

struct Equation {
    Equation(const Matrix &a, const Column &b):
        a(a),
        b(b)
    {}

    Matrix a;
    Column b;
};

struct Position {
    Position(int column, int row):
        column(column),
        row(row)
    {}

    int column;
    int row;
};

Equation ReadEquation() {
    int size;
    cin >> size;
    Matrix a(size, vector <long double> (size, 0.0));
    Column b(size, 0.0);
    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < size; ++column)
            cin >> a[row][column];
        cin >> b[row];
    }
    return Equation(move(a),move(b));
}

Position SelectPivotElement(const Matrix &a, vector <bool> &used_rows, 
  			    vector <bool> &used_columns) {

    // Select the first free element.

    Position pivot_element(0, 0);
    while (used_rows[pivot_element.row])
        ++pivot_element.row;
    while (used_columns[pivot_element.column])
        ++pivot_element.column;
    //Then pivot with the largest abs value from free elements

    double max=0.0;
    for(int p=pivot_element.row; p<a.size(); ++p){
	if(fabs(max) - std::fabs(a[p][pivot_element.column]) < EPS){

	    max= a[p][pivot_element.column];
	    pivot_element.row=p;} } 

   return pivot_element;
} 

void SwapLines(Matrix &a, Column &b, vector <bool> &used_rows, Position &pivot_element) {
    swap(a[pivot_element.column], a[pivot_element.row]);
    swap(b[pivot_element.column], b[pivot_element.row]);
    swap(used_rows[pivot_element.column], used_rows[pivot_element.row]);
    pivot_element.row = pivot_element.column;
}


int BackSubstitution(Matrix &a, Column &b){

    for(int i=a.size() -1 ;i ; --i){
	    long double v= b[i];
	    for(int j=0; j!=i;++j){

	        b[j] -= a[j][i]*b[i];
	        //a[j][i]=0; 
	        }}
    return 0;
}


void ScalePivot( Matrix &a, Column &b, const Position &pivot_element){ 
 
  const double divisor = a[pivot_element.row][pivot_element.column];
  const int size = a.size();

  for (int j = pivot_element.column; j < size; ++j)
  {
    a[pivot_element.row][j] /= divisor;
  }

  b[pivot_element.row] /= divisor;
}  


void ProcessPivotElement(Matrix &a, Column &b, const Position &pivot_element) {
    
    const int size=a.size();
    long double multiple= 0.0;
    ScalePivot(a, b, pivot_element);

    for(int i=pivot_element.row +1; i<size; ++i){
        multiple= a[i][pivot_element.column];
	    for(int j=pivot_element.column; j<size; ++j){

	        a[i][j] -= a[pivot_element.row][j] * multiple;}

	b[i] -= b[pivot_element.row] * multiple;}
}

void MarkPivotElementUsed(const Position &pivot_element, vector <bool> &used_rows, vector <bool> &used_columns) {
    used_rows[pivot_element.row] = true;
    used_columns[pivot_element.column] = true;
}

pair<int,Column> SolveEquation(Equation equation) {
    
    Matrix &a = equation.a;
    Column &b = equation.b;

    if(a.empty()){ return {};}

    int size = a.size();


    vector <bool> used_columns(size, false);
    vector <bool> used_rows(size, false);

    for (int step = 0; step < size; ++step) {

        Position pivot_element = SelectPivotElement(a, used_rows, used_columns);

	if(a[pivot_element.row][pivot_element.column]==0) { break;}

        SwapLines(a, b, used_rows, pivot_element);
        ProcessPivotElement(a, b, pivot_element);
        MarkPivotElementUsed(pivot_element, used_rows, used_columns); 
    }

    int bsub =BackSubstitution(a,b);

    return {bsub, move(b)} ;
}

vector<vector<int> > GetSubsets(const vector<int> &set, int m){

    const int n=pow(2, set.size());
    vector<vector<int> > subsets;
    std::bitset<32> bset;

    for(int i=0; i<n; ++i){
        bset=bset.to_ulong() +1l;
        std::vector<int> subset;
	    int count=0;

	    for(int j=0;j<set.size();++j){
	        if(bset[set.size() -1 -j]){
		        subset.push_back(set[j]);
		        if(++count>m){ break;} } }

	    if(count==m){
	        subsets.emplace_back(std::move(subset)); } }

    return subsets;	
}


vector<Column> SolveAllEquations(int m, const Matrix &A, const vector<long double> &b){

    vector<Column> solutions;
    vector<int> nums(A.size());

    int s=0;
    generate(nums.begin(), nums.end(), [&s] {return s++;} );
    auto subsets= GetSubsets(nums,m);

    if(A.size()==1){ subsets.emplace_back(0);}

    for(const auto &sub : subsets){
	    Matrix mat; Column col;
	    for(auto j:sub){
	        mat.push_back(A[j]);
	        col.push_back(b[j]);} 

	    Equation eq{move(mat), move(col)};
	
	    auto salva= SolveEquation(eq);
 	    if(salva.first==0 && !salva.second.empty()){
	        solutions.emplace_back(move(salva.second)); } }

    return solutions; 
}

void Prepare(int &n, const int m, Matrix &A, vector<long double> &b){

    while(n<m){
	A.emplace_back(m,0);
	b.emplace_back(0);
	++n;}

    A.emplace_back(vector<long double> (m,1));
    b.emplace_back(1.0e+9);
    ++n;

    for(int k=0; k<m; ++k){
	vector<long double> vec(m,0.0);
	vec[k]=-1;
	A.emplace_back(move(vec));
	b.emplace_back(-0.0);
	++n; }
}

pair<int, vector< long double>> solve_diet_problem(int n, int m, Matrix A, vector< long double> b, vector< long double> c) {

    Prepare(n,m,A,b);
    vector<Column> solutions=SolveAllEquations(m,A,b);

    if(solutions.size()==0){ return {-1, {} };}

    int sol_index=-1;
    long double largest_utility= -(std::numeric_limits<long double>::max()/2);

    for(int i=0;i<solutions.size(); ++i){
	    auto &sol= solutions[i];
	    bool satisfied=true;

	    for(int j=0;j<n; ++j){
	        long double constraint=b[j];
	        long double sum=0.0;

	        for(int k=0; k<m; ++k){
		        sum+= A[j][k] *sol[k];}

	        if(sum-constraint > EPS){
		    satisfied= false; 
		    break;} }

	    long double utility=0.0;
	    for(int k=0; k<m; ++k){
	        utility+=sol[k]*c[k];}

	    if(satisfied && utility>largest_utility){
	        largest_utility=utility;
	        sol_index=i;} }

    if(sol_index==-1){ return {-1, {} };} 

    auto &sol= solutions[sol_index];
    if((accumulate(sol.begin(), sol.end(), (long double) 0.0)) + EPS >=INF) {return {1, {} }; }

    return {0, move(sol)}; 
} 


int main(){
  std::ios_base::sync_with_stdio(false);    
  int n, m;
  cin >> n >> m;
  Matrix A(n, vector<long double>(m));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      cin >> A[i][j];
    }
  }
  vector<long double> b(n);
  for (int i = 0; i < n; i++) {
    cin >> b[i];
  }
  vector<long double> c(m);
  for (int i = 0; i < m; i++) {
    cin >> c[i];
  }

  auto ans = solve_diet_problem(n, m, move(A), move(b), move(c));

  switch (ans.first) {
    case -1: 
      printf("No solution\n");
      break;
    case 0: 
      printf("Bounded solution\n");
      for (int i = 0; i < m; i++) {
        printf("%.18Lf%c", ans.second[i], " \n"[i + 1 == m]);
      }
      break;
    case 1:
      printf("Infinity\n");
      break;      
  }
  return 0;
}