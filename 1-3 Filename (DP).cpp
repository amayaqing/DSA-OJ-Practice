#include<iostream>
#include<string>
#include<cmath>
#include<cstdio>
using namespace std;

int min(int a, int b) {
	return a > b ? b : a;
}
int max(int a, int b) {
	return a < b ? b : a;
}

int cost_dd(char* a, char* b, int la, int lb, int k) {
	int**n = new int*[2];
	int length = lb + 1;
	for (int i = 0; i < 2; i++) n[i] = new int[length]();
	//���
	//��һ��
	for (int i = 0; i < length; i++) n[0][i] = i;

	int index = 0;
	for (int i = 0; i < la; i++) {
		//����n[1][0]
		//�˴������������lcs���㷨
		index = 1;
		int j = max(0, i - k);
		if (j == 0) n[1][0] = n[0][0] + 1;
		else n[1][j] = (a[i] == b[j]) ? n[0][j - 1] : n[0][j] + 1;
		//������������Ԫ��
		while (j < i + k && j < lb) {
			if (a[i] == b[j]) n[1][j + 1] = n[0][j];
			else n[1][j + 1] = min(n[1][j] + 1, n[0][j + 1] + 1);
			//reference:https://www.cnblogs.com/wuyuegb2312/p/3281264.html
			j++;
		}
	
		//�������һ��
		if (j < lb) {
			if (a[i] == b[j]) n[1][j] = n[0][j-1]; 
			else n[1][j] = n[1][j - 1] + 1;
		}

		i++;

		if (i < la) {
			index = 0;
			j = max(0, i - k);
			if (j == 0) n[0][0] = n[1][0] + 1;
			else n[0][j] = (a[i] == b[j]) ? n[1][j - 1] : n[1][j] + 1;

			while (j < i + k && j < lb) {
				if (a[i] == b[j]) n[0][j + 1] = n[1][j];
				else n[0][j + 1] = min(n[0][j] + 1, n[1][j + 1] + 1);
				j++;
			}

			//�������һ��
			if (j < lb) {
				if (a[i] == b[j]) n[0][j] = n[1][j - 1]; 
				else n[0][j] = n[0][j - 1] + 1;
			}
		}
    }

	return n[index][length-1];
}

int main()
{
	int n, m, k;
	scanf("%d%d%d", &n, &m, &k);
	char*a = new char[n];
	char*b = new char[m];
	for (int i = 0; i < n; i++) cin >> a[i];
	for (int i = 0; i < m; i++) cin >> b[i];

	int judge = -1;
	if(abs(n-m)<=k) {
		int cost = cost_dd(a, b, n - 1, m - 1, k);
		if (cost <= k) judge = cost;
	}
	printf("%d\n", judge);
	return 0;
}