#ifdef WIN32
#include <windows.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define APP_NAME	"sSleep"
#define APP_VERSION	"v1.0"

#ifndef WIN32
static void Sleep(int ms)
{
	sleep(ms/1000);
}
#endif

struct {
	int delay;
	int delay_from;
	int gf_verbose;
	int gf_random;
	int gf_debug;
} gc;

static void usage(char *msg)
{
	printf("\n"APP_NAME" - "APP_VERSION", (c) kuk 1996\n%s\n"
		"usage: "APP_NAME" [-v] [-r] [-f M] N\n\n"
		"\t-r ... random delay (from 0 or M to N msec)\n"
		"\t-f ... only with -r\n"
		"\t-v ... verbose (display seconds)\n\n"
		"holds execution for given delay (millisec)...\n",
		msg?msg:"");
	exit(0);
}


int main(int argc,char **argv)
{
	int i,sleeptime,g;
	char *s,c;

	/* explain usage if necessary */
	if (argc==1) usage(NULL);

	/* initialization */
	memset(&gc,0,sizeof(gc));

	/* parsing the arguments */
	for (i=1;(s=argv[i]);i++) {
		if (strcmp(s,"-r")==0) {
			gc.gf_random=1;
		} else if (strcmp(s,"-f")==0&&argv[i+1]) {
			gc.delay_from=atoi(argv[++i]);
		} else if (strcmp(s,"-v")==0) {
			gc.gf_verbose=1;
		} else if (strcmp(s,"-D")==0) {
			gc.gf_debug=1;
		} else {
			gc.delay=atoi(s);
		}
	}

	if (gc.delay<=0||gc.delay<=gc.delay_from) usage("!! invalid delay !!");

	/* create random delay */
	if (gc.gf_random) {
		srand((unsigned)time(NULL));
		rand();
		gc.delay=rand()%(gc.delay-gc.delay_from);
		gc.delay=gc.delay+gc.delay_from;
	}

	if (gc.gf_debug) printf("Delay=%d msec\n",gc.delay);

	/* going to bed now ... */
	i=1;c='0';g=0;
	while (gc.delay>0) {
		sleeptime=gc.gf_verbose?1000:gc.delay;
		gc.delay-=sleeptime;
		g++;
		if (gc.gf_debug) printf("sleeping=%d ms, rest=%d ms\n",sleeptime,gc.delay);
		Sleep(sleeptime);
		if (gc.gf_verbose&&gc.delay>=0) {
			if (i==0) c++;
			if (c>'9') c='0';
			fprintf(stdout,"%s%c%d",(g==1?"":"\b"),(i==0?c:'.'),i++);
			fflush(stdout);
			if (i>9) i=0;
		}
	}

	if (gc.gf_verbose) {
		printf("\b ");
#ifndef WIN32
		printf("\n");
#endif
		}
	return 1;
}