[CmdletBinding()]
param (
    [Parameter()]
    [string[]]
    $ExcludeExtensions = @(".h", ".hpp", ".c", ".cpp", ".ps1", ""),

    [Parameter()]
    [Hashtable]
    $Replace = @{
        "192\.168\.4\.1" = "%laddr%"
    }
)

$currentDir = (Get-Location).Path
$staticFiles = Get-ChildItem -File -Recurse
$staticContentHeaderFiles = @()
$indexFileIncludePaths = @()

foreach ($file in $staticFiles) {
    if ($ExcludeExtensions -contains $file.Extension) {
        Write-Verbose -Message "Skipping file `"$($file.FullName)`" (extension in exclude list)"
        continue
    }

    $fileContent = Get-Content -Path $file.FullName -Raw -Encoding UTF8

    foreach($replacePattern in $Replace.Keys)
    {
        $fileContent = $fileContent -replace $replacePattern, $Replace[$replacePattern]
    }

    $fileDirectory = $file.Directory.FullName
    $headerFileName = "$($file.Name -replace '\.', '_')"
    $headerFilePath = Join-Path -Path $fileDirectory -ChildPath "$($headerFileName).h"
    $headerFileContent = "#ifndef $($headerFileName.ToUpper())_H
#define $($headerFileName.ToUpper())_H

#include `"Arduino.h`"

const char $($headerFileName)[] PROGMEM = R`"rawliteral(
$($fileContent)
)rawliteral`";

#endif /* $($headerFileName.ToUpper())_H */"

    Set-Content -Path $headerFilePath -Value $headerFileContent -Encoding UTF8

    $staticContentHeaderFiles += $headerFilePath
    $indexFileIncludePaths += $headerFilePath -replace "^$([System.Text.RegularExpressions.Regex]::Escape($currentDir))\\", ""
}

$indexFileIncludeDirectives = ($indexFileIncludePaths | ForEach-Object -Process { "#include `"$_`"" }) -join "`n"
$indexFileContent = "#ifndef STATICCONTENT_H
#define STATICCONTENT_H

$indexFileIncludeDirectives

#endif /* STATICCONTENT_H */"

$indexFilePath = Join-Path -Path $currentDir -ChildPath "staticcontent.h"
Set-Content -Path $indexFilePath -Value $indexFileContent -Encoding UTF8
