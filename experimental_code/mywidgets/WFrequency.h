#ifndef	WFREQUENCY_H
#define	WFREQUENCY_H

#include <QtWidgets>

class WFrequency : public QWidget
{
	Q_OBJECT

public:
	WFrequency();
	signed long long getFreq();
	void setFreq(signed long long freq);

protected:
	void closeEvent(QCloseEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;

private:
	void calculateFreq(int x,int up1down_1);
	signed long long curFreq;
	int curWidth;
	int curHeight;
	int drawPlus;
	int drawMinus;
};
#endif
