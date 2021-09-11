#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <stack>
#include <queue>
#include <list>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <float.h>
#include <iomanip>
#include <vector>
#include <chrono>

using namespace std;

/**************************************************************************************************
 * Defining the Graph's methods
**************************************************************************************************/

// Constructor
Graph::Graph(int order, bool directed, bool weighted_edge, bool weighted_node)
{

    this->order = order;
    this->directed = directed;
    this->weighted_edge = weighted_edge;
    this->weighted_node = weighted_node;
    this->first_node = this->last_node = nullptr;
    this->number_edges = 0;
}

// Destructor
Graph::~Graph()
{
    Node *next_node = this->first_node;

    while (next_node != nullptr)
    {
        next_node->removeAllEdges();
        Node *aux_node = next_node->getNextNode();
        delete next_node;
        next_node = aux_node;
    }
}

// Getters
int Graph::getOrder()
{
    return this->order;
}
int Graph::getNumberEdges()
{
    return this->number_edges;
}
//Function that verifies if the graph is directed
bool Graph::getDirected()
{
    return this->directed;
}
//Function that verifies if the graph is weighted at the edges
bool Graph::getWeightedEdge()
{
    return this->weighted_edge;
}
Node *Graph::getFirstNode()
{
    return this->first_node;
}
Node *Graph::getLastNode()
{
    return this->last_node;
}
bool Graph::getWeightedNode()
{
    return this->weighted_node;
}
// Setters
void Graph::setFirstNode(Node* n)
{
    this->first_node = n;
}
void Graph::setLastNode(Node* n)
{
    this->last_node = n;
}

//Function that verifies if the graph is weighted at the nodes


// Other methods
/*
    The outdegree attribute of nodes is used as a counter for the number of edges in the graph.
    This allows the correct updating of the numbers of edges in the graph being directed or not.
*/

void Graph::incrementNumberEdges()
{
    this->number_edges++;
}

void Graph::insertNode(int id)
{
    // Vertice jÃ¡ adicionado ao grafo
    if(searchNode(id)) return;

    Node *n = new Node(id, 0, 0);
    n->setNextNode(getFirstNode());

    if(getFirstNode() == nullptr)
    {
        setFirstNode(n);
        setLastNode(n);
    }
    else
    {
        setFirstNode(n);
    }
}

void Graph::insertNodeIntoInstance(int id, int x, int y)
{
    // Vertice ja adicionado ao grafo
    if(searchNode(id)) return;

    Node *n = new Node(id, x, y);
    n->setNextNode(getFirstNode());

    if(getFirstNode() == nullptr)
    {
        setFirstNode(n);
        setLastNode(n);
    }
    else
    {
        setFirstNode(n);
    }
}

void Graph::insertEdge(int id, int target_id, float weight)
{
    insertNode(id);
    insertNode(target_id);
    Node *next = getFirstNode();
    Node *n = getNode(target_id);

    while(next != nullptr)
    {
        if(next->getId() == id)
        {
            next->insertEdge(target_id, weight);

            if(!directed)
                n->insertEdge(id, weight);
            else
            {
                next->incrementOutDegree();
                n->incrementInDegree();
            }

            break;
        }

        next = next->getNextNode();
    }

    incrementNumberEdges();
}

// Esta função insere a aresta em ambas as extremidades da aresta
void Graph::insertEdgeIntoInstance(int id, int target_id, float weight)
{
    Node *next = getNode(id);

    while(next != nullptr)
    {
        if(next->getId() == id)
        {
            next->insertEdge(target_id, weight);
            break;
        }
        next = next->getNextNode();
    }
    incrementNumberEdges();
}

// Função para inserir as arestas ponderadas na instância passada
void Graph::insertEdgesIntoInstance()
{
    int cont = 0;
    int weight = 0;
    Node* currentNode = this->getFirstNode();
    Node* nextNode;
    int x1, x2, y1, y2, distI;
    float distF, var;
    while(cont < this->order - 1)
    {
        nextNode = currentNode->getNextNode();
        while(nextNode != nullptr)
        {
            // calculo do peso da aresta
            x1 = currentNode->getX();
            x2 = nextNode->getX();
            y1 = currentNode->getY();
            y2 = nextNode->getY();

            distF = sqrt((((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2))));

            distI = (int)distF;
            var = distF - distI;
            if(var >= 0.5)
                weight = distI + 1;
            else
                weight = distI;

            this->insertEdgeIntoInstance(currentNode->getId(), nextNode->getId(), weight);
            nextNode = nextNode->getNextNode();
        }
        cont++;
        currentNode = currentNode->getNextNode();
    }
}

bool Graph::searchNode(int id)
{
    if(getFirstNode() == nullptr) return false;

    Node* n = getFirstNode();
    while(n->getId() != id && n->getNextNode() != nullptr)
    {
        n = n->getNextNode();
    }
    if(n->getId() == id) return true;
    return false;
}

Node *Graph::getNode(int id)
{
    Node* n = getFirstNode();
    while(n->getId() != id && n->getNextNode() != nullptr)
    {
        n = n->getNextNode();
    }
    return n;
}

int chooseAlpha(float prob[])
{
    if(prob[0] == 0.0)
        return rand() % 5;
    while(true)
    {
        for(int i = 0; i < 5; i++)
        {
            if(prob[i] > rand() - 0.03)
            {
                return i;
            }
        }
    }
}

// Função passaa para o sort para que a ordenação seja feita em função do peso crescentemente
bool compareWeight(Edge* e1, Edge* e2)
{
    return (e1->getWeight() < e2->getWeight());
}

float Graph::greed()
{
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();

    list<Edge*> sol; // Lista com as arestas que entram na solucao
    list<Edge*> candidates; // Lista com as arestas candidatas
    int degreeOnTree[this->order] = {0}; // Lista com o grau de cada vértice na árvore, todos começam com zero
    bool coveredVertices[this->order] = {false}; // Lista booleana dizendo quais vértices já fazem parte da solução
    float weightSum = 0; // Soma dos pesos das arestas da solução
    int solSize = 0; // Variavel que guarda tamanho a solucao (sendo usada pois ao comparar sol.size() com um int dava erro)
    bool firstIteration = true; // Variavel auxiliar para a primeira iteracao

    for(Node* c = this->getFirstNode(); c != nullptr; c = c->getNextNode())  // Laço para inserir todas as arestas candidatas
    {
        Edge* e = c->getFirstEdge();
        while(e != nullptr)
        {
            candidates.push_front(e);
            e = e->getNextEdge();
        }
    }

    // Ordenando lista de candidatos em ordem crescente de peso
    // A heurísitca aqui é: quem tem menor peso é escolhido antes
    candidates.sort(compareWeight);

    // Adicionar elementos na solução até que a solução tenha n-1 arestas
    // Ou até que não tenham candidatos restantes (essa restrição nunca é violada no caso deste algoritmo)
    while(solSize < this->order - 1 && candidates.size() > 0)
    {
        Edge* e = candidates.front(); // Pegando melhor elemento segundo a função objetivo adotada
        bool notChosen = true;
        int cont = 0;

        while(notChosen)
        {
            if(!(!coveredVertices[e->getSourceId() - 1] && !coveredVertices[e->getTargetId() - 1]) || firstIteration)
            {
                sol.push_back(e); // Adicionando-o na lista de soluções
                solSize += 1;
                weightSum += e->getWeight();
                candidates.remove(e); // Retirando da lista de candidatos
                notChosen = false;
                firstIteration = false;
            }
            else
            {
                list<Edge*>::const_iterator it = candidates.begin();
                advance(it, cont);
                e = *it;
                cont++;
            }
        }

        // Atualizanoo grau na árvore e quais vértices já foram cobertos
        degreeOnTree[e->getTargetId() - 1] += 1;
        degreeOnTree[e->getSourceId() - 1] += 1;
        coveredVertices[e->getTargetId() - 1] = true;
        coveredVertices[e->getSourceId() - 1] = true;

        // Atualizando a lista de candidatos
        for(int i = 0; i < this->order; i++)  // Laço para excluir arestas com extremidades em nós com grau 3
        {
            if(degreeOnTree[i] == 3)
            {
                for(auto x: candidates)
                {
                    if(x->getTargetId() == i+1 || x->getSourceId() == i+1)
                    {
                        candidates.remove(x);
                    }
                }
            }
        }

        for(auto x: candidates)  // Laço para eliminar arestas em que ambas extremidades incidem em vértices contidos na solução
        {
            if(coveredVertices[x->getTargetId() - 1] && coveredVertices[x->getSourceId() - 1])
            {
                candidates.remove(x);
            }
        }
        // Término da atualização da lista de candidatos

    }

    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << "Tempo gasto pelo algoritmo foi de " << chrono::duration_cast<chrono::nanoseconds> (end - begin).count() << "ns" << endl;

    return weightSum;
}

float Graph::greedRandom(float alpha, int numIter, int seed)
{
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    list<Edge*> sol; // Lista com as arestas que entram na solucao
    list<Edge*> solBest; // Arestas que compoem a melhor solucao
    list<Edge*> candidates; // Lista com as arestas candidatas
    int degreeOnTree[this->order] = {0}; // Lista com o grau de cada vértice na árvore, todos começam com zero
    bool coveredVertices[this->order] = {false}; // Lista booleana dizendo quais vértices já fazem parte da solução
    float weightSum = 0; // Soma dos pesos das arestas da solução
    float weightSumBest = 0; // Soma dos pesos das arestas da melhor solucao
    bool firstIteration = true; // Variavel auxiliar para a primeira iteracao
    int solSize = 0; // Variavel que guarda tamanho a solucao (sendo usada pois ao comparar sol.size() com um int dava erro)
    Edge* bestPossibleEdges[(int)(this->order * alpha)]; // Vetor para armazenar os candidatos que podem ser escolhidos a cada iteracao

    for(Node* c = this->getFirstNode(); c != nullptr; c = c->getNextNode())  // Laço para inserir todas as arestas candidatas
    {
        Edge* e = c->getFirstEdge();
        while(e != nullptr)
        {
            candidates.push_front(e);
            e = e->getNextEdge();
        }
    }

    // Ordenando lista de candidatos em ordem crescente de peso
    // A heurísitca aqui é: quem tem menor peso é escolhido antes
    candidates.sort(compareWeight);

    int contIter = 0;
    Edge* e;

    while(contIter < numIter) // Enquanto o número de iterações especificado não for atendido
    {

        // Adicionar elementos na solução até que a solução tenha n-1 arestas
        // Ou até que não tenham candidatos restantes (essa restrição nunca é violada no caso deste algoritmo)
        while(solSize < this->order - 1 && candidates.size() > 0)
        {
            bool notChosen = true;
            int cont = 0;
            int contBestEdges = 0;

            while(notChosen)
            {
                list<Edge*>::const_iterator it = candidates.begin(); // Pegando melhor elemento segundo a função objetivo adotada
                advance(it, contBestEdges + cont);
                e = *it;
                if(!(!coveredVertices[e->getSourceId() - 1] && !coveredVertices[e->getTargetId() - 1]) || firstIteration)
                {
                    if(contBestEdges < (int)(this->order * alpha))  // Se o vetor de melhores arestas nao estiver cheio, continuar a preenche-lo
                    {
                        bestPossibleEdges[contBestEdges] = e;
                        contBestEdges++;
                    }
                    else
                    {
                        srand(seed);
                        e = bestPossibleEdges[rand() % (int)(this->order * alpha)]; // Pegando randomicamente uma aresta dentre as arestas no range de alpha
                        sol.push_back(e); // Adicionando-o na lista de soluções
                        solSize += 1;
                        weightSum += e->getWeight();
                        candidates.remove(e); // Retirando da lista de candidatos
                        notChosen = false;
                        firstIteration = false;

                        // Atualizanoo grau na árvore e quais vértices já foram cobertos
                        degreeOnTree[e->getTargetId() - 1] += 1;
                        degreeOnTree[e->getSourceId() - 1] += 1;
                        coveredVertices[e->getTargetId() - 1] = true;
                        coveredVertices[e->getSourceId() - 1] = true;

                        // Atualizando a lista de candidatos
                        for(int i = 0; i < this->order; i++)  // Laço para excluir arestas com extremidades em nós com grau 3
                        {
                            if(degreeOnTree[i] == 3)
                            {
                                for(auto x: candidates)
                                {
                                    if(x->getTargetId() == i+1 || x->getSourceId() == i+1)
                                    {
                                        candidates.remove(x);
                                    }
                                }
                            }
                        }

                        for(auto x: candidates)  // Laço para eliminar arestas em que ambas extremidades incidem em vértices contidos na solução
                        {
                            if(coveredVertices[x->getTargetId() - 1] && coveredVertices[x->getSourceId() - 1])
                            {
                                candidates.remove(x);
                            }
                        }
                        // Término da atualização da lista de candidatos
                    }

                }
                else
                {
                    cont++;
                }
            }

        }

        if(weightSum < weightSumBest || weightSumBest == 0)  // Atualizando melhor solucao
        {
            solBest = sol;
            weightSumBest = weightSum;
        }
        contIter++; // Incrementando contador de iteracoes
        firstIteration = true;
    }

    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << "Tempo gasto pelo algoritmo foi de " << chrono::duration_cast<chrono::nanoseconds> (end - begin).count() << "ns" << endl;

    return weightSumBest;
}

float Graph::greedRandomReactive(int numIter, int seed)
{
    list<Edge*> sol; // Lista com as arestas que entram na solucao
    list<Edge*> solBest; // Arestas que compoem a melhor solucao
    list<Edge*> candidates; // Lista com as arestas candidatas
    int degreeOnTree[this->order] = {0}; // Lista com o grau de cada vértice na árvore, todos começam com zero
    bool coveredVertices[this->order] = {false}; // Lista booleana dizendo quais vértices já fazem parte da solução
    float weightSum = 0; // Soma dos pesos das arestas da solução
    float weightSumBest = 0; // Soma dos pesos das arestas da melhor solucao
    bool firstIteration = true; // Variavel auxiliar para a primeira iteracao
    int solSize = 0; // Variavel que guarda tamanho a solucao (sendo usada pois ao comparar sol.size() com um int dava erro)
    float alpha[5];
    alpha[0] = 0.1;
    alpha[1] = 0.15;
    alpha[2] = 0.2;
    alpha[3] = 0.25;
    alpha[4] = 0.3;

    for(Node* c = this->getFirstNode(); c != nullptr; c = c->getNextNode())  // Laço para inserir todas as arestas candidatas
    {
        Edge* e = c->getFirstEdge();
        while(e != nullptr)
        {
            candidates.push_front(e);
            e = e->getNextEdge();
        }
    }

    // Ordenando lista de candidatos em ordem crescente de peso
    // A heurísitca aqui é: quem tem menor peso é escolhido antes
    candidates.sort(compareWeight);

    int contIter = 0;
    Edge* e;
    float alphaProbabilities[5];
    int alphaAverage[5];

    for(int i = 0; i < 5; i++)  // Zerando as médias e as probabilidades
    {
        alphaProbabilities[i] = 0.0;
        alphaAverage[i] = 0;
    }

    srand(seed);

    while(contIter < numIter) // Enquanto o número de iterações especificado não for atendido
    {
        int alphaIndex;
        if(firstIteration)  // Escolhendo alpha baseado nas probabilidades
            alphaIndex = rand() % 5;
        else
            alphaIndex = chooseAlpha(alphaProbabilities);

        Edge* bestPossibleEdges[(int)(this->order * alpha[alphaIndex])];

        // Adicionar elementos na solução até que a solução tenha n-1 arestas
        // Ou até que não tenham candidatos restantes (essa restrição nunca é violada no caso deste algoritmo)
        while(solSize < this->order - 1 && candidates.size() > 0)
        {
            bool notChosen = true;
            int cont = 0;
            int contBestEdges = 0;

            while(notChosen)
            {
                list<Edge*>::const_iterator it = candidates.begin(); // Pegando melhor elemento segundo a função objetivo adotada
                advance(it, contBestEdges + cont);
                e = *it;
                if(!(!coveredVertices[e->getSourceId() - 1] && !coveredVertices[e->getTargetId() - 1]) || firstIteration)
                {
                    if(contBestEdges < (int)(this->order * alpha[alphaIndex]))  // Se o vetor de melhores arestas nao estiver cheio, continuar a preenche-lo
                    {
                        bestPossibleEdges[contBestEdges] = e;
                        contBestEdges++;
                    }
                    else
                    {
                        e = bestPossibleEdges[rand() % (int)(this->order * alpha[alphaIndex])]; // Pegando randomicamente uma aresta dentre as arestas no range de alpha
                        sol.push_back(e); // Adicionando-o na lista de soluções
                        solSize += 1;
                        weightSum += e->getWeight();
                        candidates.remove(e); // Retirando da lista de candidatos
                        notChosen = false;
                        firstIteration = false;

                        // Atualizanoo grau na árvore e quais vértices já foram cobertos
                        degreeOnTree[e->getTargetId() - 1] += 1;
                        degreeOnTree[e->getSourceId() - 1] += 1;
                        coveredVertices[e->getTargetId() - 1] = true;
                        coveredVertices[e->getSourceId() - 1] = true;

                        // Atualizando a lista de candidatos
                        for(int i = 0; i < this->order; i++)  // Laço para excluir arestas com extremidades em nós com grau 3
                        {
                            if(degreeOnTree[i] == 3)
                            {
                                for(auto x: candidates)
                                {
                                    if(x->getTargetId() == i+1 || x->getSourceId() == i+1)
                                    {
                                        candidates.remove(x);
                                    }
                                }
                            }
                        }

                        for(auto x: candidates)  // Laço para eliminar arestas em que ambas extremidades incidem em vértices contidos na solução
                        {
                            if(coveredVertices[x->getTargetId() - 1] && coveredVertices[x->getSourceId() - 1])
                            {
                                candidates.remove(x);
                            }
                        }
                        // Término da atualização da lista de candidatos
                    }

                }
                else
                {
                    cont++;
                }
            }

        }

        // Reacalcular probabilidades
        alphaAverage[alphaIndex] = weightSum;

        float sumOfProbabilities = 0;
        for(int i = 0; i < 5; i++)
        {
            alphaProbabilities[i] = (weightSumBest / alphaAverage[i]);
            sumOfProbabilities += alphaProbabilities[i];
        }
        for(int i = 0; i < 5; i++)
        {
            alphaProbabilities[i] = alphaProbabilities[i] / sumOfProbabilities;
        }

        // Atualizando melhor solucao
        if(weightSum < weightSumBest || weightSumBest == 0)
        {
            solBest = sol;
            weightSumBest = weightSum;
        }
        contIter++; // Incrementando contador de iteracoes
    }

    return weightSumBest;
}
