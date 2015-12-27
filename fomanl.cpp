#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <cassert>

using namespace std;

typedef struct Point {
	int i,j;
	Point(){}
	Point(int a,int b):i(a),j(b){}
} Point;

typedef struct Move {
	int e,p,q,r,s;
} Move;

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
        if(ls == 1) return 5;
        if(ls == 2) return 50;
        if(ls == 3) return 300;
        if(ls == 4) return 1000;
        if(ls == 5) return 10000;
	}
	if(lst == 2)
	{
		sort(st,st+lst);
		int count = 0;
		for(int i = 1; i < lst; i++)
			if(st[i-1]!=st[i]) count = i;
		if(count == 4) return 300;
		if(count == 3 && ls == 4) return 250;
		if(count == 3 && ls == 5) return 100;
		if(count == 2 && ls == 3) return 20;
		if(count == 2 && ls == 4) return 20;
		if(count == 1 && ls == 2) return 1;
	}
	if(lst == 3) return 0;
	if(lst == 4) return -50;
	if(lst == 5) return -100;
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

int __eval(char (*bd)[10])
{
	int s = 0;
	s += evalfives(bd);
	return s;
}

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

Move *bestmove(char (*bd)[10])
{
	Move *m = NULL;
	for(int p = 0; p < 9; p++)
		for(int q = 0; q < 9; q++)
			for(int r = 0; r < 9; r++)
				for(int s = 0; s < 9; s++)
				{
					if(bd[p][q] == '.' || bd[r][s] != '.')
						continue;
					if(!connected(bd,p,q,r,s))
						continue;
					char c = bd[p][q];
					bd[p][q] = '.';
					bd[r][s] = c;
					int e = __eval(bd);
					if(!m || e > m->e)
					{
						m = new Move;
						m->e = e; m->p = p; m->q = q;
						m->r = r; m->s = s;
					}
					bd[p][q] = c;
					bd[r][s] = '.';
				}
	return m;
}

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

int main()
{
	char state[256];
	char data[9][10];
	int score;
	scanf("%s%d",state,&score);
	if(strcmp(state,"over")==0)
	{
		printf("over\n%d\n",score);
		return 0;
	}
	for(int i = 0; i < 9; i++)
		scanf("%s",data[i]);
	Move *bm = bestmove(data);
	if(!bm)
	{
		printf("over\n%d\n",score);
	}
	else
	{
		printf("normal\n%d\n",score);
		printmove(data,bm->p,bm->q,bm->r,bm->s);
		delete bm;
	}
	return 0;
}
