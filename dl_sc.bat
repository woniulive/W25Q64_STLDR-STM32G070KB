@echo off &cls

echo stm32 download script

set Current_Path=%~dp0

set Programer_path=C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\

set External_loader="D:\Workspace\STM32CubeMX\W25Q64_STLDR-STM32G070KB\W25Q64_STLDR-STM32G070KB.stldr"

set Download_file="D:\Workspace\STM32CubeMX\Double_G070KB\MAIN\Double_G070KB.hex"

set STM32_Programmer="%Programer_path%STM32_Programmer_CLI.exe"

:begin
set /p cmd="press enter>>"
if /i "%cmd%"=="" goto dl

:dl
rem STM32_Programmer_CLI.exe -c port=SWD -e all -el %External_loader_path% -d %Download_file_path% -rst
rem -vb 3 will out many log
%STM32_Programmer% -c port=SWD -vb 1 -el %External_loader% -d %Download_file% -v -rst
goto begin