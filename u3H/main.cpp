#include "u3h.h"

#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	u3H::Init_instance();

	int n = a.exec();
	return n;
}
