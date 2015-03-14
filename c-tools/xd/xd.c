#include "tools.h"
#include <stdio.h>
#include <string.h>

int main(int argc,char **argv)
{
	int ctrl = HD_CTRL_Offs | HD_CTRL_Ascii;
	char *in_file;

	if (argc==1) {
		printf(
			"\nxd - hex dumper, v1.1, kuk 1999-2014"
			"\nusage: %s [-d] [file]"
			"\n\t-d ... data only\n",
			argv[0]);
		return -1;
	}
	
	in_file = argv[1];

	if (argc > 2)
	{
		in_file = argv[2];
		if (strcmp(argv[1], "-d") == 0)
		{
			ctrl &= !HD_CTRL_Offs;
		}
	}
		//	HexDumpFile(argv[1],16,HD_CTRL_Offs|HD_CTRL_Ascii);
	HexDumpFile(in_file, 16, ctrl);
	return 0;
}
