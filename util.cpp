#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <algorithm>
#include <cassert>
#include "util.h"

using namespace std;

extern int a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11;
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
inline int scoreof(int k)
{
	switch(k)
	{
	case 0:return a1;case 1:return a2;case 2:return a3;case 3:return a4;
	case 4:return a5;case 5:return a6;case 6:return a7;case 7:return a8;
	case 8:return a9;case 9:return a10;case 10:return a11;
	}
	return 0;
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
/**
 * How good are five consecutive positions
 */
int evalfive(char c1, char c2, char c3, char c4, char c5)
{
	char l[] = {c1,c2,c3,c4,c5};
	// Get the number of dominant color
	sort(l,l+5);
	int m = 0, s = 0;
	int mn = 0;
	for(int i = 0; i < 5; i++)
	{
		if(l[i] == '.') continue;
		s++; // The number of nonempty positions
		if(i == 0 || l[i] != l[i-1]) {updatemax(&mn,m);m = 1;continue;}
		m++;
	}
	updatemax(&mn,m);
	return 5+2*mn-s;
}

void evalfives(char (*bd)[10], int &e, int &f)
{
	e = f = 0;
	for(int i = 0; i < 9; i++)
		for(int j = 0; j < 5; j++)
		{
			int ef = evalfive(bd[i][j],bd[i][j+1],bd[i][j+2],
                           bd[i][j+3],bd[i][j+4]);
			e += scoreof(ef);
			updatemax(&f,ef);
		}
	for(int i = 0; i < 9; i++)
		for(int j = 0; j < 5; j++)
		{
            int ef = evalfive(bd[j][i],bd[j+1][i],bd[j+2][i],
                           bd[j+3][i],bd[j+4][i]);
			e += scoreof(ef);
			updatemax(&f,ef);
		}
	for(int i = 0; i < 5; i++)
		for(int j = 0; j < 5; j++)
		{
            int ef = evalfive(bd[i][j],bd[i+1][j+1],bd[i+2][j+2],
                           bd[i+3][j+3],bd[i+4][j+4]);
			e += scoreof(ef);
			updatemax(&f,ef);
		}
	for(int i = 0; i < 5; i++)
		for(int j = 0; j < 5; j++)
		{
            int ef = evalfive(bd[i][j+4],bd[i+1][j+3],bd[i+2][j+2],
                           bd[i+3][j+1],bd[i+4][j]);
			e += scoreof(ef);
			updatemax(&f,ef);
		}
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
