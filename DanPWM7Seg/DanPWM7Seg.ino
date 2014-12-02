#include <Servo.h>
#include <LiquidCrystal.h>

Servo servo1;

#define PWM_PIN 11  
#define BREAK_1 A2
#define BREAK_2 A4
#define BUTTON_RIGHT 32
#define IR_PIN_1 34
#define IR_PIN_2 36
#define LED_IR_1_OUT 28
#define LED_IR_2_OUT 26
#define LIGHT_THRESHOLD 255
#define LIGHT_TIME 75

#define A_SSD 48
#define B_SSD 46
#define C_SSD 44
#define D_SSD 42
#define ONES_DISP 21
#define TENS_DISP 25
#define HUNDREDS_DISP 29

int v2;
long delayCount1 = 0;
long delayCount2;
int ir1Reading;
int ir2Reading;
int fingCount;
unsigned long SSD_Counter;
int SSD_Num = 1;
unsigned long flag_timer;
unsigned long flag_time;


void setup()  {
	//Yes this setup is a bit disgusting, but there's no other way to do it
	pinMode(1, OUTPUT);
	pinMode(IR_PIN_1, INPUT);
	pinMode(IR_PIN_2, INPUT);
	pinMode(BREAK_1, INPUT);
	pinMode(BREAK_2, INPUT);
	pinMode(LED_IR_1_OUT, OUTPUT);
	pinMode(LED_IR_2_OUT, OUTPUT);
	pinMode(A_SSD, OUTPUT);
	pinMode(B_SSD, OUTPUT);
	pinMode(C_SSD, OUTPUT);
	pinMode(D_SSD, OUTPUT);	
	pinMode(BUTTON_RIGHT, INPUT_PULLUP);
        pinMode(ONES_DISP, OUTPUT);
        pinMode(TENS_DISP, OUTPUT);
        pinMode(HUNDREDS_DISP, OUTPUT);
	Serial.begin(9600);
	servo1.attach(A7);
	//digitalWrite(LEDTest, HIGH);

	//attachInterrupt(1, int0_isr, FALLING);

}

void loop() {

        delay(1);
        // Increment the counter quickly to allow multi-plexing for SSD
        // Use SSD_Num whenever writing to speedometer
        SSD_Counter++;
        if(SSD_Counter%2 == 0) SSD_Num = 3;
        else if(SSD_Counter%3 == 0) SSD_Num = 1;
        else SSD_Num = 2;

        writeDigits(SSD_Num, 740);
        
        // Read the serial input and set the track PWM
	if (Serial.available() > 0){
		char letter = Serial.read();
		if (letter == '1'){
			fingCount = 1;
                        v2 = 125;
		}
		else if (letter == '2'){
  			fingCount = 2;
                        v2 = 255;
		}
		else if (letter == '0'){
                        fingCount = 0;
                        v2 = 0;
		}
	}
       	analogWrite(PWM_PIN, v2);

        // If the button is pressed wave the flag back and forth between 40 and 105 degrees
        flag_time = millis();
	if (digitalRead(BUTTON_RIGHT) == 1) {
                flag_timer = millis();
		servo1.write(40);
	}



        if(flag_time - flag_timer > 1200 && flag_time - flag_timer < 1500){
           servo1.write(105); 
        } else if(flag_time - flag_timer > 900){
          servo1.write(40);
        } else if(flag_time - flag_timer > 600){
          servo1.write(105);
        } else if(flag_time - flag_timer > 300){
             servo1.write(105); 
        } else {
          servo1.write(105);
        }
        
	if(analogRead(BREAK_2)< LIGHT_THRESHOLD){
           digitalWrite(LED_IR_2_OUT, HIGH);
           digitalWrite(LED_IR_1_OUT,LOW);
        }
        if(analogRead(BREAK_1) < LIGHT_THRESHOLD){
          digitalWrite(LED_IR_1_OUT, HIGH);
          digitalWrite(LED_IR_2_OUT, LOW); 
        }
        
        
        
	// Read motion sensors and set each section of LEDs
	/*delayCount2 = millis();
	
		if (analogRead(BREAK_1) < LIGHT_THRESHOLD){
			delay(LIGHT_TIME);
			if (analogRead(BREAK_1) < LIGHT_THRESHOLD){
				digitalWrite(LED_IR_1_OUT, HIGH);
				delayCount1 = millis();
				//digitalWrite(LED_IR_2_OUT, LOW);
			}
		}
		if ((delayCount2 - delayCount1) > 1000){
			digitalWrite(LED_IR_1_OUT, LOW);
		}*/
	

/*	if (analogRead(BREAK_2) < LIGHT_THRESHOLD){
		delay(LIGHT_TIME);
		if (analogRead(BREAK_1) < LIGHT_THRESHOLD){
			digitalWrite(LED_IR_2_OUT, HIGH);
			digitalWrite(LED_IR_1_OUT, LOW);
		}
	}
*/

}

/* 
Description: Writes up to three numbers to separate Seven Segment Displays

Inputs
SSDNum: The Seven Segment Display being written to
number: multi digit number to be written

Outputs
-------

*/
void writeDigits(char SSDNum, char number){
        int ones, tens, hundreds, varNum;
        // Split the written number into single digits
        
        /*if(number >99){
          ones = number%10;
          tens = (number/10)%10;
          hundreds = number/100;
        }else if(number > 9){
          ones = number%10;
          tens = number/10;
          hundreds = 0;
        }else{
          ones = number;
          tens = 0;
          hundreds = 0;
        }*/
        
        ones = 0;
        tens = 4;
        hundreds = 7;
        
	//Decide which Seven Segment Display to write to
	// 1 = ones digit   2 = tens digit  3 = hundreds digit
	if (SSDNum == 1){
            digitalWrite(ONES_DISP, HIGH);
            digitalWrite(TENS_DISP, LOW);
            digitalWrite(HUNDREDS_DISP, LOW);
            varNum = ones;
	}
	else if (SSDNum == 2){
            digitalWrite(ONES_DISP, LOW);
            digitalWrite(TENS_DISP, HIGH);
            digitalWrite(HUNDREDS_DISP, LOW);
            varNum = tens;
	}
	else if (SSDNum == 3){
            digitalWrite(ONES_DISP, LOW);
            digitalWrite(TENS_DISP, LOW);
            digitalWrite(HUNDREDS_DISP, HIGH);
            varNum = hundreds;
	}

	// Write the numbers 0-9
	if (varNum == 1){
		digitalWrite(A_SSD, HIGH);
		digitalWrite(B_SSD, LOW);
		digitalWrite(C_SSD, LOW);
		digitalWrite(D_SSD, LOW);
	}
	else if (varNum == 2){
		digitalWrite(A_SSD, LOW);
		digitalWrite(B_SSD, HIGH);
		digitalWrite(C_SSD, LOW);
		digitalWrite(D_SSD, LOW);
	}
	else if(varNum == 3){
		digitalWrite(A_SSD, HIGH);
		digitalWrite(B_SSD, HIGH);
		digitalWrite(C_SSD, LOW);
		digitalWrite(D_SSD, LOW);
	}
	else if (varNum == 4){
		digitalWrite(A_SSD, LOW);
		digitalWrite(B_SSD, LOW);
		digitalWrite(C_SSD, HIGH);
		digitalWrite(D_SSD, LOW);
	}
	else if (varNum == 5){
		digitalWrite(A_SSD, HIGH);
		digitalWrite(B_SSD, LOW);
		digitalWrite(C_SSD, HIGH);
		digitalWrite(D_SSD, LOW);
	}
	else if (varNum == 6){
		digitalWrite(A_SSD, LOW);
		digitalWrite(B_SSD, HIGH);
		digitalWrite(C_SSD, HIGH);
		digitalWrite(D_SSD, LOW);
	}
	else if (varNum == 7){
		digitalWrite(A_SSD, HIGH);
		digitalWrite(B_SSD, HIGH);
		digitalWrite(C_SSD, HIGH);
		digitalWrite(D_SSD, LOW);
	}
	else if (varNum == 8){
		digitalWrite(A_SSD, LOW);
		digitalWrite(B_SSD, LOW);
		digitalWrite(C_SSD, LOW);
		digitalWrite(D_SSD, HIGH);
	}
	else if (varNum == 9){
		digitalWrite(A_SSD, HIGH);
		digitalWrite(B_SSD, LOW);
		digitalWrite(C_SSD, LOW);
		digitalWrite(D_SSD, HIGH);
	}
	else if (varNum == 0){
		digitalWrite(A_SSD, LOW);
		digitalWrite(B_SSD, LOW);
		digitalWrite(C_SSD, LOW);
		digitalWrite(D_SSD, LOW);
	}
}
