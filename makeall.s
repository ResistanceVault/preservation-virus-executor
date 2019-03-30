echo Compiling 68060 version...
copy SCOPTIONS.0 SCOPTIONS
echo >>SCOPTIONS "CPU=68060"
delete #?.o quiet
smake
x ccc.s
Move VE VirusExecutor.060

echo Compiling 68040 version...
copy SCOPTIONS.0 SCOPTIONS
echo >>SCOPTIONS "CPU=68040"
delete #?.o quiet
smake
Move VE VirusExecutor.040
x ccc.s

echo Compiling 68000 version...
copy SCOPTIONS.0 SCOPTIONS
echo >>SCOPTIONS "CPU=68000"
delete #?.o quiet
smake
x ccc.s

scompare -oVE.060.pch VE VirusExecutor.060
scompare -oVE.040.pch VE VirusExecutor.040
