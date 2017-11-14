import os
import subprocess

import sys

import shutil

htmlComp = r"C:\repo\swifit\comphtml.bat"
makeFile = r"C:\repo\swifit\Makefile"
flashBatch = r"C:\repo\swifit\flash.bat"



os.chdir(os.path.curdir)
if len(sys.argv) > 1 and sys.argv[1] == 'clean':
    try:
        shutil.rmtree('build')
    except Exception:
        pass
else:
    p = subprocess.run([htmlComp])
    subprocess.run([r"C:\MinGW\bin\mingw32-make.exe", "-f", makeFile])
    subprocess.run([flashBatch])