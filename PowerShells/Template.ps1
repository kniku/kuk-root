<#
.Description
This is a template ps1 script.
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
	Get-Help $MyInvocation.MyCommand.Source
	exit 0
}

