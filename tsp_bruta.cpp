#include <bits/stdc++.h>

using namespace std;

struct cidade 
{
	int x, y;
};

int main (int argc, char *argv[]){

	int n, *perm, **adj, x = 0, menor = INT_MAX;
  	string s;
	vector<string> resultado_final(1);
  	fstream arq, arq2;
	
 	arq.open ( argv[1], ios::in );
	arq2.open("tsp_bruta_sol.txt", ios::out );
  	if ( not arq.is_open()){
    		cerr << "Erro ao abrir arquivo" << endl;
    		exit(-1);
  	}
  	
	arq >> n; 

  	adj = new int *[n];
  
  	for(int i = 0; i < n; i++) {
    		adj[i] = new int[n];
  	}
  	
  	struct cidade *coordenadas = new struct cidade [ n ];
	
	// le as coordenadas as X,Y das cidades do arquivo
  	for ( int i=0; i<n; i++ ) {
    		arq >> coordenadas[i].x  >> coordenadas[i].y;
  	}	
	// Gera a matriz de adjacencia 
  	for (int i = 0; i < n; i++) {
    		for (int j = 0; j < n; j++) {
      			adj[i][j] = hypot ((coordenadas[i].x - coordenadas[j].x), (coordenadas[i].y - coordenadas[j].y));
    		}    
  	}
  
  	perm = new int [n];
	// gera o array que sera permutado com valores de 1 ... n-1.
  	for ( int i = 0; i < n; i++ ){
 		perm[i] = i+1;    
  	}
  	
  	// Ordena o array perm e inverte em ordem decrescente
  	// Complexidade da função sort: O(N log(N)) comparisons (both average and worst-case), where N is last - first. 
  	// Essa função utiliza o "introsort algorithm" (D. R. Musser, "Introspective Sorting and Selection Algorithms", 
  	// Software Practice and Experience 27(8):983, 1997.)
  	std::sort (perm,perm+n);
	//Complexidade da função reverse: Linear: reverse(first, last) makes (last - first) / 2 calls to swap.
   	std::reverse (perm,perm+n); 
	
	// Gera a permutacao, considerando apenas as permutações que começam com 1, para o calculo do tour, pois nesse TSP o problema 
	// sempre começa com a cidade 1.
	// Complexidade da função prev_permutation: Linear. At most (last - first) / 2 swaps. 
  	do {
		if ( perm[0] == 1 ){
  			for ( int i = 0; i < n-1; i++) {    
       				x +=  adj[perm[i]-1][perm[i+1]-1];
    			}
     			x += adj[perm[n-1]-1][perm[0]-1];
    			if ( menor > x ) {
				menor = x;
				for ( int i = 0; i < n; i++){
					s += to_string(perm[i] ) + ' ';
				}
				s += to_string(perm[0]);
				resultado_final[0] = s;
				s.clear();
			}    
  		}
  		x=0;
		
  	} while ( std::prev_permutation(perm,perm+n) );

  	cout << menor << endl << resultado_final[0] << endl;
	resultado_final[0].pop_back();
	arq2 << menor << endl << resultado_final[0] << endl;
	
	for(int i = 0; i < n; i++) {
    		delete[] adj[i];
  	}
  	delete[] adj;
  	delete[] perm;
  	delete[] coordenadas;
	arq.close();
	arq2.close();
  
  	return 0;
}
