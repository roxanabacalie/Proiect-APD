#include <advanlys.h>
#include <utility.h>
#include <formatio.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "proiect.h"

// Constants
#define SAMPLE_RATE		0
#define NPOINTS			1


static int mainPanel;
static int panel2;
BOOL load=0;

// Global variables
int waveInfo[2]; //waveInfo[0] = sampleRate
				 //waveInfo[1] = number of elements
double sampleRate = 0.0;
int npoints = 2;
double *waveData = {0};
double *envelope=0;
double *filt=0; //aici o sa am semnalul filtrat
int plotHandle = -1; //handle pentru grafic
int start = 0, stop = 1;
int N; //nr de puncte intr-o secunda
BOOL median=0;//variabila prin care vedem ce filtru avem selectat

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((mainPanel = LoadPanel (0, "proiect.uir", MAIN_PANEL)) < 0)
		return -1;
	if ((panel2 = LoadPanel (0, "proiect.uir", PANEL2)) < 0)
		return -1;
	
	DisplayPanel (mainPanel);
	RunUserInterface ();
	DiscardPanel (mainPanel);
	return 0;
}

int CVICALLBACK OnButtonLoad (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	double min=0.0;
	double max=0.0;
	double medie=0.0;
	int minIndex;
	int maxIndex;
	double mediana=0.0;
	
	switch (event)
	{
		case EVENT_COMMIT:
			load=1;
			SetInputMode(panel, MAIN_PANEL_COMMANDBUTTON_APLICA, 1); //aici activez butonul pentru aplica
			
			//executa script python pentru conversia unui fisier .wav in .txt
			//LaunchExecutable("python main.py");
			
			//astept sa fie generate cele doua fisiere 
			//Delay(4);
			
			//incarc informatiile privind rata de esantionare si numarul de valori
			FileToArray("waveInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			sampleRate = waveInfo[SAMPLE_RATE];
			npoints = waveInfo[NPOINTS];
			
			N = npoints/6;
			
			//alocare memorie pentru numarul de puncte
			waveData = (double *) calloc(npoints, sizeof(double));
			
			//incarcare din fisierul .txt in memorie (vector)
			FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			
			//afisare pe grapf
			PlotY(panel, MAIN_PANEL_GRAPH, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			
			//LaunchExecutable("pythonAnvelopa mainAnv.py");
			
			//astept sa fie generate cele doua fisiere 
			//Delay(4);
			
			//alocare memorie pentru numarul de puncte
			envelope = (double *) calloc(npoints, sizeof(double));
			
			//incarcare din fisierul .txt in memorie (vector)
			FileToArray("anvelopa.txt", envelope, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			
			//afisare pe grapf
			PlotY(panel, MAIN_PANEL_GRAPH, envelope, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
			
			MaxMin1D(waveData, npoints, &max, &maxIndex, &min, &minIndex);
			SetCtrlVal(panel, MAIN_PANEL_Minim, min); //setez minimul
			SetCtrlVal(panel, MAIN_PANEL_Maxim, max); //setez maximul
			SetCtrlVal(panel, MAIN_PANEL_MIN_INDEX, minIndex); //setez indexul minim
			SetCtrlVal(panel, MAIN_PANEL_MAX_INDEX, maxIndex); //setez indexul maxim
			Mean(waveData, npoints, &medie); //calculez media
			double dispersie=(min-max)/(min+max); //calculez dispersia
			SetCtrlVal(panel, MAIN_PANEL_MEDIE, medie); //setez media
			SetCtrlVal(panel, MAIN_PANEL_DISPERSIE, dispersie); //setez dispersia
			Median(waveData, npoints, &mediana); //calculez mediana
			SetCtrlVal(panel, MAIN_PANEL_MEDIANA, mediana); //setez mediana
			
			//calculez nr de treceri prin zero
			int nrOfZeroes=0;
			for(int i=0; i<npoints-1; ++i)
			{
				if(waveData[i]*waveData[i+1]<=0)
				{
					nrOfZeroes++;
				}
			}
			
			SetCtrlVal(panel, MAIN_PANEL_NR_TRECERI_PRIN_ZERO, nrOfZeroes); //setez nr de treceri prin zero
			
			static int intervals=100; //aici retin nr de intervale pentru histograma
    		static ssize_t hist[300];
    		static double axis[300];
			GetCtrlVal (panel, MAIN_PANEL_NR_DE_INTERVALE, &intervals); //iau valoarea nr de intervale
			Histogram(waveData, npoints, min, max, hist, axis, intervals); //calculez histograma
			PlotXY (panel, MAIN_PANEL_HISTOGRAM, axis,  hist, intervals, VAL_DOUBLE, VAL_SSIZE_T, VAL_VERTICAL_BAR, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED); //plotez histograma
			
			break;
	}
	return 0;
}

int CVICALLBACK OnMainPanel (int panel, int event, void *callbackData,
							 int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface(0);
			break;
	}
	return 0;
}



int CVICALLBACK onApplyButton (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	int filter=0; //cu variabila asta vad ce filtru am selectat
	int n=32; //dimensiunea pentru filtru prin mediere
	switch (event)
	{
		case EVENT_COMMIT:
			
			//sterg plotarile anterioare
			DeleteGraphPlot (panel, MAIN_PANEL_GRAPH_FILTRAT, plotHandle, VAL_IMMEDIATE_DRAW);
			DeleteGraphPlot (panel, MAIN_PANEL_GRAPH_IN_SECONDS, -1, VAL_IMMEDIATE_DRAW);
			
			//activez butoanele next, prev si derivata
			SetInputMode(panel, MAIN_PANEL_COMMANDBUTTON_NEXT,1); 
			SetInputMode(panel, MAIN_PANEL_COMMANDBUTTON_PREV,1);
			SetInputMode(panel, MAIN_PANEL_BUTTON_DERIVATA, 1);
			
			filt = (double *) calloc(npoints, sizeof(double));
			
			double alpha;
			
			GetCtrlVal(panel, MAIN_PANEL_RING_FILTER, &filter); //aflu ce filtru folosesc
			
			if(filter==1) //filtrare de ordin 1
			{
				GetCtrlVal(panel, MAIN_PANEL_NUMERIC_ALPHA, &alpha);
				filt[0]=alpha*waveData[0];
				for(int i=1; i<npoints; ++i)
				{
					filt[i]=(1-alpha)*filt[i-1]+alpha*waveData[i];
				}
				median=0;
			}
			else //filtrare prin mediere
			{
				GetCtrlVal(panel, MAIN_PANEL_NUMERIC_DIM_FER, &n);
				double sum=0.0;
				for(int i=0; i<n; ++i)
				{
					sum+=waveData[i];
				}
				for(int i=0; i<n; ++i)
				{
					filt[i]=sum/n;
				}
				for(int i=n; i<npoints; ++i)
				{
					sum=sum-waveData[i-n]+waveData[i];
					filt[i]=sum/n;
				}
				median=1;
			}
			
			
			//setez intervalul initial de secunde
			SetCtrlVal(panel, MAIN_PANEL_NUMERIC_START_SECONDS,0);
			SetCtrlVal(panel, MAIN_PANEL_NUMERIC_STOP_SECONDS, 1);
			
			//afisez graficul filtrat, graficul in secunde si anvelopa
			plotHandle=PlotY(panel, MAIN_PANEL_GRAPH_FILTRAT, filt, N, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED); //plotez semnalul filtrat pe primul interval de secunde
			PlotY(panel, MAIN_PANEL_GRAPH_IN_SECONDS, waveData, N, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED); //plotez graficul initial pe primul interval de secunde
			PlotY(panel, MAIN_PANEL_GRAPH_IN_SECONDS, envelope, N, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE); //plotez anvelopa pe primul interval de secunde
			
			int imagineFiltrata;
			int imagineGraph;
			char * fileName;
			char * fileName2;
			fileName = (char *) calloc(100, sizeof(char));
			fileName2 = (char *) calloc(100, sizeof(char));
			
			GetCtrlDisplayBitmap(panel, MAIN_PANEL_GRAPH_FILTRAT, 1, &imagineFiltrata);
			GetCtrlDisplayBitmap(panel, MAIN_PANEL_GRAPH_IN_SECONDS, 1, &imagineGraph);
			
			//in functie de filtru schimb numele imaginii si le salvez
			if(median == 1)
				sprintf(fileName, "%s%d%s%d%s", "Images/filtrare_prin_mediere_de_la_secunda_", 0, "_pana_la_secunda_", 1,".png");
			else
				sprintf(fileName, "%s%d%s%d%s", "Images/filtrare_ordinul_I_de_la_secunda_", 0, "_pana_la_secunda_", 1,".png");
				
			sprintf(fileName2, "%s%d%s%d%s", "Images/graph_initial_de_la_Secunda", 0, "_pana_la_secunda_", 1,".png");
			SaveBitmapToPNGFile(imagineGraph, fileName2);
			SaveBitmapToPNGFile(imagineFiltrata, fileName);
			DiscardBitmap(imagineFiltrata);
			DiscardBitmap(imagineGraph);
			break;
	}
	return 0;
}

int CVICALLBACK onChangeSeconds (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2) //functie pentru butoanele prev si next
{
	int val = 0;
	
	
	double * tempData; //aici o sa retin temporar semnalul filtrat pe un anumit interval de secunde
	double *tempSignal; //aici o sa retin temporar semnalul initial pe un anumit interval de secunde
	double *tempEnvelope; //aici o sa retin temporar anvelopa pe un anumit interval de secunde
	
	int imagineFiltrata;
	int imagineGraph;
	char * fileName;
	char *fileName2;
	
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel, control, &val);
			
			GetCtrlVal(panel, MAIN_PANEL_NUMERIC_START_SECONDS, &start);
			GetCtrlVal(panel, MAIN_PANEL_NUMERIC_STOP_SECONDS, &stop);
			
			fileName = (char *) calloc(100, sizeof(char));
			fileName2 = (char *) calloc(100, sizeof(char));
			
			tempData = (double *) calloc(N, sizeof(double));
			tempSignal = (double *) calloc(N, sizeof(double));
			tempEnvelope = (double *) calloc(N, sizeof(double));
			
			switch(control)
			{
				case MAIN_PANEL_COMMANDBUTTON_NEXT:
					switch(event)
					{
						case EVENT_COMMIT:
							if(stop<6) //daca trec de secunda 6, sa nu mai aiba efect butonul next
							{
								DeleteGraphPlot (panel, MAIN_PANEL_GRAPH_FILTRAT, plotHandle, VAL_IMMEDIATE_DRAW);
								DeleteGraphPlot (panel, MAIN_PANEL_GRAPH_IN_SECONDS, -1, VAL_IMMEDIATE_DRAW);
								
								++start;
								++stop;
								
								SetCtrlVal(panel, MAIN_PANEL_NUMERIC_START_SECONDS, start);
								SetCtrlVal(panel, MAIN_PANEL_NUMERIC_STOP_SECONDS, stop);
							
								for(int i=0; i<N; ++i)
								{
									tempData[i]=filt[start*N+i];
									tempSignal[i]=waveData[start*N+i];
									tempEnvelope[i]=envelope[start*N+i];
								}
							
								plotHandle=PlotY(panel, MAIN_PANEL_GRAPH_FILTRAT, tempData, N, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
								PlotY(panel, MAIN_PANEL_GRAPH_IN_SECONDS, tempSignal, N, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
								PlotY(panel, MAIN_PANEL_GRAPH_IN_SECONDS, tempEnvelope, N, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
							
							}
							break;
					}
					break;
					
				case MAIN_PANEL_COMMANDBUTTON_PREV:
					switch(event)
					{
						case EVENT_COMMIT:
							if(stop>1) //daca trec de secunda 1 sa nu mai aiba efect butonul prev
							{
								DeleteGraphPlot (panel, MAIN_PANEL_GRAPH_FILTRAT, plotHandle, VAL_IMMEDIATE_DRAW);
								DeleteGraphPlot(panel, MAIN_PANEL_GRAPH_IN_SECONDS, -1, VAL_IMMEDIATE_DRAW);
								
								--start;
								--stop;
								
								SetCtrlVal(panel, MAIN_PANEL_NUMERIC_START_SECONDS, start);
								SetCtrlVal(panel, MAIN_PANEL_NUMERIC_STOP_SECONDS, stop);
							
								for(int i=0; i<N; ++i)
								{
									tempData[i]=filt[start*N+i];
									tempSignal[i]=waveData[start*N+i];
									tempEnvelope[i]=envelope[start*N+i];
								}
							
								plotHandle=PlotY(panel, MAIN_PANEL_GRAPH_FILTRAT, tempData,N, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
								PlotY(panel, MAIN_PANEL_GRAPH_IN_SECONDS, tempSignal, N, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
								PlotY(panel, MAIN_PANEL_GRAPH_IN_SECONDS, tempEnvelope, N, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
								
							}
							break;
					}
					break;
			}
			
			GetCtrlDisplayBitmap(panel, MAIN_PANEL_GRAPH_FILTRAT, 1, &imagineFiltrata);
			GetCtrlDisplayBitmap(panel, MAIN_PANEL_GRAPH_IN_SECONDS, 1, &imagineGraph);
			
			
			//in functie de filtru se schimba numele imaginii
			if(median == 1)
				sprintf(fileName, "%s%d%s%d%s", "Images/filtrare_prin_mediere_de_la_secunda_", start, "_pana_la_secunda_", stop,".png");
			else
				sprintf(fileName, "%s%d%s%d%s", "Images/filtrare_ordinul_I_de_la_secunda_", start, "_pana_la_secunda_", stop,".png");
			
			sprintf(fileName2, "%s%d%s%d%s", "Images/graph_initial_de_la_secunda_", start, "_pana_la_secunda_", stop,".png");
			
			SaveBitmapToPNGFile(imagineFiltrata, fileName);
			SaveBitmapToPNGFile(imagineGraph, fileName2);
			
			DiscardBitmap(imagineFiltrata);
			DiscardBitmap(imagineGraph);
			free(tempData);
			break;	
	}
	return 0;
}

int CVICALLBACK OnButtonDerivata (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	int plotDerivata=0;
	switch (event)
	{
		case EVENT_COMMIT:
				DeleteGraphPlot (panel, MAIN_PANEL_GRAPH_DERIVATA, -1, VAL_IMMEDIATE_DRAW);
				double* derivata, *tempDerivata;
				derivata = (double *) calloc(npoints, sizeof(double));
				tempDerivata = (double *) calloc(npoints/6, sizeof(double));
				//calculez derivata
				for(int i=0; i<npoints-1; ++i)
				{
					derivata[i]=waveData[i+1]-waveData[i];
				}
				for(int i=0; i<npoints/6-1;++i)
				{
					tempDerivata[i]=derivata[start*N+i];
				}
				//plotez derivata
				plotDerivata=PlotY(panel, MAIN_PANEL_GRAPH_DERIVATA, tempDerivata, npoints/6, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);		
			break;
	}
	return 0;
}

int CVICALLBACK OnSwitchPanel (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(panel == mainPanel)
			{
				SetCtrlVal(panel2, PANEL2_SWITCH_PANEL, 1);
				DisplayPanel(panel2);
				HidePanel(mainPanel);
			}
			else
			{
				SetCtrlVal(mainPanel, MAIN_PANEL_SWITCH_PANEL, 0);
				DisplayPanel(mainPanel);
				HidePanel(panel2);
			}
			break;
	}
	
	
	
	
	
	return 0;
}

int CVICALLBACK OnFrequencyPanel (int panel, int event, void *callbackData,
								  int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface(0);
			break;
	}
	return 0;
}

int CVICALLBACK OnClickTimer (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	WindowConst winConst;
	int numberOfPoints;
	
	double autoSpectrum[npoints];
	double df=0.0;
	double frequencyPeak;
	double powerPeak;
	double frequencySpan=0.0;
	switch (event)
	{
		case EVENT_TIMER_TICK:
			if(load==1)
			{
				
				GetCtrlVal(panel2, PANEL2_NUMERIC_NR_PUNCTE, &numberOfPoints);
				ScaledWindowEx (waveData, numberOfPoints, RECTANGLE_, 0, &winConst);
				AutoPowerSpectrum(waveData, numberOfPoints, 0, autoSpectrum, &df);
				PowerFrequencyEstimate (autoSpectrum, numberOfPoints, 0.0, winConst, df, 7, &frequencyPeak, &powerPeak);
				PlotY(panel2, PANEL2_GRAPH_SPECTRU, autoSpectrum, numberOfPoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				SetCtrlVal(panel2, PANEL2_NUMERIC_FREQ_PEAK, frequencyPeak);
				SetCtrlVal(panel2, PANEL2_NUMERIC_POWER_PEAK, powerPeak);
				
			}
			break;
	}
	return 0;
}
