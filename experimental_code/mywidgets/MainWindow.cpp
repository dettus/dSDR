#include <QtWidgets>
#include "MainWindow.h"

MainWindow::MainWindow()
{
	setWindowTitle(tr("hello World"));
	resize(500,500);
	wFrequency=new WFrequency(this);
	wFrequency->resize(500,100);
	wFrequency->move(500,50);
	wFrequency->show();
}


