#include "glab2.h"

using std::cout;
using std::cin;
using std::endl;
using std::vector;

int main(int argc, char **argv) {
	SetConsoleCP(1251); //����� ��������� ������� �� windows-1251
	SetConsoleOutputCP(1251);//

	cout << "\"���������. ��������� ����� ������������ ��������������\"\n";
	vector<Point> verts;//����������� � isSimplePolygon
	vector<Line> lines;//
	cout << "\n������� ���������� �����, �������� ��� ������������(�� ����� ����)\n>> ";
	int n;
	while (!(cin >> n) || n < 3) //�������� �� ���������� ����
		wrong_input(cin);

	cout << "������� ���������������� ������� ��������������(Xn Yn)\n";
	verts.resize(n);
	for (int i = 0; i < n; i++) {
		cout << "������� ����� " << i + 1 << ".\n>> ";
		while (!(cin >> verts[i].x >> verts[i].y)) //�������� �� ���������� ����
			wrong_input(cin);
	}
	bool is_intersect = is_simple_polygon(verts, lines);

	if (is_intersect) {
		cout << "\n���� ������������� �� �������� �������.\n";
		cout << "\n����� ����������.\n";
		system("pause");
		return 1;
	}

	double x, y;
	cout << "������� ����� ��� �������� � �������������.\n>> ";
	while (!(cin >> x >> y)) //�������� �� ���������� ����
		wrong_input(cin);

	bool is_in = is_point_in(verts, lines, {x, y});

	if (is_in)
		cout << "\n����� ��������� ������ ��������������.\n";
	else
		cout << "\n����� ��������� ������� ��������������.\n";

	char yes_no;
	cout << "\n������� �����������?(y/n)\n>> ";
	while (!(cin >> yes_no) || (yes_no != 'y' && yes_no != 'n')) //�������� �� ���������� ����
		wrong_input(cin);
	if (yes_no == 'n') {
		cout << "\n����� ����������.\n";
		system("pause");
		return 1;
	}

	glut_init(argc, argv, verts, {x, y});

	cout << "\n����� ����������.\n";
	system("pause");
}