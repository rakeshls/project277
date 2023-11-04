#include <TVout.h>
#include "a.h"
#include "u.h"
#include "o.h"
#include "m.h"
#include "e.h"
#include "dance1.h"
#include "dance2.h"
#include "dance3.h"
#include "dance4.h"
#include "cat1.h"
#include "cat2.h"
#include "cat3.h"
#include "disturbance1.h"
#include "disturbance2.h"
#include "disturbance3.h"
#include "font6x8.h"
#include <IRremote.h>

TVout tv;
IRrecv receiver(2);

int tv_state = 0, last_tv_state = 0;  //  initially off

void starting_animation(){
  for (int i = 0; i < 5; i++){
    tv.bitmap(20,16,d1);
    tv.delay_frame(3);  //  i frame = 20ms (PAL tv)
    tv.bitmap(20,16,d2);
    tv.delay_frame(3);
    tv.bitmap(20,16,d3);
    tv.delay_frame(3);
  }
  tv.clear_screen();
  tv.delay(100);
}

void disable_receiver(){
  receiver.disableIRIn();  //  disable receiver, enable tv
  tv.begin(PAL);
}

void enable_receiver(){
  tv.end();  //  disable tv, enable receiver
  receiver.enableIRIn();
}

void speaking_animation(int x, int y){
  char message [] = "Robot World News";
  tv.print(30,tv.vres()-10,message);
  for (int i = 0; i < 20; i++){
    int frame = random(0,5);
    if (frame  ==  0)tv.bitmap(x,y,a);
    else if (frame  ==  1)tv.bitmap(x,y,u);
    else if (frame  ==  2)tv.bitmap(x,y,o);
    else if (frame  ==  3)tv.bitmap(x,y,m);
    else if (frame  ==  4)tv.bitmap(x,y,e);
    tv.delay_frame(5);
  }
  tv.clear_screen();
  tv.delay(100);
}

void setup(){
  Serial.begin(9600);
  tv.select_font(font6x8);  //  selecting font

  enable_receiver();

}

void loop(){

  if (receiver.decode()){
    int response = receiver.decodedIRData.command;
    if (response  ==  162)tv_state = !tv_state;  //  flip the state
    television(response);
    receiver.resume();
  }
}

void television(int code){
   disable_receiver();
   
  if (tv_state){  //  tv is on
    if (last_tv_state  ==  0){  //  tv is just turned on
      starting_animation();
      select_channel();
      last_tv_state = tv_state;
    }
    else if (last_tv_state  ==  1){
      if (code  ==  104)news_channel();
      else if (code  ==  48)dance_channel(30,0);
      else if(code  ==  24)animal_channel(30,0);
    }
  }
  else{  //  tv is off
    tv.clear_screen();
    tv.delay(100);
    last_tv_state = tv_state;
  }
  enable_receiver();
}

void select_channel(){
  tv.println(30,40, "SELECT A CHANNEL");
  tv.println(45,50,"0 : NEWS");
  tv.println(45,60,"1 : DANCE");
  tv.println(45,70,"2 : ANIMALS");
  tv.delay(100);
}

void news_channel(){
  speaking_animation(30,0);
  select_channel();
}

void dance_channel(int x, int y){
   for (int i = 0; i < 20; i++){
    int frame = random(0,5);
    if (frame  ==  0)tv.bitmap(x,y,dance1);
    else if (frame  ==  1)tv.bitmap(x,y,dance2);
    else if (frame  ==  2)tv.bitmap(x,y,dance3);
    else if (frame  ==  3)tv.bitmap(x,y,dance4);
  
    tv.delay_frame(5);
  }
  tv.clear_screen();
  tv.delay(100);
  select_channel();
}

void animal_channel(int x, int y){
   for (int i = 0; i < 20; i++){
    int frame = random(0,5);
    if (frame  ==  0)tv.bitmap(x,y,cat1);
    else if (frame  ==  1)tv.bitmap(x,y,cat2);
    else if (frame  ==  2)tv.bitmap(x,y,cat3);
  
    tv.delay_frame(5);
  }
  tv.clear_screen();
  tv.delay(100);
  select_channel();
}