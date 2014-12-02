C=gcc
CP=g++
CPP=$(CROSS_COMPILE)g++ -Wall -g
OFLAGS=`pkg-config --libs --cflags opencv`
#localizacia, moduly jadra, autotools, 
#sockety,server,procesy, vlakna,dynamkniznice,casovace,signaly,semafor
all:server client1 client2 client3 client4 client5
local_lib:libsocket.o libsharedMem.o libdecodeArgv.o libtimer.o libsemafor.o
shared_lib:librobotArm.o
server: server.c
	$(CP) server.c -L. -I.  -lrobotArm libsharedMem.o libdecodeArgv.o libsemafor.o -lpthread -o server -lm -g
client1:client1.c
	$(CP) client1.c -L. -I. -lrobotArm libsocket.o libdecodeArgv.o -lpthread -o client1 -lm -g -std=c++11 
client2:client2.c
	$(CP) client2.c -L. -I. -lrobotArm libsocket.o libdecodeArgv.o -pthread -o client2 -lm -g -std=c++11 
client3:client3.c
	$(CP) client3.c -L. -I. -lrobotArm libsocket.o libdecodeArgv.o libtimer.o -pthread -lrt -o client3 -lm -g -std=c++11 
client4:client4.c
	$(CP) client4.c -L. -I. -lrobotArm libsocket.o libdecodeArgv.o -pthread -o client4 -lm -g -std=c++11 
client5:client5.c
	$(CPP) client5.c -L. -I. -lrobotArm libsocket.o libdecodeArgv.o -o client5 -std=c++11 $(OFLAGS)

librobotArm.o:robotArm.h robotArm.c
	$(CP) -Wall -fPIC -c robotArm.c -I.
	$(CP) -shared -Wl,-soname,librobotArm -o librobotArm.so robotArm.o -lc -lm
	cp -f librobotArm.so /usr/local/lib/librobotArm.so
	ln -sf /usr/local/lib/librobotArm.so /usr/local/librobotArm
	cp -f robotArm.h	/usr/local/include/robotArm.h
	ldconfig
libsocket.o:socket.h socket.c
	$(CP) -c socket.c -lrobotArm -o libsocket.o -lm
libsharedMem.o:sharedMem.h sharedMem.c
	$(CP) -c sharedMem.c -lrobotArm -o libsharedMem.o -lm
libdecodeArgv.o:decodeArgv.h decodeArgv.c
	$(CP) -c decodeArgv.c -o libdecodeArgv.o
libsemafor.o:semafor.c semafor.h
	$(CP) -c semafor.c -o libsemafor.o
libtimer.o: timer.c timer.h
	$(CP) -c timer.c -o libtimer.o
clean:
	rm -f client1
	rm -f client2
	rm -f client3
	rm -f client4
	rm -f client5
	rm -f server
	rm -f *.o
	rm -f *.so
	rm -f librobotArm
shared_lib_clean:
	rm /usr/local/lib/librobotArm
	rm /usr/local/lib/librobotArm.so
	rm -f /usr/local/include/robotArm.h
	ldconfig
