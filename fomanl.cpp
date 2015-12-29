#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <cassert>
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
					e += connectgain(bd,i,j,s,t,c) * a12;
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
		int tmp = fscanf(f,"%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
				&a1,&a2,&a3,&a4,&a5,&a6,&a7,&a8,&a9,&a10,&a11,&a12,&a13,&a14);
		fclose(f);
	}
	char state[256];
	char data[9][10];
	int score;
	int tmp = scanf("%s%d",state,&score);
	if(strcmp(state,"over")==0)
	{
		printf("over\n%d\n",score);
		return 0;
	}
	for(int i = 0; i < 9; i++)
		int tmp = scanf("%s",data[i]);
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
