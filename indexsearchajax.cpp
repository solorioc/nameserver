#include <iostream>
// Stuff for AJAX
#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"

//Stuff for pipes
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include "fifo.h"

using namespace std;
using namespace cgicc; // Needed for AJAX functions.

/*
//Indeed, myString needs to be a copy of the original string
std::string StringToUpper(std::string myString)
{
  const int length = myString.length();
  for(int i=0; i!=length ; ++i)
    {
      myString[i] = std::toupper(myString[i]);
    }
  return myString;
}
*/

// fifo for communication
string receive_fifo = "searchReply";
string send_fifo = "searchRequest";

int main() {
  Cgicc cgi;    // Ajax object
  char *cstr;
  
  string reply;
  // Create AJAX objects to recieve information from web page.
  form_iterator name = cgi.getElement("name");//Word input

  // create the FIFOs for communication
  Fifo recfifo(receive_fifo);
  Fifo sendfifo(send_fifo);

  // Call server to get results
  string stname = **name;//
  
	sendfifo.openwrite();
	sendfifo.send(stname);
		
	/* Get a message from a server */
	recfifo.openread();
	
	cout << "Content-Type: text/plain\n\n";
	do {
		reply = recfifo.recv();
		cout << reply;
	} while (reply != "<!--$END-->");
	
	recfifo.fifoclose();
	sendfifo.fifoclose();
  
return 0;
}
