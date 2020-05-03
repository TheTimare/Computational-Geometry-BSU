#include "glab.h"

using std::cout;
using std::cin;
using std::endl;
using std::vector;

//не показывать консоль
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

int main(int argc, char **argv) {
	SetConsoleCP(1251); //смена кодировки консоли на windows-1251
	SetConsoleOutputCP(1251);

	srand(time(0));
	
	int numberOfPoints = 3;
	int limitOfPointsValue = 10;

	int choose;
	try {
		choose = MessageBox(
			NULL,
			"This program will generate a point every half of second and then\n" 
			"will build dynamic convex hull of this set.\n"
			"There will be an animation of this.\n"
			"\nPress OK to start...",
			"Lab 7. Dynamic hull",
			MB_ICONMASK| MB_OKCANCEL| MB_DEFBUTTON2
		);
	}
	catch (...) {
		std::cerr << "Can't open MessageBox.";
		choose = 2;
	}
	if (choose == 2) {
		return 1;
	}

	glut_init(argc, argv, numberOfPoints, limitOfPointsValue);
}