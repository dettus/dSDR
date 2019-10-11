#ifndef	WFREQUENCY_H
#define	WFREQUENCY_H

#include <QtWidgets>

class WFrequency : public QWidgets
{
	Q_OBJECT

public:
	WFrequency();
	unsigned long long getFreq();
	void setFreq(unsigned long long freq);

protected:
    void closeEvent(QCloseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;


private slots:
	void clicked();
	
	unsigned long long curFreq;
	int curWidth;
	int curHeight;
};
#endif
