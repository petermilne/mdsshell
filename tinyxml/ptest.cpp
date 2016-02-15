/*
   Test program for TinyXML.
*/


#include "tinyxml.h"

#ifdef TIXML_USE_STL
	#include <iostream>
	#include <sstream>
	using namespace std;
#else
	#include <stdio.h>
#endif



//
// This file demonstrates some basic functionality of TinyXml.
// Note that the example is very contrived. It presumes you know
// what is in the XML file. But it does test the basic operations,
// and show how to add and remove nodes.
//

void handleInfo(TiXmlNode* info)
{
	if (info){
		TiXmlNode* child = 0;
		while ((child = info->IterateChildren(child)) != 0){
			fprintf(stderr, "Info:");
//			child->Print(stdout);
		}
	}
}

void handleData(TiXmlNode* data)
{
	if (data){
		TiXmlNode* child = 0;
		while ((child = data->IterateChildren(child)) != 0){
			fprintf(stderr, "Data:");
//			child->Print(stdout);
		}		
	}
}

#define IMIN 0
#define IMAX 1

#define MAXCHAN 96 

void getVoltsRange(TiXmlElement* range, int aichan)
{
	TiXmlNode *nominal_node;
	double nominal[2];
	double calibrated[MAXCHAN][2];

	if ((nominal_node = range->FirstChild("Nominal")) != 0){

		nominal_node->ToElement()->Attribute("min", nominal+IMIN);
		nominal_node->ToElement()->Attribute("max", nominal+IMAX);

		printf("getVoltsRange=%6.4fV,%6.4fV\n", nominal[IMIN], nominal[IMAX]);

//		printf( "Nominal %6.4fV,%8.4fV\n",
//			nominal->ToElement()->FirstChild("min")->
	}

	for (int ic = 0; ic != aichan; ++ic){
		calibrated[ic][IMIN] = nominal[IMIN];
		calibrated[ic][IMAX] = nominal[IMAX];
	}


	
	for ( TiXmlNode *child = 0; (child = range->IterateChildren(child)) != 0; ){
		TiXmlElement* element = child->ToElement();
		int ic;

		if ((element->Attribute("ch", &ic)) != 0){

			fprintf(stderr, "Channel ch %d min %s max %s\n",
			       ic, element->Attribute("min"), element->Attribute("max"));

			element->Attribute("min", &calibrated[ic-1][IMIN]);
			element->Attribute("max", &calibrated[ic-1][IMAX]);			
		}
	}

	for (int ic = 0; ic != aichan; ++ic){
		printf( "%.4f,%6.4f%c", 
			calibrated[ic][IMIN], calibrated[ic][IMAX],
			ic+1 == aichan? '\n': ',');
	}	
}
void getVoltsRange(TiXmlNode* data, const char* sw) {
	/* find the data element with attribute equals sw */

	int aichan = 0;
	data->ToElement()->Attribute("AICHAN", &aichan);

	printf("getVoltsRange()\n");
	if (data){
		TiXmlNode* child = 0;
		
		while((child = data->IterateChildren(child)) != 0){
			TiXmlElement* range = child->ToElement();
			if (range){
				if (range->Attribute("sw")){
					if (strcmp(range->Attribute("sw"), sw) == 0){
						printf( "Range matches switch %s\n", sw);
						getVoltsRange(range, aichan);
					}
				}
			}

		}
	}
	/* then print all the values in the correct format */
}
void handleAcqCalibrationDoc(TiXmlDocument& doc)
{
	TiXmlNode* cal = doc.FirstChild("AcqCalibration");
	handleInfo(cal->FirstChild("Info"));
	handleData(cal->FirstChild("Data"));

	getVoltsRange(cal->FirstChild("Data"), "1,0,0");
}

void processFile(const char *fname)
{
	TiXmlDocument doc(fname);
	if (doc.LoadFile()){
		doc.Print(stdout);
		
		if (doc.FirstChild("AcqCalibration")){
			handleAcqCalibrationDoc(doc);
		}else{
			fprintf(stderr, "AcqCalibrationNode not found - discarding\n");
		}
	}else{
		fprintf(stderr, "LoadFile %s failed\n", fname);
	}
}
int main(int argc, char *argv[])
{
	fprintf(stderr, "pgmwashere - files :%d\n", argc-1);

	for (int iarg = 1; iarg != argc; ++iarg){
		processFile(argv[iarg]);
	}
}


