#include "glab2.h"

using std::cout;
using std::cin;
using std::endl;
using std::vector;

int main(int argc, char **argv) {
	SetConsoleCP(1251); //смена кодировки консоли на windows-1251
	SetConsoleOutputCP(1251);//

	cout << "\"Геометрия. Положение точки относительно многоугольника\"\n";
	vector<Point> verts;//заполняются в isSimplePolygon
	vector<Line> lines;//
	cout << "\nВведите количество точек, задающих ваш многоуольник(не менее трех)\n>> ";
	int n;
	while (!(cin >> n) || n < 3) //Проверка на правильный ввод
		wrong_input(cin);

	cout << "Вводите последовательные вершины многоугольника(Xn Yn)\n";
	verts.resize(n);
	for (int i = 0; i < n; i++) {
		cout << "Введите точку " << i + 1 << ".\n>> ";
		while (!(cin >> verts[i].x >> verts[i].y)) //Проверка на правильный ввод
			wrong_input(cin);
	}
	bool is_intersect = is_simple_polygon(verts, lines);

	if (is_intersect) {
		cout << "\nЭтот многоугольник НЕ является простым.\n";
		cout << "\nКонец выполнения.\n";
		system("pause");
		return 1;
	}

	double x, y;
	cout << "Введите точку для проверки её местополжения.\n>> ";
	while (!(cin >> x >> y)) //Проверка на правильный ввод
		wrong_input(cin);

	bool is_in = is_point_in(verts, lines, {x, y});

	if (is_in)
		cout << "\nТочка находится ВНУТРИ многоугольника.\n";
	else
		cout << "\nТочка находится СНАРУЖИ многоугольника.\n";

	char yes_no;
	cout << "\nВывести изображение?(y/n)\n>> ";
	while (!(cin >> yes_no) || (yes_no != 'y' && yes_no != 'n')) //Проверка на правильный ввод
		wrong_input(cin);
	if (yes_no == 'n') {
		cout << "\nКонец выполнения.\n";
		system("pause");
		return 1;
	}

	glut_init(argc, argv, verts, {x, y});

	cout << "\nКонец выполнения.\n";
	system("pause");
}