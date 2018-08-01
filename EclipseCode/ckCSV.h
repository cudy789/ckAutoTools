/* Created by Corey Knutson

  This class strictly parses only numerical CSV files .

  The constructor MUST be supplied with BOTH the size (or "width") of
  the CSV file, and the /path/to/the/csv/file.csv .

  read_line() will take an array as an argument, and
  pass-by-reference the following numbers on the next
  line of the CSV file.  The ith CSV is the ith number
  in the array. (ex. 2,7,4,9,10 in the CSV file will translate to
  {2, 7, 4, 9, 10} in the array).  Once there are no more lines
  to be read in the file, read_line will return False.

*/
#include <iostream>
#include <fstream>
#include <string>
using string = std::string;
using namespace std;
class parser{
public:
  parser(int mySize, string myFilename){
    size = mySize;
    filename = myFilename;
    inStream.open(filename);

  }
  ~parser(){
    inStream.close();
  }
  bool read_line(double returnArray[]){
    char delim;
    string temp;
    if (inStream.good()){
      inStream >> returnArray[0];
      for (int i=1; i<size; ++i){
        inStream >> delim;
        inStream >> returnArray[i];
      }
      return true;
    }
    else{
        for (int i=0; i<size; ++i){
          returnArray[i] = -100;
        }
        return false;
      }
  }
private:
  int size;
  string filename;
  std::ifstream inStream;
};
