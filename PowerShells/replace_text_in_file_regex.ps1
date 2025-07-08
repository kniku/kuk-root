$mappingFile = '.scada-codegen/scada-modules-mapping.json'

$content = Get-Content -Raw $mappingFile
$SearchPattern = '(.*?XXX.*?",\s*)"(.*?)"'
$content = $content -replace $SearchPattern,'$1"#$2"'
$content | Set-Content -Path $mappingFile
