#define MPCORE_PRIV_TIMERL 0xFFFEC600
#define KEY_BASE 0xFF200050
#define SW_BASE 0xFF200040
#define HEX5_HEX4_BASE 0xFF200030
#define HEX3_HEX0_BASE 0xFF200020
#define LED_BASE 0xFF200000
#include <stdio.h>


void clear();
void count();
void displayms();
void start();
void stop();
void lap();
void display_lap();

volatile int* const key_ptr = ( int* )KEY_BASE;
volatile int* const hex03_ptr = ( int* )HEX3_HEX0_BASE;
volatile int* const hex45_ptr = ( int* )HEX5_HEX4_BASE;
volatile int* const switch_ptr = ( int* )SW_BASE;
volatile int* const timer_ptr = ( int* )MPCORE_PRIV_TIMERL;					
volatile int* const status_ptr = ( int* )(MPCORE_PRIV_TIMERL + 0x0C);
volatile int* const control_ptr = ( int* )(MPCORE_PRIV_TIMERL + 0x08);
volatile int* const count_ptr = ( int* )(MPCORE_PRIV_TIMERL + 0x04);

int hex_codes[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

typedef struct
{
int load;
int count;
int	control;
int status;
} Timer;

int started=0;
int clicked_button_0=0;
int clicked_button_3=0;

int c0=0;
int c1=0;
int c2 =0; 
int c3=0;
int c4=0;
int c5=0;

int hex_lap45_old = 0;
int hex_lap03_old= 0;
int hex_03 =0;int hex_45=0;
int hex_0 = 0;
int hex_1 = 0;
int hex_2 = 0;
int hex_3 = 0;
int hex_4 = 0;
int hex_5 = 0;

int main(){
if(started == 0){
start();
started = 1;
}
if(*key_ptr==1 || (clicked_button_0 && *key_ptr==0)){
count();
}
if(*key_ptr == 2){
stop();
}
if(*key_ptr == 4){
lap();
}
if(*key_ptr == 8){
clear();
}
if(*switch_ptr == 1){
display_lap();
hex_03 = hex_lap03_old;
hex_45 = hex_lap45_old;


}

main();
}

void count(){
clicked_button_0 = 1;
int count_period = 2000000;//
*timer_ptr = count_period; //loading count_period into base address


int clear_timeout = 0b01; //clearing timeout
*status_ptr = clear_timeout;

int count_down = 0b0011;//counting down
*control_ptr = count_down;

while(*status_ptr%10 != 1){}

if(*status_ptr% 10 == 1){
	displayms();
};


}
void displayms(){

hex_0 = hex_codes[c0];
hex_1 = (hex_codes[c1]);
hex_2 = (hex_codes[c2]);
hex_3 = (hex_codes[c3]);
hex_4 = (hex_codes[c4]);
hex_5 = (hex_codes[c5]);

if(hex_codes[c0] == hex_0){
	c0++;
	*hex03_ptr = ((hex_3 << 24) | (hex_2 << 16) | (hex_1 << 8) | hex_codes[c0]);
	*hex45_ptr = ((hex_5 << 8)|(hex_4));

}

if(hex_0 == hex_codes[9]){
	c1++;
	hex_1 = hex_codes[c1];
	*hex03_ptr = ((hex_3 << 24) | (hex_2 << 16) | (hex_1 << 8) | hex_codes[0]);
	*hex45_ptr = ((hex_5 << 8)|(hex_4));
	c0=0;
}
if(hex_1 == hex_codes[10]){
	c2++;
	hex_1 = hex_codes[0];
	hex_2 = hex_codes[c2];
	*hex03_ptr = ((hex_3 << 24) | (hex_2 << 16) | (hex_1 << 8) | hex_codes[0]);
	*hex45_ptr = ((hex_5 << 8)|(hex_4));
	c1=0;
	c0=0;
}
if(hex_2 == hex_codes[10]){
	c3++;
	hex_2 = hex_codes[0];
	hex_3 = hex_codes[c3];
	*hex03_ptr = ((hex_3 << 24) | (hex_2 << 16) | (hex_1 << 8) | hex_codes[0]);//reseting to 0
	*hex45_ptr = ((hex_5 << 8)|(hex_4));
	c1=0;
	c0=0;
	c2=0;
}
if(hex_3 == hex_codes[6]){
	c4++;
	hex_3 = hex_codes[0];
	hex_4 = hex_codes[c4];
	*hex03_ptr = ((hex_3 << 24) | (hex_2 << 16) | (hex_1 << 8) | hex_codes[0]);//reseting to 0
	*hex45_ptr = ((hex_5 << 8)|(hex_4));
	c1=0;
	c0=0;
	c2=0;
	c3=0;
}
if(hex_4 == hex_codes[10]){
	c5++;
	hex_4 = hex_codes[0];
	hex_5 = hex_codes[c5];
	*hex03_ptr = ((hex_3 << 24) | (hex_2 << 16) | (hex_1 << 8) | hex_codes[0]);//reseting to 0
	*hex45_ptr = ((hex_5 << 8)|(hex_4));
	c1=0;
	c0=0;
	c2=0;
	c3=0;
	c4=0;
}
if(hex_5 == hex_codes[6]){
	hex_1 = hex_codes[0];
	hex_2 = hex_codes[0];
	hex_3 = hex_codes[0];
	hex_4 = hex_codes[0];
	hex_5 = hex_codes[0];
	*hex03_ptr = ((hex_3 << 24) | (hex_2 << 16) | (hex_1 << 8) | hex_codes[0]);//reseting to 0
	*hex45_ptr = ((hex_5 << 8)|(hex_4));
	c1=0;
	c0=0;
	c2=0;
	c3=0;
	c4=0;
	c5=0;
}
}
void stop(){
clicked_button_0 = 0;
}

void start(){
*hex03_ptr = 0x3F3F3F3F;
*hex45_ptr = 0x3F3F;
}
int hex_lap45 = 0;
int hex_lap03 =0;

void lap(){
hex_lap45= *hex45_ptr;
hex_lap03 = *hex03_ptr;
}

void display_lap(){

*hex03_ptr = hex_lap03;
*hex45_ptr = hex_lap45;


}

void clear(){

*hex03_ptr = 0x3F3F3F3F;
*hex45_ptr = 0x3F3F;
c0=c1=c2=c3=c4=c5=0;
hex_lap45 =0;
hex_lap03 =0;
}
