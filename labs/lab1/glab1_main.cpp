#include "glab1.h"

using std::cout;
using std::cin;
using std::endl;

int main(int argc, char **argv) {
	SetConsoleCP(1251); //смена кодировки консоли на windows-1251
	SetConsoleOutputCP(1251);//

	int choose = 0;
	//цикл главного меню
	while (choose != 5) {
		system("cls");//очистка консоли
		cout << "\"Геометрия. Задания лабораторной\"\n"
			"\n------\n" 
			"1. Положение точки относительно прямой;\n"
			"2. Пересечение отрезков;\n"
			"3. Простой многоугольник;\n"
			"4. Выпуклость простого многоугольника;\n"
			"5. Закрыть программу.\n" 
			"------\n\n"
			"Выберите номер меню\n>> ";
		while (!(cin >> choose) || !(choose >= 1 && choose <= 5)) //Проверка на правильный ввод
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
	//glutLeaveMainLoop();//исправить выдачу сообщения при вызове функции
}