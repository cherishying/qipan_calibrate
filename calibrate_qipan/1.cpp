#include <iostream>
#include <stdio.h>

using namespace std;

void To(int **a, int m, int n);

int main()
{
	int m;
	int n;
	cin >> m;
	cin >> n;
	int **matrix;
	matrix = new int *[m];
	for (int i = 0; i < m; i++)
	{
		matrix[i] = new int[n];
	}

	cout << "cin the matrix;";
	for (int i = 0; i < m; i++)
	for (int j = 0; j < n; j++)
	{
		cin >> matrix[i][j];
		
	}

	//for (int i = 0; i < m; i++)
	//{
	//	for (int j = 0; j < n; j++)
	//	{
	//		cout << matrix[i][j] << "  ";
	//	}
	//	cout << "\n";
	//}
	//最大的问题是怎么确保两条路线的值加起来是最大的，去的线可能挡到来的线，而按顺序去走一定是不可以的（因为最大的有可能聚集在边角上）

	system("pause");
	return 0;

}

void To(int **a, int m, int n)
{

}