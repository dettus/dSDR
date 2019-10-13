#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "WFrequency.h"
#include "WSpectrum.h"
#include "WWaterfall.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		MainWindow();

	private:
		WFrequency *wFrequency;
		WSpectrum  *wSpectrum;
		WWaterfall *wWaterfall;
};
#endif
