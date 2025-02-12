#ifndef PROPOSITION
#define PROPOSITION

#pragma once

#include<cmath>
#include<cstdlib>

#include<string>
#include<vector>
#include<set>
#include<stack>

#include<iostream>
#include<windows.h>
#include<pthread.h>



#define INVAILD 0

using namespace std;

class MyCompare
{
public:
	bool operator()(const pair<char, bool>& a, const pair<char, bool>& b) const;
};

//Ωªª•ΩÁ√Ê

char cmp(char a, char b);
bool operate(bool a, char chr, bool b);
bool calc(const string& sub_s);
bool judge_expression_proper(string expr);

int first(const string& expr,int order);

bool fifth(const string& expr,int order);

int third(const string& expr,int order);

int second(const string& expr);

int fourth(const string& expression1,const string& expression2);

extern HWND hChildWnd1,hChildWnd2,hChildWnd3;
extern string output_1,output_2,output_3;
#endif // !PROPOSITION