#ifndef	WVOLUME_H
#define	WVOLUME_H
#include <QtWidgets>

class WVolume: public QWidget
{
	Q_OBJECT
	
	public:
		WVolume(QWidget* parent=nullptr);
		int getVolume() {return mVolume;};
		void setVolume(int volume) {if (volume>=0 && volume<=255) {mVolume=volume;update();}};
		QSize sizeHint() const;
	
	protected:
		void paintEvent(QPaintEvent *event) override;
		void mousePressEvent(QMouseEvent *event) override;
		void mouseReleaseEvent(QMouseEvent *event) override;
		void mouseMoveEvent(QMouseEvent *event) override;
		void wheelEvent(QWheelEvent *event) override;

	private:
		int mVolume=255;
		
};
#endif
