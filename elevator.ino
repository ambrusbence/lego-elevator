// INCLUDE LIBRARIES
#include <unistep.h>

// LIFT DATA
#define min_level 0
#define max_level 3
#define one_level_rotation 11000

#define up 0
#define down 1

// BUTTONS
#define call_0 4
#define call_1 5
#define call_2 6
#define call_3 7
#define call_down 12
#define call_up 13

// DISPLAY
#define latch_pin 1 //pin 12 on 595
#define data_pin 2 //pin 14 on  595
#define clock_pin 3 //pin 11 on 595
#define down_led 18
#define up_led 19

// LIFT 1
#define blue 8
#define pink 9
#define yellow 10
#define orange 11
#define HALFSTEP 4096
#define FULLSTEP 2048


// Create a stepper/elevator instance
unistep lift_1(blue,pink,yellow,orange,FULLSTEP,900);  //900 is step delay


// OWN VARIABLES:
int current_level = 0;// = min_level;
int needed_rotation = 0;
int current_position = current_level * one_level_rotation;
byte displayed_level[10] = {119, 65, 59, 107, 77, 110, 126, 67, 127, 111};  // Numbers 0-9


// OWN FUNCTIONS:

int tellCurrentLevel(int lift_position)
{
  int current_level;
  switch (lift_position)
  {
    case 0: current_level = 0;
    break;
    case 11000: current_level = 1;
    break;
    case 22000: current_level = 2;
    break;
    case 33000: current_level = 3;
    break;
  }
  return current_level;
}


void sevenSegment(int i)
{
  digitalWrite(latch_pin, LOW);
  shiftOut(data_pin, clock_pin, MSBFIRST, displayed_level[i]);
  digitalWrite(latch_pin, HIGH);
}


int oneLevelUp()
{
  digitalWrite(up_led,HIGH);
  lift_1.moves(one_level_rotation, up);
  digitalWrite(up_led,LOW);
  current_level += 1;
  Serial.println(current_level);
  sevenSegment(current_level);
}

int oneLevelDown()
{
  digitalWrite(down_led,HIGH);
  lift_1.moves(one_level_rotation, down);
  digitalWrite(down_led,LOW);
  current_level -= 1;
  Serial.println(current_level);
  sevenSegment(current_level);
}

/*int moveDownToLevel(int desired_level)
{
	needed_rotation = current_position - one_level_rotation * desired_level;
    lift_1.moves(needed_rotation, down);
    current_level = current_level - tellCurrentLevel(needed_rotation);
    Serial.println(current_level);
    sevenSegment(current_level);
}

int moveUpToLevel(int desired_level)
{
	needed_rotation = one_level_rotation * desired_level - current_position;
    lift_1.moves(needed_rotation,up);
    current_level = current_level + tellCurrentLevel(needed_rotation);
    Serial.println(current_level);
    sevenSegment(current_level);
}
*/

void callDownToLevel(int desired_level)
{
  for (;current_level > desired_level;)
    {
      oneLevelDown();
    }
}

void callUpToLevel(int desired_level)
{
  for (;current_level < desired_level;)
      {
        oneLevelUp();
      }
}

// INITIALIZATION

void setup()
{
pinMode(4, INPUT);  // level 0
pinMode(5, INPUT);  // level 1
pinMode(6, INPUT);  // level 2
pinMode(7, INPUT);  // level 3
pinMode(12, INPUT);  // level up
pinMode(13, INPUT);  // level down
pinMode(1, OUTPUT);  // latch_pin
pinMode(2, OUTPUT);  // data_pin
pinMode(3, OUTPUT);  // clock_pin
pinMode(18, OUTPUT);  // led_down
pinMode(19, OUTPUT);  // led_up
Serial.begin(9600);  // Serial port Baud rate
sevenSegment(0);  // display 0 on 7 segment
Serial.println(0);  // display 0 on serial port
}


//MAIN PROGRAM:
void loop()
{
current_position = current_level * one_level_rotation;

// MOVE UP
if ((digitalRead(call_up) == HIGH) && (current_level != max_level))
	{
  oneLevelUp();
	}

// MOVE DOWN
if ((digitalRead(call_down) == HIGH) && (current_level != min_level))
	{
  oneLevelDown();
	}

// MOVE TO GROUND FLOOR
if (digitalRead(call_0) == HIGH)
{
if (current_level>min_level)
	{
   callDownToLevel(min_level);
	}
}

// MOVE TO FLOOR 1
if (digitalRead(call_1) == HIGH)
{
  if (current_level == min_level)
  {
    oneLevelUp();
  }

  if( current_level > 1 )
  {
    //moveDownToLevel(1);
    callDownToLevel(1);
  }
}

// MOVE TO FLOOR 2
if (digitalRead(call_2) == HIGH)
{
    if( current_level <2 )
  {
    callUpToLevel(2);
  }
  
  if( current_level > 2 )
  {
    callDownToLevel(2);
  }
}

// MOVE TO FLOOR 3
if (digitalRead(call_3) == HIGH)
{
	if (current_level < max_level)
	{
     callUpToLevel(max_level);
	}
}
}

