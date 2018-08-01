#include <iostream>
#include "ckAutoTools.h"
using namespace std;
int main(){
	const static int AUTO_CHANNELS = 5;
// RECORD DATA TO THE FILE
		double playbackArray[AUTO_CHANNELS]; // make an array to store each iteration of control values
		double recorderArray[AUTO_CHANNELS] = {1,2.2,3.3,4.4,5.5};
		recorder coreyRecorder{AUTO_CHANNELS};
		coreyRecorder.setFile("testdata.csv");
		for (int i=0; i<200; ++i){
			coreyRecorder.recordValues(recorderArray);
			recorderArray[0] += 1;
		}
		coreyRecorder.closeFile();
// PLAYBACK DATA FROM THE FILE
		playback coreyPlayback{AUTO_CHANNELS}; // make a playback object called coreyPlayback
		coreyPlayback.setFile("testdata.csv");
		int playbackSize = coreyPlayback.getSize();
		for(int j=0; j<playbackSize; ++j){
			coreyPlayback.getLine(playbackArray); // Read the first iteration of values into playbackArray array
			cout << "queue size: " << coreyPlayback.getSize();
			for (int i=0; i<5; ++i){
				cout << " " << playbackArray[i] << " ";
			}
			cout << endl;
		}

}
