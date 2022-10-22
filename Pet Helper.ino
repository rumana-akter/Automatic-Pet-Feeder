#include <DS3231.h>
#include <Servo.h>
#include <LiquidCrystal.h>
#include <Keypad.h>

int R_LED = 22; 
int G_LED = 23; 
int B_LED = 24;

int j=39;
int k=40;
int l=41;
int m=42;

int Fan_one_Forward = 53;
int Fan_one_Forward1 = 52; 
int Fan_two_Forward = 51; 
int Fan_two_Forward1 = 50; 

int value = 0;
int lm35 = A8;

int flag = 0;
const int buzzer = 33;

int ledPin = 28; // LED 
int pirPin = 31; // PIR Out pin 
int pirStat = 0; // PIR status

const int trigPin = 37;
const int echoPin = 36;
long duration;
int distance;

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Three columns

// Define the Keymap

char keys[ROWS][COLS] = {

  {'7','8','9','/'},

  {'4','5','6','*'},

  {'1','2','3','-'},

  {'C','0','=','+'}

};

// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.

byte rowPins[ROWS] = { 2, 3, 4, 5 };

// Connect keypad COL0, COL1 and COL2 to these Arduino pins.

byte colPins[COLS] = { 6, 7, 8, 9 };

//  Create the Keypad
  Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

DS3231  rtc(A4, A5);
Servo servo_test;      //initialize a servo object for the connected servo  
Servo servo1;
LiquidCrystal lcd(A0, A1, A2, 11, 12, 13); // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7)

 //int angle = 0;    
// int potentio = A0;      // initialize the A0analog pin for potentiometer
 int t1, t2, t3, t4, t5, t6;

 
boolean feed = true; // condition for alarm

 char key;
 int r[6];
 
 void setup() 
 { 
  servo_test.attach(10);   // attach the signal pin of servo to pin9 of arduino
  servo1.attach(47);
  rtc.begin();
  lcd.begin(16,2);
  //rtc.begin();
  servo_test.write(55); 
  Serial.begin(9600);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);

   pinMode (R_LED, OUTPUT);  // declaring the LED as an output 
   pinMode (B_LED, OUTPUT);  
   pinMode (G_LED, OUTPUT);

   pinMode(j, OUTPUT);
   pinMode(k, OUTPUT);
   pinMode(l, OUTPUT);
   pinMode(m, OUTPUT);

   pinMode(Fan_one_Forward, OUTPUT);
   pinMode(Fan_one_Forward1, OUTPUT);  
   pinMode(Fan_two_Forward, OUTPUT);
   pinMode(Fan_two_Forward1, OUTPUT);

   pinMode(buzzer, OUTPUT);

   pinMode(ledPin, OUTPUT);     
   pinMode(pirPin, INPUT);    

   pinMode(trigPin, OUTPUT);
   pinMode(echoPin, INPUT);   
   
 } 
 
  void loop() 
 { 

lcd.setCursor(0,0);
int buttonPress;
buttonPress = digitalRead(A3);
//lcd.print("Time:  ");

if (buttonPress==1)
 setFeedingTime();
 

//Serial.println(buttonPress);

 lcd.print("Time:  ");
 String t = "";
 t = rtc.getTimeStr(); 
 t1 = t.charAt(0)-48;
 t2 = t.charAt(1)-48;
 t3 = t.charAt(3)-48;
 t4 = t.charAt(4)-48;
 t5 = t.charAt(6)-48;
 t6 = t.charAt(7)-48;
 
 lcd.print(rtc.getTimeStr());
 lcd.setCursor(0,1);
 lcd.print("Date: ");
 lcd.print(rtc.getDateStr());
 
 if (t1==r[0] && t2==r[1] && t3==r[2] && t4==r[3]&& t5<1 && t6<3 && feed==true)
 { 
  
  servo_test.write(100);                   //command to rotate the servo to the specified angle 
   delay(400);   
  servo_test.write(55); 
  feed=false;
  tone(buzzer,1000); // Send 1KHz sound signal...
  delay(100);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  delay(1000); 
  
 }
  
  if(flag == 1)
  {
    motion();
    trash();
    heatingLED();
  }
 }       

void setFeedingTime()
{
  feed = true;
   int i=0;

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set feeding Time");
  delay(100);
  lcd.clear();
  lcd.print("HH:MM");
  lcd.setCursor(0,1);

  
  while(1){
    key = kpd.getKey();

    char j;
    
  if(key!=NO_KEY){
    
    lcd.setCursor(j,1);
    
    lcd.print(key);
    
    r[i] = key-48;
    i++;
    j++;

    if (j==2)
    {
      lcd.print(":"); j++;
    }
    delay(500);
  }
  
  if (key == '+')
  {key=0;
  flag = 1; 
  break; }
  }
}


void motion()
{
  pirStat = digitalRead(pirPin); 
   
  if (pirStat == HIGH) {            // if motion detected
    digitalWrite(ledPin, HIGH);  // turn LED ON
    Serial.println("Motion Detected");
    delay(1000);

  } 
  else {
    digitalWrite(ledPin, LOW); // turn LED OFF if we have no motion
    Serial.println("No Motion Detected");
    delay(1000);
  }
}

void trash()
{
  digitalWrite(trigPin, LOW); //clear trigger pin
  delay(2);

  digitalWrite(trigPin, HIGH);
  delay(10);  // Set trigPin on HIGH state for 10 micro seconds


  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds

  distance= duration*0.034/2;

  if(distance<100)
  {
    servo1.write(0);  
    delay(300);       
    servo1.write(150);    
    delay(1000);
  }
  /*else
  {
    servo1.detach();
  }*/

  Serial.print("Distance(cm): ");
  Serial.println(distance);
}

void heatingLED()
{
    value = analogRead(lm35); 
    float millivolts = ( value / 1024.0) * 5 * 1000;
    float Temp_C = millivolts / 10;
    //float farh = (cel * 9) / 5 + 32;
  
    Serial.print("Temp in C: ");
    Serial.println(Temp_C);
    //Serial.print("Temp in F: ");
    //Serial.println(Temp_F);
  

    
  if ( Temp_C >= 29.0)
  {
    Serial.println("Warning: It is very hot");
    Serial.println("");
    digitalWrite(R_LED, HIGH); // Red LED on


    digitalWrite(j, LOW);
    digitalWrite(k, LOW);
    digitalWrite(l, LOW);
    digitalWrite(m, LOW);

    digitalWrite(Fan_one_Forward, HIGH);
    digitalWrite(Fan_one_Forward1, LOW);
    digitalWrite(Fan_two_Forward, HIGH);
    digitalWrite(Fan_two_Forward1, LOW);

    delay(2000);// wait for one sec
    digitalWrite(G_LED, LOW);
    digitalWrite(B_LED, LOW);
    digitalWrite(R_LED, LOW);// Red LED off

    delay(2000); // wait for one sec


  }

  else if (Temp_C >= 26.0 && Temp_C < 29.0)
  {
    Serial.println("Warning: It is hot");
    Serial.println("");
    digitalWrite(R_LED, HIGH);  // RED LED on
    digitalWrite(j, LOW);
    digitalWrite(k, LOW);
    digitalWrite(l, LOW);
    digitalWrite(m, LOW);
    digitalWrite(Fan_one_Forward, LOW);
    digitalWrite(Fan_one_Forward1, HIGH);
    digitalWrite(Fan_two_Forward, LOW);
    digitalWrite(Fan_two_Forward1, LOW);
    delay(2000);
    digitalWrite(B_LED, LOW);
    digitalWrite(G_LED, LOW);
    digitalWrite(R_LED, LOW);

    //analogWrite(Buz,0);
    delay(2000);    // wait for a sec

  }

  else if (Temp_C >= 23.0 && Temp_C < 26.0)
  {
    Serial.println("Warning: It is normal");
    Serial.println("");
    digitalWrite(G_LED, HIGH);  // GREEN LED on
   
    digitalWrite(j, HIGH);
    digitalWrite(k, LOW);
    digitalWrite(l, LOW);
    digitalWrite(m, LOW);
    digitalWrite(Fan_one_Forward, LOW);
    digitalWrite(Fan_one_Forward1, LOW);
    digitalWrite(Fan_two_Forward, LOW);
    digitalWrite(Fan_two_Forward1, LOW);
    delay(2000);
    digitalWrite(B_LED, LOW);
    digitalWrite(G_LED, LOW);
    digitalWrite(R_LED, LOW);

    //analogWrite(Buz,0);
    delay(2000);    // wait for a sec

  }


  else if (Temp_C >= 20.0 && Temp_C < 23.0)
  {
    Serial.println("Warning: It is very  cold ");
    Serial.println("");
    digitalWrite(B_LED, HIGH);  // BLUE LED on
   
    digitalWrite(j, HIGH);
    digitalWrite(k, HIGH);
    digitalWrite(l, LOW);
    digitalWrite(m, LOW);
    digitalWrite(Fan_one_Forward, LOW);
    digitalWrite(Fan_one_Forward1, LOW);
    digitalWrite(Fan_two_Forward, LOW);
    digitalWrite(Fan_two_Forward1, LOW);
    delay(2000);
    digitalWrite(B_LED, LOW);
    digitalWrite(G_LED, LOW);
    digitalWrite(R_LED, LOW);

    //analogWrite(Buz,0);
    delay(2000);    // wait for a sec

  }
  else if (Temp_C >= 17.0 && Temp_C < 20.0)
  {
   Serial.println("Warning: It is very  very cold ");
    Serial.println("");   
   digitalWrite(B_LED, HIGH);  // BLUE LED on
   
    digitalWrite(j, HIGH);
    digitalWrite(k, HIGH);
    digitalWrite(l, HIGH);
    digitalWrite(m, LOW);
    digitalWrite(Fan_one_Forward, LOW);
    digitalWrite(Fan_one_Forward1, LOW);
    digitalWrite(Fan_two_Forward, LOW);
    digitalWrite(Fan_two_Forward1, LOW);
    delay(2000);
    digitalWrite(B_LED, LOW);
    digitalWrite(G_LED, LOW);
    digitalWrite(R_LED, LOW);

    //analogWrite(Buz,0);
    delay(2000);    // wait for a sec

  }
  else
  {
    Serial.println("Warning: It extreme cold ");
    Serial.println("");

    digitalWrite(B_LED, HIGH);
    digitalWrite(j, HIGH);
    digitalWrite(k, HIGH);
    digitalWrite(l, HIGH);
    digitalWrite(m, HIGH);
    digitalWrite(Fan_one_Forward, LOW);
    digitalWrite(Fan_one_Forward1, LOW);
    digitalWrite(Fan_two_Forward, LOW);
    digitalWrite(Fan_two_Forward1, LOW);

    delay(2000);
    digitalWrite(B_LED, LOW);
    digitalWrite(R_LED, LOW);
    digitalWrite(G_LED, LOW);
    delay(2000);


  }

        
   }
        
