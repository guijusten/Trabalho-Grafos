/**************************************************************************************************
 * Implementation of the TAD Graph
**************************************************************************************************/

#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include "Node.h"
#include <fstream>
#include <stack>
#include <list>

using namespace std;

class Graph{

    //Atributes
    private:
        int order;
        int number_edges;
        bool directed;
        bool weighted_edge;
        bool weighted_node;
        Node* first_node;
        Node* last_node;

    public:
        //Constructor
        Graph(int order, bool directed, bool weighted_edge, bool weighted_node);
        //Destructor
        ~Graph();
        //Getters
        int getOrder();
        int getNumberEdges();
        bool getDirected();
        bool getWeightedEdge();
        bool getWeightedNode();
        Node* getFirstNode();
        Node* getLastNode();
        // Setters
        void setFirstNode(Node* n);
        void setLastNode(Node* n);
        //Other methods
        void insertNode(int id);
        void insertNodeIntoInstance(int id, int x, int y);
        void insertEdge(int id, int target_id, float weight);
        void insertEdgeIntoInstance(int id, int target_id, float weight);
        void removeNode(int id);
        bool searchNode(int id);
        Node* getNode(int id);
        void incrementNumberEdges();
        void insertEdgesIntoInstance();

        //methods phase2
        float greed();
        float greedRandom(float alpha, int numIter, int seed);
        float greedRandomReactive(int numIter, int seed);
    private:
        //Auxiliar methods

};

#endif // GRAPH_H_INCLUDED
