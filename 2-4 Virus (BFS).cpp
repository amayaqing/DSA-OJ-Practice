#include<iostream>
#include<cstdio>
#include<string>
using namespace std;

#define INT_MAX 2147483647

typedef enum { UNDISCOVERED, VISITED }vstatus;
struct point {
	int data;    //记录是0/1
	vstatus status;
	int priority;   //记录感染时间
	int x;    //记录在数组中的位置
	int y;    //记录在数组中的位置
	point() {
		data = 1;
		status = UNDISCOVERED;
		priority = INT_MAX;
	}
	point(int d) {
		data = d;
		status = UNDISCOVERED;
		priority = INT_MAX;
	}
};

#define ListNodePosi ListNode* 
struct ListNode {
	point data; 
	ListNodePosi pred;
	ListNodePosi succ;
	ListNode() {}
	ListNode(point e, ListNodePosi p, ListNodePosi s) {
		data = e;
		pred = p;
		succ = s;
	}
	ListNodePosi insertAsPred(const point& e) {
		ListNodePosi x = new ListNode(e, pred, this);
		pred->succ = x; pred = x;
		return x;
	}
	ListNodePosi insertAsSucc(const point& e) {
		ListNodePosi x = new ListNode(e, this, succ);
		succ->pred = x; succ = x;
		return x;
	}
};

class List {
private:
	int size; 
	ListNodePosi header; 
	ListNodePosi trailer;

public:
	List() {
		header = new ListNode;
		trailer = new ListNode;
		header->succ = trailer; header->pred = NULL;
		trailer->pred = header; trailer->succ = NULL;
		size = 0;
	}
	bool empty() { return size <= 0; }
	ListNodePosi first(){ return header->succ; }
	ListNodePosi insertAsLast(const point& e) {
		size++; 
		return trailer->insertAsPred(e);
	}
	void remove(ListNodePosi p) {
		p->pred->succ = p->succ; p->succ->pred = p->pred;
		delete p;
		size--;
	}
}; 

class Queue : public List {
public:
	void enqueue(const point& e) { insertAsLast(e); }
	point dequeue() { point p = first()->data; remove(first()); return p; }
	point& front() { return first()->data; }
};

int bfs(point**a, int n, int m, Queue &q) {
	int sum = 0;
	while (!q.empty()) {
		point v = q.dequeue();
		int si = v.x, sj = v.y;

		//遍历前后左右邻居，记录其感染时间
		if (0 <= si - 1 && a[si - 1][sj].status == UNDISCOVERED) {
			a[si - 1][sj].priority = a[si][sj].priority + 1;
			sum += a[si - 1][sj].priority;
			a[si - 1][sj].status = VISITED;
			q.enqueue(a[si - 1][sj]);
		}
		if (si + 1<n && a[si + 1][sj].status == UNDISCOVERED) {
			a[si + 1][sj].priority = a[si][sj].priority + 1;
			sum += a[si + 1][sj].priority;
			a[si + 1][sj].status = VISITED;
			q.enqueue(a[si + 1][sj]);
		}
		if (0 <= sj - 1 && a[si][sj - 1].status == UNDISCOVERED) {
			a[si][sj - 1].priority = a[si][sj].priority + 1;
			sum += a[si][sj - 1].priority;
			a[si][sj - 1].status = VISITED;
			q.enqueue(a[si][sj - 1]);
		}
		if (sj + 1<m && a[si][sj + 1].status == UNDISCOVERED) {
			a[si][sj + 1].priority = a[si][sj].priority + 1;
			sum += a[si][sj + 1].priority;
			a[si][sj + 1].status = VISITED;
			q.enqueue(a[si][sj + 1]);
		}
	}
	return sum;
}

int main() {
	int n, m;
	cin >> n;
	cin >> m;
	point**a = new point*[n];
	Queue q;

	char ch = getchar();
	for (int i = 0; i < n; i++) {
		string s;
		ch = getchar();
		while (ch != '\n') {
			s += ch;
			ch = getchar();
		}
		//初始化point数组，并将所有病毒节点入队
		a[i] = new point[m];
		for (int j = 0; j < m; j++) {
			a[i][j].data = int(s[j] - 48);
			a[i][j].status = UNDISCOVERED;
			a[i][j].priority = INT_MAX;
			a[i][j].x = i; a[i][j].y = j;

			//病毒节点全部入队
			if (a[i][j].data == 0) {
				a[i][j].status = VISITED;
				a[i][j].priority = 0;
				q.enqueue(a[i][j]);
			}
		}
	}

	//计算
	int sum = 0;
	sum = bfs(a, n, m, q);
	printf("%d\n", sum);
	return 0;
}