#include <QObject>

class FileInput : public QObject {
	Q_OBJECT

	public:
		FileInput();

	public slots:
		void process();

	signals:
		void finished();
		void error(QString err);

	private:
		// add your variables here
};