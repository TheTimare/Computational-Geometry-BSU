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

	int choose;
	try {
		choose = MessageBox(
			NULL,
			"This program will generate 10 points inside the\n" 
			"convex polygon and then show how they moving\n"
			"inside it until they get into the inner polygon."
			"\n\nPress OK to start...",
			"Lab 3. Animation of reflecting point",
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

	glut_init(argc, argv, 10, 3);
}