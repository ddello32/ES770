#include "KL25Z/es770_peripheral_board.h"
#include "Mcg/mcg_hal.h"
#include "PIT/pit_hal.h"
#include "Util/util.h"
#include "Serial/serial_hal.h"
#include "Protocolo/cmdmachine_hal.h"
#include "Util/tc_hal.h"
#include "RGBLed/rgbled_hal.h"
#include "PhotoSensor/photosensor_hal.h"
#include "AutoTest/autotest.h"
#include "Motor/motor_hal.h"
#include "Encoder/encoder_hal.h"
#include "LineSensor/linesensor.h"
#include "SpeedControl/speedController.h"
#include "Protocolo/cmdmachine_hal.h"
#include <string.h>
#include <stdio.h>

/* defines */
#define CYCLIC_EXECUTIVE_PERIOD         100*1000 /* in micro seconds */
/* globals */
volatile unsigned int uiFlagNextPeriod = 0;         /* cyclic executive flag */
static unsigned int kp[2] = {50,50};
static unsigned int ki[2] = {2,2};
static unsigned int kd[2] = {0,0};
static int ls = 50;
static int as = 0;

#define RCV_BUF_SIZE 40
#define SND_BUF_SIZE 40

/**
 * cyclic executive interrupt service routine
 */
void main_cyclicExecuteIsr(void)
{
    /* set the cyclic executive flag */
    uiFlagNextPeriod = 1;
}


/**
 * Initialize board and periferics
 */
void main_boardInit(){
	mcg_clockInit();
	serial_initUart();
	rgbled_init();
	photoSensor_init();
	motor_init();
	encoder_init();
	lineSensor_init();
	speedControl_init(kp, ki, kd, CYCLIC_EXECUTIVE_PERIOD);
	//TODO
}

void main_setSpeeds(int iLinSpeed, int iAngSpeed){
	ls = iLinSpeed;
	as = iAngSpeed;
}

void main_setControler(unsigned short motor, int ikp, int iki, int ikd){
	kp[motor] = ikp;
	ki[motor] = iki;
	kd[motor] = ikd;
	speedControl_init(kp, ki, kd, CYCLIC_EXECUTIVE_PERIOD);
}

/**
 * Reads serial comm, interprets received commands and prints output
 */
void main_protocolCheck(){
	static char rcvBuffer[RCV_BUF_SIZE];
	static char sndBuffer[SND_BUF_SIZE];
	if(UART0_BRD_S1_RDRF(UART0)){
		int iCmdSize = 0;
		iCmdSize = serial_recieveBuffer(rcvBuffer, RCV_BUF_SIZE);
		if(iCmdSize > 0){
			cmdmachine_interpretCmdBuffer(rcvBuffer, iCmdSize, sndBuffer);
			serial_sendBuffer(sndBuffer, strlen(sndBuffer));
		}
	}
}

/**
 * Main function
 */
int main(void) {
	int count = 0;
	int distance;
	char buff[100];
	main_boardInit();
	autotest_testAndCalibrate();
	util_genDelay1s();

	//Fill measure buffer
	encoder_measure();
	encoder_measure();
	encoder_measure();
	encoder_measure();


    /* configure cyclic executive interruption */
    tc_installLptmr0(CYCLIC_EXECUTIVE_PERIOD, main_cyclicExecuteIsr);
    /* cooperative cyclic executive main loop */
	while(1){
		encoder_measure();
		speedControl_execute(ls, as);
		sprintf(buff, "Motor 0: %u MOTOR 1: %u\n", encoder_getMeanSpeed(0, CYCLIC_EXECUTIVE_PERIOD), encoder_getMeanSpeed(1, CYCLIC_EXECUTIVE_PERIOD));
		serial_sendBuffer(buff, strlen(buff));
//		distance = lineSensor_measure();
//		if(distance < -15){
//			motor_setSpeed(0, 0x9999);
//			motor_setSpeed(1, -0x7777);
//		}else if(distance > 15){
//			motor_setSpeed(1, 0x9999);
//			motor_setSpeed(0, -0x7777);
//		}else{
//			motor_setSpeed(0, 0x7777);
//			motor_setSpeed(1, 0x7777);
//		}
//		sprintf(buff, "Distance: %d\n", distance);
//		serial_sendBuffer(buff, strlen(buff));
		while(!uiFlagNextPeriod){
			main_protocolCheck();
		}
		uiFlagNextPeriod = 0;
	}
    /* Never leave main */
    return 0;
}
