#include <iostream>
#include <vector>
#include "math.h"

#define MAX 500001

std::vector<std::pair<int,int> > g[MAX];

int timeInNode[MAX], timeOutNode[MAX], distanceToRoot[MAX];
int up[MAX][20];

int logarifm;

int passedNodesCount = 0;
void dfs (int v, int p = 0, int lengthToRoot = 0)
{
    int i, to;
    timeInNode[v] = passedNodesCount++;
    up[v][0] = p;
    distanceToRoot[v] = lengthToRoot;
    
    for(i = 1; i <= logarifm; i++)
        up[v][i] = up[up[v][i-1]][i-1];
    
    for(i = 0; i < g[v].size(); i++)
    {
        to = g[v][i].first;
        if (to != p) dfs (to, v, lengthToRoot + g[v][i].second);
    }
    
    timeOutNode[v] = passedNodesCount++;
}

int getParentNodeIndex(int u, int v)
{
    return (timeInNode[u] <= timeInNode[v]) && (timeOutNode[u] >= timeOutNode[v]);
}

int LCA(int u, int v)
{
    if (getParentNodeIndex(u, v)) return u;
    if (getParentNodeIndex(v, u)) return v;
    
    for (int i = logarifm; i >= 0; i--)
        if (!getParentNodeIndex(up[u][i], v)) u = up[u][i];
    
    return up[u][0];
}

int main()
{
    int nodesCount;
    std::cin >> nodesCount;
    
    for(int i = 0; i < nodesCount - 1; i++)
    {
        int u, v, w;
        std::cin >> u >> v >> w;
        
        g[u].push_back(std::make_pair(v,w));
        g[v].push_back(std::make_pair(u,w));
    }
    
    logarifm = log(nodesCount);
    dfs(0);
    
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
