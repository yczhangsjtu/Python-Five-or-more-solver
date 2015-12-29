#ifndef __UTIL_H__
#define __UTIL_H__

#include <cstdio>

typedef struct Point {
	int i,j;
	Point(){}
	Point(int a,int b):i(a),j(b){}
} Point;

inline bool operator<(Point p1, Point p2)
{
	if(p1.i < p2.i) return true;
	if(p1.i > p2.i) return false;
	return p1.j < p2.j;
}

typedef struct Move {
	int e,p,q,r,s;
} Move;

typedef struct Out {
	int out[7];
} Need;

int __eval(char (*bd)[10]);
bool connected(char (*bd)[10], int p, int q, int r, int s);
int numofarea(char (*bd)[10], int p, int q);
void getavail(char (*bd)[10], int p, int q, Point *buf, int *s);
void takemove(char (*bd)[10], int p, int q, int r, int s);
void printboard(char (*bd)[10]);
void printcolormove(char (*bd)[10], int p, int q, int r, int s);
void printmove(char (*bd)[10], int p, int q, int r, int s);
bool freadboard(FILE* f,char (*bd)[10], int *score);
int gain(char (*bd)[10], int p, int q, int s, int t, char c);
int connectgain(char (*bd)[10], int p, int q, int s, int t, char c);
Out evalout(char (*bd)[10], int p, int q);
void evalout(char (*bd)[10], Need (*nd)[9]);
int ctoi(char c);
bool addrand(char (*bd)[10]);
int cancel(char (*bd)[10]);

#endif
