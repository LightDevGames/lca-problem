#include <iostream>
#include <vector>
#include "math.h"

#define MAX 500001

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

std::vector<std::vector<ToDo>> graphAdjencyList(MAX);

std::vector<int> timeInNode(MAX);
std::vector<int> timeOutNode(MAX);
std::vector<int> distanceToRoot(MAX);

std::vector<std::vector<int>> up(MAX);

int logarithm;

int passedNodesCount = 0;
void dfs (int vertex, int parent, int lengthToRoot)
{
    timeInNode[vertex] = passedNodesCount;
    passedNodesCount++;
    up[vertex][0] = parent;
    distanceToRoot[vertex] = lengthToRoot;
    
    for(int i = 1; i <= logarithm; i++)
    {
        up[vertex][i] = up[up[vertex][i-1]][i-1];
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

int getParentNodeIndex(int u, int v)
{
    return (timeInNode[u] <= timeInNode[v]) && (timeOutNode[u] >= timeOutNode[v]);
}

int LCA(int u, int v)
{
    if (getParentNodeIndex(u, v)) return u;
    if (getParentNodeIndex(v, u)) return v;
    
    for (int i = logarithm; i >= 0; i--)
        if (!getParentNodeIndex(up[u][i], v)) u = up[u][i];
    
    return up[u][0];
}

int main()
{
    int nodesCount;
    std::cin >> nodesCount;
    
    for(int i = 0; i < up.size(); i++)
    {
        up[i].resize(20);
    }
    
    for(int i = 0; i < nodesCount - 1; i++)
    {
        int u, v, w;
        std::cin >> u >> v >> w;
        
        ToDo t1(v, w);
        ToDo t2(u, w);
        graphAdjencyList[u].push_back(t1);
        graphAdjencyList[v].push_back(t2);
    }
    
    logarithm = log(nodesCount);
    dfs(0, 0, 0);
    
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
    return 0;
}
