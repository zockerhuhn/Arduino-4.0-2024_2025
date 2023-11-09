.vscode\arduino-cli.exe config init --overwrite
.vscode\arduino-cli.exe config set directories.user "%CD%"
xcopy .vscode\to_copy %USERPROFILE%\AppData\Local\Arduino15\packages /sy
