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
	
	//int numberOfPoints = 5;
	//int limitOfPointsValue = 7;

	int choose;
	try {
		choose = MessageBox(
			NULL,
			"This program will generate 10 points and then" 
			"\ntry to build convex polygon. There will be"
			"\nanimation of this process."
			"\n\nPress OK to start...",
			"Lab 4. Convex hull building animation",
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

	glut_init(argc, argv, 10, 10);
}