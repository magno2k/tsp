#include <bits/stdc++.h>
using namespace std;

// N, armazena a quantidade de cidades
// adj, a matriz de adjacência
int N, **adj;

// Armazena a ordem final das cidades que foram visitadas
int *caminho_final;                                            //+1

// Armazena os nós que já foram visitados de um caminho particular
bool *visitado;

// Resultado final com o menor caminho percorrido
int final_res = INT_MAX;

// Estrutura que armazena as coordenadas X,Y das cidades.
struct cidade 
{
	int x, y;  
};

void copia_final(int c_caminho[])
{
       for (int i=0; i < N; i++){
	       caminho_final[i] = c_caminho[i];
       }
       caminho_final[N] = c_caminho[0];
}

int primeiroMin(int i)
{
	int min = INT_MAX;
	for (int k=0; k < N; k++){
		if (adj[i][k] < min && i != k){
			min = adj[i][k];
		}
	}
    	return min;
}

int segundoMin( int i)
{
	int primeiro = INT_MAX, segundo = INT_MAX;
  	for (int j=0; j < N; j++)
  	{
    		if (i == j){
      			continue;
    		}
    
    		if (adj[i][j] <= primeiro){
      			segundo = primeiro;
      			primeiro = adj[i][j];
    		}
    		else if (adj[i][j] <= segundo && adj[i][j] != segundo)
    		{
      			segundo = adj[i][j];
    		}
  	}
  	return segundo;
}

void PCVrec(int c_limite, int peso, int level, int c_caminho[])
{

  	if (level == N) {

    		if (adj[c_caminho[level-1]][c_caminho[0]] != 0){
		  
      			int curr_res = peso + adj[c_caminho[level-1]][c_caminho[0]];
      
      			if (curr_res < final_res){
				copia_final(c_caminho);
				final_res = curr_res;
      			}
    		}
    		return;
  	}
  
  	for (int i = 0; i < N; i++){

    		if (adj[c_caminho[level-1]][i] != 0 && visitado[i] == false){
			int temp = c_limite;
      			peso += adj[c_caminho[level-1]][i];
      
			// Calcula diferentes c_limite para level 2 dos outros leveis
      			if (level==1){
				c_limite -= ((primeiroMin( c_caminho[level-1]) + primeiroMin(i))/2);
      			}
      			else {
				c_limite -= ((segundoMin(c_caminho[level-1]) + primeiroMin( i))/2);
      			}

      			if (c_limite + peso < final_res) {    
				c_caminho[level] = i;
				visitado[i] = true;
	
			// chama a função PCVrec para o proximo level 
				PCVrec(c_limite, peso, level+1,c_caminho);
      			}
      
      			peso -= adj[c_caminho[level-1]][i];
      			c_limite = temp;
      
      			//  Zera o array visitado
      			for(int i = 0; i < N; ++i) {
				visitado[i] = 0;
      			}
      
      			for (int j=0; j<=level-1; j++) {
				visitado[c_caminho[j]] = true;
			}
		}
	}
}

void PCV()
{
  	int c_limite = 0;
	int c_caminho[N+1];
	
  	memset(c_caminho, -1, sizeof(c_caminho));
  	
  	for(int i = 0; i < N; ++i) {
    		visitado[i] = 0;
  	}  
  
  	// Calcula o bound inicial
  	for (int i=0; i<N; i++) {
    		c_limite += (primeiroMin(i) + segundoMin(i)); 
	}
  
  	c_limite = (c_limite&1)? c_limite/2 + 1 : c_limite/2;
  
  	visitado[0] = true;
  	c_caminho[0] = 0;
  
  	PCVrec(c_limite, 0, 1, c_caminho);
}

int main( int argc, char *argv[]) {

	size_t n;
  	fstream arq, arq2;
  
  	arq.open ( argv[1], ios::in );
	arq2.open ( "tsp_bb_sol.txt", ios::out );
  	if ( not arq.is_open()) {
    		cerr << "Erro ao abrir arquivo" << endl;
    		exit(-1);
  	}
  	arq >> n;    
  	
	N=n;
	
  	adj = new int *[n];
  	for(size_t i = 0; i < n; i++) {
    		adj[i] = new int[n];
  	}
  
  	caminho_final = new int[n+1];
  	visitado  = ( bool *) calloc (n,sizeof(bool));  
  
  	struct cidade *coordenadas = new struct cidade [ n ];
	 
  	for ( size_t i=0; i<n; i++ ) {
    		arq >> coordenadas[i].x  >> coordenadas[i].y;
  	} 
  	for (size_t i = 0; i < n; i++) {
    		for (size_t j = 0; j < n; j++) {
			adj[i][j] = hypot ((coordenadas[i].x - coordenadas[j].x), (coordenadas[i].y - coordenadas[j].y));
    		}
  	}
  
  	PCV();
  
  	cout << final_res << endl;
	arq2 << final_res << endl;
  
  	for (size_t i=0; i<=n; i++) {
    		cout << caminho_final[i]+1 << " ";
  	}
  	for (size_t i=0; i<n; i++) {
		arq2 << caminho_final[i]+1 << " ";
	}
  
  	cout << endl;
	 
  	arq.close();
	arq2.close();
  	delete[] caminho_final;
  	delete[] coordenadas;
  
  	for(size_t i = 0; i < n; i++) {
    		delete[] adj[i];
  	}
  	delete[] adj;
  
  	return 0;
}

