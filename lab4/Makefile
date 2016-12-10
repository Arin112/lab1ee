all :main setup

setup: clean createShMemFile

createShMemFile:
	rm shMem -f
	touch shMem

main:main.cpp
	g++ -pthread  main.cpp

clean:
	rm fifofile -f