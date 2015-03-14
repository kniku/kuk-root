
#include <stdio.h>
#include <time.h>
#include "tools.h"

void ptime(void)
{
	time_t t;
	time(&t);
	printf("%s",ctime(&t));
}

#define ftmp	"__tmp.bak"

int main (int argc, char **argv)
{
	GetOptsPtr opts;
	char f[256],p[256],o[256];

	opts=GetOpts("p:f:o:",argc,argv);

	if (opts) {
		if (GetOpt(opts,"f",f,sizeof(f))&&
			GetOpt(opts,"p",p,sizeof(p))) {
			if (GetOpt(opts,"o",o,sizeof(o))) {
				printf("crypting '%s'->'%s', wait ...\n",f,o);
				CryptFile(f,o,p);
			} else {
				printf("crypting '%s', wait ...\n",f);
				printf("renaming '%s' to %s...\n",f,ftmp);
				if (rename(f,ftmp)==0) {
					CryptFile(ftmp,f,p);
					printf("deleting %s...\n",ftmp);
					remove(ftmp);
				} else {
					printf("renaming failed!\n");
				}
			}
			printf("done.\n");
		} else {
			printf("Aufruf: %s -f infile -p pwd [-o outfile]\n",argv[0]);
			printf("ver-/entschlÅsseln von files.\n");
			printf("verschlÅsseln von file1: %s file1 passwort\n",argv[0]);
			printf("decrypt:                 %s file1.x passwort\n",argv[0]);
			printf("ist outfile gegeben, so bleibt infile erhalten.\n");
			return -1;
		}
		opts=GetOptsDestroy(opts);
	}

	return 0;
}


