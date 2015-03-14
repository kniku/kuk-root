#include "ptools.h"

void Bin2HexDump(void *buf,char *hd,long bufsize, int umbruch, long *offs, int ctrl)
{
	long l,ll;
	unsigned char *pi=buf;
	unsigned char *bufbase=buf;
	
	if (buf&&hd&&bufsize>0&&umbruch>0) {
		if ((ctrl&HD_CTRL_Offs)&&offs) {
			if (ctrl&HD_CTRL_OffsDec) {
				sprintf(hd,"%010ld: ",*offs);
			} else {
				sprintf(hd,"%010X: ",*offs);
			}
			hd+=12;
		}
		for (l=0;l<bufsize;l++) {
			if (l>0&&(l%umbruch)==0) {
				if (ctrl&HD_CTRL_Ascii) {
					for (ll=l-umbruch;ll<l;ll++) {
						sprintf(hd++,"%c",bufbase[ll]>=30&&bufbase[ll]<127?bufbase[ll]:'.');
					}
				}
				sprintf(hd++,"\n");
				if ((ctrl&HD_CTRL_Offs)&&offs) {
					if (ctrl&HD_CTRL_OffsDec) {
						sprintf(hd,"%010ld: ",*offs);
					} else {
						sprintf(hd,"%010X: ",*offs);
					}
					hd+=12;
				}
			}
			sprintf(hd,"%02X ",(int)*pi);
			if (offs) (*offs)++;
			hd+=3;pi++;
		}		
		if (l%umbruch&&(ctrl&HD_CTRL_Ascii)) {
			for (ll=0;ll<umbruch-l%umbruch;ll++) {
				sprintf(hd,"   ",bufbase[ll]>=30?bufbase[ll]:'.');
				hd+=3;
			}
			for (ll=l-l%umbruch;ll<l;ll++) {
				sprintf(hd++,"%c",bufbase[ll]>=30&&bufbase[ll]<127?bufbase[ll]:'.');
			}			
		}
	}
}

void HexDumpFile(char *fn, int line,int ctrl)
{
	char 	*pBuf=NULL,*pStr=NULL;
	FILE 	*f;
	int		br;
	long	bsize=(CF_BUFSIZE/line)*line;
	long	offs=0;
	
	if (fn) {
		if ((pBuf=(char *)malloc(bsize))&&
			(pStr=(char *)malloc(HD_HDBUFSIZE(bsize,line)))&&
			(f=fopen(fn,"rb"))) {
			while (!feof(f)) {
				br=fread(pBuf,1,bsize,f);
				Bin2HexDump(pBuf,pStr,br,line,&offs,ctrl);
				printf("%s\n",pStr);
			}
			fclose(f);
		}
		if (pBuf) free(pBuf);
	}
}

int CryptFile(char *fn,char *cfn,char *pwd)
{
	char 			*pBuf=NULL;
	unsigned int	br,i;
	long 			l=0;
	unsigned char 	xb=0;
	FILE 			*f,*fout;
	
	if (pwd&&fn) {
		for (i=0;i<strlen(pwd);i++) {
			l=l+pwd[i];
		}
		xb=l%256;
		if (cfn&&strcmp(fn,cfn)) {
			fout=fopen(cfn,"wb");
		} else {
			fout=stdout;
		}
		if (fout) {
			if (fn&&(pBuf=(char *)malloc(CF_BUFSIZE))&&
				(f=fopen(fn,"rb"))) {
 				while (!feof(f)) {
					br=fread(pBuf,1,CF_BUFSIZE,f);
					for (i=0;i<br;i++) {
						pBuf[i]^=xb;
						xb=xb<255?xb+1:0;
					}
					fwrite(pBuf,1,br,fout);	
				}
				fclose(f);
			}
			if (pBuf) free(pBuf);
			fclose(fout);
		}
	}
	return 1;
}






