#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <climits>

int vertexCount;

std::vector<std::vector<int>> graph;
std::vector<int> nodesLevels;
std::vector<int> dfsTraversal;
std::vector<int> dfsTraversalNodesLevels;
std::vector<int> nodeFirstMentionInDfs;
std::vector<int> segmentTree;
std::vector<bool> visitedNodesForDfs;

void updateNodesLevel(int root)
{
    for (int i = 0; i < graph[root].size(); i++)
    {
        int child = graph[root][i];
        if (!nodesLevels[child])
        {
            nodesLevels[child] = nodesLevels[root] + 1;
            updateNodesLevel(child);
        }
    }
}

void dfs(int root)
{
    dfsTraversal.push_back(root);
    dfsTraversalNodesLevels.push_back(nodesLevels[root]);
    
    visitedNodesForDfs[root] = 1;
    for (int i = 0; i < graph[root].size(); i++)
    {
        int childVertex = graph[root][i];
        if (!visitedNodesForDfs[childVertex])
        {
            dfs(childVertex);
            dfsTraversal.push_back(root);
            dfsTraversalNodesLevels.push_back(nodesLevels[root]);
        }
    }
}

void buildFirstMentionInDfs(int n)
{
    for (int i = 0; i <= n; i++)
    {
        nodeFirstMentionInDfs[i] = -1;
    }
    for (int i = 0; i < dfsTraversal.size(); i++)
    {
        if (nodeFirstMentionInDfs[dfsTraversal[i]] == -1)
        {
            nodeFirstMentionInDfs[dfsTraversal[i]] = i;
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
        if (dfsTraversalNodesLevels[st] < dfsTraversalNodesLevels[en])
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
    if (startIndex == finishIndex)
    {
        segmentTree[i] = startIndex;
        return;
    }
    int mid = (startIndex + finishIndex) >> 1;
    
    buildSegmentTree(startIndex, mid, 2 * i + 1);
    buildSegmentTree(mid + 1, finishIndex, 2 * i + 2);
    
    if (dfsTraversalNodesLevels[segmentTree[2 * i + 1]] < dfsTraversalNodesLevels[segmentTree[2 * i + 2]])
    {
        segmentTree[i] = segmentTree[2 * i + 1];
    }
    else
    {
        segmentTree[i] = segmentTree[2 * i + 2];
    }
}

int LCA(int u, int v)
{
    if (nodeFirstMentionInDfs[u] > nodeFirstMentionInDfs[v])
    {
        std::swap(u, v);
    }
    
    return dfsTraversal
    [
      RMQ(0, dfsTraversal.size() - 1, nodeFirstMentionInDfs[u], nodeFirstMentionInDfs[v], 0)
    ];
}

void resizeObjects()
{
    graph.resize(vertexCount);
    nodesLevels.resize(vertexCount);
    nodeFirstMentionInDfs.resize(vertexCount);
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
    nodesLevels[0] = 1;
    updateNodesLevel(0);
    dfs(0);
    buildFirstMentionInDfs(vertexCount - 1);
    buildSegmentTree(0, dfsTraversalNodesLevels.size() - 1, 0);
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

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    std::cin >> vertexCount;
    
    resizeObjects();
    fillGraph();
    buildDataStructure();
    answearQuestions();
    
    return 0;
}
