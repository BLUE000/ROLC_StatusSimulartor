# ==============================================================================
# ROLC_StatusSimulator Automated Test Runner
# ==============================================================================
param (
    [switch]$Clean
)

$ErrorActionPreference = "SilentlyContinue"
$ProgressPreference = "SilentlyContinue"

$WorkspaceRoot = $PSScriptRoot
$TestsDir = Join-Path $WorkspaceRoot "tests"
$TestLogDir = Join-Path $WorkspaceRoot "test_log"
$BuildDir = Join-Path $WorkspaceRoot "build_debug"

# Ensure test_log folder exists
if (-not (Test-Path $TestLogDir)) {
    New-Item -ItemType Directory -Path $TestLogDir -Force | Out-Null
}

$BuildLogPath = Join-Path $TestLogDir "build.log"
$BuildSummaryPath = Join-Path $TestLogDir "build_summary.txt"
$TestLogPath = Join-Path $TestLogDir "test.log"
$TestSummaryPath = Join-Path $TestLogDir "test_summary.txt"

# Reset logs
Set-Content -Path $BuildLogPath -Value "" -Encoding utf8
Set-Content -Path $TestLogPath -Value "" -Encoding utf8

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

# Clean build if requested
if ($Clean) {
    if (Test-Path $BuildDir) { Remove-Item -Recurse -Force $BuildDir -ErrorAction SilentlyContinue }
    if (Test-Path $TestsDir) { Remove-Item -Recurse -Force $TestsDir -ErrorAction SilentlyContinue }
}

# ------------------------------------------------------------------------------
# 1. Debug Build
# ------------------------------------------------------------------------------
$BuildSw = [System.Diagnostics.Stopwatch]::StartNew()
$BuildSuccess = $true
$BuildRawOutput = @()

$cmakeConfigureArgs = @(
    "-B", $BuildDir,
    "-G", "Ninja",
    "-DCMAKE_BUILD_TYPE=Debug",
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

# Write Build Summary
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
    $testFailSummary = @(
        "==================================================",
        "テストログ サマリ (Test Log Summary)",
        "==================================================",
        "・テスト結果: NG (ビルド失敗のため未実行)",
        "・テストにかかった時間: 0.00 秒",
        "・テストNGの一覧:",
        "  - ビルドが失敗したためテストは実行されませんでした。",
        "=================================================="
    )
    $testFailSummary | Out-File -FilePath $TestSummaryPath -Encoding utf8
    exit 1
}

# ------------------------------------------------------------------------------
# 2. Deploy to tests/
# ------------------------------------------------------------------------------
if (-not (Test-Path $TestsDir)) {
    New-Item -ItemType Directory -Path $TestsDir -Force | Out-Null
}

$ExeFiles = Get-ChildItem -Path $BuildDir -Filter "*.exe" -File -ErrorAction SilentlyContinue
foreach ($exe in $ExeFiles) {
    Copy-Item -Path $exe.FullName -Destination $TestsDir -Force
}

if (Test-Path $WindeployqtPath) {
    $mainExe = Join-Path $TestsDir "ROLC_StatusSimulator.exe"
    if (Test-Path $mainExe) {
        & $WindeployqtPath --no-translations --compiler-runtime $mainExe 2>&1 | Out-Null
    }
    $coreTestExe = Join-Path $TestsDir "ROLC_CoreTest.exe"
    if (Test-Path $coreTestExe) {
        & $WindeployqtPath --no-translations $coreTestExe 2>&1 | Out-Null
    }
    $presenterTestExe = Join-Path $TestsDir "ROLC_PresenterTest.exe"
    if (Test-Path $presenterTestExe) {
        & $WindeployqtPath --no-translations $presenterTestExe 2>&1 | Out-Null
    }
}

$MinGWBin = Join-Path $MinGWDir "bin"
if (Test-Path $MinGWBin) {
    $RuntimeDlls = @("libstdc++-6.dll", "libgcc_s_seh-1.dll", "libwinpthread-1.dll")
    foreach ($dll in $RuntimeDlls) {
        $srcDll = Join-Path $MinGWBin $dll
        if (Test-Path $srcDll) {
            Copy-Item -Path $srcDll -Destination $TestsDir -Force
        }
    }
}

$ConfigDst = Join-Path $TestsDir "config.ini"
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

# ------------------------------------------------------------------------------
# 3. Execute Tests in tests/
# ------------------------------------------------------------------------------
$TestSw = [System.Diagnostics.Stopwatch]::StartNew()
$AllTestsPassed = $true
$TestNgList = @()
$TotalTestCount = 0

$TestTargets = @("ROLC_CoreTest.exe", "ROLC_PresenterTest.exe")

Push-Location $TestsDir
try {
    foreach ($testExeName in $TestTargets) {
        $testExePath = Join-Path $TestsDir $testExeName
        if (-not (Test-Path $testExePath)) {
            $TestNgList += "${testExeName}: 実行ファイルが見つかりません"
            $AllTestsPassed = $false
            continue
        }

        $TotalTestCount++
        $tempOutFile = Join-Path $TestsDir "temp_${testExeName}.log"
        
        # Run QTest with -o option to capture full details
        & $testExePath -o "${tempOutFile},txt" 2>&1 | Out-Null
        $testExitCode = $LASTEXITCODE

        $rawTestLines = @()
        if (Test-Path $tempOutFile) {
            $rawTestLines = Get-Content $tempOutFile -Encoding utf8
            Remove-Item -Force $tempOutFile -ErrorAction SilentlyContinue
        }

        $header = "`n========== [TEST: $testExeName] ==========`n"
        $header | Out-File -FilePath $TestLogPath -Encoding utf8 -Append
        $rawTestLines | Out-File -FilePath $TestLogPath -Encoding utf8 -Append

        $hasFail = $false
        foreach ($tLine in $rawTestLines) {
            $tStr = $tLine.ToString()
            if ($tStr -match "^FAIL!" -or ($tStr -match "Totals:.*failed" -and $tStr -notmatch "Totals:.*0 failed")) {
                $hasFail = $true
            }
        }

        if ($testExitCode -ne 0 -or $hasFail) {
            $AllTestsPassed = $false
            $failLines = @()
            foreach ($tLine in $rawTestLines) {
                $tStr = $tLine.ToString()
                if ($tStr -match "^FAIL!" -or $tStr -match "Loc: \[") {
                    $failLines += $tStr.Trim()
                }
            }
            $failSummary = if ($failLines.Count -gt 0) { ($failLines -join "; ") } else { "ExitCode: $testExitCode" }
            $TestNgList += "${testExeName} -> $failSummary"
        }
    }
}
finally {
    Pop-Location
}

$TestSw.Stop()
$TestDuration = [math]::Round($TestSw.Elapsed.TotalSeconds, 2)

# Write Test Summary
$testResultText = if ($AllTestsPassed) { "OK" } else { "NG" }
$testSummaryLines = @(
    "==================================================",
    "テストログ サマリ (Test Log Summary)",
    "==================================================",
    "・テスト結果: $testResultText",
    "・テストにかかった時間: $TestDuration 秒",
    "・実行テスト数: $TotalTestCount 件",
    "・テストNG数: $($TestNgList.Count) 件",
    "",
    "【テストNGの一覧】"
)
if ($TestNgList.Count -eq 0) {
    $testSummaryLines += "  (なし - 全テスト合格 OK)"
} else {
    foreach ($ng in $TestNgList) {
        $testSummaryLines += "  - $ng"
    }
}
$testSummaryLines += "=================================================="
$testSummaryLines | Out-File -FilePath $TestSummaryPath -Encoding utf8

if (-not $AllTestsPassed) {
    exit 1
}
exit 0
