#include <bits/stdc++.h>

using namespace std;

// classe que representa o grafo

class Grafo
{
	private:
		int V; // numero de vertices
		int total_arestas; // total de arestas
		int vertice_inicial; // vertice inicial
		std::map<std::pair<int, int>, int> map_edges; // map de arestas
	public:
		Grafo(int V, int vertice_inicial); // construtor
		void add_aresta(int v1, int v2, int peso); // adiciona arestas
		int existe_aresta(int src, int dest); // verifica se existe uma arestas
		friend class Genetico; // Para acessar membros privados dessa classe
};

// classe que representa o algoritmo genetico
class Genetico
{
	private:
		Grafo* graph; // O grafo
		std::vector<pair<vector<int>, int>> populacao; // cade elemento tem um par: vector e custo total
		int tamanho_populacao; // tamanho da populacao
		int tamanho_real_populacao; // tamanho real da populaçao
		int generations; // quantidade de geracoes
		int mutation_rate; // mutação
		bool exibir_populacao; // mostra a populacao
		void gera_populacao(); // Gera a populacao inicial
	public:
		Genetico(Grafo* graph, int total_populacao, int generations, int mutation_rate, bool exibir_populacao = true); // construtor
		int solucaoValida(std::vector<int>& solution); // verifica se a soluçao é valida
		void mostra_populacao(); // mostra a populaçao
		void crossOver(std::vector<int>& parente1, std::vector<int>& parente2); 
		void insertBinarySearch(std::vector<int>& filho, int total_cost); //pesquisa binaria
		void run(); // executa o algoritmo
		int melhor_solucao(); // 
		bool existe_chromo(const std::vector<int> & v); 
};

Grafo::Grafo(int V, int vertice_inicial) 
{
	if(V < 1) // verifica se o numero de vertice é menos que 1
	{
		cout << "Numero de vertices menor que 1" << endl;
		exit(1);
	}
	
	this->V = V; // numero de vertices
	this->vertice_inicial = vertice_inicial; // vertice inicial
	this->total_arestas = 0; // inicialmente o total de arestas é 0
}

void Grafo::add_aresta(int src, int dest, int peso) 
{
	map_edges[make_pair(src, dest)] = peso; // adiciona vertices no map
}

int Grafo::existe_aresta(int src, int dest) // verifica se existe arestas
{
	auto it = map_edges.find(make_pair(src, dest)); 
	
	if(it != map_edges.end())
		return it->second; 
	return -1;
}

Genetico::Genetico(Grafo* graph, int tamanho_populacao, int generations, int mutation_rate, bool exibir_populacao)
{
	if(tamanho_populacao < 1) // verifica se o tamanho da população é menos que 1
	{
		cout << "Error: tamanho da populacao < 1" << endl;
		exit(1);
	}
	else if(mutation_rate < 0 || mutation_rate > 100) // verifica se a mutaçao é menos que 0 
	{
		cout << "Error: mutation_rate tem que ser >= 0 e <= 100" << endl;
		exit(1);
	}
	this->graph = graph;
	this->tamanho_populacao = tamanho_populacao;
	this->tamanho_real_populacao = 0;
	this->generations = generations;
	this->mutation_rate = mutation_rate;
	this->exibir_populacao = exibir_populacao;
}

int Genetico::solucaoValida(vector<int>& solution)
{
	int total_cost = 0;
	set<int> set_solution;
	
	for(int i = 0; i < graph->V; i++)
		set_solution.insert(solution[i]);
	
	if(set_solution.size() != (unsigned)graph->V) 
		return -1;

	// verifica se as conexões são válidas
	for(int i = 0; i < graph->V; i++)
	{
		if(i + 1 <  graph->V)
		{
			int cost = graph->existe_aresta(solution[i], solution[i+1]);
			
			// verifica se existe conexão
			if(cost == -1)  
				return -1;
			else
				total_cost += cost;
		}
		else
		{
			int cost = graph->existe_aresta(solution[i], solution[0]);
			
			//verifica se existe conexão
			if(cost == -1) 
				return -1;
			else
				total_cost += cost;
			break;
		}
	}
	return total_cost;
}


bool Genetico::existe_chromo(const vector<int> & v)
{

	for(auto it= begin(populacao); it != end(populacao); it++)
	{
		const vector<int>& vec = (*it).first; 
		if(equal(v.begin(), v.end(), vec.begin())) 
			return true;
	}
	return false;
}


void Genetico::gera_populacao() 
{
	vector<int> parente;
	
	// inseri o vertice inicial em parente
	parente.push_back(graph->vertice_inicial);
	
	// cria o parente
	for(int i = 0; i < graph->V; i++)
	{
		if(i != graph->vertice_inicial)
			parente.push_back(i);
	}
		
	int total_cost = solucaoValida(parente);
	
	if(total_cost != -1) // verifica se o parente é valido
	{
		populacao.push_back(make_pair(parente, total_cost)); // inseri na população
		tamanho_real_populacao++; //incrementa
	}
	
	//  gera as permutaçoes 
	for(int i = 0; i < generations; i++)
	{
		// gera uma permutação aleatoriamente 
		random_shuffle(begin(parente) + 1, begin(parente) + (rand() % (graph->V - 1) + 1));
		
		int total_cost = solucaoValida(parente); // verifica se a solução é valida
		
		// verifica se a permutação é uma solução valida e se já existe
		if(total_cost != -1 && !existe_chromo(parente))
		{
			populacao.push_back(make_pair(parente, total_cost)); // adiciona população
			tamanho_real_populacao++; 
		}
		if(tamanho_real_populacao == tamanho_populacao) // verifica o tamanho da população
			break;
	}
	
	if(tamanho_real_populacao == 0)
		cout << "\nPopulacao inicial vazia ...";
	else
		sort(begin(populacao), end(populacao), []( auto f, auto s ) { return f.second < s.second; }); 
}

void Genetico::mostra_populacao()
{
	cout << "\nSolucoes...\n\n";
	for(auto it= begin(populacao); it != end(populacao); it++)
	{
		const vector<int>& vec = (*it).first;
		
		for(int i = 0; i < graph->V; i++)
			cout << vec[i]+1 << " ";
		cout << graph->vertice_inicial+1;
		cout << " | custo: " << (*it).second << "\n\n";
	}
	cout << endl <<"Tamanho da populacao: " << tamanho_real_populacao << endl;
}


void Genetico::insertBinarySearch(vector<int>& filho, int total_cost)
{
	int imin = 0;
	int imax = tamanho_real_populacao - 1;
	
	while(imax >= imin)
	{
		int imid = imin + (imax - imin) / 2; 
		
		if(total_cost == populacao[imid].second)
		{
			populacao.insert(begin(populacao) + imid, make_pair(filho, total_cost));
			return;
		}
		else if(total_cost > populacao[imid].second)
			imin = imid + 1;
		else
			imax = imid - 1;
	}
	populacao.insert(begin(populacao) + imin, make_pair(filho, total_cost)); 
}

void Genetico::crossOver(vector<int>& parente1, vector<int>& parente2)
{
	vector<int> filho1, filho2;
	
	// map de genes, verifica se eles já são selecionados
	map<int, int> genes1, genes2;
	
	// O(n)
	for(int i = 0; i < graph->V; i++)
	{
		// inicialemente os genes não são usados
		genes1[parente1[i]] = 0; 
		genes2[parente2[i]] = 0;
	}
	
	// gera pontos aleatorio 
	int point1 = rand() % (graph->V - 1) + 1;
	int point2 = rand() % (graph->V - point1) + point1;
	
	
	if(point1 == point2)
	{
		if(point1 - 1 > 1)
			point1--;
		else if(point2 + 1 < graph->V)
			point2++;
		else
		{
			
			int point = rand() % 10 + 1; 
			if(point <= 5)
				point1--;
			else
				point2++;
		}
	}
	

	for(int i = 0; i < point1; i++)
	{
		// adiciona genes
		filho1.push_back(parente1[i]); 
		filho2.push_back(parente2[i]);
		// marca genes
		genes1[parente1[i]] = 1; 
		genes2[parente2[i]] = 1;
	}
	
	// complexidade linear
	for(int i = point2 + 1; i < graph->V; i++)
	{
		genes1[parente1[i]] = 1;
		genes2[parente2[i]] = 1;
	}
		

	for(int i = point2; i >= point1; i--)
	{
		if(genes1[parente2[i]] == 0) // se gene não for usado
		{
			filho1.push_back(parente2[i]);
			genes1[parente2[i]] = 1; // marca the gene	
		}
		else
		{
			// Escolha um gene que foi usado
			for ( auto it = begin(genes1); it != end(genes1); it++ )
			{
				if(it->second == 0) // verifica se n foi usado
				{
					filho1.push_back(it->first);
					genes1[it->first] = 1; // marca que foi usado
					break; // sai do loop
				}
			}
		}
		
		if(genes2[parente1[i]] == 0) 
		{
			filho2.push_back(parente1[i]);
			genes2[parente1[i]] = 1; // marks the gene
		}
		else
		{
			// Escolha um gene que foi usado
			for ( auto it = begin(genes2); it != end(genes2); it++ )
			{
				if(it->second == 0) // verifica se n foi usado
				{
					filho2.push_back(it->first);
					genes2[it->first] = 1; // marca que foi usado
					break; 
				}
			}
		}
	}
	
	for(int i = point2 + 1; i < graph->V; i++)
	{
		filho1.push_back(parente1[i]);
		filho2.push_back(parente2[i]);
	}
		
	// mutação
	int mutation = rand() % 100 + 1; 
	if(mutation <= mutation_rate) 
	{
		
		int index_gene1, index_gene2;
		index_gene1 = rand() % (graph->V - 1) + 1;
		index_gene2 = rand() % (graph->V - 1) + 1;
		
		
		int aux = filho1[index_gene1];
		filho1[index_gene1] = filho1[index_gene2];
		filho1[index_gene2] = aux;
		
		
		aux = filho2[index_gene1];
		filho2[index_gene1] = filho2[index_gene2];
		filho2[index_gene2] = aux;
	}
	
	int total_cost_filho1 = solucaoValida(filho1);
	int total_cost_filho2 = solucaoValida(filho2);
	
	if(total_cost_filho1 != -1 && !existe_chromo(filho1))
	{
		insertBinarySearch(filho1, total_cost_filho1);
		tamanho_real_populacao++; 
	}
	
	
	if(total_cost_filho2 != -1 && !existe_chromo(filho2))
	{
		insertBinarySearch(filho2, total_cost_filho2);
		tamanho_real_populacao++; 
	}
}



void Genetico::run()
{
	gera_populacao(); 
	fstream arq2;
	
	if(tamanho_real_populacao == 0)
		return;
	
	//complexidade O(n)
	
	for(int i = 0; i < generations; i++)
	{
		int  old_tamanho_populacao = tamanho_real_populacao;
		
		if(tamanho_real_populacao >= 2)
		{	
			if(tamanho_real_populacao == 2)
			{
				// applying crossover in the parentes
				crossOver(populacao[0].first, populacao[1].first);
			}
			else
			{
				int parente1, parente2;
			
				do
				{
					parente1 = rand() % tamanho_real_populacao;
					parente2 = rand() % tamanho_real_populacao;
				
				  
				}while(parente1 == parente2);
				
				crossOver(populacao[parente1].first, populacao[parente2].first);
			}
			 
			int diff_populacao = tamanho_real_populacao - old_tamanho_populacao;
			
			if(diff_populacao == 2)
			{
				if(tamanho_real_populacao > tamanho_populacao)
				{
					
					populacao.pop_back();
					populacao.pop_back();
					
					tamanho_real_populacao -= 2;
				}
			}
			else if(diff_populacao == 1)
			{
				if(tamanho_real_populacao > tamanho_populacao)
				{
					populacao.pop_back(); 
					tamanho_real_populacao--; 
				}
			}
		} 
		else
		{

			crossOver(populacao[0].first, populacao[0].first);
			
			if(tamanho_real_populacao > tamanho_populacao)
			{
				populacao.pop_back(); 
				tamanho_real_populacao--; 
			}
		}
	}
	
	if(exibir_populacao == true)
		mostra_populacao();
	
	arq2.open("tsp_genetic_sol.txt", ios::out);
	cout << populacao[0].second << endl;
	const vector<int>& vec = populacao[0].first;
	arq2 << populacao[0].second << endl;
	for(int i = 0; i < graph->V; i++)
	{
		cout << vec[i]+1 << " ";
		arq2 << vec[i]+1 << " ";
	}
	arq2 << endl;
	cout << graph->vertice_inicial+1;
	cout << endl;
	arq2.close();
}


int Genetico::melhor_solucao()
{
	if(tamanho_real_populacao > 0)
		return populacao[0].second;
	return -1;
}

struct cidade
{
	int x, y;
};

int main( int argc, char *argv[])
{
	ifstream arq;
	int n, **adj;
	Grafo *grafo;	
	srand(time(NULL));
	
	arq.open(argv[1], ios::in );	
	if ( not arq.is_open())
	{
	  cout << "erro ao abir arquivo" << endl;
	  exit(-1);
	}
	arq >> n;
	
	adj = new int *[n];
	for(int i = 0; i < n; i++) {
	    adj[i] = new int[n];
        }
	
	grafo = new Grafo(n, 0);
	
	auto coordenadas = new  cidade [ n * sizeof ( struct cidade )];


	for ( int i=0; i<n; i++ )
	{
		arq >> coordenadas[i].x  >> coordenadas[i].y;
	}	

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			adj[i][j] = hypot ((coordenadas[i].x - coordenadas[j].x), (coordenadas[i].y - coordenadas[j].y));
			grafo->add_aresta(i, j, adj[i][j]); 
		}
	}
	
	// parametros: o grafo, tamanho da populacao, geracoes e mutação
	// opcional mostrar populacao
	Genetico genetic(grafo, 100, 1000000, 0, false);
	genetic.run(); 
	
	arq.close();
	
	delete[] coordenadas;
	delete grafo;
  
	for(int i = 0; i < n; i++) {
	    delete[] adj[i];
	}
	delete[] adj;
	
	return 0;
}
