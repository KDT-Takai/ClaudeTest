# Start-Claude Script
# This script prepares the environment and starts the application

Write-Host "=== Starting ClaudeTest ===" -ForegroundColor Cyan

# Set working directory
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $scriptDir

# Check if required files exist
$requiredFiles = @(
    "$scriptDir\main.cpp",
    "$scriptDir\ClaudeTest.vcxproj"
)

foreach ($file in $requiredFiles) {
    if (Test-Path $file) {
        Write-Host "  [OK] Found: $($file.Split('\')[-1])" -ForegroundColor Green
    } else {
        Write-Host "  [MISSING] $file" -ForegroundColor Red
    }
}

# Check for source files
$sourceFiles = @(
    "$scriptDir\Engine\Window\Window.cpp",
    "$scriptDir\Engine\Window\Window.h",
    "$scriptDir\System\Singleton\Singleton.h"
)

foreach ($file in $sourceFiles) {
    if (Test-Path $file) {
        Write-Host "  [OK] Found: $($file.Split('\')[-1])" -ForegroundColor Green
    } else {
        Write-Host "  [MISSING] $file" -ForegroundColor Red
    }
}

Write-Host ""
Write-Host "=== Build Configuration ===" -ForegroundColor Cyan
Write-Host "  Platform: x64 (default)"
Write-Host "  Configuration: Debug (default)"
Write-Host ""

# Open Visual Studio with the project
Write-Host "Opening Visual Studio..." -ForegroundColor Yellow
& "$scriptDir\ClaudeTest.vcxproj" /projectpath:"$scriptDir" /property:"Configuration=Debug;Platform=x64"
