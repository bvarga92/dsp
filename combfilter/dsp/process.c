/*********************************************************************/
/*                                                                   */
/* Univerzalis fesuszuro kesleltetoeffektekhez     Varga Balazs 2015 */
/*                                                                   */
/* A sztereo bemenetet osszeadja, megeffektezi, es kiteszi a         */
/* 4-es kimenetre.                                                   */
/*                                                                   */
/* Beallithato parameterek:                                          */
/*      M  - a kesleltetovonal hossza (M/48 a kesl. ido ms-ban)      */
/*      FF - feedforward                                             */
/*      FB - feedback                                                */
/*      BL - blend                                                   */
/*                                                                   */
/*********************************************************************/

#include "bsp/ad1835.h"

#pragma optimize_for_speed

char currentEffect;
float delayLine[9600]; //ez maga a kesleltetovonal
int ptr=9599; //mindig a legregebbi mintara mutat

/* itt tortenik a lenyegi feldolgozas (futasido merheto a DAIP15 labon) */
void process(){
	unsigned i, M;
	float in, out, temp, FF, FB, BL;
	in=0.5*(leftIn+rightIn);

	if(currentEffect==0){ //nincs effekt
		rightOut4=leftOut4=in;
		return;
	}
	else if(currentEffect==1){ //echo
		M=9600;
		FF=0;
		FB=0.2;
		BL=1;
	}
	else{ //slapback
		M=2400;
		FF=0.5;
		FB=0;
		BL=1;
	}
	temp=delayLine[ptr];
	delayLine[ptr]=in+FB*temp;
	out=FF*temp+BL*delayLine[ptr];
	ptr=ptr?(ptr-1):(M-1);

	rightOut4=leftOut4=out;
}
