
#include "ckAutoTools.h" // our playback and recording tools
#include <Drive/MecanumDrive.h>
#include <IterativeRobot.h>
#include <Joystick.h>
#include <Spark.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <SmartDashboard/SmartDashboard.h>

const static int AUTO_CHANNELS = 4; // the number of channels we want to control during autonomous (plus teleopiteration so we can keep track of what step we're on)

class Robot : public frc::IterativeRobot {
public:

	void RobotInit() {
		m_frontLeft.SetInverted(true);
		m_rearLeft.SetInverted(true);

	}
	void TeleopInit() {
		myAutoRecorder.setFile("/home/lvuser/testdata.csv"); // set file to record to, if file doesn't exist, one will be created
//		myAutoRecorder.changeWriteMode("append");
		myAutoRecorder.changeWriteMode("overwrite"); // comment out this line or the one above it to choose either overwrite mode, or append mode
		teleopIteration = 0;

	}
	void TeleopPeriodic() {

		myRecordingArray[0] = m_stick.GetX(); // record joystick x value into the 0th element of the array
		myRecordingArray[1] = m_stick.GetY(); // record joystick y value into the 1rst element of the array
		myRecordingArray[2] = m_stick.GetZ(); // record joystic z value into the 2nd element of the array
		myRecordingArray[3] = teleopIteration; // record teleop iteration into the 3rd element of the array
		SmartDashboard::PutNumber("Teleop Iteration", myRecordingArray[3]); // so we can see what step we're recording to real-time
		myAutoRecorder.recordValues(myRecordingArray); // send 1 line of values to our CSV file

		m_robotDrive.DriveCartesian(
				m_stick.GetX(), m_stick.GetY(), m_stick.GetZ());
		++teleopIteration;
	}
	void AutonomousInit(){
		myAutoPlayback.setFile("/home/lvuser/testdata.csv"); // open and load our previously recorded autonomous file - USE FULL PATH!

	}
	void AutonomousPeriodic() {
		myAutoPlayback.getLine(myRecordingArray); // get the next line (or 1rst line if its our first time in this loop), and put the values into the array called myRecordingArray
		SmartDashboard::PutNumber("Recorded Teleop Iteration", myRecordingArray[3]);

		if(!myAutoPlayback.isEmpty()){ // if we still have data to read from our file, continue
			m_robotDrive.DriveCartesian(myRecordingArray[0], myRecordingArray[1], myRecordingArray[2]);
		}
		else m_robotDrive.DriveCartesian(0,0,0); // else we have no more data to read, so set motors to 0 to signal we're finished
	}
	void DisabledPeriodic(){ // IMPORTANT - YOU NEED TO ADD THIS FUNCTION AND THE LINE BELOW IT
		myAutoRecorder.closeFile(); // this line closes the file that we record to, so our reader can open it. If we don't include this line in this spot, we won't be able to read data
	}

private:
	recorder myAutoRecorder{AUTO_CHANNELS}; // create our recorder and name it myAutoRecorder, with 4 channels to record data to
	playback myAutoPlayback{AUTO_CHANNELS}; // create our playback object and name it myAutoPlayback. Read from 4 channels
	double myRecordingArray[AUTO_CHANNELS]; // create an array to give to our recording and playback objects so we can read and write data to them
	int teleopIteration = 0; // keep track of iterations for recording


	static constexpr int kFrontLeftChannel = 0;
	static constexpr int kRearLeftChannel = 1;
	static constexpr int kFrontRightChannel = 2;
	static constexpr int kRearRightChannel = 3;

	static constexpr int kJoystickChannel = 0;

	frc::Spark m_frontLeft{kFrontLeftChannel};
	frc::Spark m_rearLeft{kRearLeftChannel};
	frc::Spark m_frontRight{kFrontRightChannel};
	frc::Spark m_rearRight{kRearRightChannel};
	frc::MecanumDrive m_robotDrive{
			m_frontLeft, m_rearLeft, m_frontRight, m_rearRight};

	frc::Joystick m_stick{kJoystickChannel};
};

START_ROBOT_CLASS(Robot)
