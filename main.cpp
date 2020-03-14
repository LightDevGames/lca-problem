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
std::vector<bool> isNodeVisitedInDfs;

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
    
    isNodeVisitedInDfs[root] = 1;
    for (int i = 0; i < graph[root].size(); i++)
    {
        int childVertex = graph[root][i];
        if (!isNodeVisitedInDfs[childVertex])
        {
            dfs(childVertex);
            dfsTraversal.push_back(root);
            dfsTraversalNodesLevels.push_back(nodesLevels[root]);
        }
    }
}

void updateFirstMentionInDfs()
{
    for (int i = 0; i < vertexCount; i++)
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

int rmq(int fullRangeStart, int fullRangeEnd, int rangeStart, int rangeEnd, int currentIndex)
{
    if (fullRangeStart > fullRangeEnd)
    {
        return -1;
    }
    
    if (fullRangeEnd < rangeStart || rangeEnd < fullRangeStart)
    {
        return -1;
    }
    
    if (rangeStart <= fullRangeStart && fullRangeEnd <= rangeEnd)
    {
        return segmentTree[currentIndex];
    }
    
    int mid = (fullRangeStart + fullRangeEnd) >> 1;
    int st = rmq(fullRangeStart, mid, rangeStart, rangeEnd, 2 * currentIndex + 1);
    int en = rmq(mid + 1, fullRangeEnd, rangeStart, rangeEnd, 2 * currentIndex + 2);
    
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

void buildSegmentTree(int startIndex, int finishIndex, int currentIndex)
{
    if (startIndex > finishIndex)
        return;
    if (startIndex == finishIndex)
    {
        segmentTree[currentIndex] = startIndex;
        return;
    }
    int mid = (startIndex + finishIndex) >> 1;
    
    buildSegmentTree(startIndex, mid, 2 * currentIndex + 1);
    buildSegmentTree(mid + 1, finishIndex, 2 * currentIndex + 2);
    
    if (dfsTraversalNodesLevels[segmentTree[2 * currentIndex + 1]] < dfsTraversalNodesLevels[segmentTree[2 * currentIndex + 2]])
    {
        segmentTree[currentIndex] = segmentTree[2 * currentIndex + 1];
    }
    else
    {
        segmentTree[currentIndex] = segmentTree[2 * currentIndex + 2];
    }
}

int LCA(int vertexU, int vertexV)
{
    if (nodeFirstMentionInDfs[vertexU] > nodeFirstMentionInDfs[vertexV])
    {
        std::swap(vertexU, vertexV);
    }
    
    return dfsTraversal
    [
      rmq(0, dfsTraversal.size() - 1, nodeFirstMentionInDfs[vertexU], nodeFirstMentionInDfs[vertexV], 0)
    ];
}

void resizeVectors()
{
    graph.resize(vertexCount);
    
    nodesLevels.resize(vertexCount);
    
    isNodeVisitedInDfs.resize(vertexCount);
    nodeFirstMentionInDfs.resize(vertexCount);
    
    segmentTree.resize(5 * vertexCount);
}

void fillGraph()
{
    for(int i = 1; i < vertexCount; i++)
    {
        int parent;
        std::cin >> parent;
        
        graph[i].push_back(parent);
        graph[parent].push_back(i);
    }
}

void buildDataStructure()
{
    nodesLevels[0] = 1;
    updateNodesLevel(0);
    dfs(0);
    updateFirstMentionInDfs();
    buildSegmentTree(0, dfsTraversal.size() - 1, 0);
}


int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int questionsCount, a1, a2, x, y, z;;
    std::cin >> vertexCount >> questionsCount;
    
    resizeVectors();
    fillGraph();
    buildDataStructure();
    
    std::cin >> a1 >> a2 >> x >> y >> z;
    
    int sum = 0;
    int res = LCA(a1, a2);
    sum += res;

    for(int i = 1; i < questionsCount; i++)
    {
        if(res == a1 || res == a2)
            a1 = (x * a1 + y * a2 + z + res) % vertexCount;
        else
            a1 = (x * a1 + y * a2 + z) % vertexCount;
        a2 = (x * a2 + y * a1 + z) % vertexCount;
        
        res = LCA(a1, a2);
        sum += res;
    }
    
    std::cout << sum;
    
    return 0;
}
