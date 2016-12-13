all :main setup

setup: clean createShMemFile

createShMemFile:
	rm shMem -f
	touch shMem

main:main1.cpp
	g++ -pthread  main1.cpp

clean:
	rm fifofile -f
	rm fifofile2 -f