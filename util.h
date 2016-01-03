#ifndef __UTIL_H__
#define __UTIL_H__

#include <cstdio>

using namespace std;

extern int maxBranch;

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
	int e,f,l,h,p,q,r,s;
	Move():f(-1),e(-1),l(0),h(10),p(-1),q(-1),r(-1),s(-1){}
	Move(int P,int Q, int R, int S):e(-1),l(0),h(10),p(P),q(Q),r(R),s(S){}
	Move(int E,int F,int P,int Q, int R, int S):e(E),f(F),l(0),h(10),p(P),q(Q),r(R),s(S){}
} Move;

inline bool operator<(const Move & m1, const Move & m2)
{
	if(m1.h > m2.h) return true;
	if(m1.h < m2.h) return false;
	if(m1.f < m2.f) return true;
	if(m1.f > m2.f) return false;
	return m1.e < m2.e;
	return m1.e > m2.e;
}

int evalfive(char c1, char c2, char c3, char c4, char c5);
void evalfives(char (*bd)[10], int &e, int &f);
bool connected(char (*bd)[10], int p, int q, int r, int s);
int numofarea(char (*bd)[10], int p, int q);
void getavail(char (*bd)[10], int p, int q, Point *buf, int *s);
void takemove(char (*bd)[10], int p, int q, int r, int s);
void printboard(char (*bd)[10]);
void printcolormove(char (*bd)[10], int p, int q, int r, int s);
void printmove(char (*bd)[10], int p, int q, int r, int s);
bool freadboard(FILE* f,char (*bd)[10], int *score);
int gain(char (*bd)[10], int p, int q, int s, int t, char c);
int ctoi(char c);
bool addrand(char (*bd)[10]);
int cancel(char (*bd)[10]);

#endif
