import subprocess
htmlComp = r"C:\repo\swifit\comphtml.bat"
makeFile = r"C:\repo\swifit\Makefile"
flashBatch = r"C:\repo\swifit\flash.bat"

p = subprocess.run([htmlComp])
subprocess.run([r"C:\MinGW\bin\mingw32-make.exe", "-f", makeFile])
subprocess.run([flashBatch])