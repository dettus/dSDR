#ifndef	TUNERDIALOG_H
#define	TUNERDIALOG_H
#include <QtWidgets>
#include <QVBoxLayout>
#include <QPushButton>
	enum eTunerId {TUNER_UNDEF=-1,TUNER_QUIT=0,
		TUNER_DUMMY	=1,
		TUNER_RTLTCP	=2
	};

class TunerDialog: public QObject
{

	Q_OBJECT

	public: 
	typedef	 struct _tSelectedTuner
	{
		eTunerId id;
		QString text;
	} tSelectedTuner;
	#define	NUMBUTTONS		3

	const tSelectedTuner cTunerButtons[NUMBUTTONS]=
	{
		{TUNER_DUMMY ,"Dummy Tuner"},
		{TUNER_RTLTCP,"RTL TCP Client"},
		{TUNER_QUIT  ,"Quit!"}
	};
	public:
		TunerDialog();
		~TunerDialog();
		enum eTunerId getTunerValue();
	public slots:
		void buttonReleased();

	private:
		QWidget *mWindow=nullptr;
		QVBoxLayout *mLayout=nullptr;
		QPushButton *mButton[NUMBUTTONS]={nullptr};
		enum eTunerId mTunerIdReturnvalue=TUNER_UNDEF;
};
#endif
