Param (
	[Parameter(Mandatory=$true)][string]$Path,
	[string]$Name
)

if (!$Name) {
	Write-Host -ForegroundColor green @"
Usage $($MyInvocation.MyCommand.Name) [name]
"@
	exit
}

Write-Host $Path $Name

