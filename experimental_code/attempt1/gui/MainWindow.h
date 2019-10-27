#ifndef MAINWINDOW_H
#define	MAINWINDOW_H
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtWidgets>
#include "SimpleFft.h"

class MainWindow : public QWidget
{
	Q_OBJECT
	
	public:
		MainWindow(QWidget* parent=nullptr);
		void setWTuner(QWidget* wTuner);
		void setWSpectrum(QWidget* wSpectrum);
		void setWWaterfall(QWidget* wWaterfall);
	
	protected:
		void paintEvent(QPaintEvent *event) override;
		void resizeEvent(QResizeEvent *event) override;

	private:
		QWidget* mWSpectrum=nullptr;
		QWidget* mWWaterfall=nullptr;
		QWidget* mWTuner=nullptr;
		QWidget* mWFrequency;

		SimpleFft* mFft;
	
};

#endif
