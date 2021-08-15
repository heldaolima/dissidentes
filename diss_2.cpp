#include <iostream>
#include <list>

using namespace std;

#define TOT_VERTICES 11
#define N_CELAS 10
#define SAIDA 10

typedef struct {
    //caso em que o vertice, indo ao destino, libera um vertice adjacente
    int vertice; 
    int dest; // o proximo
    int adj; //o que se destroi quando vai para o proximo  
}paredeUnida;

class Grafo
{
    int V; 
    list<int> *adj; 
    list<paredeUnida> paredes; //as paredes especiais no meu mapa
public:
    Grafo(int V); 

    void addAresta(int v, int w);
    bool checa_adjacencia(int a, int b);
    bool addparedeUnida(int v, int d, int a);
    int fuga(int s, bool imprimir);
    void print(); //imprime os vértices do grafo e os seus adjacentes
};

Grafo::Grafo(int V) //inicializa o grafo
{
    this->V = V;
    adj = new list<int>[V]; 
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
        string s = converte(v);
        
        cout << "No vértice " << s << ": ";
        for (i = adj[v].begin(); i != adj[v].end(); ++i)
        {
            string adj = converte(*i);
            cout << adj << " ";
        }
        
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
    {
        cela = converte(*i);
        cout << cela << " ";
    }
    cout << endl;
}

int Grafo::fuga(int ini, bool imprimir)
//algoritmo de busca em largura modificado para mostrar o caminho da cela s até a saída, passando por todas as celas
//aqui é a interpretação com paredes
{
    bool *visited = new bool[V]; 
    for (int i = 0; i < V; i++)
        visited[i] = false; 
    
    list<int> fila;

    visited[ini] = true; 
    fila.push_back(ini);

    list<int>::iterator i;
    list<paredeUnida>::iterator j; //para iterar pela lista de paredes 

    int cont_explosoes = 0; 
    
    while(!fila.empty()) 
    {
        // imprimeFila(fila);
        ini = fila.front(); 
        if (imprimir)
        {
            string cela = converte(ini);
            cout << cela << " -> ";
        }

        fila.pop_front(); 
        
        for (i = adj[ini].begin(); i != adj[ini].end(); ++i)
        {
            if (!visited[*i])
            {
                visited[*i] = true;

                //sendo ini o vert e *i um seu adjacente, pode ser que no caminho de ini para *i
                //acabe sendo liberado outra cela
                //entao eu procuro nas paredes unidas o vertice atual e vejo se ele tem *i como destino
                //se sim, entao a cela ao lado deve ser marcada como liberada, sem que o contador se some 
                for(j = paredes.begin(); j != paredes.end(); j++)
                {
                    if (j->vertice == ini && j->dest == *i) 
                    {
                        // cout << "\nveritce: " << j->vertice << " dest: " << j->dest << " adj: " << j->adj << endl;
                        if (!visited[j->adj])
                        {
                            visited[j->adj] = true;
                            fila.push_back(j->adj);
                        }
                    }
                }
                cont_explosoes++;
                if (*i != SAIDA) 
                    fila.push_back(*i);
            }
        }
    }
    return cont_explosoes;
}

bool Grafo::checa_adjacencia(int a, int b)
{
    list<int>::iterator i;

    for (i = adj[a].begin(); i != adj[a].end(); i++)
    {
        if (*i == b) return true;
    }
    return false;
}

bool Grafo::addparedeUnida(int a, int dest, int adj)
{
    
    if (this->checa_adjacencia(a, dest))
    {
        paredeUnida nova;
        
        nova.vertice = a;
        nova.dest = dest;
        nova.adj = adj;
        
        paredes.push_back(nova);
        
        return true;
    }
    else
        return false;
}

int main()
{
    cout << "---<PROBLEMA DOS DISSIDENTES POLÍTICOS v2>---\n";
    
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
    cout << endl;

    //digo no mapa quais são os casos de parede unida
    g.addparedeUnida(1, 3, 4);
    g.addparedeUnida(1, 4, 3);
    g.addparedeUnida(1, 5, 6);
    g.addparedeUnida(1, 5, 4);
    g.addparedeUnida(3, 4, 9);
    g.addparedeUnida(3, 9, 4);
    g.addparedeUnida(4, 1, 3);
    g.addparedeUnida(4, 3, 9);
    g.addparedeUnida(4, 5, 6);
    g.addparedeUnida(4, 6, 5);
    g.addparedeUnida(5, 1, 4);
    g.addparedeUnida(5, 1, 6);
    g.addparedeUnida(6, 4, 1);
    g.addparedeUnida(6, 4, 5);

    int escolha;
    cout << "[1] Escolher uma cela por onde começar a fuga\n";
    cout << "[2] Encontrar o menor número de paredes possível\n";
    cout << "[3] Encerrar\n";
    cout << "Escolha: ";
    cin >> escolha;
    while (escolha < 1 || escolha > 3)
    {
        cout << "Opção inválida! Faça a sua escolha: ";
        cin >> escolha;
    }

    int c;
    switch (escolha)
    {
    case 1:
        int inicio;
        
        cout << "Escolha a cela a partir da qual se iniciará o processo de fuga [0, " << N_CELAS -1 << "]: ";
        cin >> inicio;
        while (inicio < 0 || inicio >= N_CELAS || inicio == SAIDA)
        {
            cout << "Entrada inválida! ";
            cout << "Escolha a cela a partir da qual se iniciará o processo de fuga [0, " << N_CELAS -1 << "]: ";
            cin >> inicio;
        }
        cout << "A partir da cela " << converte(inicio) << endl;    
        c = g.fuga(inicio, true);

        cout << "Número de paredes explodidas: " << c << endl;
        break;
    case 2:
        int resultados[N_CELAS];
        int menor, i;
        for (i = 0; i < N_CELAS; i++)
        {
            c = g.fuga(i, false);
            if (i == 0)
                menor = c;
            else
                if (c < menor)
                    menor = c;
            resultados[i] = c;
        }
        cout << "Menor número de paredes explodidas: " << menor << endl;
        cout << "Esse foi o resultado obtido começando a partir das celas: ";
        
        for (i = 0; i < N_CELAS; i++)
        {
            if (resultados[i] == menor)
                cout << converte(i) << " ";
        }
        cout << endl;
        break;
    
    default:
        break;
    }

    return 0;
}

/*
P7) Problema dos dissidentes políticos

A figura abaixo apresente o diagrama de um prisão para dissidentes políticos. 
Os prisioneiros estão divididos em várias celas como mostrado. 
Um espião planeja ajudá-los a escapar explodindo os portões das Paredes_Especiaiss das prisão. 
Dado o risco do plano, ele deseja destruir o menor número possível de portões e ainda assim 
garantir que todos os prisioneiros escapem. Quantos portões devem ser explodidos? 
Como poderia um problema mais geral ser resolvido?

Essa é uma segunda interpretação, segundo a qual o espião explode paredes entre celas, de acordo
com o mapa. Sendo assim, algumas paredes, se explodidas, dão acesso a mais de uma cela.
Este programa lida com essa possibilidade.

R: Assim, com esse número de celas, o número mínimo de exposões é 7, mas dessa vez ele varia a
depender de qual cela é a inicial.

*/