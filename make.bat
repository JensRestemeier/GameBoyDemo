@ECHO OFF
tasm -tgb -b -s -fff gbdemo.asm
copy gbdemo.obj gbdemo.gb
rgbfix -p -v gbdemo.gb
