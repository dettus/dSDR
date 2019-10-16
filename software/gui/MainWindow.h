#ifndef MAINWINDOW_H
#define	MAINWINDOW_H
#include <QVBoxLayout>

#include <QtWidgets>

class MainWindow : public QWidget
{
	Q_OBJECT
	
	public:
		MainWindow(QWidget* parent=nullptr);
	
	protected:
		void paintEvent(QPaintEvent *event) override;		
	private:
		QWidget* mWSpectrum;
		QWidget* mWWaterfall;
		QWidget* mWTuner;
		QWidget* mWFrequency;

		QVBoxLayout* mLayout;
};

#endif
