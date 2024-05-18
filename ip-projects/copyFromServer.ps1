Param (
	[Parameter(Mandatory=$true)][string]$SourceFile,
	[Parameter(Mandatory=$true)][string]$TargetFile
)
echo "Transfer root@vmanager8619.premium-vserver.net:$SourceFile => $TargetFile - Ok?"
pause

PSCP.EXE -pwfile .local-secret root@vmanager8619.premium-vserver.net:$SourceFile $TargetFile
