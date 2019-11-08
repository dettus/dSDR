// the purpose of this class is to show
// a small startup dialog, where the user 
// picks the tuner, it would like to instantiiate.
// 
// it has a function getSelectedType, which returns
// UNDEF until a tuner has been selected. the other
// possibility is a QUIT.
//
// Additionally, tuners are being instantiated in this class,
// but initialized later.
//
// $Id$
#ifndef	TUNERDIALOG_H
#define TUNERDIALOG_H
#include <QtWidgets>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMutex>
#include "DataTypes.h"
#include "Tuners.h"
#include "TDummy.h"	// tuner header file
#include "TMirics.h"	// tuner header file
#include "TRtlTcp.h"	// tuner header file


class TunerDialog: public QWidget
{
	Q_OBJECT

	public:
		TunerDialog(QWidget* parent=nullptr);
		~TunerDialog();
		enum eTunerType getSelectedTunerType();
		Tuners* getTuner();

	public slots:
		void buttonReleased();

	private:
	typedef	struct _tSelectedTuner
	{
		eTunerType	type;	// the tuner type (see DataTypes.h)
		QString		buttonlabeltext;	// the text on the pushbutton
	} tSelectedTuner;

	// ---> in case you want to implement a new tuner. they have to be registerd HERE
	#define	NUM_TUNER_BUTTONS			4
	const tSelectedTuner cTunerButtons[NUM_TUNER_BUTTONS]={
		{TUNER_DUMMY,	"Dummy Tuner"},
		{TUNER_MIRICS,	"Mirics (SDRplay devices)"},
		{TUNER_RTLTCP,	"RTL TCP Client"},
		// room for new tuners
		{TUNER_QUIT,	"Quit!"}	
	};
	TDummy *mTunerDummy=nullptr;
	TMirics *mTunerMirics=nullptr;
	TRtlTcp *mTunerRtlTcp=nullptr;
	// ---> until HERE.


	
	// this const struct is registering the tuner types with matching pushbuttons.
	QVBoxLayout *mLayout=nullptr;
	QPushButton *mButton[NUM_TUNER_BUTTONS]={nullptr};
	enum eTunerType mTunerType=TUNER_UNDEF;

	Tuners *mTuner=nullptr;
	QMutex mLock;
	
};
#endif
