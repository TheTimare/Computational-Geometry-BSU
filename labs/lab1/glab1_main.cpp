#include "glab1.h"

using std::cout;
using std::cin;
using std::endl;

int main(int argc, char **argv) {
	SetConsoleCP(1251); //����� ��������� ������� �� windows-1251
	SetConsoleOutputCP(1251);//

	int choose = 0;
	//���� �������� ����
	while (choose != 5) {
		system("cls");//������� �������
		cout << "\"���������. ������� ������������\"\n"
			"\n------\n" 
			"1. ��������� ����� ������������ ������;\n"
			"2. ����������� ��������;\n"
			"3. ������� �������������;\n"
			"4. ���������� �������� ��������������;\n"
			"5. ������� ���������.\n" 
			"------\n\n"
			"�������� ����� ����\n>> ";
		while (!(cin >> choose) || !(choose >= 1 && choose <= 5)) //�������� �� ���������� ����
			wrong_input(cin);

		switch (choose) {
		case 1:
			menu_case1(argc, argv);
			break;
		case 2:
			menu_case2(argc, argv);
			break;
		case 3:
			menu_case3(argc, argv);
			break;
		case 4:
			menu_case4(argc, argv);
		}
	}
	//glutLeaveMainLoop();//��������� ������ ��������� ��� ������ �������
}