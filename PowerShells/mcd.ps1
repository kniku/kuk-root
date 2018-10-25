# Param ([Parameter(Mandatory=$true)][string]$Path)
Param ([string]$Path)

if (!$Path) {
	Write-Host -ForegroundColor green "usage $($MyInvocation.MyCommand.Name) [DIRECTORY]
	...create and enter new directory..."
	exit
}

if (-not (Test-Path -Path $Path)) { mkdir $Path| Out-Null }
else {Write-Host -ForegroundColor green "hint: directory already exists"}

Push-Location $Path
