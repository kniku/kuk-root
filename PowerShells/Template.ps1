<#PSScriptInfo
.VERSION 1.0.0
.GUID 2bf3a9df-611c-4a0b-9b17-2bbe85fb7209
.AUTHOR kuk
.COMPANYNAME Privat
#>

<#
.DESCRIPTION
my description
#>

[CmdletBinding(DefaultParameterSetName = 'seta')] # default parameter set if mutually exclusive parameters used
Param (
    # Description of: help...
	[switch]
	[parameter(ParameterSetName="seta",position=0)] # mutually exclusive parameters
	$help = $false,
    # Description of: help2...
	[switch]
	[parameter(ParameterSetName="setb",position=0)] # mutually exclusive parameters
	$help2 = $true
)

if ($PSBoundParameters.Count -eq 0) {
	Get-Help $MyInvocation.MyCommand.Source -Detailed
	exit 0
}

# current directory
Write-Host (Get-Location).Path
# running script directory
Write-Host $PSScriptRoot
