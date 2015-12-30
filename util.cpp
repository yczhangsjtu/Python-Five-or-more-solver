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
inline bool all(char c1,char c2,char c3,char c4,char c5)
{
	return c1 != '.' && c1==c2 && c1==c3 && c1==c4 && c1==c5;
}
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
inline int chi(bool b)
{
	return b?1:0;
}
inline int chi(bool b1,bool b2,bool b3,bool b4,bool b5)
{
	return chi(b1)+chi(b2)+chi(b3)+chi(b4)+chi(b5);
}
inline int scale(int k)
{
	switch(k)
	{
	case 0:return a2;case 1:return a3;case 2:return a4;case 3:return a5;
	case 4:return a6;case 5:return a7;
	}
	return 0;
}
inline int chi(char c,char c1,char c2,char c3,char c4,char c5)
{
	int k = chi(c==c1,c==c2,c==c3,c==c4,c==c5);
	return scale(k);
}

inline void updatemax(int *n, int m)
{
	if(m > *n) *n = m;
}

inline bool blocked(char (*bd)[10], int p, int q)
{
	return bd[p][q] != '.'
		&& (p==0 || bd[p-1][q] != '.')
		&& (q==0 || bd[p][q-1] != '.')
		&& (p==8 || bd[p+1][q] != '.')
		&& (q==8 || bd[p][q+1] != '.');
}

inline int outway(char (*bd)[10], int p, int q)
{
	return chi(p>0 || bd[p-1][q] == '.')
	   +chi(q>0 || bd[p][q-1] == '.')
	   +chi(p<8 || bd[p+1][q] == '.')
	   +chi(q<8 || bd[p][q+1] == '.');
}

bool addrand(char (*bd)[10])
{
	int p = 0;
	Point buf[81];
	for(int i = 0; i < 9; i++)
		for(int j = 0; j < 9; j++)
			if(bd[i][j] == '.') buf[p++]=Point(i,j);
	if(p < 3) return false;
	random_shuffle(buf,buf+p);
	for(int i = 0; i < 3; i++)
		bd[buf[i].i][buf[i].j] = colors[rand()%7];
	return true;
}

int cancel(char (*bd)[10])
{
	bool bl[9][9];
	for(int i = 0; i < 9; i++)
		for(int j = 0; j < 9; j++)
			bl[i][j] = false;
	for(int i = 0; i < 9; i++)
		for(int j = 0; j < 5; j++)
		{
            if(all(bd[i][j],bd[i][j+1],bd[i][j+2],bd[i][j+3],bd[i][j+4]))
                bl[i][j] = bl[i][j+1] = bl[i][j+2] = bl[i][j+3] = bl[i][j+4] = true;
            if(all(bd[j][i],bd[j+1][i],bd[j+2][i],bd[j+3][i],bd[j+4][i]))
                bl[j][i] = bl[j+1][i] = bl[j+2][i] = bl[j+3][i] = bl[j+4][i] = true;
		}
	for(int i = 0; i < 5; i++)
		for(int j = 0; j < 5; j++)
		{
            if(all(bd[i][j],bd[i+1][j+1],bd[i+2][j+2],bd[i+3][j+3],bd[i+4][j+4]))
				bl[i][j] = bl[i+1][j+1] = bl[i+2][j+2] = bl[i+3][j+3] = bl[i+4][j+4] = true;
            if(all(bd[i][j+4],bd[i+1][j+3],bd[i+2][j+2],bd[i+3][j+1],bd[i+4][j]))
				bl[i][j+4] = bl[i+1][j+3] = bl[i+2][j+2] = bl[i+3][j+1] = bl[i+4][j] = true;
		}
	int s = 0;
	for(int i = 0; i < 9; i++)
		for(int j = 0; j < 9; j++)
		{
			if(bl[i][j])
			{
				bd[i][j] = '.';
				s++;
			}
		}
	return s;
}

/****************** Evaluation thing ****************************/
void getscore(int *n, char (*bd)[10], char c, int my, int p, int q, int dx, int dy)
{
	int k = my-1, s = 1, discount = 0;
	while(k >= 0)
	{
		int x = p+k*dx, y = q+k*dy;
		if(x < 0 || x > 8 || y < 0 || y > 8) return;
		if(bd[x][y] == c) s++;
		else if(blocked(bd,x,y)) break;
		else if(outway(bd,x,y)<2) discount += a8;
		k--;
	}
	k = my+1;
	while(k < 5)
	{
		int x = p+k*dx, y = q+k*dy;
		if(x < 0 || x > 8 || y < 0 || y > 8) return;
		if(bd[x][y] == c) s++;
		else if(blocked(bd,x,y)) break;
		else if(outway(bd,x,y)<2) discount += a8;
		k++;
	}
	updatemax(n,scale(s));
	*n = *n > discount? *n-discount: 0;
}

void countmax(char (*bd)[10], int *n, int p, int q, int k)
{
	char cc = colors[k];
	*n = 0;
	for(int i = 0; i < 5; i++) getscore(n,bd,cc,i,p-i,q,1,0);
	for(int i = 0; i < 5; i++) getscore(n,bd,cc,i,p,q-i,0,1);
	for(int i = 0; i < 5; i++) getscore(n,bd,cc,i,p-i,q-i,1,1);
	for(int i = 0; i < 5; i++) getscore(n,bd,cc,i,p-i,q+i,1,-1);
}

int gain(char (*bd)[10], int p, int q, int s, int t, char c)
{
	assert(bd[p][q] == c);
	assert(bd[s][t] == '.');
	bd[p][q] = '.';
	int n1, n2, ic=ctoi(c);
	countmax(bd,&n1,p,q,ic);
	countmax(bd,&n2,s,t,ic);
	bd[p][q] = c;
	return n2-n1;
}

int connectgain(char (*bd)[10], int p, int q, int s, int t, char c)
{
	assert(bd[p][q] == c);
	assert(bd[s][t] == '.');
	int c1 = numofarea(bd,p,q);
	bd[p][q] = c;
	bd[s][t] = '.';
	int c2 = numofarea(bd,s,t);
	bd[p][q] = c;
	bd[s][t] = '.';
	return c1-c2;
}

inline int maxexcept(int e, int *n)
{
	int max = 0;
	for(int i = 0; i < 7; i++)
		if(i!=e) updatemax(&max,n[i]);
	return max;
}



Out evalout(char (*bd)[10], int p, int q)
{
	Out out;
	int n[7] = {0,0,0,0,0,0,0};
	for(int k = 0; k < 7; k++)
		countmax(bd,&n[k],p,q,k);
	for(int ic = 0; ic < 7; ic++)
		out.out[ic] = maxexcept(ic,n);
	return out;
}

void evalout(char (*bd)[10], Out (*out)[9])
{
	int m[] = {a1,a2,a3,a4};
	for(int i = 0; i < 9; i++)
		for(int j = 0; j < 9; j++)
			out[i][j] = evalout(bd,i,j);
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

int fill(char (*bd)[10], int p, int q)
{
	if(p > 8 || p < 0 || q > 8 || q < 0) return 0;
	if(bd[p][q] != '.') return 0;
	bd[p][q] = '\0';
	fill(bd,p+1,q);
	fill(bd,p-1,q);
	fill(bd,p,q+1);
	fill(bd,p,q-1);
	return 1;
}

int numofarea(char (*bd)[10], int p, int q)
{
	int s = 0;
	if(bd[p][q] == '.') return 1;
	s += fill(bd,p-1,q);
	s += fill(bd,p+1,q);
	s += fill(bd,p,q-1);
	s += fill(bd,p,q+1);
	for(int i = 0; i < 9; i++)
		for(int j = 0; j < 9; j++)
			if(bd[i][j] == '\0') bd[i][j] = '.';
	return s;
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

void takemove(char (*bd)[10], int p, int q, int r, int s)
{
	char c = bd[p][q];
	char d = bd[r][s];
	assert(c != '.' && d == '.');
	bd[p][q] = '.';
	bd[r][s] = c;
}



/**********************Print functions************************/
void printboard(char (*bd)[10])
{
	for(int i = 0; i < 9; i++)
		printf("%s\n",bd[i]);
}

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
    int sys = system("clear");
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

bool freadboard(FILE *f, char (*bd)[10], int *score)
{
	char state[256];
	int tmp = scanf("%s%d",state,score);
	if(strcmp(state,"over")==0)
		return false;
	for(int i = 0; i < 9; i++)
		int tmp = scanf("%s",bd[i]);
	return true;
}
