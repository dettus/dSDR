#ifndef	CBDUMMY_H
#define	CBDUMMY_H
class CBDummy
{
	public:
		virtual	void startStop(bool started)=0;
		virtual	void fileOpen(char* filename)=0;
};
#endif

