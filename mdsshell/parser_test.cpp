/** @filer parsert_test.cpp - test harness for parser class. */


#include <stdio.h>

#include <vector>
#include "parser.h"


int main(int argc, const char* argv[])
{
	char* buf = (char*)malloc(4096);


	buf[0] = '\0';

	for (int ibuf = 0; ibuf < argc; ++ibuf){
		strcat(buf, argv[ibuf]);
		strcat(buf, " ");
	}

	printf("Buffer:%s\n", buf);

	Parser<vector<string> > parser(buf);

	int rc = 0;
	for (vector<string>::const_iterator i = parser.list().begin();
		     i != parser.list().end(); ++i){
		printf( "[%02d] = %2d:\"%s\"\n", 
			rc, strlen(i->c_str()), i->c_str());
		if (rc == 1){
			parser.list()[rc] = string("Substitution Made Here");
		}
		++rc;
	}	

	for (vector<string>::const_iterator i = parser.list().begin();
		     i != parser.list().end(); ++i){
		printf( "[%02d] = %2d:\"%s\"\n", 
			rc, strlen(i->c_str()), i->c_str());
		++rc;
	}	


}
