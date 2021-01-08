#include<iostream>
#include <cstdlib>
#include "temperature.h"
using namespace std;

#define lc(i) i*2+1
#define rc(i) i*2+2
#define parent(i) ((i-1)>>1)
#define isLc(i) (i-1)%2==0?1:0
#define isRc(i) (i-1)%2==1?1:0

template<typename T>
class vector {
private:
	int size;
	int capacity;
	T* element;
public:
	vector(int c) {
		element = new T[c];
		capacity = c;
		size = c;
	}
	vector() {
		element = new T[3];
		capacity = 3;
		size = 0;
	}
	void resize(int n) {
		element = new T[n];
		capacity = n;
		size = n;
	}
	int getSize() { return size; }
	T& operator[] (int r) { return element[r]; }
};

vector<int> merge(vector<int> a, vector<int> b) {//num��ʾ����Ҷ�ӵĺϲ�
	vector<int> n(a.getSize() + b.getSize());
	for (int i = 0; i < a.getSize(); i++) {
		n[i] = a[i];
		n[i + a.getSize()] = b[i];
	}
	return n;
}
void mergeLayer(vector<vector<int>>&y, int start, int end) {
	for (int i = start; i <= end; i++) {
		y[i] = merge(y[lc(i)], y[rc(i)]);
	}
}

int yrange(vector<int>y, int y1, int y2, int m) {
	int sum = 0;
	int lo = 0, hi = y.getSize();
	while (lo < hi) {
		int mi = (lo + hi) >> 1;
		y2 < mi ? hi = mi : lo = mi + 1;
	}
	int start = y1, end = --lo;
	int n = y.getSize() / m;      //merge�˼���Ҷ��
	for (int i = end; i >= start; i--) {
		for (int k = 0; k < n; k++) {
			sum += y[i + k * m];
		}
	}
	return sum;
}

//����һ����
vector<int> tree;
//����һ��ͬ����vector
vector<vector<int>> y;

void init(int n, int m, int**a) {
	int fullsize = 0;
	if ((n&(n - 1)) != 0) {
		int count = 0;
		int ntemp = n;
		while (ntemp >>= 1) count++;
		count++;
		fullsize = 1;
		while (count--) fullsize <<= 1;
	}
	else fullsize = n;
	//����һ����
	tree.resize(2 * fullsize - 1);      //��n��Ԫ�أ�ǰn-1��Ԫ��
										//����һ��ͬ����vector
	y.resize(2 * fullsize - 1);
	for (int i = fullsize - 1, j = 0; i < 2 * fullsize - 1; i++, j++) {
		//���Ҷ�ӽڵ�
		tree[i] = j;
		y[i].resize(m);
		for (int k = 0; k < m; k++) y[i][k] = (j < n) ? a[j + 1][k + 1] : 0;
		//������ṹ��ǰx-1���ڵ�
		int temp = i;
		while (isRc(temp) && parent(temp) > 0) temp = parent(temp);
		tree[parent(temp)] = j;
	}
	tree[tree.getSize() - 1] = fullsize - 1;    //���һ������д��
	y[2 * fullsize - 2].resize(m);
	for (int k = 0; k < m; k++) y[2 * fullsize - 2][k] = (fullsize - 1 < n) ? a[fullsize][k + 1] : 0;    

	int start = fullsize - 1;
	int end = tree.getSize() - 1;
	while (parent(start) != parent(end)) {     //parent(start)==parent(end)ʱ����root
		mergeLayer(y, parent(start), parent(end));
		start = parent(start);
		end = parent(end);
	}
	y[0] = merge(y[1], y[2]);   //root�������

}

int query(int x1, int y1, int x2, int y2) {
	x1--; x2--; y1--; y2--;
	//������������
	int i = 0;
	int lca = 0;
	int sum = 0;
	int num = ((x2 - x1) + 1)*((y2 - y1) + 1);
	int average = 0;
	//�ҵ�lca
	while (rc(i) <= tree.getSize()) {
		if ((x1 <= tree[i]) && (x2 <= tree[i])) {
			i = lc(i);
		}
		else if ((x1 > tree[i]) && (x2 > tree[i])) i = rc(i);
		else {
			break;
		}
	}
	lca = i;
	int report[100]{ 0 };    //��¼��Ҫһ���������index
	int index = 0;
	if (rc(lca) > tree.getSize()) {      //x1=x2���������
		report[index++] = lca;
	}
	else {
		int a1 = lc(lca);
		int a2 = rc(lca);

		while (a1 < tree.getSize() && a2 < tree.getSize()) {
			if (x1 <= tree[a1]) {
				report[index++] = (rc(a1) < tree.getSize()) ? rc(a1) : a1;
				a1 = lc(a1);
			}
			else a1 = rc(a1);

			if (x2 <= tree[a2]) {
				if (x2 == tree[a2] && lc(a2)>tree.getSize()) report[index++] = a2;    //���һ�����ж�
				a2 = lc(a2);
			}
			else {
				report[index++] = (lc(a2) < tree.getSize()) ? lc(a2) : a2;
				a2 = rc(a2);
			}
		}
	}

	//��y���м���
	for (int j = 0; j < index; j++) {
		sum += yrange(y[report[j]], y1, y2, y[y.getSize() - 1].getSize());
		average = int(sum / num);
	}
	return average;
}

void change(int x1, int y1, int temp) {
	x1--; y1--;
	int leaf = (y.getSize() + 1) / 2 - 1 + x1;        //leaf�����￪ʼ
	y[leaf][y1] = temp;
	//�����������д������
	while (parent(leaf) >= 0) {
		if (isLc(leaf)) y[parent(leaf)][y1] = temp;
		else y[parent(leaf)][y1 += y[leaf].getSize()] = temp;
		leaf = parent(leaf);
	}
}