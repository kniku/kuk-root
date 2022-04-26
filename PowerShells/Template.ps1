<#
.Description
This is a template ps1 script.
#>
Param (
    # Description of: help...
	[switch]$help = $false
)

if ($PSBoundParameters.Count -eq 0) {
	Get-Help $MyInvocation.MyCommand.Source
	exit 0
}

