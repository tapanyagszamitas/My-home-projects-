/*DC Motor rotation controlling
  code by Peter Jolankai 2019

   _____     _            _
  |  __ \   | |          ( )
  | |__) |__| |_ ___ _ __|/ ___
  |  ___/ _ \ __/ _ \ '__| / __|
  | |  |  __/ ||  __/ |    \__ \
  |_|  _\___|\__\___|_|    |___/
  |  \/  |          | |   (_)
  | \  / | __ _  ___| |__  _ _ __   ___  ___
  | |\/| |/ _` |/ __| '_ \| | '_ \ / _ \/ __|
  | |  | | (_| | (__| | | | | | | |  __/\__ \
  |_|  |_|\__,_|\___|_| |_|_|_| |_|\___||___/


  This is a code for demonstrating an easy DC motor rotation control.
  hardware and wireing:
  Arduino mega
  L298HN motor shield

  DC motor with gear shaft with 5  spokes
  line following adapter (but it's better to use a single Infrared LED )
  wireing: from motor control shield B+ and B- to DC motor
  Arduino Mega to line following adapter:

  GND-> GND
  5V -> VCC
  pin 18 -> L (left sensor this case)

  it is important to adjust sensibility of the IR sensor!
*/


void setup() {

  Serial.begin(9600);
  pinMode(13, OUTPUT); //Initiates Motor Channel A pin
  pinMode(8, OUTPUT);  //Initiates Brake Channel A pin

  pinMode(18, INPUT); // IR sensor

}
byte val, previous  = 0;
int pwm = 0;
unsigned long measured_time = 0;
unsigned long previoustime = 0;
int rotation_time, rpm, previous_rpm = 0;
/**/
int rpm_goal = 56;
/**/
void loop() {

  digitalWrite(13, HIGH); //Establishes forward direction of Channel B
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B

  analogWrite(11, pwm); // PWM to motor

  val = digitalRead(18);

  if ((previous == 0) && (val == 1))
  {
    Serial.println("rising edge ");
    measured_time = millis();
    rotation_time = (measured_time - previoustime) * 5; //5 = number of spokes
    if (rotation_time != 0) {
      rpm = 60000 / rotation_time;
    }
  }
  if (rpm_goal >= rpm) 
  {
    pwm = pwm + 1;
  }
  else
  {
    pwm = pwm - 2;
  }

if (previous_rpm == rpm)  // it was nessecery to add this logical if the sensor is not measuring a value.
   {
    pwm = pwm + 1;
  }
  // not to send out bad PWM signal
  if (pwm > 255)
  {
    pwm = 255;
  };
  if (pwm < 0)
  {
    pwm = 0;
  };
  previous_rpm = rpm;
  previoustime = measured_time;
  previous = val;

  Serial.print(" rpm "); // send datas to serial monitor / plotter
  Serial.print(rpm);
  Serial.print(" rpm_goal  ");
  Serial.print(rpm_goal);
  Serial.print(" pwm  ");
  Serial.print(pwm);
  Serial.println(" ");
}
