#include <iostream>
#include <fstream>
#include <locale>
#include <stdlib.h>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;

int N; // кол-во вершин
int M; // кол-во ребер
const int INF = 999; // бесконечность

					 /*ввод графа из файла, заданного при помощи матрицы с весом рёбер*/
void input_matrix(ifstream &in, int **graph)
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			in >> graph[i][j];
}

/* печать графа c веом рёбер(вывод в graphic.png) */
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

/*Посчет количества рёбер*/
int count_rib(int **graph) {
	int count = 0;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (graph[i][j] != 0 && i < j)
				count++;
	return count;
}

/*перевод из массива в заданный вектор*/
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

/* Основная функция
ifs - входной поток;
ofs - выходной поток;
*/

int main() {
	ifstream ifs;
	ofstream ofs;
	setlocale(LC_ALL, "rus");

	try {
		int ans;
		do {
			cout << "Редактировать файл ввода? (1-да, 0 -нет)." << endl;
			cin >> ans;
			if (ans == 1) system("input.txt");
		} while (ans != 1 && ans != 0);


		ifs.open("input.txt");
		ifs >> N;

		int **graph_cost_m = new int*[N]; //зануляем матрицу
		for (int i = 0; i < N; i++)
		{
			graph_cost_m[i] = new int[N];
			for (int j = 0; j < N; j++)
				graph_cost_m[i][j] = 0;
		}


		input_matrix(ifs, graph_cost_m);
		ifs.close();

		do {
			cout << "Вывести граф? (1-да, 0 -нет)." << endl;
			cin >> ans;
			if (ans == 1) print_graph(graph_cost_m);
		} while (ans != 1 && ans != 0);

		/*матрица смежности в заданный вектор*/
		M = count_rib(graph_cost_m);
		vector< vector< pair<int, int> > > graph_cost_v(M); // вектор вершин [вектор ребер[пара конечная вершина - вес]]
		matrix_to_vector(graph_cost_m, graph_cost_v);


		/*Алгоритм Дейкстры для всех вершин*/
		ofs.open("output.txt");
		ofs << "Рейсы для челнока ( до какой вершины: дистанция (рейс) )" << endl << endl;

		for (int point = 0; point < N; point++) {

			vector<int> distance(N, INF); // вектор растояний кратчайших путей
			vector<int>	parents(N); // вектор предков
			vector<char> used(N); // вектор меток
			vector<int> S;
			distance[point] = 0;

			for (int i = 0; i < N; ++i) {
				int k = -1;

				/*находим вершину k, имеющию наименьшее расстояние d[] среди непомеченных вершин*/
				for (int j = 0; j < N; ++j)
					if (!used[j] && (k == -1 || distance[j] < distance[k]))
						k = j;

				if (distance[k] == INF) // если растояние равно бесконечности прерываем
					break;

				used[k] = true; //вершина становится как помеченная

				/*просматриваются все рёбра, исходящие из данной вершины*/
				for (size_t j = 0; j < graph_cost_v[k].size(); ++j) {

					/*вдоль каждого ребра выполняется улучшение растояния*/
					int to = graph_cost_v[k][j].first,
						len = graph_cost_v[k][j].second;

					/*если улучшение успешно (т.е. расстояние d[to] меняется), то пересчитывается расстояние d[to] и сохраняется предок p[] */
					if (distance[k] + len < distance[to]) {
						distance[to] = distance[k] + len;
						parents[to] = k;
					}
				}

			}

			/*вывод результата в файл*/
			ofs << "Кратчайшие пути от вершины " << point + 1 << ":\n";
			for (int i = 0; i < N; i++) {
				ofs << "до " << i + 1 << ":   " << distance[i] << " (";

				/*составление рейсов*/
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

		/*открытие результата*/
		do {
			cout << "Вывести результат? (1-в тестовом документе, 0 -не выводить)." << endl;
			cin >> ans;
			if (ans == 1) system("output.txt");

		} while (ans != 1 && ans != 0);

	}
	catch (...) {
		cout << "Ошибка файла!";
	}

	cout << "Программа выполнена." << endl;
	return 0;
}

