/***************************************************************************
* searchserver.cpp - Searches through the complete works of Shakespeare for a word
*
* copyright : (C) 2017 by Christian Solorio
*
* This program ....
*
***************************************************************************/

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <map>
#include "fifo.h"

using namespace std;

/*Function Declarations*/

vector<int> indexSearch(string word); // Searches map, returns vector
int createindexmap(map<string, vector<int> > &refsmap, string filename);//Creates the index map

/* Fifo names */


map<string, vector<int> > refs;//The map of words/references

/* Server main line,create name MAP, wait for and serve requests */
int main() {

	string inMessage, outMessage, word, line, search;
	string p = "<p>";
	string pend = "</p>";
	
	string path = "/home/class/SoftDev/Shakespeare/";//Path of the file
	string filename = path + "Shakespeare.txt";//The file to be read
	
	ifstream infile(filename.c_str());//Puts file into stream
	
	if(createindexmap(refs, filename) != 1) { //If createindexmap fcn fails, print and exit
		cout << "Index could not be made \n";
		exit(0);
	}
	
	infile.close();//Close file
	infile.open(filename.c_str());//Reopen file
	
	string send_fifo = "searchReply";//
	string receive_fifo = "searchRequest";//

	Fifo sendfifo(send_fifo);//Send FIFO
	Fifo recfifo(receive_fifo);//Receive FIFO
		
	do{//Main loop of program
		
	    recfifo.openread();//Opens the recieve FIFO
	    
    	inMessage = recfifo.recv();//Takes the incoming message and stores it
    	cout << inMessage << endl;
	
		vector <int> vec = indexSearch(inMessage);
		
		sendfifo.openwrite();//Opens send FIFO
		
		if(vec.empty()) { //If there are no results, ...
			sendfifo.send("<!--$END-->");
		} else{ //If vector is not empty, ...
			
			/*Loop that outputs the line with the search term*/
			for(int i=0;i < vec.size();++i) {
				infile.seekg(vec[i], infile.beg); //Changes current line to the one just printed
				getline(infile, line); //Gets that line
		
				outMessage = p+line+pend;//Message to be sent is of form <p> words searchTerm words words </p>
				cout << outMessage << endl;
				/*Sends this message through the FIFO*/
				sendfifo.send(outMessage);	
			} 
			  sendfifo.send("<!--$END-->");
		}
		
		sendfifo.fifoclose();
		recfifo.fifoclose() ;//Closes FIFO
	} while(1);
	

return 0;//End program

 }

/*Function Definitions*/

vector<int> indexSearch(string word) {//Searches the map for input word

	map<string, vector<int> >::iterator it;//Creates an iterator for the map
	vector<int> empty;//An empty vector
	
	it = refs.find(word);//Checks map for key: word. If no match, returns end of map
	
	if(it == refs.end()) {//If the iterator is the end of the map, 
		return(empty);//return the empty vector
	} else {
		return(refs[word]);//return the vector associated with that word
	}
}

int createindexmap(map<string, vector<int> > &refsmap, string filename) {//Function that makes the map. 

	int pos = 0;//Starts at beginning of file
	string word, line;//Creates strings for word and line
	
	ifstream infile(filename.c_str());
	
	if(infile.good()) {//If the file opens, ...
		while (!infile.fail()) {//While NOT at the end of the file, ...

			getline(infile, line);//Gets the next line of the code
			
			stringstream lineStream(line);//Create a string stream of the line
	
			while (lineStream >> word) { //Looks at the string of the line, finds the words
				refsmap[word].push_back(pos); //Adds the word and its position to the vector
			}
	
			pos = infile.tellg();//Sets the position to the start of the next line, loop continues
		}
		cout << "Index Created! Search Shakespeare's complete works\n";
		return(1);//If the file does open, returns 1
	} else {
		return(0);//If the file fails to open, returns 0
	}

}


//How to tell CGI is done recieving lines - message with "$END", if statement if(...!=$END) {...}
//and push that into a div

/*In CGI
 *Open send 
  .
  .
  .
  open recv fifo
  do {
  	mess = recvfifo
  	cout << mess
  } while (mess != "$END")
  close recv
  close send
*/

