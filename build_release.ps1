# ==============================================================================
# ROLC_StatusSimulator Release Build & Packaging Script
# ==============================================================================
param ()

$ErrorActionPreference = "SilentlyContinue"
$ProgressPreference = "SilentlyContinue"

$WorkspaceRoot = $PSScriptRoot
$BuildLogsDir = Join-Path $WorkspaceRoot "build_logs"
$BuildDir = Join-Path $WorkspaceRoot "build_release"
$DistDir = Join-Path $WorkspaceRoot "dist"
$PackageDir = Join-Path $DistDir "ROLC_StatusSimulator"

# Ensure build_logs directory exists
if (-not (Test-Path $BuildLogsDir)) {
    New-Item -ItemType Directory -Path $BuildLogsDir -Force | Out-Null
}

$BuildLogPath = Join-Path $BuildLogsDir "build.log"
$BuildSummaryPath = Join-Path $BuildLogsDir "build_summary.txt"

# Reset build log
Set-Content -Path $BuildLogPath -Value "" -Encoding utf8

# Tool Paths
$QtDir = "C:\Qt\6.10.1\mingw_64"
$MinGWDir = "C:\Qt\Tools\mingw1310_64"
if (-not (Test-Path $QtDir)) {
    $qtFind = Get-ChildItem "C:\Qt" -Directory -Filter "*mingw*" -Recurse -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($qtFind) { $QtDir = $qtFind.FullName }
}
if (-not (Test-Path $MinGWDir)) {
    $mingwFind = Get-ChildItem "C:\Qt\Tools" -Directory -Filter "*mingw*" -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($mingwFind) { $MinGWDir = $mingwFind.FullName }
}

$GccPath = Join-Path $MinGWDir "bin\gcc.exe"
$GxxPath = Join-Path $MinGWDir "bin\g++.exe"
$WindeployqtPath = Join-Path $QtDir "bin\windeployqt.exe"

$CMakeCmd = (Get-Command cmake -ErrorAction SilentlyContinue).Source
if (-not $CMakeCmd) { $CMakeCmd = "C:\Qt\Tools\CMake_64\bin\cmake.exe" }

# ------------------------------------------------------------------------------
# 1. Clean & Release Build (Time measurement starts from clean)
# ------------------------------------------------------------------------------
$BuildSw = [System.Diagnostics.Stopwatch]::StartNew()
$BuildSuccess = $true
$BuildRawOutput = @()

# Clean existing directories
if (Test-Path $BuildDir) {
    Remove-Item -Recurse -Force $BuildDir -ErrorAction SilentlyContinue
}
if (Test-Path $PackageDir) {
    Remove-Item -Recurse -Force $PackageDir -ErrorAction SilentlyContinue
}

# CMake Configure (Release)
$cmakeConfigureArgs = @(
    "-B", $BuildDir,
    "-G", "Ninja",
    "-DCMAKE_BUILD_TYPE=Release",
    "-DCMAKE_PREFIX_PATH=$QtDir",
    "-DCMAKE_C_COMPILER=$GccPath",
    "-DCMAKE_CXX_COMPILER=$GxxPath"
)

$cfgOutput = & $CMakeCmd @cmakeConfigureArgs 2>&1
$BuildRawOutput += $cfgOutput
$cfgOutput | Out-File -FilePath $BuildLogPath -Encoding utf8 -Append

if ($LASTEXITCODE -ne 0) {
    $BuildSuccess = $false
} else {
    # CMake Build
    $buildOutput = & $CMakeCmd --build $BuildDir 2>&1
    $BuildRawOutput += $buildOutput
    $buildOutput | Out-File -FilePath $BuildLogPath -Encoding utf8 -Append
    if ($LASTEXITCODE -ne 0) {
        $BuildSuccess = $false
    }
}

$BuildSw.Stop()
$BuildDuration = [math]::Round($BuildSw.Elapsed.TotalSeconds, 2)

# Extract Warnings
$WarningList = @()
foreach ($line in $BuildRawOutput) {
    $str = $line.ToString()
    if ($str -match "warning[:\s]" -or $str -match "Warning[:\s]") {
        $WarningList += $str.Trim()
    }
}

# Write Build Summary (Same format as debug build summary)
$buildResultText = if ($BuildSuccess) { "OK" } else { "NG" }
$summaryLines = @(
    "==================================================",
    "ビルドログ サマリ (Build Log Summary)",
    "==================================================",
    "・ビルド結果: $buildResultText",
    "・ビルドにかかった時間: $BuildDuration 秒",
    "・ワーニング数: $($WarningList.Count) 件",
    "",
    "【ワーニング一覧】"
)
if ($WarningList.Count -eq 0) {
    $summaryLines += "  (なし - ワーニング 0 件)"
} else {
    foreach ($w in $WarningList) {
        $summaryLines += "  - $w"
    }
}
$summaryLines += "=================================================="
$summaryLines | Out-File -FilePath $BuildSummaryPath -Encoding utf8

if (-not $BuildSuccess) {
    exit 1
}

# ------------------------------------------------------------------------------
# 2. Packaging & Zip Generation in dist/
# ------------------------------------------------------------------------------
# Extract Version from CMakeLists.txt
$AppVersion = "1.0.0"
$CMakeListsPath = Join-Path $WorkspaceRoot "CMakeLists.txt"
if (Test-Path $CMakeListsPath) {
    $cContent = Get-Content $CMakeListsPath -Raw
    if ($cContent -match 'project\(ROLC_StatusSimulator\s+VERSION\s+([0-9\.]+)') {
        $AppVersion = $matches[1]
    }
}

if (-not (Test-Path $DistDir)) {
    New-Item -ItemType Directory -Path $DistDir -Force | Out-Null
}
if (-not (Test-Path $PackageDir)) {
    New-Item -ItemType Directory -Path $PackageDir -Force | Out-Null
}

# Copy main executable
$MainExe = Join-Path $BuildDir "ROLC_StatusSimulator.exe"
if (Test-Path $MainExe) {
    Copy-Item -Path $MainExe -Destination $PackageDir -Force
}

# windeployqt
if (Test-Path $WindeployqtPath) {
    $pkgExe = Join-Path $PackageDir "ROLC_StatusSimulator.exe"
    if (Test-Path $pkgExe) {
        & $WindeployqtPath --no-translations --compiler-runtime $pkgExe 2>&1 | Out-Null
    }
}

# MinGW runtime DLLs
$MinGWBin = Join-Path $MinGWDir "bin"
if (Test-Path $MinGWBin) {
    $RuntimeDlls = @("libstdc++-6.dll", "libgcc_s_seh-1.dll", "libwinpthread-1.dll")
    foreach ($dll in $RuntimeDlls) {
        $srcDll = Join-Path $MinGWBin $dll
        if (Test-Path $srcDll) {
            Copy-Item -Path $srcDll -Destination $PackageDir -Force
        }
    }
}

# config.ini
$ConfigDst = Join-Path $PackageDir "config.ini"
if (-not (Test-Path $ConfigDst)) {
    $ConfigSrc = Join-Path $WorkspaceRoot "config.ini"
    if (Test-Path $ConfigSrc) {
        Copy-Item -Path $ConfigSrc -Destination $ConfigDst -Force
    } else {
        "[Logging]`
EnableLogging=false`
" | Out-File -FilePath $ConfigDst -Encoding utf8
    }
}

# Create Zip Archive
$ZipPath = Join-Path $DistDir "ROLC_StatusSimulator_v$AppVersion.zip"
if (Test-Path $ZipPath) {
    Remove-Item -Force $ZipPath -ErrorAction SilentlyContinue
}
Compress-Archive -Path "$PackageDir\*" -DestinationPath $ZipPath -Force

exit 0
