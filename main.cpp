#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <climits>

int vertexCount;

std::vector<std::vector<int>> graph;
std::vector<int> nodeLevel;
std::vector<int> dfsTraversal;
std::vector<int> l;
std::vector<int> h;
std::vector<int> segmentTree;
std::vector<bool> visitedNodesForDfs;

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


void dfs(int root)
{
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

void setting_l(int n)
{
    for (int i = 0; i < dfsTraversal.size(); i++)
    {
        l.push_back(nodeLevel[dfsTraversal[i]]);
    }
}

void setting_h(int n)
{
    for (int i = 0; i <= n; i++)
    {
        h[i] = -1;
    }
    for (int i = 0; i < dfsTraversal.size(); i++)
    {
        if (h[dfsTraversal[i]] == -1)
        {
            h[dfsTraversal[i]] = i;
        }
    }
}

int RMQ(int ss, int se, int qs, int qe, int i)
{
    if (ss > se)
    {
        return -1;
    }
    
    if (se < qs || qe < ss)
    {
        return -1;
    }
    
    if (qs <= ss && se <= qe)
    {
        return segmentTree[i];
    }
    
    int mid = (ss + se) >> 1;
    int st = RMQ(ss, mid, qs, qe, 2 * i + 1);
    int en = RMQ(mid + 1, se, qs, qe, 2 * i + 2);
    
    if (st != -1 && en != -1)
    {
        if (l[st] < l[en])
        {
            return st;
        }
        return en;
    }
    else if (st != -1)
    {
        return st;
    }
    else if (en != -1)
    {
        return en;
    }
    
    return -1;
}

void buildSegmentTree(int startIndex, int finishIndex, int i)
{
    if (startIndex > finishIndex)
        return;
    if (startIndex == finishIndex) // leaf
    {
        segmentTree[i] = startIndex;
        return;
    }
    int mid = (startIndex + finishIndex) >> 1;
    
    buildSegmentTree(startIndex, mid, 2 * i + 1);
    buildSegmentTree(mid + 1, finishIndex, 2 * i + 2);
    
    if (l[segmentTree[2 * i + 1]] < l[segmentTree[2 * i + 2]])
        segmentTree[i] = segmentTree[2 * i + 1];
    else
        segmentTree[i] = segmentTree[2 * i + 2];
}

int LCA(int u, int v) {
    if (h[u] > h[v])
        std::swap(u, v);
    return dfsTraversal[RMQ(0, l.size() - 1, h[u], h[v], 0)];
}

void resizeObjects()
{
    graph.resize(vertexCount);
    nodeLevel.resize(vertexCount);
    h.resize(vertexCount);
    visitedNodesForDfs.resize(vertexCount);
    segmentTree.resize(5 * vertexCount);
}

void fillGraph()
{
    for(int i = 0; i < vertexCount - 1; i++)
    {
        int u, v;
        std::cin >> u >> v;
        
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
}

void buildDataStructure()
{
    nodeLevel[0] = 1;
    updateNodesLevel(0);
    dfs(0);
    setting_l(vertexCount - 1);
    setting_h(vertexCount - 1);
    buildSegmentTree(0, l.size() - 1, 0);
}

void answearQuestions()
{
    int c;
    std::cin >> c;
    for(int i = 0; i < c; i++)
    {
        int a, b;
        std::cin >> a >> b;
        std::cout << LCA(a, b) << std::endl;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    std::cin >> vertexCount;
    
    resizeObjects();
    fillGraph();
    buildDataStructure();
    answearQuestions();
    
    return 0;
}
