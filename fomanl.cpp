#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <cassert>
#include <ctime>
#include <set>
#include "util.h"

using namespace std;

int a1 = 0;
int a2 = 0;
int a3 = 0;
int a4 = 0;
int a5 = 0;
int a6 = 0;
int a7 = 1;
int a8 = 100;
int a9 = 1000;
int a10 = 10000;
int a11 = 100000;
int maxBranch = 3;
char colors[7] = {'r','g','b','y','o','p','d'};

Move *bestmove(char (*bd)[10], int depth)
{
	set<Move> mvs;
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
					int e,f;
					int s = buf[k].i, t = buf[k].j;
					bd[i][j] = '.';
					bd[s][t] = c;
					evalfives(bd,e,f);
					Move m(e,f,i,j,s,t);
					if(f == 10) m.h = 0;
					mvs.insert(m);
					if(mvs.size() > maxBranch) mvs.erase(mvs.begin());
					bd[i][j] = c;
					bd[s][t] = '.';
				}
			}
		}
	if(depth > 0)
	{
		set<Move>::iterator *buf = new set<Move>::iterator[maxBranch];
		int s = 0;
		for(set<Move>::iterator iter = mvs.begin();
				iter != mvs.end(); iter++)
			buf[s++] = iter;
		for(int i = 0; i < s; i++)
		{
			set<Move>::iterator iter = buf[i];
			Move m = *iter;
			if(m.h < 10) continue;
			char c = bd[m.p][m.q];
			assert(bd[m.r][m.s] == '.');
			bd[m.p][m.q] = '.';
			bd[m.r][m.s] = c;

			Move * bm = bestmove(bd,depth-1);
			if(bm && bm->h < 10)
			{
				m.h = bm->h+1;
				mvs.erase(iter);
				mvs.insert(m);
			}
			if(bm) delete bm;

			bd[m.p][m.q] = c;
			bd[m.r][m.s] = '.';
		}
		delete []buf;
	}
	if(mvs.size() > 0)
	{
		Move *mv = new Move;
		*mv = *mvs.rbegin();
		return mv;
	}
	return NULL;
}

Move *bestmove(char (*bd)[10])
{
	return bestmove(bd,5);
}

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
	bool onlymove = false;
	while((ch = getopt(argc,argv,"caei:m")) != -1)
	{
		switch(ch)
		{
		case 'c': color = true; break;
		case 'a': add = true; break;
		case 'e': empty = true; break;
		case 'i': index = atoi(optarg); break;
		case 'm': onlymove = true; break;
		}
	}
	/*
	FILE *f = fopen("param","r");
	if(f)
	{
		int tmp = fscanf(f,"%d%d%d%d%d%d%d%d%d",
				&a1,&a2,&a3,&a4,&a5,&a6,&a7,&a8,&a9);
		fclose(f);
	}
	*/
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
	Move *bm = bestmove(data);
	if(!bm)
	{
		printf("over\n%d\n",score);
	}
	else
	{
		if(!onlymove) {
			if(!color)
				printmove(data,bm->p,bm->q,bm->r,bm->s);
			else
				printcolormove(data,bm->p,bm->q,bm->r,bm->s);
		} else {
			printf("%s\n%d\n%d\n%d\n%d\n%d\n","normal",score,bm->p,bm->q,bm->r,bm->s);
		}
		delete bm;
	}
	return 0;
}
