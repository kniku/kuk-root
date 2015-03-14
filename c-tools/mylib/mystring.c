#include "ptools.h"

void StrRemoveChar(char *s,char c) 
{
	while (s) {
		s = strchr(s,c);
		if (s) {
			memmove(s,s+1,strlen(s));
		}
	}
}

char *StrTrimLeft(char *s,char c) 
{
	int i;

	if (s) {
		for (i=0;*(s+i)&&*(s+i)==c;i++);
		if (i>0) {
			memmove(s,s+i,strlen(s));
		}
	}
	return s;
}

char *StrTrimRight(char *s,char c) 
{
	int i;

	if (s) {
		for (i=strlen(s)-1;i>=0&&s[i]==c;i--) {
			s[i] = '\0';
		}
	}
	return s;
}

char *StrTrim(char *s,char c) 
{
	char *rv;
	
	rv=StrTrimLeft(s,c);
	rv=StrTrimRight(rv,c);
	return rv;
}

char *StrRemoveQuota(char *s) 
{
	if (s) {
		while ((*s=='"'||*s=='\'')&&
			   (s[strlen(s)-1]=='"'||s[strlen(s)-1]=='\'')) {
			s[strlen(s)-1]=0;
			*s=' ';
			s=StrTrimLeft(s,' ');
		}
	}
	return s;
}

