#include <iostream>
#include <string.h>
#include "Leap.h"
#include <windows.h>


#define WINVER 0x0500
using namespace Leap;
INPUT ip;

class SampleListener : public Listener {
public:
	virtual void onInit(const Controller&);
	virtual void onConnect(const Controller&);
	virtual void onDisconnect(const Controller&);
	virtual void onExit(const Controller&);
	virtual void onFrame(const Controller&);
	virtual void onFocusGained(const Controller&);
	virtual void onFocusLost(const Controller&);
	virtual void onDeviceChange(const Controller&);
	virtual void onServiceConnect(const Controller&);
	virtual void onServiceDisconnect(const Controller&);

private:
};


void SampleListener::onFrame(const Controller& controller) {
	// Get the most recent frame and report some basic information
	const Frame frame = controller.frame();
	int extendedFingers = 0;

	// Check if frame has something in it
	if (!frame.hands().isEmpty()){
		// Create an array of each hand in the frame
		HandList hands1 = frame.hands();
		Hand currentHand = hands1[0];
		// Loop through each hand in the frame
		for (int i = 0; i < hands1.count(); i++){
			// Set the hand currently being analyzed
			currentHand = hands1[i];
			// Loop through each finger on the current hand
			for (int f = 0; f < currentHand.fingers().count(); f++){
				Finger finger = currentHand.fingers()[f];
				// if the finger is extended, increase the count
				if (finger.isExtended()) extendedFingers++;
			}
		}
		std::cout << "# of hands: " << hands1.count() << ", Extended fingers " << extendedFingers << std::endl;
		Sleep(10);
	}
	else{
		std::cout << "Frame empty" << std::endl;
		Sleep(10);

	}

	// Emulates a keystroke
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0; 
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	if (extendedFingers > 2){
		extendedFingers = 2;
	}
	if(extendedFingers < 9){
	ip.ki.wVk = extendedFingers+48; // virtual-key code for the 0-9 keys
	} else extendedFingers = 9;
	ip.ki.dwFlags = 0; // 0 for key press
	SendInput(1, &ip, sizeof(INPUT));

	// Release the "A" key
	ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	SendInput(1, &ip, sizeof(INPUT));

}


int main(int argc, char** argv) {
	// Create a sample listener and controller
	SampleListener listener;
	Controller controller;

	// Have the sample listener receive events from the controller
	controller.addListener(listener);

	// Set this program to run in the background
	controller.setPolicyFlags(Leap::Controller::POLICY_BACKGROUND_FRAMES);

	// Keep this process running until Enter is pressed
	std::cout << "Press Enter to quit..." << std::endl;
	std::cin.get();

	// Remove the sample listener when done
	controller.removeListener(listener);

	return 0;
}