#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <cassert>
#include <ctime>
#include "util.h"

using namespace std;

int a1 = 2;
int a2 = 1;
int a3 = 3;
int a4 = 8;
int a5 = 27;
int a6 = 54;
int a7 = 135;
int a8 = 162;
int a9 = 163;
int a10 = 163;
int a11 = 3;
int a12 = 10;
int a13 = 50;
int a14 = 100;
char colors[7] = {'r','g','b','y','o','p','d'};

#if 1
Move *bestmove(char (*bd)[10])
{
	Move *m = NULL;
	Out out[9][9];
	evalout(bd,out);
	for(int i = 0; i < 9; i++)
		for(int j = 0; j < 9; j++)
		{
			if(bd[i][j] != '.')
			{
				char c = bd[i][j];
				int ic = ctoi(c);
				Point buf[81];
				int navl = 0;
				getavail(bd,i,j,buf,&navl);
				if(navl == 0) continue;
				for(int k = 0; k < navl; k++)
				{
					int s = buf[k].i, t = buf[k].j;
					int e = out[i][j].out[ic] - out[s][t].out[ic];
					e += gain(bd,i,j,s,t,c) * a1;
					e += connectgain(bd,i,j,s,t,c) * a9;
					if(!m || m->e < e)
					{
						delete m;
						m = new Move;
						m->e = e; m->p = i; m->q = j;
						m->r = s; m->s = t;
					}
				}
			}
		}
	return m;
}

#else
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
						delete m;
						m = new Move;
						m->e = e; m->p = p; m->q = q;
						m->r = r; m->s = s;
					}
					bd[p][q] = c;
					bd[r][s] = '.';
				}
	return m;
}
#endif

bool cycle(char (*bd)[10],int *score)
{
	Move *bm = bestmove(bd);
	if(!bm)
	{
		return false;
	}
	else
	{
		takemove(bd,bm->p,bm->q,bm->r,bm->s);
		delete bm;
		int addscore = cancel(bd);
		*score += addscore;
		if(!addscore)
		{
			if(!addrand(bd))
				return false;
			else
				*score += cancel(bd);
		}
	}
	return true;
}

bool color = false;
bool add = false;
bool empty = false;

int main(int argc, char *argv[])
{
	int ch, index;
	while((ch = getopt(argc,argv,"caei:")) != -1)
	{
		switch(ch)
		{
		case 'c': color = true; break;
		case 'a': add = true; break;
		case 'e': empty = true; break;
		case 'i': index = atoi(optarg); break;
		}
	}
	FILE *f = fopen("param","r");
	if(f)
	{
		int tmp = fscanf(f,"%d%d%d%d%d%d%d%d%d",
				&a1,&a2,&a3,&a4,&a5,&a6,&a7,&a8,&a9);
		fclose(f);
	}
	char data[9][10];
	int score;
	if(empty)
	{
		for(int i = 0; i < 9; i++)
			for(int j = 0; j < 9; j++)
				data[i][j] = '.';
		score = 0;
	}
	else
	{
		if(!freadboard(stdin,data,&score))
		{
			printf("over\n%d\n",score);
			return 0;
		}
	}
	srand((unsigned)time(0)+index);
	if(add)
	{
		if(!addrand(data))
		{
			printf("over\n%d\n",score);
			return 0;
		}
	}
	if(!color)
	{
		while(cycle(data,&score))
			;
		printf("over\n%d\n",score);
	}
	else
	{
		Move *bm = bestmove(data);
		if(!bm)
		{
			printf("over\n%d\n",score);
		}
		else
		{
			printcolormove(data,bm->p,bm->q,bm->r,bm->s);
			delete bm;
		}
	}
	return 0;
}
