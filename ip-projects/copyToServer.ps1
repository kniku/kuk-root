Param (
	[Parameter(Mandatory=$true)][string]$SourceFile,
	[Parameter(Mandatory=$true)][string]$TargetFile,
	[switch]$Forced
)
if (!$Forced) {
	echo "Transfer $SourceFile => root@vmanager8619.premium-vserver.net:$TargetFile - Ok?"
	pause
}

Invoke-Expression "$PSScriptRoot\Tools\PSCP.EXE -pwfile $PSScriptRoot\pwd.txt $SourceFile root@vmanager8619.premium-vserver.net:$TargetFile"
