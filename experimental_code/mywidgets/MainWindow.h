#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "WFrequency.h"
class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		MainWindow();

	private:
		WFrequency *wFrequency;
};
#endif
