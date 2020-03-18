#include <iostream>
#include <vector>
#include "math.h"

#define MAX 500001

using namespace std;

vector<pair<int,int> > g[MAX];

int timeInNode[MAX], timeOutNode[MAX], dist[MAX];
int up[MAX][20];

int logarifm;

int passedNodesCount = 0;
void dfs (int v, int p = 0, int lengthToRoot = 0)
{
    int i, to;
    timeInNode[v] = passedNodesCount++;
    up[v][0] = p;
    dist[v] = lengthToRoot;
    
    for(i = 1; i <= logarifm; i++)
        up[v][i] = up[up[v][i-1]][i-1];
    
    for(i = 0; i < g[v].size(); i++)
    {
        to = g[v][i].first;
        if (to != p) dfs (to, v, lengthToRoot + g[v][i].second);
    }
    
    timeOutNode[v] = passedNodesCount++;
}

int Parent(int a, int b)
{
    return (timeInNode[a] <= timeInNode[b]) && (timeOutNode[a] >= timeOutNode[b]);
}

int LCA (int a, int b)
{
    if (Parent(a, b)) return a;
    if (Parent(b, a)) return b;
    
    for (int i = logarifm; i >= 0; i--)
        if (!Parent(up[a][i], b)) a = up[a][i];
    
    return up[a][0];
}

int main()
{
    int n;
    cin >> n;
    
    logarifm = log(n);
    
    for(int i = 0; i < n - 1; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        
        g[u].push_back(make_pair(v,w));
        g[v].push_back(make_pair(u,w));
    }
    
    dfs(0);
    
    int questionsCount;
    cin >> questionsCount;
    for(int i = 0; i < questionsCount; i++)
    {
        int u, v;
        cin >> u >> v;
        
        int lca = LCA(u, v);
        int res = dist[u] - dist[lca] + dist[v] - dist[lca];
        cout << res << endl;
    }
    return 0;
}
