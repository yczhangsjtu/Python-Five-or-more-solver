#ifndef __UTIL_H__
#define __UTIL_H__

typedef struct Point {
	int i,j;
	Point(){}
	Point(int a,int b):i(a),j(b){}
} Point;

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
void printcolormove(char (*bd)[10], int p, int q, int r, int s);
void printmove(char (*bd)[10], int p, int q, int r, int s);
int gain(char (*bd)[10], int p, int q, int s, int t, char c);
int connectgain(char (*bd)[10], int p, int q, int s, int t, char c);
Out evalout(char (*bd)[10], int p, int q);
void evalout(char (*bd)[10], Need (*nd)[9]);
int ctoi(char c);

#endif
