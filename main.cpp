#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

const int maxn = 100005;

// the graph
vector<vector<int>> graph(maxn);

// level of each node
int nodeLevel[maxn];

vector<int> dfsTraversal;
vector<int> l;
int h[maxn];

int segmentTree[5 * maxn];

void updateNodesLevel(int root)
{
    for (int i = 0; i < graph[root].size(); i++)
    {
        int child = graph[root][i];
        if (!nodeLevel[child])
        {
            nodeLevel[child] = nodeLevel[root] + 1;
            updateNodesLevel(child);
        }
    }
}

bool visitedNodesForDfs[maxn];

void dfs(int root) {
    dfsTraversal.push_back(root);
    visitedNodesForDfs[root] = 1;
    for (int i = 0; i < graph[root].size(); i++)
    {
        int childVertex = graph[root][i];
        if (!visitedNodesForDfs[childVertex])
        {
            dfs(childVertex);
            dfsTraversal.push_back(root);
        }
    }
}

// making the array l
void setting_l(int n) {
    for (int i = 0; i < dfsTraversal.size(); i++)
        l.push_back(nodeLevel[dfsTraversal[i]]);
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
        return segmentTree[i];
    
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
        segmentTree[i] = ss;
        return;
    }
    int mid = (ss + se) >> 1;
    
    SegmentTreeConstruction(ss, mid, 2 * i + 1);
    SegmentTreeConstruction(mid + 1, se, 2 * i + 2);
    
    if (l[segmentTree[2 * i + 1]] < l[segmentTree[2 * i + 2]])
        segmentTree[i] = segmentTree[2 * i + 1];
    else
        segmentTree[i] = segmentTree[2 * i + 2];
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
        
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
    
    nodeLevel[1] = 1;
    updateNodesLevel(1);
    
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
