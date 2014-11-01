

#include <Servo.h>
#include <LiquidCrystal.h>



Servo servo1;

#define PWM_PIN 11    
#define BUTTON_RIGHT 2

#define FLAG_INTERRUPT_0 0x01


volatile int mainEventFlags = 0;
int delayTime = 2000;
int v2;
int delayCount;
int count;


void setup()  {
	pinMode(1, OUTPUT);
	Serial.begin(9600);
	servo1.attach(A7);


	attachInterrupt(0, int0_isr, FALLING);

}

void loop() {
	pinMode(BUTTON_RIGHT, INPUT_PULLUP);
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
		else if (letter == '3'){
			servo1.write(180);
			delay(15);
		}
		else if (letter == '4'){
			servo1.write(0);
			delay(15);
		}
		else if (letter == '5'){
			servo1.detach();
			
		}
	}



	if (count == 1){
		digitalWrite(pinChoice, LOW);
		v2 = 125;
		
	}
	else if (count == 2){
		digitalWrite(pinChoice, LOW);
		v2 = 255;
		
	}
	else{
		digitalWrite(pinChoice, LOW);
		v2 = 0;
		

	}

	if (mainEventFlags & FLAG_INTERRUPT_0) {
		delay(20);
		mainEventFlags &= ~FLAG_INTERRUPT_0;
		if (!digitalRead(BUTTON_RIGHT)) {
			servo1.write(0);
			delay(1000);
			servo1.write(120);
			delay(1000);
			servo1.write(0);
			delay(1000);
			servo1.write(120);
			delay(1000);
		}

	}
	

	analogWrite(PWM_PIN, v2);
	digitalWrite(pinChoice, HIGH);

	
}

void int0_isr(){
	mainEventFlags |= FLAG_INTERRUPT_0;
}