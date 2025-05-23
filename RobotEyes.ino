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
      top.setAngle(-90);
      bottom.setAngle(90);
    }

    void open() {
      top.setAngle(90);
      bottom.setAngle(-90);
    }
};

class Eyes {
  public:
      Eyes(EyelidSet& left_eye, EyelidSet& right_eye, Muscle& up_down, Muscle& left_right)
        : left_eye(left_eye), right_eye(right_eye), up_down(up_down), left_right(left_right) {}

      void open() {
        left_eye.open();
        right_eye.open();
      }

      void close() {
        left_eye.close();
        right_eye.close();
      }

      void blink() {
        this->close();
        delay(100);
        this->open();
      }

      void double_blink(){
        this->blink();
        delay(100);
        this->blink();
      }

      void look_left() {
        left_right.setAngle(90);
      }

      void look_right() {
        left_right.setAngle(-90);
      }

      void look_up() {
        up_down.setAngle(90);
      }

      void look_down() {
        up_down.setAngle(-90);
      }

      void look_ahead() {
        left_right.setAngle(0);
        up_down.setAngle(0);
      }

      void reset() {
        this->open();
        this->look_ahead();
      }

  private:
      EyelidSet& left_eye;
      EyelidSet& right_eye;
      Muscle& up_down;
      Muscle& left_right;
};




// Define each muscle object
Muscle left_right_look(SERVO_0, -45, 45);
Muscle up_down_look(SERVO_1, -25, 25);

Muscle top_right_eyelid(SERVO_2, -45, 0);

Muscle bottom_right_eyelid(SERVO_3, -45, 0);

Muscle bottom_left_eyelid(SERVO_4, 0, 45);

Muscle top_left_eyelid(SERVO_5, -45, 0);

EyelidSet right_eyelid_set(top_right_eyelid, bottom_right_eyelid);
EyelidSet left_eyelid_set(bottom_left_eyelid, top_left_eyelid);

Eyes eyes(left_eyelid_set, right_eyelid_set, up_down_look, left_right_look);


void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);
  delay(10);

  // Set all servos to 0 degrees
  eyes.reset();
}

void loop() {
  eyes.reset();
  delay(1000);

  eyes.look_down();
  delay(1000);
  eyes.look_up();
  delay(1000);

  eyes.reset();
  delay(1000);

  eyes.look_left();
  delay(1000);
  eyes.look_right();
  delay(1000);

  eyes.reset();
  delay(1000);

  eyes.blink();
  delay(500);
  eyes.double_blink();
  delay(500);

}

