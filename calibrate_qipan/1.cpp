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
	//������������ôȷ������·�ߵ�ֵ�����������ģ�ȥ���߿��ܵ��������ߣ�����˳��ȥ��һ���ǲ����Եģ���Ϊ�����п��ܾۼ��ڱ߽��ϣ�

	system("pause");
	return 0;

}

void To(int **a, int m, int n)
{

}