#ifndef MAINWINDOW_H
#define	MAINWINDOW_H
#include <QVBoxLayout>

#include <QtWidgets>

class MainWindow : public QWidget
{
	Q_OBJECT
	
	public:
		MainWindow(QWidget* parent=nullptr);
		void setWTuner(QWidget* wTuner);
	
	protected:
		void paintEvent(QPaintEvent *event) override;
		void resizeEvent(QResizeEvent *event) override;

	private:
		QWidget* mWSpectrum;
		QWidget* mWWaterfall;
		QWidget* mWTuner=nullptr;
		QWidget* mWFrequency;

		QVBoxLayout* mLayout;
};

#endif
