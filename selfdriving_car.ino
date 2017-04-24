#include <Servo.h>
#include <MsTimer2.h>
//http://playground.arduino.cc/Main/MsTimer2

// defines pins numbers
#define trigPin 10  //Ultrasonic_Sensor triger pin
#define echoPin 11 //Ultrasonic_Sensor echopin

#define trigPortB 2  //Ultrasonic_Sensor triger pin
#define echoPortB 3 //Ultrasonic_Sensor echopin

#define MOTOR_FD_PIN 2
#define MOTOR_BD_PIN  4
#define MOTOR_SPEED_PIN 5
#define STEERING_SERVO_PIN 9 //turn_rotationing servo


#define ROTATION_LOW  75    //servo left
#define ZERO_ROTATION 85    //servo center
#define ROTATION_HIGH 95    //servo right

#define FOWARD_SPEED 150
#define BACKWARD_SPEED 150

// we use these for setting the port bits directly for faster digitalWrite
#define CLR(x,y) (x&=(~(1<<y)))
#define SET(x,y) (x|=(1<<y))


#define STOPPING_DISTANCE 20

Servo myservo;  // create servo object to control a servo

// defines variables
long duration;
int distance;
int randNum;

void setup() {	
	// Serial.begin(9600);
	MsTimer2::set(100, callback); // 100ms period
	MsTimer2::start();
	pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
	pinMode(echoPin, INPUT); // Sets the echoPin as an Input
	pinMode(STEERING_SERVO_PIN, OUTPUT);
	myservo.attach(STEERING_SERVO_PIN);  // attaches the servo on pin 9 to the servo object
	turn_rotation(ZERO_ROTATION);
}

void callback()
{
	noInterrupts();
	distance = sense();
	// Serial.print("Distance: ");
	// Serial.print(distance);  
	// Serial.println(" Cm ");
	interrupts();
}

void loop() {
	turn_rotation(ZERO_ROTATION);
	if (distance <= STOPPING_DISTANCE){
		backward(200);
		randNum = random(0, 2);
		turn_rotation(ROTATION_LOW);
		backward(1000);
		turn_rotation(ZERO_ROTATION);
	} if (distance > STOPPING_DISTANCE){
		forward();
	}
}

void turn_rotation(int d) {
	myservo.write(d);		// sets the servo position according to the scaled value
	delay(100);
}

int sense() {
	// Clears the trigPin
	CLR(PORTB, trigPortB);	// digitalWrite(trigPin, LOW);
	delayMicroseconds(2);
	// Sets the trigPin on HIGH state for 10 micro seconds
	SET(PORTB, trigPortB);	// digitalWrite(trigPin, HIGH);
	delayMicroseconds(10);
	CLR(PORTB, trigPortB);	// digitalWrite(trigPin, LOW);
	// Reads the echoPin, return_rotations the sound wave travel time in microseconds
	duration = pulseIn(echoPin, HIGH);
	// Calculating the distance
	distance= (duration*0.034/2)+1;
	return distance;
}

void forward() {
	CLR(PORTD, MOTOR_FD_PIN);	// digitalWrite(MOTOR_FD_PIN, LOW);
	SET(PORTD, MOTOR_BD_PIN);	// digitalWrite(MOTOR_BD_PIN, HIGH);
	analogWrite(MOTOR_SPEED_PIN, FOWARD_SPEED);
}

void backward(int t) {
	SET(PORTD, MOTOR_FD_PIN);	// digitalWrite(MOTOR_FD_PIN, HIGH);
	CLR(PORTD, MOTOR_BD_PIN);	// digitalWrite(MOTOR_BD_PIN, LOW);
	analogWrite(MOTOR_SPEED_PIN, BACKWARD_SPEED);
	delay(t);
}

void stop(int t){
	CLR(PORTD, MOTOR_FD_PIN);	// digitalWrite(MOTOR_FD_PIN, LOW);
	CLR(PORTD, MOTOR_BD_PIN);	// digitalWrite(MOTOR_BD_PIN, LOW);
	analogWrite(MOTOR_SPEED_PIN, 0);
	delay(t);
}

