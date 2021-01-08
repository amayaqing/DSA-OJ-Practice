#include<iostream>
#include<cstdlib>
#include"sort.h"

int a = 0, b = 0;
int*min = &a;
int*max = &b;

inline void swap(int &a, int &b) {
	int c = a;
	a = b;
	b = c;
}

//process2-- 递归处理只剩两组需要归并的情况
void process2(int*d, int*a, int*b, int&asize, int&bsize, int&ai, int&bi, int&k, int*max, int*min) {
	if (!(ai < asize) && (bi < bsize)) return;
	if (ai + 1 < asize) {
		compare(a[ai], a[ai + 1], b[bi], max, min);
		if (*max == b[bi]) {         //若a[ai]<a[ai+1]<b[bi]，则a[ai]和a[ai+1]都可直接入队
			d[k++] = a[ai++];
			d[k++] = a[ai++];
			process2(d, a, b, asize, bsize, ai, bi, k, max, min);
		}
		else if (*min == a[ai]) {   //若a[ai]<b[bi]<a[ai+1],则a[ai],b[bi]可相继入队
			d[k++] = a[ai++];
			d[k++] = b[bi++];
			process2(d, b, a, bsize, asize, bi, ai, k, max, min);
		}
		else if (*min == b[bi]) {    //若b[bi]<a[ai]<a[ai+1],则无法判断a[ai]和b[bi+1]的大小关系，则只将b[bi]入队
			d[k++] = b[bi++];
			process2(d, b, a, bsize, asize, bi, ai, k, max, min);
		}
	}
	else {     //ai+1>=asize的情况，直接比较a[ai],b[bi]
		compare(a[ai], a[ai], b[bi], max, min);
		if (*min == a[ai]) d[k++] = a[ai++];
		else d[k++] = b[bi++];
	}
}

void merge(int*d, int lo, int lmi, int mi, int hmi, int hi) {
	int asize = lmi - lo, bsize = mi - lmi, csize = hmi - mi;
	int*a = new int[asize];
	int*b = new int[bsize];
	int*c = new int[csize];
	//复制
	int j = lo;
	for (int i = 0; i < asize; i++, j++) a[i] = d[j];
	for (int i = 0; i < bsize; i++, j++) b[i] = d[j];
	for (int i = 0; i < csize; i++, j++) c[i] = d[j];

	//归并
	int k = lo;    //从lo开始对原数组进行更新
	int ai = 0, bi = 0, ci = 0, di = hmi;
	compare(a[ai], b[bi], c[ci], max, min);    //先进行一次compare，初始化max,min

	while (k < hi) {

		//判断flag -- 有几个超出范围
		int flag = 0;
		if (ai >= asize) flag++;
		if (bi >= bsize) flag++;
		if (ci >= csize) flag++;
		if (di >= hi) flag++;

		if (flag == 0) {    //四路都在
			if (a[ai] == *max) compare(b[bi], c[ci], d[di], max, min);
			else if (b[bi] == *max) compare(a[ai], c[ci], d[di], max, min);
			else if (c[ci] == *max) compare(a[ai], b[bi], d[di], max, min);
			else compare(a[ai], b[bi], c[ci], max, min);

			if (*min == a[ai]) d[k++] = a[ai++];
			else if (*min == b[bi]) d[k++] = b[bi++];
			else if (*min == c[ci]) d[k++] = c[ci++];
			else d[k++] = d[di++];

		}
		else if (flag == 1) {     //只有三路
			if (ai >= asize) compare(b[bi], c[ci], d[di], max, min);
			if (bi >= bsize) compare(a[ai], c[ci], d[di], max, min);
			if (ci >= csize) compare(a[ai], b[bi], d[di], max, min);
			if (di >= hi) compare(a[ai], b[bi], c[ci], max, min);

			if (*min == a[ai]) d[k++] = a[ai++];
			else if (*min == b[bi]) d[k++] = b[bi++];
			else if (*min == c[ci]) d[k++] = c[ci++];
			else d[k++] = d[di++];
		}
		else if (flag == 2) {    //只有两路
			if (ai >= asize) {
				if (bi >= bsize) process2(d, c, d, csize, hi, ci, di, k, max, min);
				else if (ci >= csize) process2(d, b, d, bsize, hi, bi, di, k, max, min);
				else if (di >= hi) process2(d, b, c, bsize, csize, bi, ci, k, max, min);
			}
			else if (bi >= bsize) {
				if (ci >= csize) process2(d, a, d, asize, hi, ai, di, k, max, min);
				else if (di >= hi) process2(d, a, c, asize, csize, ai, ci, k, max, min);
			}
			else {
				process2(d, a, b, asize, bsize, ai, bi, k, max, min);
			}
		}
		else {     //只有一路
			*min = (ai >= asize) ? *min : a[ai];
			*min = (bi >= bsize) ? *min : b[bi];
			*min = (ci >= csize) ? *min : c[ci];
			*min = (di >= hi) ? *min : d[di];

			if (*min == a[ai]) d[k++] = a[ai++];
			else if (*min == b[bi]) d[k++] = b[bi++];
			else if (*min == c[ci]) d[k++] = c[ci++];
			else d[k++] = d[di++];
		}
	}
}

void mergeSort(int*a, int lo, int hi) {
	//三种递归基 -- 余3,2,1个元素
	if (hi - lo < 2) return;
	else if (hi - lo == 2) {
		compare(a[lo], a[lo], a[lo + 1], max, min);
		if (*min == a[lo + 1]) swap(a[lo], a[lo + 1]);
		return;
	}
	else if (hi - lo == 3) {
		compare(a[lo], a[lo + 1], a[lo + 2], max, min);
		if (*min == a[lo]) {
			if (*max == a[lo + 1]) swap(a[lo + 1], a[lo + 2]);
		}
		else if (*min == a[lo + 1]) {
			swap(a[lo], a[lo + 1]);
			if (a[lo + 1] == *max) swap(a[lo + 1], a[lo + 2]);
		}
		else {
			swap(a[lo], a[lo + 2]);
			if (a[lo + 1] == *max) swap(a[lo + 1], a[lo + 2]);
		}
		return;
	}
	else {
		int mi = (lo + hi) / 2;
		int lmi = (lo + mi) / 2;
		int hmi = (mi + hi) / 2;
		mergeSort(a, lo, lmi);
		mergeSort(a, lmi, mi);
		mergeSort(a, mi, hmi);
		mergeSort(a, hmi, hi);
		merge(a, lo, lmi, mi, hmi, hi);
	}
}

void sort(int n, int limit, int *a) {
	mergeSort(a, 0, n);
}