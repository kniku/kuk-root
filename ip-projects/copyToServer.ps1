Param (
	[Parameter(Mandatory=$true)][string]$SourceFile,
	[Parameter(Mandatory=$true)][string]$TargetFile,
	[switch]$Forced
)
if (!$Forced) {
	echo "Transfer $SourceFile => root@vmanager8619.premium-vserver.net:$TargetFile - Ok?"
	pause
}

PSCP.EXE -pwfile $PSScriptRoot\.local-secret $SourceFile root@vmanager8619.premium-vserver.net:$TargetFile
