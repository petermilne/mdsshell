#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define MAXCHAN 96
#define MEANFILE "/dev/mean/XX"
#define DIO32_BIT "/dev/rtmdio32/dio32_bit"
#define THRESHOLD 500

int maxchan = MAXCHAN;

void getMeans(int* means) 
{
	FILE* fp = fopen(MEANFILE, "r");
	int nread;

	if (!fp){
		perror("failed to open " MEANFILE);
		exit(errno);
	}
	nread = fread(means, sizeof(int), maxchan, fp);
	if (nread != maxchan){
		perror("read came up short");
		exit(errno);
	}	

	fclose(fp);
}

void setDIO32_bit(int bit, int value)
{
	FILE* fp = fopen(DIO32_BIT, "w");
	if (fp == 0){
		perror("failed to open" DIO32_BIT);
		exit(errno);		
	}
	fprintf(fp, "%d %d\n", bit, value);
	fclose(fp);
}





int over_threshold(int *means)
{
	int ch;
	int over = 0;

	for (ch = 0; ch != maxchan; ++ch){
		if (means[ch] > THRESHOLD){
			++over;
		}
	}
	return over;
}

void show_threshold(int *means, int ok)
{
	int ch;
	
	for (ch = 0; ch != maxchan; ++ch){
		putchar(means[ch]>THRESHOLD? 
			(ch%32)==ok? 'X': '?' : '_');
	}
	putchar('\n');
}

int main(int argc, char* argv[]){
	int data[MAXCHAN];
	int bit = 0;
	int retries;

	if (argc > 1){
		maxchan = atoi(argv[1]);
	}

	printf( "%s maxchan %d\n", "$Version:$", maxchan);

	for (bit = 0; bit < 32; ++bit){
//		printf("bit %d\n", bit);

		setDIO32_bit(bit, 1);

		retries = 0;
		do {
			getMeans(data);
//			show_treshold(data);
		} while(over_threshold(data) == 0 && ++retries < 20);

		if (retries >= 20){
			show_threshold(data, bit);
			fprintf(stderr, "ERROR:TIMEOUT waiting for non-zero\n");
		}

		if (over_threshold(data) < maxchan/32){
			fprintf(stderr, "ERROR: bust channel? %d\n",
					bit+1);
		}else if (over_threshold(data) != maxchan/32){
			fprintf(stderr, "ERROR: crossed wires! %d\n",
					bit + 1);
		}
		show_threshold(data, bit);

		setDIO32_bit(bit, 0);

		retries = 0;
		do {
			getMeans(data);
//			show_threshold(data);
		} while (over_threshold(data) && ++retries < 20);

		if (retries >= 20){
			show_threshold(data, bit);
			fprintf(stderr, "ERROR:TIMEOUT waiting for zero\n");
		}
	}

	return 0;
}
