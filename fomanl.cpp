#include <cstdio>
#include <cstdlib>
#include <unistd.h>
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

int a1 = 5;
int a2 = 50;
int a3 = 300;
int a4 = 1000;
int a5 = 10000;
int a6 = 300;
int a7 = 250;
int a8 = 100;
int a9 = 20;
int a10 = 20;
int a11 = 3;
int a12 = 0;
int a13 = -50;
int a14 = -100;

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

bool color = false;

int main(int argc, char *argv[])
{
	int ch;
	while((ch = getopt(argc,argv,"c")) != -1)
	{
		switch(ch)
		{
		case 'c': color = true; break;
		}
	}
	FILE *f = fopen("param","r");
	if(f)
	{
		fscanf(f,"%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
				&a1,&a2,&a3,&a4,&a5,&a6,&a7,&a8,&a9,&a10,&a11,&a12,&a13,&a14);
		fclose(f);
	}
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
		if(color)
			printcolormove(data,bm->p,bm->q,bm->r,bm->s);
		else
		{
			printf("normal\n%d\n",score);
			printmove(data,bm->p,bm->q,bm->r,bm->s);
		}
		delete bm;
	}
	return 0;
}
