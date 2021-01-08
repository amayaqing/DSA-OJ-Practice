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

//process2-- �ݹ鴦��ֻʣ������Ҫ�鲢�����
void process2(int*d, int*a, int*b, int&asize, int&bsize, int&ai, int&bi, int&k, int*max, int*min) {
	if (!(ai < asize) && (bi < bsize)) return;
	if (ai + 1 < asize) {
		compare(a[ai], a[ai + 1], b[bi], max, min);
		if (*max == b[bi]) {         //��a[ai]<a[ai+1]<b[bi]����a[ai]��a[ai+1]����ֱ�����
			d[k++] = a[ai++];
			d[k++] = a[ai++];
			process2(d, a, b, asize, bsize, ai, bi, k, max, min);
		}
		else if (*min == a[ai]) {   //��a[ai]<b[bi]<a[ai+1],��a[ai],b[bi]��������
			d[k++] = a[ai++];
			d[k++] = b[bi++];
			process2(d, b, a, bsize, asize, bi, ai, k, max, min);
		}
		else if (*min == b[bi]) {    //��b[bi]<a[ai]<a[ai+1],���޷��ж�a[ai]��b[bi+1]�Ĵ�С��ϵ����ֻ��b[bi]���
			d[k++] = b[bi++];
			process2(d, b, a, bsize, asize, bi, ai, k, max, min);
		}
	}
	else {     //ai+1>=asize�������ֱ�ӱȽ�a[ai],b[bi]
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
	//����
	int j = lo;
	for (int i = 0; i < asize; i++, j++) a[i] = d[j];
	for (int i = 0; i < bsize; i++, j++) b[i] = d[j];
	for (int i = 0; i < csize; i++, j++) c[i] = d[j];

	//�鲢
	int k = lo;    //��lo��ʼ��ԭ������и���
	int ai = 0, bi = 0, ci = 0, di = hmi;
	compare(a[ai], b[bi], c[ci], max, min);    //�Ƚ���һ��compare����ʼ��max,min

	while (k < hi) {

		//�ж�flag -- �м���������Χ
		int flag = 0;
		if (ai >= asize) flag++;
		if (bi >= bsize) flag++;
		if (ci >= csize) flag++;
		if (di >= hi) flag++;

		if (flag == 0) {    //��·����
			if (a[ai] == *max) compare(b[bi], c[ci], d[di], max, min);
			else if (b[bi] == *max) compare(a[ai], c[ci], d[di], max, min);
			else if (c[ci] == *max) compare(a[ai], b[bi], d[di], max, min);
			else compare(a[ai], b[bi], c[ci], max, min);

			if (*min == a[ai]) d[k++] = a[ai++];
			else if (*min == b[bi]) d[k++] = b[bi++];
			else if (*min == c[ci]) d[k++] = c[ci++];
			else d[k++] = d[di++];

		}
		else if (flag == 1) {     //ֻ����·
			if (ai >= asize) compare(b[bi], c[ci], d[di], max, min);
			if (bi >= bsize) compare(a[ai], c[ci], d[di], max, min);
			if (ci >= csize) compare(a[ai], b[bi], d[di], max, min);
			if (di >= hi) compare(a[ai], b[bi], c[ci], max, min);

			if (*min == a[ai]) d[k++] = a[ai++];
			else if (*min == b[bi]) d[k++] = b[bi++];
			else if (*min == c[ci]) d[k++] = c[ci++];
			else d[k++] = d[di++];
		}
		else if (flag == 2) {    //ֻ����·
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
		else {     //ֻ��һ·
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
	//���ֵݹ�� -- ��3,2,1��Ԫ��
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