#include <iostream>
#include <vector>
#include "math.h"

struct ToDo
{
    int adjacentVertex;
    int weight;
    
    ToDo(int vertex, int w)
    {
        adjacentVertex = vertex;
        weight = w;
    }
};

std::vector<std::vector<ToDo>> graphAdjencyList;
std::vector<std::vector<int>> logarithmUp;
std::vector<int> distanceToRoot;
std::vector<int> timeInNode;
std::vector<int> timeOutNode;

int nodesCount;
int logarithm = 1;

int passedNodesCount = 0;
void dfs (int vertex, int parent, int lengthToRoot)
{
    timeInNode[vertex] = passedNodesCount;
    passedNodesCount++;
    logarithmUp[vertex][0] = parent;
    distanceToRoot[vertex] = lengthToRoot;
    
    for(int i = 1; i <= logarithm; i++)
    {
        logarithmUp[vertex][i] = logarithmUp[logarithmUp[vertex][i-1]][i-1];
    }
    
    for(int i = 0; i < graphAdjencyList[vertex].size(); i++)
    {
        int adjencyVertex = graphAdjencyList[vertex][i].adjacentVertex;
        if (adjencyVertex != parent)
        {
            dfs(adjencyVertex, vertex, lengthToRoot + graphAdjencyList[vertex][i].weight);
        }
    }
    
    timeOutNode[vertex] = passedNodesCount;
    passedNodesCount++;
}

int isParent(int parentVertex, int vertex)
{
    return (timeInNode[parentVertex] <= timeInNode[vertex]) && (timeOutNode[parentVertex] >= timeOutNode[vertex]);
}

int LCA(int u, int v)
{
    if (isParent(u, v)) return u;
    if (isParent(v, u)) return v;
    
    for (int i = logarithm; i >= 0; i--)
    {
        if (!isParent(logarithmUp[u][i], v))
        {
            u = logarithmUp[u][i];
        }
    }
    
    return logarithmUp[u][0];
}

void resizeVectors()
{
    while ((1 << logarithm) <= nodesCount)
    {
        logarithm++;
    }

    graphAdjencyList.resize(nodesCount);
    distanceToRoot.resize(nodesCount);
    timeInNode.resize(nodesCount * 2);
    timeOutNode.resize(nodesCount * 2);
    
    logarithmUp.resize(nodesCount + 2);
    for(int i = 0; i < logarithmUp.size(); i++)
    {
        logarithmUp[i].resize(logarithm + 2);
    }
}

void readData()
{
    for(int i = 0; i < nodesCount - 1; i++)
    {
        int u, v, w;
        std::cin >> u >> v >> w;
        
        ToDo t1(v, w);
        ToDo t2(u, w);
        graphAdjencyList[u].push_back(t1);
        graphAdjencyList[v].push_back(t2);
    }
}

void answearQuestions()
{
    int questionsCount;
    std::cin >> questionsCount;
    for(int i = 0; i < questionsCount; i++)
    {
        int u, v;
        std::cin >> u >> v;
        
        int lca = LCA(u, v);
        int res = distanceToRoot[u] - distanceToRoot[lca] + distanceToRoot[v] - distanceToRoot[lca];
        std::cout << res << std::endl;
    }
}

int main()
{
    std::cin >> nodesCount;
    
    resizeVectors();
    readData();
    dfs(0, 0, 0);
    answearQuestions();
    
    return 0;
}
