#include <iostream>
#include <vector>

#define MAX 500001

using namespace std;

vector<pair<int,int> > g[MAX];

int d[MAX], f[MAX], dist[MAX];
int up[MAX][20];

int l;
int timer = 0;

void dfs (int v, int p = 0, int len = 0)
{
    int i, to;
    d[v] = timer++;
    up[v][0] = p; dist[v] = len;
    
    for(i = 1; i <= l; i++)
        up[v][i] = up[up[v][i-1]][i-1];
    
    for(i = 0; i < g[v].size(); i++)
    {
        to = g[v][i].first;
        if (to != p) dfs (to, v, len + g[v][i].second);
    }
    
    f[v] = timer++;
}

int Parent(int a, int b)
{
    return (d[a] <= d[b]) && (f[a] >= f[b]);
}

int LCA (int a, int b)
{
    if (Parent(a, b)) return a;
    if (Parent(b, a)) return b;
    
    for (int i = l; i >= 0; i--)
        if (!Parent(up[a][i], b)) a = up[a][i];
    
    return up[a][0];
}

int main()
{
    int n;
    cin >> n;
    
    l = 1;
    while ((1 << l) <= n)
    {
        l++;
    }
    
    for(int i = 0; i < n - 1; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        
        g[u].push_back(make_pair(v,w));
        g[v].push_back(make_pair(u,w));
    }
    
    dfs(0);
    
    int m;
    cin >> m;
    for(int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;
        
        int lca = LCA(u, v);
        int res = dist[u] - dist[lca] + dist[v] - dist[lca];
        cout << res << endl;
    }
    return 0;
}
