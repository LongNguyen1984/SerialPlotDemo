#include <TimerEvent.h>

const unsigned int timerOnePeriod = 100;

TimerEvent timerOne;

const int LEDPIN = 13;
String CommandString = "";
String Inputdata = "";
bool is_data_complete = false;
void setup() {
  // put your setup code here, to run once:
  pinMode(LEDPIN, OUTPUT);
  Serial.begin(115200);

  randomSeed( (unsigned long)( micros()%millis() ) );
  // Set the interval timing as well as pass the function name
  timerOne.set(timerOnePeriod,timerOneFunc);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial_data_Event();

  if (is_data_complete == true)
  {
    if (CommandString=="On")
    {
      digitalWrite(LEDPIN,HIGH);
      Serial.println("LED IS ON");
    }
    else if (CommandString == "Off")
    {
      digitalWrite(LEDPIN, LOW);
      Serial.println("LED IS OFF");
    }
    is_data_complete = false;
    CommandString ="";
    Inputdata ="";
  }

  // Remember to run the update() function in loop()!
  timerOne.update();
}
void Serial_data_Event()
{
  Serial.flush();
  while(Serial.available())
  {
    char inchar = (char)Serial.read();
    if (inchar == '\n')
    {
      is_data_complete = true;
      CommandString = Inputdata;
    }else
    {
      Inputdata +=inchar;
    }
  }
}

// This function will be called every timerOnePeriod
void timerOneFunc(){
  int sensorValue = analogRead(A0);
  Serial.print("Resistor: ");
  Serial.println(sensorValue);
  //Serial.print(" (Update in ");
  //Serial.print(timerOnePeriod);
  //Serial.println("ms)\n");
}
