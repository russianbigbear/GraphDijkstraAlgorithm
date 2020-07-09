#include <iostream>
#include <fstream>
#include <locale>
#include <stdlib.h>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

int N; // ���-�� ������
int M; // ���-�� �����
const int INF = 999; // �������������

					 /*���� ����� �� �����, ��������� ��� ������ ������� � ����� ����*/
void input_matrix(ifstream &in, int **graph)
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			in >> graph[i][j];
}

/* ������ ����� c ���� ����(����� � graphic.png) */
void print_graph(int **graph) {
	ofstream out;
	out.open("graphic.dot");
	out << "graph G{" << "\n";
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (graph[i][j] != 0 && i < j)
				out << i + 1<< " -- " << j + 1<< " [label=\" " << graph[i][j] << "\"];\n";
	out << "}";
	out.close();
	system("dot -Tpng graphic.dot -o graphic.png ");
	system("graphic.png");
}

/*������ ���������� ����*/
int count_rib(int **graph) {
	int count = 0;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (graph[i][j] != 0 && i < j)
				count++;
	return count;
}

/*������� �� ������� � �������� ������*/
void matrix_to_vector(int **graph, vector< vector< pair<int, int> >  >& vector) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
			if (graph[i][j] != 0) 
				vector[i].push_back({ j, graph[i][j] });
			else
				vector[i].push_back({ j, INF });
			
	}
}

/*------------------------------------------------------------------------------------------------------*/

/* �������� �������
ifs - ������� �����;
ofs - �������� �����;
*/

int main() {
	ifstream ifs;
	ofstream ofs;
	setlocale(LC_ALL, "rus");

	try {
		int ans;
		do {
			cout << "������������� ���� �����? (1-��, 0 -���)." << endl;
			cin >> ans;
			if (ans == 1) system("input.txt");
		} while (ans != 1 && ans != 0);


		ifs.open("input.txt");
		ifs >> N;

		int **graph_cost_m = new int*[N]; //�������� �������
		for (int i = 0; i < N; i++)
		{
			graph_cost_m[i] = new int[N];
			for (int j = 0; j < N; j++)
				graph_cost_m[i][j] = 0;
		}


		input_matrix(ifs, graph_cost_m);
		ifs.close();

		do {
			cout << "������� ����? (1-��, 0 -���)." << endl;
			cin >> ans;
			if (ans == 1) print_graph(graph_cost_m);
		} while (ans != 1 && ans != 0);

		/*������� ��������� � �������� ������*/
		M = count_rib(graph_cost_m);
		vector< vector< pair<int, int> > > graph_cost_v(M); // ������ ������ [������ �����[���� �������� ������� - ���]]
		matrix_to_vector(graph_cost_m, graph_cost_v);


		/*�������� �������� ��� ���� ������*/
		ofs.open("output.txt");
		ofs << "����� ��� ������� ( �� ����� �������: ��������� (����) )" << endl << endl;

		for (int point = 0; point < N; point++) {

			vector<int> distance(N, INF); // ������ ��������� ���������� �����
			vector<int>	parents(N); // ������ �������
			vector<char> used(N); // ������ �����
			vector<int> S;
			distance[point] = 0;

			for (int i = 0; i < N; ++i) {
				int k = -1;

				/*������� ������� k, ������� ���������� ���������� d[] ����� ������������ ������*/
				for (int j = 0; j < N; ++j)
					if (!used[j] && (k == -1 || distance[j] < distance[k]))
						k = j;

				if (distance[k] == INF) // ���� ��������� ����� ������������� ���������
					break;

				used[k] = true; //������� ���������� ��� ����������

				/*��������������� ��� ����, ��������� �� ������ �������*/
				for (size_t j = 0; j < graph_cost_v[k].size(); ++j) {

					/*����� ������� ����� ����������� ��������� ���������*/
					int to = graph_cost_v[k][j].first,
						len = graph_cost_v[k][j].second;

					/*���� ��������� ������� (�.�. ���������� d[to] ��������), �� ��������������� ���������� d[to] � ����������� ������ p[] */
					if (distance[k] + len < distance[to]) {
						distance[to] = distance[k] + len;
						parents[to] = k;
					}
				}

			}

			/*����� ���������� � ����*/
			ofs << "���������� ���� �� ������� " << point + 1 << ":\n";
			for (int i = 0; i < N; i++) {
				ofs << "�� " << i + 1 << ":   " << distance[i] << " (";

				/*����������� ������*/
				vector<int> path;
				for (int v = i; v != point; v = parents[v])
					path.push_back(v);
				path.push_back(point);
				reverse(path.begin(), path.end());

				for (int j = 0; j < path.size(); j++)
					if (j < path.size() - 1)
						ofs << path[j] + 1 << "->";
					else
						ofs << path[j] + 1<< ")" << endl;

			}
			ofs << endl;	

		}

		ofs.close();

		/*�������� ����������*/
		do {
			cout << "������� ���������? (1-� �������� ���������, 0 -�� ��������)." << endl;
			cin >> ans;
			if (ans == 1) system("output.txt");

		} while (ans != 1 && ans != 0);

	}
	catch (...) {
		cout << "������ �����!";
	}

	cout << "��������� ���������." << endl;
	return 0;
}

