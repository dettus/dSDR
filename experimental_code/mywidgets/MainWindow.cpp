#include <QtWidgets>
#include "MainWindow.h"

MainWindow::MainWindow()
{
	setWindowTitle(tr("hello World"));
	resize(500,500);
	wFrequency=new WFrequency();
	wFrequency->show();
}


