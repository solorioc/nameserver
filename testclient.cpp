#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "fifo.h"

using namespace std;


int main() {

  string message, name, searchTerm;
  string reply;

  while (1) {
	string receive_fifo = "searchReply";
	string send_fifo = "searchRequest";
	
	// create the FIFOs for communication
	Fifo recfifo(receive_fifo);
	Fifo sendfifo(send_fifo);

	cout << "Enter a word to search: ";
	cin >>  searchTerm;
	
	message = searchTerm;
	
	sendfifo.openwrite();
	sendfifo.send(message);
		
	/* Get a message from a server */
	recfifo.openread();
	
	do {
		reply = recfifo.recv();
		cout << reply;
	} while (reply != "$END");
	
	recfifo.fifoclose();
	sendfifo.fifoclose();
  }


}
