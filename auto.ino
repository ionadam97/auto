#include "GyverButton.h"
#include "functii.h"

#define a 10
#define b 11
#define c 12
#define d 13
#define pc4 A4
#define pc3 A3
#define pot_pin A5

bool flag_cip = 0;
bool flag_avarie = 0;
bool flag_starter = 0;
bool flag_panou = 0;
bool flag_periferice = 0;
bool flag_incuie = 0;
bool flag_descuie = 0;
bool flag_pornire = 0;  

int secunda = 1000;
uint16_t minut = 60 * secunda;
uint32_t timp_panou = 2 * minut;
uint32_t timp_periferice;
uint32_t timp_cip;
uint32_t timp_pot;
int timp_starter = 0.5 *secunda;
int timp_avarie;
int val;

GButton b_incuie(a);
GButton b_panou(b);
GButton b_descuie(c);
GButton b_starter(d);
GButton b_frina(pc4);
GButton b_usi(pc3);



#define avarie_output 2
#define incuie_output 3
#define descuie_output 4
#define cip_output 5
#define periferice_output 6
#define panou_output 7
#define starter_output 8
#define alarma 9

OUT cip(cip_output);
OUT starter(starter_output);
OUT avarie(avarie_output);
OUT incuie(incuie_output);
OUT descuie(descuie_output);
OUT panou(panou_output);
OUT periferice(periferice_output);
void setup() {
//  b_incuie.setType(LOW_PULL);
//  b_panou.setType(LOW_PULL);
//  b_descuie.setType(LOW_PULL);
//  b_starter.setType(LOW_PULL);
  
  

  
  Serial.begin(9600);
}

void loop() {  
  b_incuie.tick();
  b_descuie.tick();
  b_panou.tick();
  b_starter.tick();
  b_frina.tick();
  b_usi.tick();
 
  if (flag_incuie) flag_incuie = incuie.out();
  if (flag_descuie) flag_descuie = descuie.out();
  if (flag_avarie) flag_avarie = avarie.out(timp_avarie);
  if (flag_panou) flag_panou = panou.out(timp_panou);
  if (flag_starter) flag_starter = starter.out(timp_starter);
  if (flag_periferice) flag_periferice = periferice.out(timp_periferice);
  if (flag_cip) flag_cip = cip.out(timp_cip);
  
  if (b_incuie.isPress()){
    Serial.println("incuie");
   flag_incuie = incuie.out();
   flag_avarie = avarie.out(timp_avarie = 1.5 *secunda,1 );
   flag_periferice = periferice.out(0);
   flag_cip = cip.out(0);
 }

 if (b_incuie.isDouble()){
  flag_incuie = incuie.out();
  flag_avarie = avarie.out(timp_avarie = 1.5 *secunda,1 );
  flag_periferice = periferice.out(timp_periferice = 30 *minut, 1);
  flag_cip = cip.out(0);
 }

 if (b_descuie.isPress()){
   Serial.println("descuie");
   flag_descuie = descuie.out();
   flag_avarie = avarie.out(timp_avarie = 1 *secunda, 1); 
   flag_periferice = periferice.out(timp_periferice = 30 *minut, 1);
   flag_cip = cip.out(timp_cip = 30 *minut, 1);
 }

 if (b_panou.isPress()){
   Serial.println("panou");
   if(!flag_panou){
    flag_avarie = avarie.out(timp_avarie = 0.5 *secunda, 1);
    flag_cip = cip.out(timp_cip = timp_panou, 1);
    flag_panou = panou.out(timp_panou);
   } 
   else{
    flag_pornire = 0;
    flag_cip = cip.out(0);
    flag_panou = panou.out(0);     
   } 
}
 
 if (b_starter.isPress()&& flag_panou && !flag_pornire && b_frina.state()){
   Serial.println("start");
  flag_starter = starter.out(timp_starter);
  flag_pornire = 1;  

 }
 
 if (b_starter.isHold() && !flag_panou){
  Serial.println("hold");
  flag_avarie = avarie.out(timp_avarie = 6 *secunda, 1);
  for (uint32_t tmr = millis(); millis() - tmr < 6 *secunda;) {
    b_incuie.tick();
    b_descuie.tick();
    b_panou.tick();
    b_starter.tick();
 
    if (b_incuie.isPress()){
      timp_panou = 4 *minut;
      Serial.println("4 min");
      flag_avarie = avarie.out(0);
      break;
    }
    if (b_panou.isPress()){
      timp_panou = 6 *minut;
      Serial.println("6 min");
      flag_avarie = avarie.out(0);
      break;
    }
    if (b_descuie.isPress()){
      timp_panou = 8 *minut;
      Serial.println("8 min");
      flag_avarie = avarie.out(0);
      break;
    }
    if (b_starter.isPress()){
      flag_avarie = avarie.out(0);
      break;
    }   
   }
  }
  
if (flag_panou && flag_pornire && millis() - timp_pot > 0.5 * secunda){
    val = voltaj(pot_pin);
    timp_pot = millis();
    if (val >= 1350){
      Serial.println(val);
      flag_avarie = avarie.out(timp_avarie = 1.2 *secunda, 1);
    }
  }
  

  



 
}
