#include <cstdio>

#include <cassert>

#include <vector>

#include <algorithm>

using namespace std;



typedef long long INT;



#define MOD 1000000007

#define NN 210000



inline INT M (INT x, INT y) {

	return x * y % MOD;

}



INT f[NN];

int vis[NN], deg[NN], dep[NN], size[NN], last[NN], N;

vector<int> g[NN];



void init (int x, int ax = 0) {

	for (int i = 0; i < deg[x]; ++i) {

		if (g[x][i] == ax) { swap(g[x][i], g[x][deg[x] - 1]); break; }

	}

	++size[x];

	dep[x] = dep[ax] + 1;

	for (int i = 0, ie = ax ? deg[x] - 1 : deg[x]; i < ie; ++i) {

		init(g[x][i], x);

		size[x] += size[g[x][i]];

		if (! last[x] && last[g[x][i]]) last[x] = last[g[x][i]];

	}

	if (deg[x] != 2) last[x] = x;

}



INT calc (int, int) ;



INT calc (int x) {

	if (vis[x]) return f[x];

	vis[x] = 1;

	INT &res = f[x]; res = 0;

	if (size[x] % 2 == 1) return 0;

	if (deg[last[x]] == 1) return res = size[x] / 2;

	if (deg[x] == 2) {

		if (deg[g[x][0]] == 1) res ++;

		else if (deg[g[x][0]] == 2) res += calc(g[g[x][0]][0]);

	}

	int y = last[x], l = g[y][0], r = g[y][1], d = dep[y] - dep[x];

	for (int i = 0; i < 2; ++i) {

		if (deg[last[l]] == 1 && d - 1 == size[l]) res += calc(r), res %= MOD;

		swap(l, r);

	}

	for (int i = 0; i < 2; ++i) {

		if (deg[last[l]] == 1 && d + 1 == size[l]) {

			res += calc(r), res %= MOD;

		}

		if (y == x && deg[l] == 2) {

			res += calc(r, g[l][0]), res %= MOD;

		}

		if (deg[l] == 3) {

			int a = g[l][0], b = g[l][1];

			for (int j = 0; j < 2; ++j) {

				if (deg[last[a]] == 1 && d == size[a])

					res += calc(b, r), res %= MOD;

				swap(a, b);

			}

		}

		swap(l, r);

	}

	return res %= MOD;

}



INT calc (int x, int y) {

	if ((size[x] + size[y]) % 2 == 1) return 0;

	if (deg[x] == 1 && deg[y] == 1) return 1;

	if (deg[x] > 2 || deg[y] > 2) return 0;

	if (deg[x] == 2 && deg[y] == 2) return calc(g[x][0], g[y][0]);

	if (deg[x] == 2 && deg[y] == 1) return calc(g[x][0]);

	if (deg[x] == 1 && deg[y] == 2) return calc(g[y][0]);

}



void solve () {

	int rt = -1;

	for (int i = 1; i <= N; ++i) {

		if (deg[i] > 3) {

			printf("0\n");

			return;

		}

		if (deg[i] == 3) {

			rt = i;

		}

	}

	if (rt == -1) {

		printf("%I64d\n", N == 2 ? 2 : (1LL * N * N / 2 - N + 4) % MOD);

		return;

	}

	init (rt);

	int l[3] = {g[rt][0], g[rt][1], g[rt][2]};

	sort(l, l + 3);

	INT res = 0;

	do {

		int p = l[1];

		if (deg[p] == 1) {

			res += M(calc(l[0]), calc(l[2]));

		}

		else if (deg[p] == 2) {

			res += M(calc(l[0]), calc(l[2], g[p][0])) + M(calc(l[0], g[p][0]), calc(l[2]));

		}

		else {

			res += M(calc(l[0], g[p][0]), calc(l[2], g[p][1])) + 

			       M(calc(l[0], g[p][1]), calc(l[2], g[p][0]));

		}

		res %= MOD;

	} while (next_permutation(l, l + 3));

	res = (res * 2 % MOD + MOD) % MOD;

	printf("%I64d\n", res);

}



int main () {

	scanf("%d", &N); N *= 2;

	for (int i = 1, a, b; i < N; ++i) {

		scanf("%d%d", &a, &b);

		g[a].push_back(b); ++deg[a];

		g[b].push_back(a); ++deg[b];

	}

	solve();

	return 0;

}