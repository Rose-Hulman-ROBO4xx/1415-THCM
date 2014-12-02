#include <Servo.h>
#include <LiquidCrystal.h>

Servo servo1;

#define PWM_PIN 11    
#define BUTTON_RIGHT 32
#define IR_PIN_1 34
#define IR_PIN_2 36
#define LED_IR_1_OUT 26
#define LED_IR_2_OUT 28
#define B1_SSD 27
#define C1_SSD 29
#define D1_SSD 35
#define A1_SSD 37
#define B2_SSD 43
#define C2_SSD 45
#define D2_SSD 47
#define A2_SSD 49
#define B3_SSD 43
#define C3_SSD 45
#define D3_SSD 47
#define A3_SSD 49
#define LEDTest 65
#define FLAG_INTERRUPT_0 0x01
#define onesDigit  1
#define tensDigit  2
#define hundredsDigit  3

volatile int mainEventFlags = 0;
int delayTime = 2000;
int v2;
int delayCount;
int ir1Reading, ir2Reading;
int count;
int SSD_counter;
int digCount;
int decCount;
int aPin, bPin, cPin, dPin;
int SSDNum = 1;
int A_SSD, B_SSD, C_SSD, D_SSD;
int dispNum, speedVal;

void setup()  {
	//Yes this setup is a bit disgusting, but there's no other way to do it
	pinMode(1, OUTPUT);
	pinMode(IR_PIN_1, INPUT);
	pinMode(IR_PIN_2, INPUT);
	pinMode(LED_IR_1_OUT, OUTPUT);
	pinMode(LED_IR_2_OUT, OUTPUT);
	pinMode(A1_SSD, OUTPUT);
	pinMode(B1_SSD, OUTPUT);
	pinMode(C1_SSD, OUTPUT);
	pinMode(D1_SSD, OUTPUT);
	pinMode(A2_SSD, OUTPUT);
	pinMode(B2_SSD, OUTPUT);
	pinMode(C2_SSD, OUTPUT);
	pinMode(D2_SSD, OUTPUT);
	pinMode(LEDTest, OUTPUT);
	pinMode(BUTTON_RIGHT, INPUT_PULLUP);
	Serial.begin(9600);
	servo1.attach(A7);
	digitalWrite(LEDTest, HIGH);

	//attachInterrupt(1, int0_isr, FALLING);

}

void loop() {

	// Control servo angle based on serial input
	if (Serial.available() > 0){
		char letter = Serial.read();
		if (letter == '1'){
			count = 1;
		}
		else if (letter == '2'){
			count = 2;
		}
		else if (letter == '0'){
			count = 0;
		}
	}

	// Set the PWM value to the track 0-255
	if (count == 1){
		v2 = 125;

	}
	else if (count == 2){
		v2 = 255;

	}
	else{
		v2 = 0;


	}

	//  If the button is pressed wave the flag back and forth
	if (digitalRead(BUTTON_RIGHT) == 1) {
		digitalWrite(LEDTest, LOW);
		servo1.write(0);
		delay(300);
		servo1.write(120);
		delay(300);
		servo1.write(0);
		delay(300);
		servo1.write(120);
		delay(300);
		servo1.write(0);
		delay(300);
		servo1.write(120);
		delay(300);
		servo1.write(0);
		delay(300);
		servo1.write(120);
		delay(300);
	}
	
	// Read motion sensors and set each section of LEDs
	if (digitalRead(IR_PIN_1) == LOW){
		digitalWrite(LED_IR_1_OUT, HIGH);
		digitalWrite(LED_IR_2_OUT, LOW);
	}

	if (digitalRead(IR_PIN_2) == LOW){
		digitalWrite(LED_IR_2_OUT, HIGH);
		digitalWrite(LED_IR_1_OUT, LOW);
	}

	analogWrite(PWM_PIN, v2);
}


void segDisplay(int speedVal, int dispNum){
  int ones, tens, hundreds;
  
  ones = speedVal%100;
}

void writeDigits(char SSDNum, char number){

	//Decide which Seven Segment Display to write to
	// 1 = ones digit   2 = tens digit  3 = hundreds digit
	if (SSDNum == 1){
		A_SSD = A1_SSD;
		B_SSD = B1_SSD;
		C_SSD = C1_SSD;
		D_SSD = D1_SSD;
	}
	else if (SSDNum == 2){
		A_SSD = A2_SSD;
		B_SSD = B2_SSD;
		C_SSD = C2_SSD;
		D_SSD = D2_SSD;
	}
	else if (SSDNum == 3){
		A_SSD = A3_SSD;
		B_SSD = B3_SSD;
		C_SSD = C3_SSD;
		D_SSD = D3_SSD;
	}

	// Write the numbers 0-9
	if (number == 1){
		digitalWrite(A_SSD, HIGH);
		digitalWrite(B_SSD, LOW);
		digitalWrite(C_SSD, LOW);
		digitalWrite(D_SSD, LOW);
	}
	else if (number == 2){
		digitalWrite(A_SSD, LOW);
		digitalWrite(B_SSD, HIGH);
		digitalWrite(C_SSD, LOW);
		digitalWrite(D_SSD, LOW);
	}
	else if (number == 3){
		digitalWrite(A_SSD, HIGH);
		digitalWrite(B_SSD, HIGH);
		digitalWrite(C_SSD, LOW);
		digitalWrite(D_SSD, LOW);
	}
	else if (number == 4){
		digitalWrite(A_SSD, LOW);
		digitalWrite(B_SSD, LOW);
		digitalWrite(C_SSD, HIGH);
		digitalWrite(D_SSD, LOW);
	}
	else if (number == 5){
		digitalWrite(A_SSD, HIGH);
		digitalWrite(B_SSD, LOW);
		digitalWrite(C_SSD, HIGH);
		digitalWrite(D_SSD, LOW);
	}
	else if (number == 6){
		digitalWrite(A_SSD, LOW);
		digitalWrite(B_SSD, HIGH);
		digitalWrite(C_SSD, HIGH);
		digitalWrite(D_SSD, LOW);
	}
	else if (number == 7){
		digitalWrite(A_SSD, HIGH);
		digitalWrite(B_SSD, HIGH);
		digitalWrite(C_SSD, HIGH);
		digitalWrite(D_SSD, LOW);
	}
	else if (number == 8){
		digitalWrite(A_SSD, LOW);
		digitalWrite(B_SSD, LOW);
		digitalWrite(C_SSD, LOW);
		digitalWrite(D_SSD, HIGH);
	}
	else if (number == 9){
		digitalWrite(A_SSD, HIGH);
		digitalWrite(B_SSD, LOW);
		digitalWrite(C_SSD, LOW);
		digitalWrite(D_SSD, HIGH);
	}
	else if (number == 0){
		digitalWrite(A_SSD, LOW);
		digitalWrite(B_SSD, LOW);
		digitalWrite(C_SSD, LOW);
		digitalWrite(D_SSD, LOW);
	}
}
