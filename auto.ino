#include "GyverButton.h"
#include "functii.h"

#define avarie_output 2
#define incuie_output 3
#define descuie_output 4
#define cip_output 5
#define periferice_output 6
#define panou_output 7
#define starter_output 8
#define alarma 9
#define a 10
#define b 11
#define c 12
#define d 13
#define pc4 A4
#define pc3 A3
#define pot_pin A5

bool flag_pornire = 0;
int volt;  
uint32_t secunda = 500;
uint32_t timp_incuie_descuie = 0.6 *secunda;
uint32_t minut = 60 * secunda;
uint32_t timp_panou = 4 * minut;
uint32_t timp_periferice;
uint32_t timp_per;
uint32_t timp_cip;
uint32_t timp_pot;
uint32_t timp_starter = 1 *secunda;
uint32_t timp_avarie;

GButton b_incuie(a);
GButton b_panou(b);
GButton b_descuie(c);
GButton b_starter(d);
GButton b_frina(pc4);
GButton b_usi(pc3);

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
  b_incuie.setClickTimeout(0.3 *secunda);
  b_incuie.setTimeout(4 * secunda);
  b_starter.setTimeout(4 * secunda);

  
  Serial.begin(9600);
}

void loop() {  
  b_incuie.tick();
  b_descuie.tick();
  b_panou.tick();
  b_starter.tick();
  b_frina.tick();
  b_usi.tick();
 
  incuie.out(timp_incuie_descuie);
  descuie.out(timp_incuie_descuie);
  avarie.out(timp_avarie);
  panou.out(timp_panou);
  starter.out(timp_starter);
  periferice.out(timp_periferice);
  cip.out(timp_cip);
  
  if (b_incuie.isClick()){
    Serial.println("incuie");
   incuie.out(timp_incuie_descuie, 1);
   avarie.out(timp_avarie = 2.5 *secunda,1 );
   periferice.out(0);
   cip.out(0);
 }

 if (b_incuie.isHold()){
  incuie.out(timp_incuie_descuie, 1);
  avarie.out(timp_avarie = 2.5 *secunda,1 );
  periferice.out(timp_periferice = 30 *minut, 1);
  cip.out(0);
 }

 if (b_descuie.isPress()){
   Serial.println("descuie");
   descuie.out(timp_incuie_descuie, 1);
   avarie.out(timp_avarie = 1 *secunda, 1); 
   periferice.out(timp_periferice = 30 *minut, 1);
   cip.out(timp_cip = 30 *minut, 1);
 }

 if (b_panou.isPress()){
   Serial.println("panou");
   if(!panou.out(timp_panou)){
    avarie.out(timp_avarie = 0.5 *secunda, 1);
    cip.out(timp_cip = timp_panou, 1);
    panou.out(timp_panou, 1);
   } 
   else{
    flag_pornire = 0;
    cip.out(0);
    panou.out(0);     
   } 
 }
 
 if (b_starter.isPress() && b_frina.state() && panou.out(timp_panou) && !flag_pornire){
  Serial.println("start");
  starter.out(timp_starter, 1);
  flag_pornire = 1;  

 }
 
 if (b_starter.isHold() && !panou.out(timp_panou)){
  Serial.println("hold");
  avarie.out(timp_avarie = 6 *secunda, 1);
  for (uint32_t tmr = millis(); millis() - tmr < 6 *secunda;) {
    b_incuie.tick();
    b_descuie.tick();
    b_panou.tick();
    b_starter.tick();
 
    if (b_incuie.isPress()){
      timp_panou = 4 *minut;
      Serial.println("4 min");
      avarie.out(0);
      break;
    }
    if (b_panou.isPress()){
      timp_panou = 6 *minut;
      Serial.println("6 min");
      avarie.out(0);
      break;
    }
    if (b_descuie.isPress()){
      timp_panou = 8 *minut;
      Serial.println("8 min");
      avarie.out(0);
      break;
    }
    if (b_starter.isPress()){
      avarie.out(0);
      break;
    }   
   }
  }

if (periferice.out(timp_periferice) && millis() - timp_per > 2 * secunda){
    volt = voltaj(pot_pin);
    timp_per = millis(); 
    if (volt >= 1300){
      periferice.out(timp_periferice = 30 *minut, 1);
      cip.out(timp_cip = 30 *minut, 1);
    }
  }
if (flag_pornire && !b_frina.state()){
    flag_pornire = 0;
    panou.out(0);     
   } 

if (panou.out(timp_panou) && flag_pornire && millis() - timp_pot > 0.5 * secunda){
    volt = voltaj(pot_pin);
    timp_pot = millis();
    if (volt >= 1300){
      Serial.println(volt);
      avarie.out(timp_avarie = 1.2 *secunda, 1);
    }
  }
  
 
}
