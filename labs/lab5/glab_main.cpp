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
	
	int numberOfPoints = 25;
	int limitOfPointsValue = 10;

	int choose;
	try {
		choose = MessageBox(
			NULL,
			"This program will generate 10 points and then\n" 
			"try to find diameter of flat set using Jarvis\n"
			"Algorithm. There will be an animation of this.\n"
			"\nPress OK to start...",
			"Lab 5. Jarvis and Diameter",
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