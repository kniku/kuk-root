Param (
	[Parameter(Mandatory=$true)][string]$SourceFile,
	[Parameter(Mandatory=$true)][string]$TargetFile
)
echo "Transfer root@vmanager8619.premium-vserver.net:$SourceFile => $TargetFile - Ok?"
pause

Invoke-Expression "$PSScriptRoot\Tools\PSCP.EXE -pwfile $PSScriptRoot\pwd.txt root@vmanager8619.premium-vserver.net:$SourceFile $TargetFile"
