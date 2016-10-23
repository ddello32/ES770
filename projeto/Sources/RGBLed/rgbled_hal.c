/* ***************************************************************** */
/* File name:        rgbled_hal.c                                    */
/* File description: File containing the functions/methods   		 */
/*                   for rgbled control	  						     */
/* Author name:      ddello		                                     */
/* Creation date:    13maio2016                                      */
/* Revision date:    13maio2016                                      */
/* ***************************************************************** */

#include "rgbled_hal.h"
#include "GPIO/gpio_hal.h"
#include "KL25Z/es770_peripheral_board.h"

/**
 * Initialize the cooler module
 */
void rgbled_init(void){
#ifdef USERGBPWM
	/* Init cooler pin */
	SIM_SCGC5 |= SIM_SCGC5_PORTB(1u);
	SIM_SCGC5 |= SIM_SCGC5_PORTD(1u);
	PORT_PCR_REG(RGBLED_R_PORT_BASE_PNT , RGBLED_R_PIN) = PORT_PCR_MUX(RGBLED_R_PIN_MUX_ALT);
	PORT_PCR_REG(RGBLED_G_PORT_BASE_PNT , RGBLED_G_PIN) = PORT_PCR_MUX(RGBLED_G_PIN_MUX_ALT);
	PORT_PCR_REG(RGBLED_B_PORT_BASE_PNT , RGBLED_B_PIN) = PORT_PCR_MUX(RGBLED_B_PIN_MUX_ALT);

	/* INIT TPM */
	//Init TPM Clock source as OSCERCLOCK
	SIM_WR_SOPT2_TPMSRC(SIM,0x2);
	//Enable clock to TPM0
	SIM_SCGC6 |= SIM_SCGC6_TPM0(0x1);
	//Enable clock to TPM2
	SIM_SCGC6 |= SIM_SCGC6_TPM2(0x1);
	//PWM mode
	TPM_WR_SC_CPWMS(RGBLED_R_TPM_BASE_PNT, 0x0);	//Edge Aligned
	TPM_WR_SC_CPWMS(RGBLED_G_TPM_BASE_PNT, 0x0);	//Edge Aligned
	TPM_WR_SC_CPWMS(RGBLED_B_TPM_BASE_PNT, 0x0);	//Edge Aligned

	// Config R Channel mode
	/* Channel mode configuration should be done with the channel disabled and need acknowledge*/
	TPM_WR_CnSC(RGBLED_R_TPM_BASE_PNT, RGBLED_R_TPM_CHANNEL_INDEX, TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
    /* Wait till mode change is acknowledged */
    while (!
    		(
				TPM_RD_CnSC(RGBLED_R_TPM_BASE_PNT, RGBLED_R_TPM_CHANNEL_INDEX)
				& (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1))
			)
		  );
	//Config G Channel mode
	TPM_WR_CnSC(RGBLED_G_TPM_BASE_PNT, RGBLED_G_TPM_CHANNEL_INDEX, TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
    /* Wait till mode change is acknowledged */
    while (!
    		(
				TPM_RD_CnSC(RGBLED_G_TPM_BASE_PNT, RGBLED_G_TPM_CHANNEL_INDEX)
				& (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1))
			)
		  );

	//Config B Channel mode
	TPM_WR_CnSC(RGBLED_B_TPM_BASE_PNT, RGBLED_B_TPM_CHANNEL_INDEX, TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
    /* Wait till mode change is acknowledged */
    while (!
    		(
				TPM_RD_CnSC(RGBLED_B_TPM_BASE_PNT, RGBLED_B_TPM_CHANNEL_INDEX)
				& (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1))
			)
		  );
	//Same period as OSCRCLK
	TPM_WR_MOD(RGBLED_R_TPM_BASE_PNT, 0xFFFE);
	TPM_WR_MOD(RGBLED_G_TPM_BASE_PNT, 0xFFFE);
	TPM_WR_MOD(RGBLED_B_TPM_BASE_PNT, 0xFFFE);
	//Duty cycle
	TPM_WR_CnV_VAL(RGBLED_R_TPM_BASE_PNT, RGBLED_R_TPM_CHANNEL_INDEX, 0xFFFF);
	TPM_WR_CnV_VAL(RGBLED_G_TPM_BASE_PNT, RGBLED_G_TPM_CHANNEL_INDEX, 0xFFFF);
	TPM_WR_CnV_VAL(RGBLED_B_TPM_BASE_PNT, RGBLED_B_TPM_CHANNEL_INDEX, 0xFFFF);
	//Prescale 1:1
	TPM_WR_SC_PS(RGBLED_R_TPM_BASE_PNT, 0x0);
	TPM_WR_SC_PS(RGBLED_G_TPM_BASE_PNT, 0x0);
	TPM_WR_SC_PS(RGBLED_B_TPM_BASE_PNT, 0x0);
	//Select LTPM Clock Source for TPM module
	TPM_WR_SC_CMOD(RGBLED_R_TPM_BASE_PNT, 0x1);
	TPM_WR_SC_CMOD(RGBLED_G_TPM_BASE_PNT, 0x1);
	TPM_WR_SC_CMOD(RGBLED_B_TPM_BASE_PNT, 0x1);
#else
	GPIO_UNGATE_PORT(RGBLED_R_PORT_ID);
	GPIO_INIT_PIN(RGBLED_R_PORT_ID, RGBLED_R_PIN, GPIO_OUTPUT);
	GPIO_WRITE_PIN(RGBLED_R_PORT_ID, RGBLED_R_PIN, GPIO_HIGH);
	GPIO_UNGATE_PORT(RGBLED_G_PORT_ID);
	GPIO_INIT_PIN(RGBLED_G_PORT_ID, RGBLED_G_PIN, GPIO_OUTPUT);
	GPIO_WRITE_PIN(RGBLED_G_PORT_ID, RGBLED_G_PIN, GPIO_HIGH);
	GPIO_UNGATE_PORT(RGBLED_B_PORT_ID);
	GPIO_INIT_PIN(RGBLED_B_PORT_ID, RGBLED_B_PIN, GPIO_OUTPUT);
	GPIO_WRITE_PIN(RGBLED_B_PORT_ID, RGBLED_B_PIN, GPIO_HIGH);
#endif
}


/**
 * Set the rgb led color
 * @param uiR red intensity from 0 (black) to 0xFFFF (red)
 * @param uiG green intensity from 0 (black) to 0xFFFF (green)
 * @param uiB blue intensity from 0 (black) to 0xFFFF (blue)
 */
void rgbled_setColor(uint16_t uiR, uint16_t uiG, uint16_t uiB){
#ifdef USERGBPWM
	TPM_WR_CnV_VAL(RGBLED_R_TPM_BASE_PNT, RGBLED_R_TPM_CHANNEL_INDEX, 0xFFFF - uiR);
	TPM_WR_CnV_VAL(RGBLED_G_TPM_BASE_PNT, RGBLED_G_TPM_CHANNEL_INDEX, 0xFFFF - uiG);
	TPM_WR_CnV_VAL(RGBLED_B_TPM_BASE_PNT, RGBLED_B_TPM_CHANNEL_INDEX, 0xFFFF - uiB);
#else
	GPIO_WRITE_PIN(RGBLED_R_PORT_ID, RGBLED_R_PIN, uiR > 0? GPIO_LOW: GPIO_HIGH);
	GPIO_WRITE_PIN(RGBLED_G_PORT_ID, RGBLED_G_PIN, uiG > 0? GPIO_LOW: GPIO_HIGH);
	GPIO_WRITE_PIN(RGBLED_B_PORT_ID, RGBLED_B_PIN, uiB > 0? GPIO_LOW: GPIO_HIGH);
#endif
}
