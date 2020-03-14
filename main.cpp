#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

const int maxn = 100005;

// the graph
vector<vector<int>> g(maxn);

// level of each node
int level[maxn];

vector<int> dfsTraversal;
vector<int> l;
int h[maxn];

// the segment tree
int st[5 * maxn];

// assigning level to nodes
void leveling(int src) {
    for (int i = 0; i < g[src].size(); i++) {
        int des = g[src][i];
        if (!level[des]) {
            level[des] = level[src] + 1;
            leveling(des);
        }
    }
}

bool visited[maxn];

void dfs(int startVertex) {
    dfsTraversal.push_back(startVertex);
    visited[startVertex] = 1;
    for (int i = 0; i < g[startVertex].size(); i++)
    {
        int childVertex = g[startVertex][i];
        if (!visited[childVertex])
        {
            dfs(childVertex);
            dfsTraversal.push_back(startVertex);
        }
    }
}

// making the array l
void setting_l(int n) {
    for (int i = 0; i < dfsTraversal.size(); i++)
        l.push_back(level[dfsTraversal[i]]);
}

// making the array h
void setting_h(int n) {
    for (int i = 0; i <= n; i++)
        h[i] = -1;
    for (int i = 0; i < dfsTraversal.size(); i++) {
        // if is already stored
        if (h[dfsTraversal[i]] == -1)
            h[dfsTraversal[i]] = i;
    }
}

// Range minimum query to return the index
// of minimum in the subarray L[qs:qe]
int RMQ(int ss, int se, int qs, int qe, int i) {
    if (ss > se)
        return -1;
    
    // out of range
    if (se < qs || qe < ss)
        return -1;
    
    // in the range
    if (qs <= ss && se <= qe)
        return st[i];
    
    int mid = (ss + se) >> 1;
    int st = RMQ(ss, mid, qs, qe, 2 * i + 1);
    int en = RMQ(mid + 1, se, qs, qe, 2 * i + 2);
    
    if (st != -1 && en != -1) {
        if (l[st] < l[en])
            return st;
        return en;
    } else if (st != -1)
        return st;
    else if (en != -1)
        return en;
    
    return -1;
}

// constructs the segment tree
void SegmentTreeConstruction(int ss, int se, int i) {
    if (ss > se)
        return;
    if (ss == se) // leaf
    {
        st[i] = ss;
        return;
    }
    int mid = (ss + se) >> 1;
    
    SegmentTreeConstruction(ss, mid, 2 * i + 1);
    SegmentTreeConstruction(mid + 1, se, 2 * i + 2);
    
    if (l[st[2 * i + 1]] < l[st[2 * i + 2]])
        st[i] = st[2 * i + 1];
    else
        st[i] = st[2 * i + 2];
}

// Funtion to get LCA
int LCA(int u, int v) {
    if (h[u] > h[v])
        swap(u, v);
    return dfsTraversal[RMQ(0, l.size() - 1, h[u], h[v], 0)];
}

int main() {
    int vertexCount;
    std::cin >> vertexCount;
    
    for(int i = 0; i < vertexCount - 1; i++)
    {
        int u, v;
        std::cin >> u >> v;
        
        g[u].push_back(v);
        g[v].push_back(u);
    }
    
    level[1] = 1;
    leveling(1);
    
    dfs(1);
    
    setting_l(vertexCount);
    setting_h(vertexCount);
    
    SegmentTreeConstruction(0, l.size() - 1, 0);

    int c;
    std::cin >> c;
    for(int i = 0; i < c; i++)
    {
        int a, b;
        std::cin >> a >> b;
        std::cout << LCA(a, b) << std::endl;
    }
    
    return 0;
}
