#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <algorithm>
#include <cassert>
#include "util.h"

using namespace std;

extern int a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14;
extern char colors[7];

/****************** Small utilities *****************************/
int ctoi(char c)
{
	switch(c)
	{
	case 'r': return 0; case 'g': return 1; case 'b': return 2;
	case 'y': return 3; case 'o': return 4; case 'p': return 5;
	case 'd': return 6;
	}
	return 0;
}

/****************** Evaluation thing ****************************/
void evalneed(char (*bd)[10], Need (*nd)[9])
{
	int m[] = {a1,a2,a3,a4};
	int in[9][9][7][4];
	for(int i = 0; i < 9; i++)
		for(int j = 0; j < 9; j++)
			for(int k = 0; k < 7; k++)
				for(int l = 0; l < 4; l++)
					in[i][j][k][l] = 1;
	for(int i = 0; i < 9; i++)
		for(int j = 0; j < 9; j++)
		{
			char c = bd[i][j];
			nd[i][j].c = c;
			if(c == '.') continue;
			int ic = ctoi(c);
			for(int k = 0; k < 4; k++)
			{
				if(i+1+k< 9) in[i+1+k][j][ic][0] *= m[k];
				if(i-1-k>=0) in[i-1-k][j][ic][0] *= m[k];
				if(j+1+k< 9) in[i][j+1+k][ic][1] *= m[k];
				if(j-1-k>=0) in[i][j-1-k][ic][1] *= m[k];
				if(abs(i-j)<=4)
				{
					if(i+1+k< 9&&j+1+k< 9) in[i+1+k][j+1+k][ic][2] *= m[k];
					if(i-1-k>=0&&j-1-k>=0) in[i-1-k][j-1-k][ic][2] *= m[k];
				}
				if(i+j>=4 && i+j<=12)
				{
					if(i+1+k< 9&&j-1-k>=0) in[i+1+k][j-1-k][ic][3] *= m[k];
					if(i-1-k>=0&&j+1+k< 9) in[i-1-k][j+1+k][ic][3] *= m[k];
				}
			}
		}
	for(int i = 0; i < 9; i++)
		for(int j = 0; j < 9; j++)
		{
			int mx = 0;
			for(int k = 0; k < 7; k++)
			{
				nd[i][j].in[k] = in[i][j][k][0]+in[i][j][k][1]
								+in[i][j][k][2]+in[i][j][k][3];
				if(nd[i][j].in[k] > mx) mx = nd[i][j].in[k];
			}
			int ic = ctoi(nd[i][j].c);
			int sum = 0;
			int fix = nd[i][j].c == '.'? 0: nd[i][j].in[ic] *= a6;
			for(int k = 0; k < 7; k++)
				sum += nd[i][j].in[k];
			for(int k = 0; k < 7; k++)
			{
				nd[i][j].in[k] *= a5;
				nd[i][j].in[k] -= sum;
			}
			nd[i][j].in[ic] += fix;
		}
}

/**
 * How good are five consecutive positions
 */
int __evalfive(char c1, char c2, char c3, char c4, char c5)
{
	char l[] = {c1,c2,c3,c4,c5};
	char s[5];
	char st[5];
	int ls = 0;
	int lst = 0;
	for(int i = 0; i < 5; i++)
		if(l[i]!='.') s[ls++] = l[i];
	for(int i = 0; i < ls; i++)
	{
		bool e = false;
		for(int j = 0; j < lst; j++)
			if(st[j] == s[i]) e = true;
		if(!e) st[lst++] = s[i];
	}
	if(lst == 0) return 4;
	if(lst == 1)
	{
        if(ls == 1) return a1;
        if(ls == 2) return a2;
        if(ls == 3) return a3;
        if(ls == 4) return a4;
        if(ls == 5) return a5;
	}
	if(lst == 2)
	{
		sort(st,st+lst);
		int count = 0;
		for(int i = 1; i < lst; i++)
			if(st[i-1]!=st[i]) count = i;
		if(count == 4) return a6;
		if(count == 3 && ls == 4) return a7;
		if(count == 3 && ls == 5) return a8;
		if(count == 2 && ls == 3) return a9;
		if(count == 2 && ls == 4) return a10;
		if(count == 1 && ls == 2) return a11;
	}
	if(lst == 3) return a12;
	if(lst == 4) return a13;
	if(lst == 5) return a14;
	return 0;
}

int evalfives(char (*bd)[10])
{
	int s = 0;
	for(int i = 0; i < 9; i++)
		for(int j = 0; j < 5; j++)
            s += __evalfive(bd[i][j],bd[i][j+1],bd[i][j+2],
                           bd[i][j+3],bd[i][j+4]);
	for(int i = 0; i < 9; i++)
		for(int j = 0; j < 5; j++)
            s += __evalfive(bd[j][i],bd[j+1][i],bd[j+2][i],
                           bd[j+3][i],bd[j+4][i]);
	for(int i = 0; i < 5; i++)
		for(int j = 0; j < 5; j++)
            s += __evalfive(bd[i][j],bd[i+1][j+1],bd[i+2][j+2],
                           bd[i+3][j+3],bd[i+4][j+4]);
	for(int i = 0; i < 5; i++)
		for(int j = 0; j < 5; j++)
            s += __evalfive(bd[i][j+4],bd[i+1][j+3],bd[i+2][j+2],
                           bd[i+3][j+1],bd[i+4][j]);
    return s;
}

/**
 * Evaluate the board
 */
int __eval(char (*bd)[10])
{
	int s = 0;
	s += evalfives(bd);
	return s;
}
 
/*************** Algorithms **************************/
/**
 * If a pawn at (p,q) can be moved to (r,s)?
 */
bool connected(char (*bd)[10], int p, int q, int r, int s)
{
	Point qe[81];
	int f = 0, t = 0;
	bool v[9][9];
	qe[t++] = Point(p,q);
	for(int i = 0; i < 9; i++)
		for(int j = 0; j < 9; j++)
			v[i][j] = false;
	while(t > f)
	{
		Point c = qe[f++];
		Point nb[] = {Point(c.i+1,c.j),Point(c.i-1,c.j),
			Point(c.i,c.j+1),Point(c.i,c.j-1)};
		for(int i = 0; i < 4; i++)
		{
			Point n = nb[i];
			if(n.i == r && n.j == s) return true;
			if(n.i < 0 || n.i >= 9 || n.j < 0 || n.j >= 9) continue;
			if(bd[n.i][n.j] != '.') continue;
			if(v[n.i][n.j]) continue;
			qe[t++] = n;
			v[n.i][n.j] = true;
		}
	}
	return false;
}

void getavail(char (*bd)[10], int p, int q, Point *buf, int *s)
{
	Point qe[81];
	int f = 0, t = 0;
	bool v[9][9];
	qe[t++] = Point(p,q);
	for(int i = 0; i < 9; i++)
		for(int j = 0; j < 9; j++)
			v[i][j] = false;
	while(t > f)
	{
		Point c = qe[f++];
		if(bd[c.i][c.j] == '.') buf[(*s)++] = c;
		Point nb[] = {Point(c.i+1,c.j),Point(c.i-1,c.j),
			Point(c.i,c.j+1),Point(c.i,c.j-1)};
		for(int i = 0; i < 4; i++)
		{
			Point n = nb[i];
			if(n.i < 0 || n.i >= 9 || n.j < 0 || n.j >= 9) continue;
			if(bd[n.i][n.j] != '.') continue;
			if(v[n.i][n.j]) continue;
			qe[t++] = n;
			v[n.i][n.j] = true;
		}
	}
}



/**********************Print functions************************/
/*
 * Print how to move the pawn
 */
void printcolormove(char (*bd)[10], int p, int q, int r, int s)
{
    char c = bd[p][q];
    char d = bd[r][s];
	assert(c != '.' && d == '.');
    bd[p][q] = 'O';
    bd[r][s] = '*';
    system("clear");
	for(int i = 0; i < 9; i++)
	{
        for(int j = 0; j < 9; j++)
		{
            if(bd[i][j] == 'O')
				printf("\x1b[37;44m\033[1m.\033[0m");
			else if(bd[i][j] == '*')
                printf("\033[0;31m%c\033[0m",c);
            else
				printf("%c",bd[i][j]);
		}
		printf("\n");
	}
	bd[p][q] = c;
	bd[r][s] = d;
}

/**
 * Print the state after move (without changing the state)
 */
void printmove(char (*bd)[10], int p, int q, int r, int s)
{
	char c = bd[p][q];
	char d = bd[r][s];
	assert(c != '.' && d == '.');
	bd[p][q] = '.';
	bd[r][s] = c;
	for(int i = 0; i < 9; i++)
		printf("%s\n",bd[i]);
	bd[p][q] = c;
	bd[r][s] = d;
}

