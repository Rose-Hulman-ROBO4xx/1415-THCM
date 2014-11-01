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

const std::string fingerNames[] = { "Thumb", "Index", "Middle", "Ring", "Pinky" };
const std::string boneNames[] = { "Metacarpal", "Proximal", "Middle", "Distal" };
const std::string stateNames[] = { "STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END" };

void SampleListener::onInit(const Controller& controller) {
	std::cout << "Initialized" << std::endl;
}

void SampleListener::onConnect(const Controller& controller) {
	std::cout << "Connected" << std::endl;
	controller.enableGesture(Gesture::TYPE_CIRCLE);
	controller.enableGesture(Gesture::TYPE_KEY_TAP);
	controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
	controller.enableGesture(Gesture::TYPE_SWIPE);
}

void SampleListener::onDisconnect(const Controller& controller) {
	// Note: not dispatched when running in a debugger.
	std::cout << "Disconnected" << std::endl;
}

void SampleListener::onExit(const Controller& controller) {
	std::cout << "Exited" << std::endl;
}

void SampleListener::onFrame(const Controller& controller) {
	// Get the most recent frame and report some basic information
	const Frame frame = controller.frame();
	int extendedFingers = 0;
	/*std::cout << "Frame id: " << frame.id()
	<< ", timestamp: " << frame.timestamp()
	<< ", hands: " << frame.hands().count()
	<< ", fingers: " << frame.fingers().count()
	<< ", tools: " << frame.tools().count()
	<< ", gestures: " << frame.gestures().count() << std::endl;*/

	if (!frame.hands().isEmpty()){
		HandList hands1 = frame.hands();
		Hand hand1 = hands1[0];
		for (int i = 0; i < hands1.count(); i++){
			hand1 = hands1[i];
			for (int f = 0; f < hand1.fingers().count(); f++){
				Finger finger = hand1.fingers()[f];
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

	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0; // hardware scan code for key
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	if (extendedFingers > 2){
		extendedFingers = 2;
	}
	ip.ki.wVk = extendedFingers+48; // virtual-key code for the 0-2 keys
	ip.ki.dwFlags = 0; // 0 for key press
	SendInput(1, &ip, sizeof(INPUT));

	// Release the "A" key
	ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	SendInput(1, &ip, sizeof(INPUT));


	HandList hands = frame.hands();
	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
		// Get the first hand
		const Hand hand = *hl;
		/*std::string handType = hand.isLeft() ? "Left hand" : "Right hand";
		std::cout << std::string(2, ' ') << handType << ", id: " << hand.id()
		<< ", palm position: " << hand.palmPosition() << std::endl;*/
		// Get the hand's normal vector and direction
		const Vector normal = hand.palmNormal();
		const Vector direction = hand.direction();

		// Calculate the hand's pitch, roll, and yaw angles
		/*std::cout << std::string(2, ' ') <<  "pitch: " << direction.pitch() * RAD_TO_DEG << " degrees, "
		<< "roll: " << normal.roll() * RAD_TO_DEG << " degrees, "
		<< "yaw: " << direction.yaw() * RAD_TO_DEG << " degrees" << std::endl;*/

		// Get the Arm bone
		Arm arm = hand.arm();
		/*std::cout << std::string(2, ' ') <<  "Arm direction: " << arm.direction()
		<< " wrist position: " << arm.wristPosition()
		<< " elbow position: " << arm.elbowPosition() << std::endl;*/

		// Get fingers
		const FingerList fingers = hand.fingers();
		for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
			const Finger finger = *fl;
			/*std::cout << std::string(4, ' ') <<  fingerNames[finger.type()]
			<< " finger, id: " << finger.id()
			<< ", length: " << finger.length()
			<< "mm, width: " << finger.width() << std::endl;*/

			// Get finger bones
			for (int b = 0; b < 4; ++b) {
				Bone::Type boneType = static_cast<Bone::Type>(b);
				Bone bone = finger.bone(boneType);
				/*std::cout << std::string(6, ' ') <<  boneNames[boneType]
				<< " bone, start: " << bone.prevJoint()
				<< ", end: " << bone.nextJoint()
				<< ", direction: " << bone.direction() << std::endl;*/
			}
		}
	}

	// Get tools
	const ToolList tools = frame.tools();
	for (ToolList::const_iterator tl = tools.begin(); tl != tools.end(); ++tl) {
		const Tool tool = *tl;
		/*std::cout << std::string(2, ' ') <<  "Tool, id: " << tool.id()
		<< ", position: " << tool.tipPosition()
		<< ", direction: " << tool.direction() << std::endl;*/
	}


	/*if (frame.hands().isEmpty()) {
	std::cout << "Empty Frame" << std::endl;
	}
	else{
	std::cout << "HAND FOUND!!!" << std::endl;
	}*/
}

void SampleListener::onFocusGained(const Controller& controller) {
	std::cout << "Focus Gained" << std::endl;
}

void SampleListener::onFocusLost(const Controller& controller) {
	std::cout << "Focus Lost" << std::endl;
}

void SampleListener::onDeviceChange(const Controller& controller) {
	std::cout << "Device Changed" << std::endl;
	const DeviceList devices = controller.devices();

	for (int i = 0; i < devices.count(); ++i) {
		std::cout << "id: " << devices[i].toString() << std::endl;
		std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
	}
}

void SampleListener::onServiceConnect(const Controller& controller) {
	std::cout << "Service Connected" << std::endl;
}

void SampleListener::onServiceDisconnect(const Controller& controller) {
	std::cout << "Service Disconnected" << std::endl;
}

int main(int argc, char** argv) {
	// Create a sample listener and controller
	SampleListener listener;
	Controller controller;

	// Have the sample listener receive events from the controller
	controller.addListener(listener);

	//if (argc > 1 && strcmp(argv[1], "--bg") == 0)
		controller.setPolicyFlags(Leap::Controller::POLICY_BACKGROUND_FRAMES);

	// Keep this process running until Enter is pressed
	std::cout << "Press Enter to quit..." << std::endl;
	std::cin.get();

	// Remove the sample listener when done
	controller.removeListener(listener);

	return 0;
}


