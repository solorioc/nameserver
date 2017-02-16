#MakeFile to build and deploy the Sample US CENSUS Name Data using ajax
# For CSC3004 Software Development

# Put your user name below:
USER= solorioc

CC= g++

#For Optimization
#CFLAGS= -O2
#For debugging
CFLAGS= -g

RM= /bin/rm -f

all: searchserver testclient indexsearchajax PutCGI PutHTML
#all: searchserver testclient 

testclient.o: testclient.cpp fifo.h
	$(CC) -c $(CFLAGS) testclient.cpp

searchserver.o: searchserver.cpp fifo.h
	$(CC) -c $(CFLAGS) searchserver.cpp

namefetchajax.o: indexsearchajax.cpp fifo.h
	$(CC) -c $(CFLAGS) indexsearchajax.cpp

testclient: testclient.o fifo.o
	$(CC) testclient.o fifo.o -o testclient

searchserver: searchserver.o fifo.o
	$(CC) searchserver.o  fifo.o -o searchserver

fifo.o:		fifo.cpp fifo.h
		g++ -c fifo.cpp
indexsearchajax: indexsearchajax.o  fifo.h
	$(CC) indexsearchajax.o  fifo.o -o indexsearchajax -L/usr/local/lib -lcgicc

PutCGI: indexsearchajax
	chmod 757 namefetchajax
	cp indexsearchajax /usr/lib/cgi-bin/$(USER)_indexsearchajax.cgi 

	echo "Current contents of your cgi-bin directory: "
	ls -l /usr/lib/cgi-bin/

PutHTML:
	cp index_ajax.html /var/www/html/class/softdev/$(USER)
	cp index_ajax.css /var/www/html/class/softdev/$(USER)

	echo "Current contents of your HTML directory: "
	ls -l /var/www/html/class/softdev/$(USER)

clean:
	rm -f *.o index_ajax searchserver testclient
