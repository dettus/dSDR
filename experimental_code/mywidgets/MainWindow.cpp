#include <QtWidgets>
#include "MainWindow.h"

MainWindow::MainWindow()
{
double fftspec[256]={52395.27408,50617.80270,50628.73158,51349.92093,49454.42528,48736.21686,49140.15275,48483.68014,48113.78296,51733.62490,49770.88743,49970.20853,51675.96194,53631.09027,53372.73552,57732.11658,60865.14586,58843.90261,62055.27732,60579.28417,59979.06971,63236.24499,64258.37208,67952.81950,66257.64361,68148.63029,69269.96572,72986.69367,73218.39620,75056.83163,87938.18680,187283.44630,165097.81278,203149.23966,191327.85180,184049.63913,189625.13554,173868.32894,162765.02157,193564.54543,209309.52984,193957.00934,211679.47054,196594.22753,177096.14595,198494.76273,155213.84032,173493.32154,223146.62820,186871.74591,221493.67661,206767.22152,168420.00684,201534.34808,199192.88764,204586.06754,236617.84753,206418.99434,186419.81680,246674.59954,192657.78307,228646.95077,254151.78457,191671.06780,250164.73935,210959.00455,180772.63233,242909.68678,187293.41846,224044.56528,235891.36757,180549.35145,224125.92023,187789.58224,178956.45376,201776.69638,179731.65773,230716.17937,221786.15741,211250.42729,190070.61759,156944.25068,188859.99409,162960.19629,193322.56680,203658.51362,169788.40668,184521.92650,157511.48390,155587.68746,190036.99554,196209.86829,207880.14428,214695.06588,179120.77656,189273.22912,178356.32858,178561.19244,229913.08098,213043.50719,201102.45379,198531.89278,158612.30008,147492.77347,192044.75960,194531.61543,202023.45596,202134.98734,153061.87148,158936.88184,177030.13504,170714.36951,234859.94523,223293.84739,193585.55946,197519.25163,167943.74913,194028.35115,222946.43967,229241.43552,245978.95897,237220.53916,182452.53715,150913.01433,171913.64758,145390.23662,178102.43516,198271.97836,200268.44539,197273.72214,162452.35462,182251.64116,223692.54626,204285.90530,224985.13995,216672.11722,180162.37935,168840.54066,171669.30149,155302.52306,192857.25527,225316.84477,162307.79273,196334.99556,158254.60724,154546.10613,355880.37759,164056.21664,208218.48120,192180.38535,152154.56985,167043.02671,157638.19025,156319.68356,196735.98030,192598.34747,177089.72587,205371.40280,151246.40330,144257.45607,174466.56912,160924.73548,173005.43472,177482.54354,128449.07070,132882.31777,136199.09045,130300.49517,171119.35584,166123.68957,149453.48906,174558.72440,152932.09431,139585.12927,201743.11372,177516.72859,205720.00854,251791.18899,172359.81120,200402.75323,186364.94206,166072.63696,214539.44673,152952.59792,169955.62531,168098.23926,132454.45203,161952.31566,190197.71784,171867.18451,204243.22476,217175.35789,162026.73361,197360.78921,177999.46861,185284.69507,233667.83864,198041.84474,233333.32936,220691.41796,150767.38456,180470.04132,164509.61921,175080.09405,198789.42584,178040.19585,164057.67374,189254.77480,171016.47989,178487.13026,221854.13841,191071.60749,232829.35040,211318.18533,172230.60321,205879.28288,170226.40242,189940.91674,188633.56323,197561.22171,199648.18171,170633.15113,158518.56877,97212.93467,83795.47735,84828.00265,80111.40852,78090.83174,79502.85117,78393.75663,76360.07664,76631.82610,74846.85387,76751.23318,74683.01356,72189.33463,72264.59270,68416.04017,68472.96278,68066.65373,66020.15372,66294.92034,71801.91387,66786.79455,64407.32053,63153.27458,64870.01786,62377.68679,59225.57953,58838.31127,55502.88871,57930.96943,56240.58079,53440.60347,53955.72187,52472.02682};

	int i;
	setWindowTitle(tr("hello World"));
	resize(500,500);
	wFrequency=new WFrequency(this);
	wFrequency->resize(500,100);
	wFrequency->move(500,50);
	wFrequency->show();
	wFrequency->repaint();


	wSpectrum=new WSpectrum(this);
	wSpectrum->resize(500,500);
	wSpectrum->move(500,200);
	wSpectrum->show();
	wSpectrum->setFFTsize(256);
	wSpectrum->plotSpectrum(fftspec,256);
	wSpectrum->zoomFit();
}


