/* ------------------------------------------------------------------------- */
/* mdsshell.c - provides a shell interface to mdstcpip                       */
/* ------------------------------------------------------------------------- */
/*   Copyright (C) 2003 Peter Milne, D-TACQ Solutions Ltd
 *                      <Peter dot Milne at D hyphen TACQ dot com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of Version 2 of the GNU General Public License
    as published by the Free Software Foundation;
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                */
/* ------------------------------------------------------------------------- */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <fcntl.h>


#include <ipdesc.h>

/**
 * usage - multicall binary
 * mdsshell - creates links a la busybox for:
 *
 - mdsConnect URL
 - mdsValue EXPR
 - mdsPut field value
*/


static void help(void)
{
	fprintf(stderr, 
		"mdsshell\n"
		"mdsConnect HOST\n"
		"mdsValue EXPR\n"
		"mdsPut FIELD VALUE\n");
	_exit(1);
}

#define HELP  help()



static int doMdsPut(int argc, char **argv)
{
	if (getenv("MDS_SOCK")){
		int sock = atoi(getenv("MDS_SOCK"));

		fprintf(stderr, "MDS_SOCK is %s\n", getenv("MDS_SOCK"));

		switch(argc){
		case 3: {
			char* field = argv[1];
			char* value = argv[2];

			float fval = atof(value);
			struct descrip vald = {DTYPE_FLOAT,0};
			int status;

			

			fprintf(stderr, 
				"MdsPut sock:%d field:%s v:%s value:%g\n",
			       sock, field, value, fval);

			vald.ptr = (void *)&fval;
			status = MdsPut(sock, field,"$",&vald,0); 
			if (!(status & 1)){
				fprintf(stderr, 
					"Error during put %d\n",status);
			}
			return 0;
		} default:
			fprintf(stderr, "ERROR: args field value\n");
		}
	}else{
		fprintf(stderr, "MDS_SOCK environment var is missing\n");
	}
	return -1;
}

static int doMdsValue(int argc, char **argv)
{
	if (getenv("MDS_SOCK")){
		int sock = atoi(getenv("MDS_SOCK"));

		fprintf(stderr, "MDS_SOCK is %s\n", getenv("MDS_SOCK"));

		switch(argc){
		case 2: {
			char* expr = argv[1];

			struct descrip ans = {DTYPE_FLOAT, 0};
			int status;

			fprintf(stderr, "MdsValue sock:%d expr %s\n",
			       sock, expr);

			status = MdsValue(sock, expr, &ans, 0); 
			if (!(status & 1)){
				fprintf(stderr, 
					"Error during value %d\n",status);
			}else{
				fprintf(stderr, "Success %g\n", 
					*(float*)ans.ptr);
				printf("%g\n", *(float*)ans.ptr);
			}
			return 0;
		} default:
			fprintf(stderr, "ERROR: args field value\n");
		}
	}else{
		fprintf(stderr, "MDS_SOCK environment var is missing\n");
	}
	return -1;
}

char SOCK_STRING[80];

static int doMdsConnect(int argc, char **argv)
{
	int status;
	struct descrip ans;
	float val = 9876;
	struct descrip vald = {DTYPE_FLOAT,0};
	long sock = ConnectToMds((argc > 1) ? argv[1] : 
				 "lost.pfc.mit.edu:9000");

	if (sock != -1)	{
		pid_t child;

/* we want to inherit this path */
		long flags = fcntl(sock, F_GETFD);
		
		fcntl(sock, F_SETFD, flags &= ~ FD_CLOEXEC);

		printf("status from MdsOpen = %d\n",MdsOpen(sock,"main",-1));

		sprintf(SOCK_STRING, "%ld", sock);
		setenv("MDS_SOCK", SOCK_STRING, 1);

		if (!(child = fork())){
			printf("about to execl /bin/sh");
			execl("/bin/sh", "/bin/sh", NULL);
		}else{
			int status;
			wait(&status);

			printf("Child exit %d\n", status);
		}
	}
	return 0;
}

static int doMdsIpTest(int argc, char **argv)
{
  int status;
  struct descrip ans;
  float val = 9876;
  struct descrip vald = {DTYPE_FLOAT,0};
  long sock = ConnectToMds((argc > 1) ? argv[1] : "lost.pfc.mit.edu:9000");
  if (sock != -1)
  {
    printf("status from MdsOpen = %d\n",MdsOpen(sock,"main",-1));
    ans.ptr = 0;
    if (MdsValue(sock,"f_float(member)",&ans,0) & 1)
    {
      printf("%g\n",*(float *)ans.ptr);
      val = *(float *)ans.ptr;
      val = val + (float)1.;
    }
    else
      printf("%s\n",ans.ptr);
    if (ans.ptr) 
    {
      free(ans.ptr);
      ans.ptr = 0;
    }
    vald.ptr = (void *)&val;
    status = MdsPut(sock,"member","$",&vald,0); 
    if (!(status & 1)) printf("Error during put %d\n",status);
    if (MdsValue(sock,"42.0",&ans,0) & 1)
      printf("1:%g\n",*(float *)ans.ptr);
    else
      printf("0:%s\n",ans.ptr);


    if (MdsValue(sock,"42.0/7",&ans,0) & 1)
      printf("1:%g\n",*(float *)ans.ptr);
    else
      printf("0:%s\n",ans.ptr);
    if (ans.ptr) free(ans.ptr);

  }
  return 0;
}


int main( int argc, char **argv)
{
	printf("mdsshell %s\n", argv[0]);

	if (strstr(argv[0], "mdsPut")){
		return doMdsPut(argc, argv);
	}else if (strstr(argv[0], "mdsValue")){
		return doMdsValue(argc, argv);
	}else if (strstr(argv[0], "mdsConnect")){
		return doMdsConnect(argc, argv);
	}else if (strstr(argv[0], "mdsiptest")){
		return doMdsIpTest(argc, argv);
	}else{
		HELP;
	}
}


