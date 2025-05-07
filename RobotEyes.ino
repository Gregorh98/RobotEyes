#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVO_FREQ 50 // Analog servos run at ~50 Hz

// Define pin numbers for each servo
#define SERVO_0 0
#define SERVO_1 1
#define SERVO_2 2
#define SERVO_3 3
#define SERVO_4 4
#define SERVO_5 5
#define SERVO_6 6

class Muscle {
  public:
    uint8_t pin;
    int minDegree;
    int maxDegree;

    Muscle(uint8_t p, int minD, int maxD) {
      pin = p;
      minDegree = minD;
      maxDegree = maxD;
    }

    void setAngle(int angle) {
      angle = constrain(angle, minDegree, maxDegree);
      int pulseLength = map(angle, -90, 90, 150, 600);
      pwm.setPWM(pin, 0, pulseLength);
    }
};

class EyelidSet {
  public:
    Muscle& top;
    Muscle& bottom;

    EyelidSet(Muscle& t, Muscle& b) : top(t), bottom(b) {}
  
    void close() {
      top.setAngle(0);
      bottom.setAngle(0);
    }

    void open() {
      top.setAngle(45);
      bottom.setAngle(-45);
    }
};



// Define each muscle object
Muscle left_right_look(SERVO_0, -45, 45);
Muscle up_down_look(SERVO_1, -45, 45);
Muscle top_right_eyelid(SERVO_2, 0, 45);
Muscle bottom_right_eyelid(SERVO_3, -45, 0);
Muscle bottom_left_eyelid(SERVO_4, 0, 45);
Muscle top_left_eyelid(SERVO_5, -45, 0);

EyelidSet right_eyelid_set(top_right_eyelid, bottom_right_eyelid);
EyelidSet left_eyelid_set(bottom_left_eyelid, top_left_eyelid);


void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);
  delay(10);

  // Set all servos to 0 degrees
  left_right_look.setAngle(0);
  up_down_look.setAngle(0);
  top_right_eyelid.setAngle(0);
  bottom_right_eyelid.setAngle(0);
  bottom_left_eyelid.setAngle(0);
  top_left_eyelid.setAngle(0);

}

void loop() {
  right_eyelid_set.open();
  left_eyelid_set.open();
  delay(1000);
  right_eyelid_set.close();
  left_eyelid_set.close();
  delay(1000);

}

