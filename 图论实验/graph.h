#pragma once
#ifndef _GRAPH_H_
#define _GRAPH_H_
#include"constant.h"
#include<stdio.h>
#include<iostream>
#include<vector>
using namespace std;
#define MAX_VERTEX_NUM 20


typedef struct ArcCell
{
	int adj;
	int info;
}ArcCell, AdjMartrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
typedef struct
{
	int vexs[MAX_VERTEX_NUM];
	AdjMartrix arcs;
	int vexnum, arcnum;
}MGraph;

int LocateVex(MGraph G, int v);//定位顶点位置
//邻接矩阵建图
Status CreateGraph(MGraph& G);//建立无向网
Status CreateUDN(MGraph& G); //无向网
void output_graph(MGraph G);//输出邻接矩阵
void FindInDegree(MGraph G);//求度数
void is_Euler_Graph(MGraph G);//判断是否是欧拉图

//邻接矩阵遍历
void DFS(MGraph G, int v);
void DFSTraverse(MGraph G);//判断图是否是连通图

int TopologicalSort(MGraph G);//拓扑排序，判断是否有环
void is_tree(MGraph G);//判断图是否是树



//Prime算法
void MiniSpanTree_PRIM(MGraph G, int u);
/*Kruskal算法*/
//并查集
void init();
int getf(int v);
int merge(int v, int u);
int Kruskal(MGraph G);






#endif
