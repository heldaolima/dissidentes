#include <iostream>
#include <list> 

using namespace std;

#define TOT_VERTICES 11 
#define N_CELAS 10
#define SAIDA 10


//este tipo tenta dar conta dos casos em que uma explosão libera mais de uma cela
typedef struct {
    //caso em que no caminho de uma cela (vertice) a outra (dest), libera-se uma terceira (adj)
    int vertice; 
    int dest; // o proximo
    int adj; //o que se destroi quando vai para o proximo  
}celasUnidas;

class Grafo
{
    int V; //numero de vértices
    list<int> *adj; ////ponteiro pra array de listas de adjacência
                    //com a lista de adjacência, o índice significa a cela e cada item da lista nele corresponde às celas adjacentes
    list<celasUnidas> paredes; //as paredes especiais no mapa
public:
    Grafo(int V); 

    void addAresta(int v, int w);
    bool checa_adjacencia(int a, int b);
    bool addcelasUnidas(int v, int d, int a);
    int fuga(int s, bool imprimir);
    void print();
};

Grafo::Grafo(int V) //inicializa o grafo
{
    this->V = V;
    adj = new list<int>[V]; 
}

void Grafo::addAresta(int a, int b) //adiciona aresta entre os vertices a e b
{ //grafo nao-direcionado: vai a para b e de b para a 
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
            cout << converte(*i) << " ";
        cout << endl;
        v++;
    }
}

void imprimeFila(list<int> fila)
{
    cout << "Fila agora: ";
    
    list<int>::iterator i;
    for (i = fila.begin(); i != fila.end(); i++)
        cout << converte(*i) << " ";
    cout << endl;
}

int Grafo::fuga(int ini, bool imprimir)
//algoritmo de busca em largura modificado para mostrar o caminho da cela s até a saída, 
//passando por todas as celas. Segundo esta interpretação, o espião explode paredes
//tem o flag 'imprimir', pois numa das opções não se imprime o percurso
{
    bool *visited = new bool[V]; //array da quantidade de vertices, para representar os visitados
    for (int i = 0; i < V; i++)
        visited[i] = false; //cada indice representa uma sala, como na lista de adjacência;
                            //inicialmente, nenhuma foi visitada, então são todas false
    
    list<int> fila; //fila útil para o algoritmo

    visited[ini] = true; //marca o inicial como visitado 
    fila.push_back(ini); //coloca-o na fila

    list<int>::iterator i; //ponteiro para iterar por listas de inteiro
    list<celasUnidas>::iterator j; //para iterar pela lista de paredes 

    int cont_explosoes = 0; //contador de explosões
    
    while(!fila.empty()) //lista vazia significa fim da busca
    {
        // imprimeFila(fila);
        ini = fila.front(); //vertice "atual"
        
        if (imprimir)
            cout << converte(ini) << " -> "; //imprime o item atual, visitado
        
        fila.pop_front(); //remove o item visitado
        
        //pega todos os vértices adjacentes ao vértice que acabou de ser desenfileirado (s)
        //Se o vértice não foi visitado, então marca como visitado e o
        //coloca na fila, para que se passe por ele mais tarde, ainda que ele já esteja visitado
        for (i = adj[ini].begin(); i != adj[ini].end(); ++i)
        {
            if (!visited[*i])
            {
                visited[*i] = true; //marca o vértice

                //sendo 'ini' a cela atual e '*i' um seu adjacente, entre os quais há uma parede, 
                //pode ser que no caminho de ini para *i a explosão acabe liberando outra cela
                //entao procura na lista de paredes unidas o vertice ini e vejo se ele tem *i como destino
                //se sim, entao a cela adj (definida no tipo) deve ser marcada como liberada, sem que o contador se some
                //pois ocorreu apenas uma explosão 
                for(j = paredes.begin(); j != paredes.end(); j++)
                {
                    if (j->vertice == ini && j->dest == *i) 
                    {
                        // cout << "\nvertice: " << j->vertice << " dest: " << j->dest << " adj: " << j->adj << endl; //verifica-se o exposto acima
                        if (!visited[j->adj])
                        {
                            visited[j->adj] = true; //a cela que se liberou 'por acidente' é marcada como visitada
                                                    //mas o contador de explosões não incrementa
                            fila.push_back(j->adj); //ela é inserida na fila, para representar na hora de imprimir 
                        }
                    }
                }

                cont_explosoes++;
                if (*i != SAIDA) //não colocamos a saída na fila, pois parece fazer mais sentido que ela seja o último destino; 
                                 //apesar disso, consideramos que no percurso uma parede para ela foi destruída
                                 //e que o grupo retornará para ela assim que passar por todas as celas
                    fila.push_back(*i);
            }
        }
    }
    if (imprimir) cout << "saída\n"; //sendo assim, a saída sempre é impressa por último
    return cont_explosoes; //retorna o número de explosões
}

bool Grafo::checa_adjacencia(int a, int b) //para a hora de inserir celas unidas
//checa se os vertices a e b são adjacentes
{
    list<int>::iterator i;

    for (i = adj[a].begin(); i != adj[a].end(); i++) //procuro b na lista de adjacência do vértice a
    {
        if (*i == b) return true; //retorna true se forem
    }
    return false; //retorna false se não forem
}

bool Grafo::addcelasUnidas(int a, int dest, int adj)
//na explosão de 'a' para 'dest', libera-se 'adj'
{
    if (this->checa_adjacencia(a, dest)) //só pode existir uma parede unida entre a e dest se eles forem adjacentes
    {
        celasUnidas nova;
        
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
    //construção do grafo:
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

    //informar quais são os casos de "celas unidas" no mapa
    g.addcelasUnidas(1, 3, 4);
    g.addcelasUnidas(1, 4, 3);
    g.addcelasUnidas(1, 5, 6);
    g.addcelasUnidas(1, 5, 4);
    g.addcelasUnidas(3, 4, 9);
    g.addcelasUnidas(3, 9, 4);
    g.addcelasUnidas(4, 1, 3);
    g.addcelasUnidas(4, 3, 9);
    g.addcelasUnidas(4, 5, 6);
    g.addcelasUnidas(4, 6, 5);
    g.addcelasUnidas(5, 1, 4);
    g.addcelasUnidas(5, 1, 6);
    g.addcelasUnidas(6, 4, 1);
    g.addcelasUnidas(6, 4, 5);

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
    case 1: //mostrar o processo de uma cela até a saída, passando por todas as outras,
            //e quantas explosões foram necessárias
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
        c = g.fuga(inicio, true); //imprimir vai com 'true', para mostrar o caminho entre as celas

        cout << "Número de paredes explodidas: " << c << endl; //numero de explosões a partir da cela escolhida
        break;

    case 2: //para encontrar o menor resultado, partindo de cada uma das celas
        int resultados[N_CELAS]; //array para armazenar os resultados, o número de explosões ao partir de cada cela
                                 //cada índice corresponde a uma cela
        int menor, i;
        for (i = 0; i < N_CELAS; i++)
        {
            c = g.fuga(i, false); //vai com 'false' em imprimir, pois aqui não interessam os caminhos
            if (i == 0)     //definindo o menor resultado
                menor = c;
            else
                if (c < menor)
                    menor = c;
            resultados[i] = c;
        }
        cout << "Menor número de paredes explodidas: " << menor << endl;
        cout << "Esse foi o resultado obtido começando das celas: ";
        
        for (i = 0; i < N_CELAS; i++)
        {
            if (resultados[i] == menor) //mostro apenas os valores correspondentes ao menor encontrado
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

Essa é uma interpretação segundo a qual o espião explode paredes entre celas, de acordo
com o mapa. Sendo assim, algumas paredes, se explodidas, dão acesso a mais de uma cela.
Este programa lida com essa possibilidade.

R: Assim, com essa configurações, o número mínimo de exposões é 7, segundo o nosso algoritmo; este número varia a depender
de qual é a cela de partida

*/