#include<iostream>
#include<cstdio>
using namespace std;

#define lc(i) i*2+1
#define rc(i) i*2+2
#define parent(i) ((i-1)>>1)
#define isLc(i) (i-1)%2==0?1:0
#define isRc(i) (i-1)%2==1?1:0


struct node {
	int index;
	int data;
	node() {
		index = 0;
		data = 0;
	}
};

class vector {
private:
	int size;
	int capacity;
	node* element;
public:
	vector(int c) {
		element = new node[c];
		capacity = c;
		size = c;
	}
	vector() {
		element = new node[3];
		capacity = 3;
		size = 0;
	}
	void resize(int n) {
		element = new node[n];
		capacity = n;
		size = n;
	}
	int getSize() { return size; }
	node& operator[] (int r) { return element[r]; }
	void operator()(int t) {
		for (int i = 0; i < size; i++) element[i].data = t;
	}
};

vector a;

void init(int*num, int n) {
	
	//�ҵ���n�����2^n����������
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

	a.resize(2 * fullsize - 1);
	a(0);

	int k = fullsize - 1, p = 0;
	for (k, p; k < 2 * fullsize - 2; k++, p++) {
		//��index
		a[k].index = p;
		//ͬʱ����ǰ���index
		int temp = k;
		while (isRc(temp) && parent(temp) > 0) temp = parent(temp);
		a[parent(temp)].index = p;
		//��data
		a[k].data = (p < n) ? num[p + 1] : 0;
	}
	//���һλ��������
	a[k].index = p;
	a[k].data = (p < n) ? num[p + 1] : 0;
	//ͳһ����ǰ���data
	for (int w = fullsize - 2; w >= 0; w--) {
		a[w].data = a[lc(w)].data + a[rc(w)].data;
	}
}

int query(int x1, int x2) {
	x1--; x2--;

	int i = 0;
	int lca = 0;
	int sum = 0;

	while (rc(i) <= a.getSize()) {
		if ((x1 <= a[i].index) && (x2 <= a[i].index)) {
			i = lc(i);
		}
		else if ((x1 > a[i].index) && (x2 > a[i].index)) i = rc(i);
		else {
			break;
		}
	}
	lca = i;

	int index = 0;
	if (rc(lca) > a.getSize()) {      //x1=x2���������
		sum += a[lca].data;
	}
	else {
		int a1 = lc(lca);
		int a2 = rc(lca);

		while (a1 < a.getSize() && a2 < a.getSize()) {
			if (x1 <= a[a1].index) {
				sum+= (rc(a1) < a.getSize()) ? a[rc(a1)].data : a[a1].data;
				a1 = lc(a1);
			}
			else a1 = rc(a1);

			if (x2 <= a[a2].index) {
				
				if (x2 == a[a2].index && lc(a2) > a.getSize()){
					//���һ�����ж�
					sum += a[a2].data;
				}
				a2 = lc(a2);
			}
			else {
				sum+= (lc(a2) < a.getSize()) ? a[lc(a2)].data : a[a2].data;
				a2 = rc(a2);
			}
		}
	}
	return sum;
}

void change(int x1, int temp) {
	x1--;
	int xleaf = (a.getSize() + 1) / 2 - 1 + x1;        //xleaf�����￪ʼ
	int diff = temp - a[xleaf].data;

	//����һ���ڵ�
	a[xleaf].data += diff;
	//����������
	while (parent(xleaf) >= 0) {
		a[parent(xleaf)].data += diff;
		xleaf = parent(xleaf);
	}
}

int main() {
	int n, k;
	scanf("%d%d", &n, &k);

	int *a = new int [n + 1];
	for (int i = 1; i <= n; i++) {
		scanf("%d", &a[i]);
	}

	init(a, n);

	for (int i = 0; i < k; i++) {
		int op;
		scanf("%d", &op);
		if (op == 0) {
			int x, temp;
			scanf("%d%d", &x,  &temp);
			change(x, temp);
		}
		else {
			int x1, x2;
			scanf("%d%d", &x1, &x2);
			printf("%d\n", query(x1, x2));
		}
	}

	delete[] a;

	return 0;
}