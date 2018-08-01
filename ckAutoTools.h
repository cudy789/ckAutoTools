/*
Created by Corey Knutson
========== Summary ==========
 	The 2 classes in this file are used to read and write to
	a CSV file that contains all driver inputs during autonomous.
	Both classes can interact with CSV files of varying "width",
	and "length", but can only read and write doubles (decimal numbers).
	The CSV file being read in must be complete (ex. the last row of a 7
	"wide" CSV file must have 7 values).  The recorder class will always
	make complete CSV files.
========== Recorder Class ==========
	The recorder class records user input one iteration at a time,
	with 1 iteration corresponding to 1 line on the CSV file. The user
	has the option to either overwrite an existing file (default option),
	or append to an existing file.  If either option is selected and the
	file specified at the path does not exist, then a file will be Created
	with the given path. If nofile name/path is provided, the default
	is "default.csv".

	Functions available to users:
	 	setFile(path/to/file.csv) : void
		changeWriteMode("append" or "overwrite") : void
		recordValues(array name) : void
		closeFile() : void

========== Playback Class ==========
	The playback class reads data the same way the recorder class writes
	data - 1 iteration corresponding to 1 line on the CSV file.  Once there
	is no more data, this class will return the value -100 to each element
	in the array in the getLine() function.

		Functions available to the users:
			setFile(path/to/file.csv) : void
			getLine(array name) : void
			getSize() : int
			isEmpty() : bool

========== Dependencies ==========
	-Need the myCSV.h file in the same folder as the main .cpp file
	-May need to use the -lpthread compiler flag**
	-Need to compile with the flag std=c++11,
	std=c++14, or std=c++17**

	** - I didn't need to enable these in Eclipse Oxygen (2017-2018 FRC software package)

*/
#include <cstdlib>
#include <iostream>
#include <queue>
#include <vector>
#include <list>
#include <string>
#include "ckCSV.h" // CSV reader
#include <chrono>
#include <ctime>
#include <fstream>

using string = std::string;


class recorder{
public:
	recorder(int mySize){ // Constructor that you should use, specifies the "width" of the file
		CSVsize = mySize;
	}
	recorder(int mySize, string filename, string inputAppendOrOverwrite = "overwrite"){
		CSVsize = mySize;
		FileName = filename;
		appendOrOverwrite = inputAppendOrOverwrite;
		setFile(FileName);
	}
	~recorder() { myFileStream.close();} // Destructor makes sure to close our file once we're done recording

	void setFile(string filename){ // Set filename - USE FULL PATH!
		FileName=filename;
		if (appendOrOverwrite == "append"){ // Check to see if we need to append to the file, or overwrite it
			myFileStream.open(FileName, std::ofstream::out | std::ofstream::app);
		}else	myFileStream.open(FileName, std::ofstream::out | std::ofstream::trunc);
	}
	void changeWriteMode(string inputAppendOrOverwrite){ // Options are "append" or "overwrite"
		myFileStream.close();
		appendOrOverwrite = inputAppendOrOverwrite;
		setFile(FileName);
	}
	void recordValues(double returnArray[]){ // Write the current array values to the file
		for (int i=0; i<CSVsize-1; ++i){ // Writes CSVsize values per line
			myFileStream << returnArray[i] << ",";
		}
		myFileStream << returnArray[CSVsize-1] << "\n";
	}
	void closeFile(){ // Need to close the file before reading from it again, make sure to call this function, or the destructor!
		myFileStream.close();
	}
private:
	string appendOrOverwrite;
	int CSVsize;
	string FileName = "default.csv"; // Default filename
	std::ofstream myFileStream; // Object to write data to file
};





class playback{
public:
	playback(int mySize){ // Constructor that you should use, specifies the "width" of the file
		CSVsize = mySize;
		for (int i=0; i<CSVsize; ++i){ // Creates CSVsize number of queues to store data
			myQueues.push_back(std::queue<double>());
		}
	}
  playback(int mySize, string filename){ // Optional constructor
		CSVsize = mySize;
		for (int i=0; i<CSVsize; ++i){
			myQueues.push_back(std::queue<double>());
		}
		FileName=filename;
		start = std::chrono::system_clock::now();
		importCSV();
		end = std::chrono::system_clock::now();
		elapsed_seconds = end - start;
		// std::cout << "duration: " << elapsed_seconds.count()*1000.<<" ms\n"; // uncomment to track time to read file into memory
	}
	~playback() = default; // Parser class automatically closes file, no need to here

	void setFile(string filename){ // Removes current file if there is one, adds new file
		FileName=filename;
		emptyQueues();
		importCSV();
	}

	void getLine(double (returnArray)[]){ // Pass in an array, get CSVsize number of values back
		for (int i=0; i<CSVsize; ++i){
			returnArray[i] = get(i);
		}
	}
	int getSize(){ // Returns the number of items in a single queue (each queue will be the same length)
		return myQueues[0].size();
	}
	bool isEmpty(){ // Returns true if the queues is empty
		return myQueues[0].empty();
	}

private: // Note - Functions and variables below this point will not be accessible to users
	std::chrono::time_point<std::chrono::system_clock> start, end;
	std::chrono::duration<double> elapsed_seconds; // Timing variables

	int CSVsize;
	std::vector<std::queue<double>> myQueues; // A vector of queues - this is where all of the data is stored
	string FileName = "default.csv"; // Default file name

	void importCSV(){ // Reads data from CSV file and writes it to memory
		parser myParser(CSVsize, FileName); // Create a parser object to parse a file Filename that has CSVsize "width"
		double myData[CSVsize]; // Create an array to read values with
		while(myParser.read_line(myData)){ // Read each line into array, if there is data..
			for (int i=0; i<CSVsize; ++i){ // push data from the ith index of the array to the ith queue in our vector
				myQueues[i].push(myData[i]);// ^^^
			}
		}
	}
	void emptyQueues(){ // Empties the queues
		for (int i=0; i<CSVsize; ++i){
			while (!myQueues[i].empty()){
				myQueues[i].pop();
			}
		}
	}
	double get(int input){ // Reads 1 value from 1 queue
		double returnValue = -100; // Default value if array index is empty
		if(!myQueues[input].empty()){ // If the queue is not empty, then read the first value and remove it from the queue
			returnValue = myQueues[input].front(); // ^^
			myQueues[input].pop(); // ^^
		}
		return returnValue;
	}
};
