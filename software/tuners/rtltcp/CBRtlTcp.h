#ifndef	CBRTLTCP_H
#define	CBRTLTCP_H
class CBRtlTcp
{
	public:
		virtual	bool setFrequency(int freqHz)=0;
		virtual void gainUp()=0;
		virtual	void gainDown()=0;
};
#endif

