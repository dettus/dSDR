#ifndef	TUNERDIALOG_H
#define	TUNERDIALOG_H
#include <QtWidgets>
#include <QVBoxLayout>
#include <QPushButton>

class TunerDialog
{
	Q_OBJECT

	typedef	 struct _tSelectedTuner
	{
		int id;
		QString text;
	} tSelectedTuner;
	#define	NUMBUTTONS		3

	const tSelectedTuner cTunerButtons[NUMBUTTONS]=
	{
		{1,"Dummy Tuner"},
		{2,"RTL TCP Client"},
		{0,"Quit!"}
	};
	public:
		TunerDialog();
		~TunerDialog();
		int askForTuner();
	public slots:
		void buttonReleased();

	private:
		QWidget *mWindow=nullptr;
		QVBoxLayout *mLayout=nullptr;
		QPushButton *mButton[NUMBUTTONS]={nullptr};
		int mTunerIdReturnvalue=-1;
};
#endif
