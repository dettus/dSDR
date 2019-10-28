// the purpose of this class is to show
// a small startup dialog, where the user 
// picks the tuner, it would like to instantiiate.
// 
// it has a function getSelectedType, which returns
// UNDEF until a tuner has been selected. the other
// possibility is a QUIT.
//
// $Id$
#ifndef	STARTUPDIALOG_H
#define STARTUPDIALOG_H
#include <QtWidgets>
#include <QVBoxLayout>
#include <QPushButton>
#include "DataTypes.h"


class StartupDialog: public QWidget
{
	Q_OBJECT

	public:
		StartupDialog(QWidget* parent=nullptr);
		~StartupDialog();
		enum eTunerType	getSelectedTunerType();

	public slots:
		void buttonReleased();

	private:
	typedef	struct _tSelectedTuner
	{
		eTunerType	type;	// the tuner type (see DataTypes.h)
		QString		buttonlabeltext;	// the text on the pushbutton
	} tSelectedTuner;

	// in case you want to implement a new tuner.
	#define	NUM_TUNER_BUTTONS			3
	const tSelectedTuner cTunerButtons[NUM_TUNER_BUTTONS]={
		{TUNER_DUMMY,	"Dummy Tuner"},
		{TUNER_RTLTCP,	"RTL TCP Client"},
		// room for new tuners
		{TUNER_QUIT,	"Quit!"}	
	};
	// this const struct is registering the tuner types with matching pushbuttons.
	QVBoxLayout *mLayout=nullptr;
	QPushButton *mButton[NUM_TUNER_BUTTONS]={nullptr};
	enum eTunerType mTunerType=TUNER_UNDEF;
	
};
#endif
