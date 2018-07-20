$Env:ASPNETCORE_ENVIRONMENT = "Development"
# start dotnet ..\WebAPI\bin\Debug\netcoreapp2.0\WebAPI.dll
Push-Location ..\WebAPI
start dotnet .\bin\Debug\netcoreapp2.0\WebAPI.dll
Pop-Location
