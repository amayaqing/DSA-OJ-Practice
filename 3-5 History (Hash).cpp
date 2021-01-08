#include<iostream>
using namespace std;

#define d 500009
#define hash(n) n%d

inline int read() {
	int x = 0, f = 1;
	char c = getchar();
	while (c < '0' || c > '9') { if (c == '-') f = -1; c = getchar(); }
	while (c >= '0' && c <= '9') { x = x * 10 + c - '0'; c = getchar(); }
	return x * f;
}
#define in(a) a=read()

struct point {
	int id;      //ѧ��ID
	int time;    //�ύ��¼

	point(int a,int b) {
		id = a;
		time = b;
	}
	point() {};
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
	ListNodePosi first() { return header->succ; }
	ListNodePosi insertAsLast(const point& e) {
		size++;
		return trailer->insertAsPred(e);
	}
	void remove(ListNodePosi p) {
		p->pred->succ = p->succ; p->succ->pred = p->pred;
		delete p;
		size--;
	}
	ListNodePosi find(int const& e) const {
		int n = size - 1;
		ListNodePosi p = trailer;
		while (0 <= n--)
			if (e == (p = p->pred)->data.id) return p;
		return NULL; 
	}
};


List* table[d];    //hashtable
int last = 0;     //��һ�εļ�¼����Ϊȫ�ֱ�������

void init() {   //��hashtable���������ʼ��ΪNULL
	for (int i = 0; i < d; i++) {
		table[i] = NULL;
	}
}

void insertHash(int id, int time) {   //����hash��
	point p(id, time);
	int index = hash(id);
	if (table[index] == NULL) { table[index] = new List(); }
	if (table[index]->find(id) != NULL) {
		table[index]->find(id)->data.time = time;   //�Ѿ����ڵ�Ԫ�أ�����time
	}
	else {
		table[index]->insertAsLast(p);   //��δ�����Ԫ�أ����в���
	}
}

int main() {

	init();   //��ʼ��

	int n;
	in(n);
	for (int i = 0; i < n; i++) {
		int id;
		int result;
		in(id);
		id ^= last;
		List*l = table[hash(id)];
		if (l == NULL || l->find(id) == NULL)     //��id��һ�γ��ֵ��������δ����hashtable�������
			result = i + 1;
		else 
			result = l->find(id)->data.time;     //֮ǰ�Ѳ����������ҵ���һ�ε�time
		
		printf("%d\n", result);
		insertHash(id, i + 1);    //��δ�����Ԫ�ؽ��в��룬���Ѿ������Ԫ�ؽ���time����
		last = result;   //����last
	}
	return 0;
}