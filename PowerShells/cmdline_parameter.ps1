Param (
	[Parameter(Mandatory=$true)][string]$Path,
	[string]$Name,
	[string]$ParamWithDefault = 'default',
	[switch]$Recurse = $false,
    [ValidateSet('x', 'y', 'z')][string]$MySelection
)

# Anm.: Alle Parameter sind über $args zugänglich

if (!$Name) {
	Write-Host -ForegroundColor green @"
Usage $($MyInvocation.MyCommand.Name) [name]
"@
	exit
}

Write-Host $Path $Name

