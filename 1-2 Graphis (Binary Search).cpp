#include<iostream>
#include<cstdio>
using namespace std;


class point {
public:
	int x;
	int y;
	point(int a, int b) { x = a; y = b; }
	point() { x = 0; y = 0; }
};

bool toleft(int x, int y, point r) {
	long long mul = (long long)x*(long long)r.y + (long long)y*(long long)r.x - (long long)y*(long long)x;
	return mul >= 0 ? true : false;
}

void merge(int a[], int lo, int mi, int hi) {
	int lb = mi - lo;
	int lc = hi - mi;
	int *b = new int[lb];
	for (int i = lo, j = 0; j < lb; i++) b[j++] = a[i];
	int *c = a + mi;
	for (int i = lo, j = 0, k = 0; j < lb;) {
		a[i++] = (k >= lc) || (b[j] <= c[k]) ? b[j++] : c[k++];
	}
}
void mergeSort(int a[], int lo, int hi) {
	int mi = (lo + hi) / 2;
	if (hi - lo < 2) return;
	//分解
	mergeSort(a, lo, mi);
	mergeSort(a, mi, hi);
	//合并
	merge(a, lo, mi, hi);
}

int check(point r, int*x, int*y, int lo, int hi) {
	while (lo < hi) {
		int mi = (lo + hi) >> 1;
		toleft(x[mi], y[mi], r) ? lo = mi + 1 : hi = mi;
	}
	return lo;
}

int main()
{
	int n;
	scanf("%d", &n);
	int*x = new int[n];
	int*y = new int[n];
	for (int i = 0; i < n; i++) scanf("%d", &x[i]);
	for (int i = 0; i < n; i++) scanf("%d", &y[i]);

	int m;
	scanf("%d", &m);
	point*r = new point[m];
	int*result = new int[m];
	for (int i = 0; i < m; i++) {
		scanf("%d%d", &r[i].x, &r[i].y);
	}
	mergeSort(x, 0, n);
	mergeSort(y, 0, n);

	//output
	for (int i = 0; i < m; i++) {
		result[i] = check(r[i], x, y, 0, n);
		printf("%d\n", result[i]);
	}
	return 0;
}