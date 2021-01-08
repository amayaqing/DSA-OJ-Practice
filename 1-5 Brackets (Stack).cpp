#include<iostream>
#include<string>

using namespace std;

class stack {
private:
	int capacity = 10;
	char*data;
	int _size = 0;
public:
	stack();
	stack(int n);
	int getsize() { return _size; }
	int getcapacity() { return capacity; }
	void expand();
	bool isEmpty();
	bool isFull();
	void push(char e);
	char pop();
	char& top();
};

stack::stack() { data = new char[capacity]; }
stack::stack(int n) {
	capacity = n;
	data = new char[capacity];
}
void stack::expand() {
	int old_capacity = capacity;
	//capacity <<= 1;
	char*old_data = data;
	data = new char[capacity<<=1];
	for (int i = 0; i < old_capacity; i++) data[i] = old_data[i];
	delete[]old_data;
}
bool stack::isEmpty() { return (_size == 0); }
bool stack::isFull() { return(_size == capacity); }
void stack::push(char e) {
	if (isFull()) { expand(); }
	data[_size] = e; 
	_size++;
}

char stack::pop() {
	char value = data[_size - 1];
	_size--;
	return value;
}
char& stack::top() { return data[_size - 1]; }

bool paren(char a, char b) {
	if (a == ')') {
		switch (b) {
		case '(':return true;
		case'{':return false;
		case'[':return false;
		}
	}
	else if (a == '}') {
		switch (b) {
		case '(':return false;
		case'{':return true;
		case'[':return false;
		}
	}
	else if (a == ']') {
		switch (b) {
		case '(':return false;
		case'{':return false;
		case'[':return true;
		}
	}
	else return false;
}

bool legal(const char a[], int l) {
	stack s;
	for (int i = 0; i < l; i++) {
		if (a[i] == '(' || a[i] == '{' || a[i] == '[') s.push(a[i]);
		else if (!s.isEmpty() && paren(a[i], s.top())) s.pop();
		else return false;
	}
	return s.isEmpty();
}

int main()
{
	int T;
	cin >> T;
	bool* judge = new bool[T];
	for (int i = 0; i < T; i++) {
		string s1;
		cin >> s1;
		int l = s1.size();
		char*a = new char[l];
		for (int j = 0; j < l; j++) a[j] = s1[j];
		judge[i] = legal(a, l);
	}

	for (int i = 0; i < T; i++) {
		if (judge[i] == 1) cout << "Yes" << endl;
		else cout << "No" << endl;
	}
	return 0;
}