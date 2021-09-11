#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <utility>
#include <tuple>
#include <iomanip>
#include <stdlib.h>
#include <chrono>
#include "Graph.h"
#include "Node.h"

using namespace std;

Graph* leituraInstancia(ifstream& input_file, int directed, int weightedEdge, int weightedNode){

    //Variáveis para auxiliar na criação dos nós no Grafo
    int idNode, x, y;
    int order;

    //Pegando a ordem do grafo
    input_file >> order;

    //Criando objeto grafo
    Graph* graph = new Graph(order, directed, weightedEdge, weightedNode);

    //Leitura de arquivo
    while(input_file >> idNode >> x >> y)
        graph->insertNodeIntoInstance(idNode, x, y);

    graph->insertEdgesIntoInstance();
    return graph;
}

void selecionar(int selecao, Graph* graph){

    switch (selecao) {
        case 0:{
            cout << "Saindo do programa..." << endl;
            break;
        }

        case 1:{
            int weightSum = graph->greed();
            cout << "A soma dos custos das arestas da AGM com restricao de grau atraves do algoritmo guloso foi " << weightSum << endl << endl;
            break;
        }

        case 2:{
            float alpha;
            int numIter;
            int seed;
            cout << "Digite o valor de alpha, depois, o numero de iteracoes, depois a seed de randomizacao" << endl;
            cin >> alpha;
            cin >> numIter;
            cin >> seed;
            int weightSum = graph->greedRandom(alpha, numIter, seed);
            cout << "A soma dos custos das arestas da AGM com restricao de grau atraves do algoritmo guloso randomizado foi " << weightSum << endl << endl;
            break;
        }

        case 3:{
            int numIter;
            int seed;
            cout << "Digite o numero de iteracoes, depois a seed de randomizacao" << endl;
            cin >> numIter;
            cin >> seed;
            int weightSum = graph->greedRandomReactive(numIter, seed);
            cout << "A soma dos custos das arestas da AGM com restricao de grau atraves do algoritmo guloso randomizado reativo foi " << weightSum << endl << endl;
            break;
        }

        default:{
            cout << "Nenhuma funcionalidade atende a esse comando" << endl;
        }
    }
}

int menu(Graph* graph){

    int selecao = 1;

    while(selecao != 0){
        cout << "MENU" << endl;
        cout << "----" << endl;
        cout << "[1] Algoritmo Guloso" << endl;
        cout << "[2] Algoritmo Guloso Randomizado " << endl;
        cout << "[3] Algoritmo Guloso Randomizado Reativo" << endl;
        cout << "[0] Sair" << endl;
        cin >> selecao;
        selecionar(selecao, graph);
    }
    return 0;
}

int main(int argc, char const *argv[]) {
    //Verificando se todos os parâmetros do programa foram entrados
    if(argc == 2){
        string program_name(argv[0]);
        string input_file_name(argv[1]);

        //Abrindo arquivo de entrada
        ifstream input_file;
        input_file.open(argv[1], ios::in);

        Graph* graph;

        if(input_file){
            graph = leituraInstancia(input_file, 0, 1, 0);
        } else{
            cout << "Unable to open " << argv[1];
        }

        menu(graph);

        //Fechando arquivo de entrada
        input_file.close();

        return 0;
    } else{
        cout << "Os parametros passados via linha de comando nao estao de acordo com a especificacao" << endl;
    }
}
