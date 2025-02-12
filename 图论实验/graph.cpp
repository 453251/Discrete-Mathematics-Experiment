#include"graph.h"
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<queue>
#include<stack>
using namespace std;
int flag1 = 1, flag2 = 1;//用于记录连通性和是否有环


int LocateVex(MGraph G, int v)
{
	for (int i = 0; i < G.vexnum; i++)	if (G.vexs[i] == v) return i;
	return -1;
}
Status CreateGraph(MGraph& G)
{
	return CreateUDN(G);
}
Status CreateUDN(MGraph& G)
{
	cout << "请输入顶点数和边数目:" << endl << ">>>";
	cin >> G.vexnum >> G.arcnum;
	cout << "请输入图中顶点编号:\n" << ">>>";
	for (int i = 0; i < G.vexnum; i++) cin >> G.vexs[i];
	cout << "读入边:\n";
	for (int i = 0; i < G.vexnum; i++)
		for (int j = 0; j < G.vexnum; j++)
		{
			G.arcs[i][j].adj = G.vexs[i];
			G.arcs[i][j].info = 0x3f3f;
		}
	for (int k = 0; k < G.arcnum; k++)
	{
		int v1, v2, w;
		cout << ">>>";
		cin >> v1 >> v2 >> w;
		int i = LocateVex(G, v1), j = LocateVex(G, v2);
		G.arcs[i][j] = { v1,w }; G.arcs[j][i] = { v2,w };
	}
	return OK;
}

void output_graph(MGraph G)
{
	cout << "加权图的邻接矩阵:" << endl;
	for (int i = 0; i < G.vexnum; i++)
	{
		if (i == 0) cout << "  ";
		if (i)	cout << "\t";
		cout << G.vexs[i];
	}
	cout << endl;
	for (int i = 0; i < G.vexnum; i++)
	{
		cout << G.vexs[i] << "|";
		for (int j = 0; j < G.vexnum; j++)
		{
			if (j != G.vexnum - 1)
			{
				if (G.arcs[i][j].info != 0x3f3f)
				{
					cout << G.arcs[i][j].info << "\t";
				}
				else
					cout << "∞\t";
			}
			else
			{
				if (G.arcs[i][j].info != 0x3f3f)
				{
					cout << G.arcs[i][j].info << endl;
				}
				else
					cout << "∞\n";
			}
		}
	}
}
int visited[MAX_VERTEX_NUM];//标记顶点是否已经访问

void DFS(MGraph G, int v)
{
	visited[v] = 1;
	for (int i = 0; i < G.vexnum; i++)
	{
		if (G.arcs[v][i].info && G.arcs[v][i].info!=0x3f3f)
		{
			if (!visited[i])
				DFS(G, i);
		}
	}
}

void DFSTraverse(MGraph G)
{
	int flag = 1;
	for (int i = 0; i < MAX_VERTEX_NUM; i++) visited[i] = 0;
	for (int i = 0; i < G.vexnum; i++)
	{
		if (!visited[i] && i)
			flag = 0;
		if (!visited[i]) DFS(G, i);
	}
	if (flag)
	{
		cout << "该加权无向图是连通图" << endl;
	}
	else
	{
		cout << "该加权无向图不是连通图" << endl;
		flag1 = 0;
	}
}

int degree[MAX_VERTEX_NUM];
void FindInDegree(MGraph G)
{
	for (int i = 0; i < G.vexnum; i++)
	{
		for (int j = 0; j < G.vexnum; j++)
		{
			if (G.arcs[i][j].info && G.arcs[i][j].info!=0x3f3f)
			{
				degree[j]++;
			}
		}
	}
}
void is_Euler_Graph(MGraph G)
{
	FindInDegree(G);
	for (int i = 0; i < G.vexnum; i++)
	{
		if (degree[i] % 2)
		{
			cout << "该加权无向图不是欧拉图" << endl;
			return;
		}
	}
	cout << "该加权无向图是欧拉图" << endl;
}

int TopologicalSort(MGraph G)
{
	stack<int>S; //零入度顶点栈S
	memset(degree, 0, sizeof(degree)); //初始化入度数组
	FindInDegree(G); //对各顶点求入度
	for (int i = 0; i < G.vexnum; i++)
		if (!degree[i]) S.push(i);
	int count = 0; //对输出顶点计数
	while (!S.empty())
	{
		int i = S.top(); S.pop();
		cout << G.vexs[i];
		count++;
		for (int j = 0; j < G.vexnum; j++)
		{
			if (G.arcs[i][j].info && G.arcs[i][j].info!=0x3f3f)
			{
				degree[j]--; //对i号顶点的每个邻接点的入度减1
				if (!degree[j]) S.push(j); //若入度为0，则入栈
			}
		}
	}
	if (count < G.vexnum) { flag2 = 0; return 0; }
	else  return 1;
}

void is_tree(MGraph G)
{
	/*图如果是树，需满足边数等于顶点数-1，无环且连通*/
	if (G.arcnum != G.vexnum - 1)
	{
		cout << "该加权无向图不是树" << endl;
		return;
	}
	if (flag1 && flag2)
		cout << "该加权无向图是树" << endl;
}

struct
{
	int adjvex;
	int lowcost;
}closedge[MAX_VERTEX_NUM]; //closedge中存储的是非树顶点到树的最短距离
int sum = 0;//用来存储最小生成树的权重和
int minimum(MGraph G, int u)
{
	int k = LocateVex(G, u);
	int min = 99999999, idx = -1;
	for (int i = 0; i < G.vexnum; i++)
	{
		if (closedge[i].lowcost && closedge[i].lowcost < min && closedge[i].lowcost < 0x3f3f) //这里设置小于0x3f3f是为了处理没边的情况
		{
			min = closedge[i].lowcost;
			idx = i;
		}
	}
	if (idx != -1)
		sum += min;
	return idx;
}

void MiniSpanTree_PRIM(MGraph G, int u)
{
	if (!flag1)
	{
		cout << "该加权无向图不是连通图，没有最小生成树" << endl;
		return;
	}
	int m = LocateVex(G, u);
	for (int i = 0; i < G.vexnum; i++)
	{
		if (i != m) closedge[i] = { u,G.arcs[m][i].info };
	}
	closedge[m].lowcost = 0;
	for (int i = 1; i < G.vexnum; i++)
	{
		int k = minimum(G, u);
		closedge[k].lowcost = 0; //第k顶点并入U集
		for (int j = 0; j < G.vexnum; j++)
		{
			if (G.arcs[k][j].info && G.arcs[k][j].info < 1000)
			{
				if (G.arcs[k][j].info < closedge[j].lowcost)
					closedge[j] = { G.vexs[k],G.arcs[k][j].info };
			}
		}
	}
	cout << "PRIM算法的最小生成树的权重和:" << sum << endl;
}

typedef struct edge
{
	int v;//顶点
	int u;//顶点
	int info;//权重
	friend bool operator<(edge a, edge b)
	{
		return a.info > b.info;
	}
}edge;
int f[MAX_VERTEX_NUM]; //存储并查集合
void init()
{
	for (int i = 0; i < MAX_VERTEX_NUM; i++)
		f[i] = i;
}
//找爹函数
int getf(int v)
{
	if (f[v] == v)
		return v;
	else
	{
		f[v] = getf(f[v]); //路径压缩
		return f[v];
	}
}
int merge(int v, int u)
{
	int t1, t2;
	t1 = getf(v);
	t2 = getf(u);
	if (t1 != t2)
	{
		f[t2] = t1; //靠左原则
		return 1; //两条边不在同一个集合中，因此这条边要选
	}
	return 0;
}

int Kruskal(MGraph G)
{
	if (!flag1)
	{
		cout << "该加权无向图不是连通图，没有最小生成树" << endl;
		return 0;
	}
	priority_queue<edge>q; //这里用优先队列存边
	init();//并查集初始化
	queue<edge>qq;//存最小生成树的边
	int k = 0;
	for (int i = 0; i < G.vexnum; i++)
	{
		int a = G.vexs[i];
		for (int j = 0; j < G.vexnum; j++)
		{
			if (G.arcs[i][j].info && G.arcs[i][j].info!=0x3f3f)
			{
				int b = G.vexs[j];
				edge e = { a,b,G.arcs[i][j].info };
				q.push(e);
			}
		}
	}
	//退出时k为边的数目
	int cnt = 0, sum = 0;
	while (1)
	{
		edge e = q.top(); q.pop();
		int v = e.v, u = e.u;
		if (merge(v, u))
		{
			cnt++;
			sum += e.info;
			qq.push(e);
		}
		if (cnt == G.vexnum - 1)
			break;
	}
	cout << "该加权无向图的最小生成树:" << endl;
	while (!qq.empty())
	{
		edge e = qq.front(); qq.pop();
		cout << e.v << " " << e.u << " " << e.info << endl;
	}
	cout << "Kruskal算法的最小生成树的权重和:" << sum << endl;
	return sum;
}