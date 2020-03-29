#include "glab1.h"

using std::cout;
using std::cin;
using std::endl;
using std::vector;

//������ ������� ����� ��� ��������� ��������� ���������
void wrong_input(std::istream &in) {
	in.clear();
	in.ignore(32767, '\n');
	cout << "������������ ����, ������� ��� ���\n>> ";
}

//| a b |
//| c d | ���������� ������������
double q_det(double a, double b, double c, double d) {
	return a * d - b * c;
}

void gl_display_coordinates(double xy_len) {
	//�������� ����� ���������
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(-xy_len, 0);
	glVertex2f(xy_len, 0);
	glVertex2f(0, -xy_len);
	glVertex2f(0, xy_len);

	//����� ����� ���������
	glColor3f(0.9, 0.9, 0.9);
	for (int i = 1; i < xy_len; i++) {
		glVertex2f(-xy_len, i);
		glVertex2f(xy_len, i);
		glVertex2f(-xy_len, -i);
		glVertex2f(xy_len, -i);

		glVertex2f(i, -xy_len);
		glVertex2f(i, xy_len);
		glVertex2f(-i, -xy_len);
		glVertex2f(-i, xy_len);
	}

	//��������� �������� ������������ ������
	glColor3f(1, 0.5, 0.5);
	for (int i = 0; i < xy_len; i++) {
		glVertex2f(0.1, i);
		glVertex2f(-0.1, i);
		glVertex2f(0.1, -i);
		glVertex2f(-0.1, -i);

		glVertex2f(i, 0.1);
		glVertex2f(i, -0.1);
		glVertex2f(-i, 0.1);
		glVertex2f(-i, -0.1);
	}
	glEnd();
}

//����� ����� ������������� glut
void glut_init_part() {
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //������ ����� RGB
	glutInitWindowSize(700, 700); //������ ����
	glutInitWindowPosition(25, 50); //������������ ����
	glutCreateWindow("Figures output"); //�������� ����
	glClearColor(1, 1, 1, 1);//����� ��� ������
	glMatrixMode(GL_PROJECTION); //��������� ���������� ����
	glLoadIdentity();
}
void glut_init_end_part() {
	//���������� ���������� ��������� ��� �������� ���� � �������
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE
		, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE
		, GLUT_ACTION_CONTINUE_EXECUTION);
	glutMainLoop(); //���������� �������� ����
}

//���������� ���������� ��� �������� ������ � ������� ���������(��������� glut opengl)
vector<Point> dp_points;
double xy_len;

//����������� ������ 1
void gl_display1() {
	glClear(GL_COLOR_BUFFER_BIT); //������� �������� ������
	gl_display_coordinates(xy_len); //����������� ���������

	glPointSize(7);
	glBegin(GL_POINTS); //������ ���������
	glColor3f(0, 0.7, 0.3);
	glVertex2d(dp_points[2].x, dp_points[2].y);
	glColor3f(1, 0, 0);
	glVertex2d(dp_points[0].x, dp_points[0].y);
	glColor3f(0, 0, 1);
	glVertex2d(dp_points[1].x, dp_points[1].y);
	
	glEnd();

	//����� ���������� ��� ������ �����
	double A = dp_points[1].y - dp_points[0].y //������������ ������ ��������� ������
		, B = dp_points[0].x - dp_points[1].x
		, C = dp_points[0].y*dp_points[1].x - dp_points[0].x*dp_points[1].y
		, lx1 = -xy_len, ly1 //����� �� �������� �����������
		, lx2 = xy_len, ly2;
	//���������� ������� ����� ����� ��-��� ������
	if ((ly1 = -(A*lx1 + C) / B) > xy_len || ly1 < -xy_len) {
		ly1 = -xy_len;
		if ((lx1 = -(B*ly1 + C) / A) > xy_len || lx1 < -xy_len) {
			ly1 = xy_len;
			lx1 = -(B*ly1 + C) / A;
		}
	}
	if ((ly2 = -(A*lx2 + C) / B) > xy_len || ly2 < -xy_len) {
		ly2 = xy_len;
		if ((lx2 = -(B*ly2 + C) / A) > xy_len || lx2 < -xy_len) {
			ly2 = -xy_len;
			lx2 = -(B*ly2 + C) / A;
		}
	}

	glBegin(GL_LINES);
	glColor3f(0, 0.7, 0.3);
	glVertex2d(lx1, ly1);
	glVertex2d(lx2, ly2);
	glEnd();

	glFlush(); //��������� �����
}
//������� ������������� opengl ��� ������ 1
void glut_init1(int argc, char **argv, Line l, Point p) {
	glutInit(&argc, argv); //������������� glut
	glut_init_part();

	//����� �������� ���������
	xy_len = 5;
	xy_len = p.x > xy_len ? p.x : xy_len;
	xy_len = p.y > xy_len ? p.y : xy_len;
	xy_len = l.A.x > xy_len ? l.A.x : xy_len;
	xy_len = l.A.y > xy_len ? l.A.y : xy_len;	//����� ������7
	xy_len = l.B.x > xy_len ? l.B.x : xy_len;
	xy_len = l.B.y > xy_len ? l.B.y : xy_len;

	glOrtho(-xy_len, xy_len, -xy_len, xy_len, -xy_len, xy_len); //������� ��������� ��������� x,y,z
	dp_points = {l.A, l.B, p}; //�������� ����� � ������� ������
	glutDisplayFunc(gl_display1);//������� ������ �� �����
	
	glut_init_end_part();
}
void menu_case1(int argc, char **argv) {
	system("cls");

	cout << "\"���������. ��������� ����� ������������ ������\"\n"
		"\n������� ��� �����, �������� ���� ������(\"x1 y1 x2 y2\")\n>> ";
	double x1, y1, x2, y2, x0, y0;
	while (!(cin >> x1 >> y1 >> x2 >> y2) || (x1 == x2 && y1 == y2)) //�������� �� ���������� ����
		wrong_input(cin);

	cout << "������� ����������� �����(\"x0, y0\")\n>> ";
	while (!(cin >> x0 >> y0)) //�������� �� ���������� ����
		wrong_input(cin);

	double det = q_det(x2 - x1, y2 - y1, x0 - x1, y0 - y1);
	cout << "\n������ ����� ����������� ";
	if (det > 0)
		cout << "\"�����\" ��������� ������.";
	if (det < 0)
		cout << "\"������\" ��������� ������.";
	if (det == 0)
		cout << "�� ��������� ������.";

	char yn;
	cout << "\n������� �����������?\n����������� ������ �� ������� � ����� �����.(y/n)\n>> ";
	while (!(cin >> yn) || (yn != 'y' && yn != 'n')) //�������� �� ���������� ����
		wrong_input(cin);

	if (yn == 'n') //������� � ������� ����
		return;

	glut_init1(argc, argv, { {x1, y1}, {x2, y2} }, {x0, y0});
}

//����������� ������ 2
void gl_display2() {
	glClear(GL_COLOR_BUFFER_BIT); //������� �������� ������
	gl_display_coordinates(xy_len); //����������� ���������

	glPointSize(7); //������ ����� � ��������
	glBegin(GL_POINTS);
	glColor3f(1, 0, 0);
	for (int i = 0; i < dp_points.size(); i++)
		glVertex2d(dp_points[i].x, dp_points[i].y);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0, 0.7, 0.3);
	glVertex2d(dp_points[0].x, dp_points[0].y);
	glVertex2d(dp_points[1].x, dp_points[1].y);

	glColor3f(0, 0.3, 0.7);
	glVertex2d(dp_points[2].x, dp_points[2].y);
	glVertex2d(dp_points[3].x, dp_points[3].y);
	glEnd();

	glFlush(); //������������ ���
}
//������� ������������� opengl ��� ������ 2
void glut_init2(int argc, char **argv, Line ln1, Line ln2) {
	glutInit(&argc, argv); //������������� glut
	glut_init_part();

	//����� �������� ���������
	xy_len = 5;
	//
	xy_len = ln1.A.x > xy_len ? ln1.A.x : xy_len;
	xy_len = ln1.A.y > xy_len ? ln1.A.y : xy_len; //�������� �� ������
	xy_len = ln1.B.x > xy_len ? ln1.B.x : xy_len;
	xy_len = ln1.B.y > xy_len ? ln1.B.y : xy_len;
	//
	xy_len = ln2.A.x > xy_len ? ln2.A.x : xy_len;
	xy_len = ln2.A.y > xy_len ? ln2.A.y : xy_len;
	xy_len = ln2.B.x > xy_len ? ln2.B.x : xy_len;
	xy_len = ln2.B.y > xy_len ? ln2.B.y : xy_len;

	glOrtho(-xy_len, xy_len, -xy_len, xy_len, -xy_len, xy_len); //������� ��������� ��������� x,y,z
	dp_points = { ln1.A, ln1.B, ln2.A, ln2.B }; //�������� ����� � ������� ������
	glutDisplayFunc(gl_display2);//������� ������ �� �����

	glut_init_end_part();
}
void menu_case2(int argc, char **argv) {
	system("cls");

	cout << "\"���������. ����������� ��������\"\n"
		"\n������� ����� �������� ������ �������(x1 y1 x2 y2)\n>> ";
	double x1, y1, x2, y2, x3, y3, x4, y4;
	while (!(cin >> x1 >> y1 >> x2 >> y2) || (x1 == x2 && y1 == y2)) //�������� �� ���������� ����
		wrong_input(cin);
	cout << "\n������� ����� �������� ������ �������(x3 y3 x4 y4)\n>> ";
	while (!(cin >> x3 >> y3 >> x4 >> y4) || (x3 == x4 && y3 == y4)) //�������� �� ���������� ����
		wrong_input(cin);

	Line ln1 = { { x1, y1 }, { x2, y2 } } //������������ ������
		, ln2 = { { x3, y3 }, { x4, y4 } };

	if (ln1.isIntersects(ln2))
		cout << "\n��� ������� ������������.\n";
	else
		cout << "\n��� ������� �� ������������.\n";

	char yn;
	cout << "\n������� �����������?(y/n)\n>> ";
	while (!(cin >> yn) || (yn != 'y' && yn != 'n')) //�������� �� ���������� ����
		wrong_input(cin);

	if (yn == 'n') //������� � ������� ����
		return;

	glut_init2(argc, argv, ln1, ln2); //����� ������������� opengl
}

//����� ����� menu_case3 � 4(������� �� �������������)
bool menu_case3_4(vector<Point> &verts, vector<Line> &lines) {
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

	lines.resize(verts.size());
	for (int i = 0; i < verts.size() - 1; i++) //�������� �������� ����� �������
		lines[i] = { verts[i], verts[i + 1] };
	lines[lines.size() - 1] = { verts[verts.size() - 1], verts[0] }; //������� ����� ������ � ��������� ������

	bool is_intersect;
	for (int i = 0; i < lines.size(); i++) { //�������� ����������� ������ ������� � ������
		for (int j = i + 1; j < lines.size(); j++) {
			is_intersect = lines[i].isIntersects(lines[j]);
			if (is_intersect) //���������� ����� ��� ���� �� ����� �����������
				break;
		}
		if (is_intersect)
			break;
	}
	
	return is_intersect;
}

//����������� ������ 3
void gl_display3() {
	glClear(GL_COLOR_BUFFER_BIT); //������� �������� ������
	gl_display_coordinates(xy_len); //����������� ���������

	glPointSize(7);
	glBegin(GL_POINTS);
	glColor3f(1, 0, 0);
	for(int i = 0; i < dp_points.size(); i++) //��������� ���� �����
		glVertex2d(dp_points[i].x, dp_points[i].y);
	glEnd();

	glBegin(GL_LINE_LOOP); //LINE_LOOP ��������� ���������������� ������� + ������ � ��������� �������
	glColor3f(0, 0.7, 0.3);
	for (int i = 0; i < dp_points.size(); i++)//���������� ��������� ��������������
		glVertex2d(dp_points[i].x, dp_points[i].y);
	glEnd();

	glFlush(); //������������ ���
}
//������� ������������� opengl ��� ������ 3
void glut_init3(int argc, char **argv, vector<Point> verts) {
	glutInit(&argc, argv); //������������� glut
	glut_init_part();

	//����� �������� ���������
	xy_len = 5;
	for (int i = 0; i < verts.size(); i++) {
		xy_len = verts[i].x > xy_len ? verts[i].x : xy_len;
		xy_len = verts[i].y > xy_len ? verts[i].y : xy_len;
	}

	glOrtho(-xy_len, xy_len, -xy_len, xy_len, -xy_len, xy_len); //������� ��������� ��������� x,y,z
	dp_points = verts; //�������� ����� � ������� ������
	glutDisplayFunc(gl_display3);//������� ������ �� �����

	glut_init_end_part();
}
void menu_case3(int argc, char **argv) {
	system("cls");
	cout << "\"���������. ������� �������������\"\n";
	vector<Point> verts; //����������� � menu_case3_4
	vector<Line> lines;
	bool is_intersect = menu_case3_4(verts, lines);

	if (is_intersect)
		cout << "\n���� ������������� �� �������� �������.\n";
	else
		cout << "\n���� ������������� �������� �������.\n";

	char yn;
	cout << "\n������� �����������?(y/n)\n>> ";
	while (!(cin >> yn) || (yn != 'y' && yn != 'n')) //�������� �� ���������� ����
		wrong_input(cin);

	if (yn == 'n')
		return;

	glut_init3(argc, argv, verts);
}

//����������� ������ 4
void gl_display4() {
	glClear(GL_COLOR_BUFFER_BIT); //������� �������� ������
	gl_display_coordinates(xy_len); //����������� ���������

	glPointSize(7); //��������� ����� ��������������
	glBegin(GL_POINTS);
	glColor3f(1, 0, 0);
	for (int i = 0; i < dp_points.size(); i++)
		glVertex2d(dp_points[i].x, dp_points[i].y);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glColor3f(0, 0.7, 0.3);
	for (int i = 0; i < dp_points.size(); i++)
		glVertex2d(dp_points[i].x, dp_points[i].y);
	glEnd();

	glFlush(); //������������ ���
}
//������� ������������� opengl ��� ������ 4
void glut_init4(int argc, char **argv, vector<Point> verts) {
	glutInit(&argc, argv); //������������� glut
	glut_init_part();

	//����� �������� ���������
	xy_len = 5;
	for (int i = 0; i < verts.size(); i++) {
		xy_len = verts[i].x > xy_len ? verts[i].x : xy_len;
		xy_len = verts[i].y > xy_len ? verts[i].y : xy_len;
	}

	glOrtho(-xy_len, xy_len, -xy_len, xy_len, -xy_len, xy_len); //������� ��������� ��������� x,y,z
	dp_points = verts; //�������� ����� � ������� ������
	glutDisplayFunc(gl_display4);//������� ������ �� �����

	glut_init_end_part();
}
void menu_case4(int argc, char **argv) {
	system("cls");
	cout << "\"���������. ���������� �������� ��������������\"\n";
	vector<Point> verts;//����������� � menu_case3_4
	vector<Line> lines;//
	bool is_intersect = menu_case3_4(verts, lines);

	if (is_intersect) {
		cout << "\n���� ������������� �� �������� �������. �� ��������� � ������� ����\n";
		system("pause");
		return;
	}

	char yn;
	cout << "\n��������� �� ������� �������(y/n)?\n>> ";
	while (!(cin >> yn) || (yn != 'y' && yn != 'n'))//�������� �� ���������� ����
		wrong_input(cin);
	bool hour_hand = (yn == 'y') ? true : false;
									
	bool is_convex = true;
	for (int i = 0; i < lines.size(); i++) {
		double x1 = lines[i].A.x, y1 = lines[i].A.y
			, x2 = lines[i].B.x, y2 = lines[i].B.y;
		for (int j = 0; j < verts.size(); j++) {
			double det, x = verts[j].x, y = verts[j].y;		//if (det >= 0) ����� "�����" ��� "��"
			det = q_det(x2 - x1, y2 - y1, x - x1, y - y1);	//if (det <= 0) ����� "������" ��� "��"
			if (hour_hand && det > 0) {
				is_convex = false;
				break;
			}
			if (!hour_hand && det < 0) {
				is_convex = false;
				break;
			}
		}
		if (!is_convex) {
			break;
		}
	}


	if (is_convex)
		cout << "������������� ��������.\n";
	else
		cout << "�������������� �� ��������.\n";

	cout << "\n������� �����������?(y/n)\n>> ";
	while (!(cin >> yn) || (yn != 'y' && yn != 'n')) //�������� �� ���������� ����
		wrong_input(cin);
	if (yn == 'n')
		return;

	glut_init4(argc, argv, verts);

	system("pause");
}