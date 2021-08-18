#include <iostream>
#include <list>

using namespace std;

#define TOT_VERTICES 11
#define N_CELAS 10
#define SAIDA 10

class Grafo
{
    int V; //numero de vertices;
    list<int> *adj; //ponteiro pra array de listas de adjacência
                    //com a lista de adjacência, o índice significa a cela e cada item da lista nele corresponde às celas adjacentes
public:
    Grafo(int V); //construtor

    void addAresta(int v, int w); 
    int fuga(int s);
    void print(); 
};

Grafo::Grafo(int V) //inicializa o grafo
{
    this->V = V;
    adj = new list<int>[V]; //equivalente em c++ do malloc em C; adj recebe V espaços do tamanho de list<int> 
}

void Grafo::addAresta(int a, int b) //adiciona aresta entre os vertices a e b
{ //grafo nao-direcionado: pode-se ir de a para b e de b para a 
    adj[a].push_back(b);
    adj[b].push_back(a);
}

string converte(int i) //seguindo a convencao da questao, converte um indice em um nome de cela
{
    string cela;
    switch (i)
    {
    case 0:
        cela = "a1";
        break;
    case 1:
        cela = "a2";
        break;
    case 2:
        cela = "a3";
        break;
    case 3:
        cela = "a4";
        break;
    case 4:
        cela = "a5";
        break;
    case 5:
        cela = "a6";
        break;
    case 6:
        cela = "a7";
        break;
    case 7:
        cela = "a8";
        break;
    case 8:
        cela = "a9";
        break;
    case 9:
        cela = "a10";
        break;
    case 10:
        cela = "saída";
        break;
    default:
        break;
    }
    return cela;
}

void Grafo::print()
{
    int v = 0;
    list<int>::iterator i;
    while (v < V)
    {
        cout << "No vértice " << converte(v) << ": ";
        for (i = adj[v].begin(); i != adj[v].end(); ++i)
            cout << converte(*i) << " ";
        cout << endl;
        v++;
    }
}

void imprimeFila(list<int> fila)
{
    cout << "Fila agora: ";
    list<int>::iterator i;
    string cela;
    for (i = fila.begin(); i != fila.end(); i++)
        cout << converte(*i) << " ";
    cout << endl;
}

int Grafo::fuga(int s)
//algoritmo de busca em largura modificado para mostrar o caminho da cela s até a saída, passando por todas as celas
{
    bool *visited = new bool[V]; //array da quantidade de vertices, para representar os visitados
    for(int i = 0; i < V; i++)
        visited[i] = false; //mais uma vez, cada indice representa uma sala; inicialmente, nenhuma foi visitada, então são todas false

    list<int> fila; //fila útil para o algoritmo

    visited[s] = true; //marca o atual como visitado
    fila.push_back(s); //coloca-o na fila

    list<int>::iterator i; //ponteiro

    int cont_portoes = 0; //contador de portões (arestas), para saber quantos portões foram quebrados
    
    while(!fila.empty()) //fila vazia significa que todos foram visitados
    {
        // imprimeFila(fila); //pra ver o que está acontecendo

        s = fila.front(); //a nossa referência é o primeiro elemento da fila
        
        cout << converte(s) << " -> "; //imprimo o item, que já foi visitado
        fila.pop_front(); //e removo-o.

        //pega todos os vértices adjacentes ao vértice que acabou de ser desenfileirado (s)
        //Se o vértice não foi visitado, então marca como visitado e o
        //coloca na fila, para que se passe por ele mais tarde, ainda que ele já esteja visitado
        for (i = adj[s].begin(); i != adj[s].end(); ++i)
        {
            if (!visited[*i])
            {
                cout << "Marcado como visitado: " << converte(*i) << endl;
                visited[*i] = true;

                cont_portoes++;
                
                if (*i != SAIDA) //nao adiciono a saída na fila, para garantir que eles passem por ela só no fim
                    fila.push_back(*i);
            }
        }
    }
    cout << "saida\n";
    return cont_portoes;
}

int main()
{
    cout << "---<PROBLEMA DOS DISSIDENTES POLÍTICOS v1>---\n";
    Grafo g(TOT_VERTICES);
    g.addAresta(0, 5);
    g.addAresta(0, 10);
    g.addAresta(1, 4);
    g.addAresta(1, 4);
    g.addAresta(1, 5);
    g.addAresta(1, 10);
    g.addAresta(2, 3);
    g.addAresta(2, 10);
    g.addAresta(3, 4);
    g.addAresta(3, 9);
    g.addAresta(4, 6);
    g.addAresta(4, 8);
    g.addAresta(4, 9);
    g.addAresta(5, 6);
    g.addAresta(6, 7);
    g.addAresta(6, 8);
    g.addAresta(6, 10);
    g.addAresta(7, 10);
    g.addAresta(8, 9);
    g.addAresta(8, 10);
    g.addAresta(9, 10);

    // g.print();
    
    int inicio;
    
    cout << "Escolha a cela a partir da qual se iniciará o processo de fuga [0, " << N_CELAS -1 << "]: ";
    cin >> inicio;
    while (inicio < 0 || inicio >= N_CELAS || inicio == SAIDA)
    {
        cout << "Entrada inválida! ";
        cout << "Escolha a cela a partir da qual se iniciará o processo de fuga [0, " << N_CELAS -1 << "]: ";
        cin >> inicio;
    }
    
    int c = g.fuga(inicio);

    cout << "Portões atravessados: " << c << endl;
    
    return 0;
}

/*
P7) Problema dos dissidentes políticos

A figura abaixo apresente o diagrama de um prisão para dissidentes políticos. 
Os prisioneiros estão divididos em várias celas como mostrado. 
Um espião planeja ajudá-los a escapar explodindo os portões das paredes das prisão. 
Dado o risco do plano, ele deseja destruir o menor número possível de portões e ainda assim 
garantir que todos os prisioneiros escapem. Quantos portões devem ser explodidos? 
Como poderia um problema mais geral ser resolvido?

Consideramos que, pela figura, cada cela é separada da outra por um portão. Deste modo,
pode-se ir de uma cela a outra, quando elas são adjacentes, se esse portão é destruído.
O algoritmo simula o processo de destruir os portões, passando de uma cela a outra,
até que todas tenham sido visitadas e os dissidentes possam ir para a saída. 
A saída é sempre o último destino, mas eles se certificam de que o primeiro portão
que encontraram para ela foi destruído, para que possam voltar até ele depois que 
completarem o grupo.

R: Assim, com esse número de celas, o número mínimo de portões é sempre 10.
*/