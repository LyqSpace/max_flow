#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<cstdlib>
#include<string>
#include<queue>
#include<vector>
#include<algorithm>
using namespace std;

const int INF = 0xfffffff;

int n, m, cnt;

struct typeEdge {
	int x, y, w, next;
};

inline void addEdge( vector<int> &edgeHead, vector<typeEdge> &edge, int x, int y, int w) {

	typeEdge oneEdge;
	oneEdge.x = x;
	oneEdge.y = y;
	oneEdge.w = w;
	oneEdge.next = edgeHead[x];
	edgeHead[x] = edge.size();
	edge.push_back( oneEdge );

}
bool bfsGraphForward( vector<int> &tag, vector<int> &edgeHead, vector<typeEdge> &edge ) {

	int N = edgeHead.size();
	queue<int> que;
	tag = vector<int>( N, -1 );
	tag[0] = 0;
	que.push( 0 );
	while ( !que.empty() ) {

		int nowP = que.front();
		que.pop();
		for ( int p = edgeHead[nowP]; p != -1; p = edge[p].next ) {

			int nextP = edge[p].y;
			if ( tag[nextP] == -1 && edge[p].w > 0 ) {
				tag[nextP] = tag[nowP] + 1;
				que.push( nextP );
			}
		}
	}

	if ( tag[N - 1] > 0 ) {
		return 1;
	} else {
		return 0;
	}
}
void bfsGraphBackward( vector<int> &tag, vector<int> &edgeHead, vector<typeEdge> &edge ) {

	int N = edgeHead.size();
	queue<int> que;
	tag = vector<int>( N, -1 );
	tag[N - 1] = 0;
	que.push( N - 1 );
	while ( !que.empty() ) {

		int nowP = que.front();
		que.pop();
		for ( int p = edgeHead[nowP]; p != -1; p = edge[p].next ) {

			int nextP = edge[p].y;
			if ( tag[nextP] == -1 ) {
				que.push( nextP );
				tag[nextP] = tag[nowP] + 1;
			}
		}
	}
}
int dfsDinic( int nowP, int minFlow, vector<int> &tag, vector<int> &edgeHead, vector<typeEdge> &edge ) {

	//cout << nowP << " " << tag[nowP] << " " << minFlow << endl;
	if ( minFlow == 0 ) return 0;
	if ( nowP == tag.size() - 1 ) return minFlow;
	int allFlow = 0;
	for ( int p = edgeHead[nowP]; p != -1; p = edge[p].next ) {

		int nextP = edge[p].y;
		if ( tag[nextP] != tag[nowP] + 1 ) continue;
		if ( edge[p].w <= 0 ) continue;

		int flow = dfsDinic( nextP, min( minFlow, edge[p].w ), tag, edgeHead, edge );
		if ( flow > 0 ) {
			edge[p].w -= flow;
			edge[p ^ 1].w += flow;
			allFlow += flow;
			minFlow -= flow;
		}
	}
	if ( allFlow == 0 ) tag[nowP] = -1;
	return allFlow;
}

void SAP( vector<int> &edgeHead, vector<typeEdge> &edge) {


	int N = edgeHead.size();
	int S = 0, T = N - 1;
	vector<int> curEdge = edgeHead;
	vector<int> gap( N + 2, 0 );
	vector<int> tag;
	vector<int> preEdge( N, 0 );

	bfsGraphBackward( tag, edgeHead, edge );
	for ( int i = 0; i < N; i++ ) gap[tag[i]]++;

	int nowP = S;
	int ans = 0;

	while ( tag[S] < N ) {

		if ( nowP == T ) {

			int neckFlow = INF;
			int neckPoint;

			for ( int i = S; i != T; i = edge[curEdge[i]].y ) {

				if ( neckFlow > edge[curEdge[i]].w ) {
					neckFlow = edge[curEdge[i]].w;
					neckPoint = i;
				}
			}
			for ( int i = S; i != T; i = edge[curEdge[i]].y ) {

				edge[curEdge[i]].w -= neckFlow;
				edge[curEdge[i] ^ 1].w += neckFlow;
			}

			ans += neckFlow;
			nowP = neckPoint;
		}

		int p0 = -1;
		for ( int p = curEdge[nowP]; p != -1; p = edge[p].next ) {

			if ( edge[p].w > 0 && tag[nowP] == tag[edge[p].y] + 1 ) {
				p0 = p;
				break;
			}
		}

		if ( p0 != -1 ) {

			curEdge[nowP] = p0;
			preEdge[edge[p0].y] = nowP;
			nowP = edge[p0].y;
		} else {

			gap[tag[nowP]]--;
			if ( gap[tag[nowP]] == 0 ) break;
			curEdge[nowP] = edgeHead[nowP];

			int tempTag = N;
			for ( int p = edgeHead[nowP]; p != -1; p = edge[p].next ) {
				if ( edge[p].w > 0 ) tempTag = min( tempTag, tag[edge[p].y] );
			}
			tag[nowP] = tempTag + 1;
			gap[tag[nowP]]++;
			if ( nowP != S ) nowP = preEdge[nowP];
		}
	}

	cout << "SAP " << ans << endl;
}

void Dinic( vector<int> &edgeHead, vector<typeEdge> &edge ) {

	long long ans = 0;
	vector<int> tag;

	while ( bfsGraphForward( tag, edgeHead, edge ) ) {

		int tans = 1;
		while ( tans > 0 ) {

			tans = dfsDinic( 0, 0x7fffffff, tag, edgeHead, edge );
			ans += tans;
		}
	}

	cout << "Dinic " << ans << endl;
}
void work1(FILE *file) {
	

	fscanf( file, "%d%d", &n, &m );
	vector<int> edgeHead( n, -1 );
	vector<typeEdge> edge;
	int u, v, w;
	for ( int i = 0; i < m; i++ ) {
		fscanf( file, "%d%d%d", &u, &v, &w );
		addEdge( edgeHead, edge, u, v, w );
		addEdge( edgeHead, edge, v, u, 0 );
	}
	SAP( edgeHead, edge );
}
void work2(FILE *file) {
	
	fscanf( file, "%d%d", &n, &m );
	vector<int> edgeHead( n, -1 );
	vector<typeEdge> edge;
	int u, v, w;
	for ( int i = 0; i < m; i++ ) {
		fscanf( file, "%d%d%d", &u, &v, &w );
		addEdge( edgeHead, edge, u, v, w );
		addEdge( edgeHead, edge, v, u, 0 );
	}
	Dinic( edgeHead, edge );
}
int main() {

	FILE *file;
	
	file= fopen( "graph0.txt", "r" );
	work1(file);
	fclose( file );
	
	file = fopen( "graph0.txt", "r" );
	work2(file);
	fclose( file );
	
	system( "pause" );
	return 0;
}