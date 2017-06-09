#include "bsp/ad1835.h"
#include "fdnParamsBigHall.h"
#include "fdnParamsSmallRoom.h"

#pragma optimize_for_speed

unsigned char wet; //hozzakeveresi arany szazalekban
unsigned char currentEffect; //nagy terem (0) vagy kis szoba (1)

void computeBigHall(bool zero){
	static float dl1[M1_1], dl2[M1_2], dl3[M1_3], dl4[M1_4], dl5[M1_5], dl6[M1_6], dl7[M1_7], dl8[M1_8], dlL[M1_L], dlR[M1_R], temp[8];
	static unsigned ptr[10]={M1_1-1, M1_2-1, M1_3-1, M1_4-1, M1_5-1, M1_6-1, M1_7-1, M1_8-1, M1_L-1, M1_R-1};
	if(zero){
		dl1[ptr[0]]=dl2[ptr[1]]=dl3[ptr[2]]=dl4[ptr[3]]=dl5[ptr[4]]=dl6[ptr[5]]=dl7[ptr[6]]=dl8[ptr[7]]=0;
	}
	else{
		/* olvasas a kesleltelovonalak "vegerol", es a szurok lefuttatasa */
		temp[0]=P1[0]*temp[0]+G1[0]*(1.0-P1[0])*dl1[ptr[0]];
		temp[1]=P1[1]*temp[1]+G1[1]*(1.0-P1[1])*dl2[ptr[1]];
		temp[2]=P1[2]*temp[2]+G1[2]*(1.0-P1[2])*dl3[ptr[2]];
		temp[3]=P1[3]*temp[3]+G1[3]*(1.0-P1[3])*dl4[ptr[3]];
		temp[4]=P1[4]*temp[4]+G1[4]*(1.0-P1[4])*dl5[ptr[4]];
		temp[5]=P1[5]*temp[5]+G1[5]*(1.0-P1[5])*dl6[ptr[5]];
		temp[6]=P1[6]*temp[6]+G1[6]*(1.0-P1[6])*dl7[ptr[6]];
		temp[7]=P1[7]*temp[7]+G1[7]*(1.0-P1[7])*dl8[ptr[7]];
		/* iras a kesleltetovonalak "elejere" */
		dl1[ptr[0]]=B1L[0]*leftIn+B1R[0]*rightIn+A1[0][0]*temp[0]+A1[0][1]*temp[1]+A1[0][2]*temp[2]+A1[0][3]*temp[3]+A1[0][4]*temp[4]+A1[0][5]*temp[5]+A1[0][6]*temp[6]+A1[0][7]*temp[7];
		dl2[ptr[1]]=B1L[1]*leftIn+B1R[1]*rightIn+A1[1][0]*temp[0]+A1[1][1]*temp[1]+A1[1][2]*temp[2]+A1[1][3]*temp[3]+A1[1][4]*temp[4]+A1[1][5]*temp[5]+A1[1][6]*temp[6]+A1[1][7]*temp[7];
		dl3[ptr[2]]=B1L[2]*leftIn+B1R[2]*rightIn+A1[2][0]*temp[0]+A1[2][1]*temp[1]+A1[2][2]*temp[2]+A1[2][3]*temp[3]+A1[2][4]*temp[4]+A1[2][5]*temp[5]+A1[2][6]*temp[6]+A1[2][7]*temp[7];
		dl4[ptr[3]]=B1L[3]*leftIn+B1R[3]*rightIn+A1[3][0]*temp[0]+A1[3][1]*temp[1]+A1[3][2]*temp[2]+A1[3][3]*temp[3]+A1[3][4]*temp[4]+A1[3][5]*temp[5]+A1[3][6]*temp[6]+A1[3][7]*temp[7];
		dl5[ptr[4]]=B1L[4]*leftIn+B1R[4]*rightIn+A1[4][0]*temp[0]+A1[4][1]*temp[1]+A1[4][2]*temp[2]+A1[4][3]*temp[3]+A1[4][4]*temp[4]+A1[4][5]*temp[5]+A1[4][6]*temp[6]+A1[4][7]*temp[7];
		dl6[ptr[5]]=B1L[5]*leftIn+B1R[5]*rightIn+A1[5][0]*temp[0]+A1[5][1]*temp[1]+A1[5][2]*temp[2]+A1[5][3]*temp[3]+A1[5][4]*temp[4]+A1[5][5]*temp[5]+A1[5][6]*temp[6]+A1[5][7]*temp[7];
		dl7[ptr[6]]=B1L[6]*leftIn+B1R[6]*rightIn+A1[6][0]*temp[0]+A1[6][1]*temp[1]+A1[6][2]*temp[2]+A1[6][3]*temp[3]+A1[6][4]*temp[4]+A1[6][5]*temp[5]+A1[6][6]*temp[6]+A1[6][7]*temp[7];
		dl8[ptr[7]]=B1L[7]*leftIn+B1R[7]*rightIn+A1[7][0]*temp[0]+A1[7][1]*temp[1]+A1[7][2]*temp[2]+A1[7][3]*temp[3]+A1[7][4]*temp[4]+A1[7][5]*temp[5]+A1[7][6]*temp[6]+A1[7][7]*temp[7];
	}
	/* a pointerek leptetese */
	ptr[0]=ptr[0]?(ptr[0]-1):(M1_1-1);
	ptr[1]=ptr[1]?(ptr[1]-1):(M1_2-1);
	ptr[2]=ptr[2]?(ptr[2]-1):(M1_3-1);
	ptr[3]=ptr[3]?(ptr[3]-1):(M1_4-1);
	ptr[4]=ptr[4]?(ptr[4]-1):(M1_5-1);
	ptr[5]=ptr[5]?(ptr[5]-1):(M1_6-1);
	ptr[6]=ptr[6]?(ptr[6]-1):(M1_7-1);
	ptr[7]=ptr[7]?(ptr[7]-1):(M1_8-1);
	/* a kimenet osszeallitasa */
	if(zero) return;
	leftOut4=dlL[ptr[8]]+C1L[0]*temp[0]+C1L[1]*temp[1]+C1L[2]*temp[2]+C1L[3]*temp[3]+C1L[4]*temp[4]+C1L[5]*temp[5]+C1L[6]*temp[6]+C1L[7]*temp[7];
	leftOut4=0.01*wet*leftOut4+(1.0-0.01*wet)*leftIn;
	rightOut4=dlR[ptr[9]]+C1R[0]*temp[0]+C1R[1]*temp[1]+C1R[2]*temp[2]+C1R[3]*temp[3]+C1R[4]*temp[4]+C1R[5]*temp[5]+C1R[6]*temp[6]+C1R[7]*temp[7];
	rightOut4=0.01*wet*rightOut4+(1.0-0.01*wet)*rightIn;
	dlL[ptr[8]]=D1L*leftIn;
	dlR[ptr[9]]=D1R*rightIn;
	ptr[8]=ptr[8]?(ptr[8]-1):(M1_L-1);
	ptr[9]=ptr[9]?(ptr[9]-1):(M1_R-1);
}

void computeSmallRoom(bool zero){
	static float dl1[M2_1], dl2[M2_2], dl3[M2_3], dl4[M2_4], dl5[M2_5], dl6[M2_6], dl7[M2_7], dl8[M2_8], dl9[M2_9], dl10[M2_10], dl11[M2_11], dl12[M2_12], dl13[M2_13], dl14[M2_14], dl15[M2_15], dl16[M2_16], dl17[M2_17], dl18[M2_18], dl19[M2_19], dl20[M2_20], dl21[M2_21], dl22[M2_22], dl23[M2_23], dl24[M2_24], dlL[M2_L], dlR[M2_R], temp[24];
	static unsigned ptr[26]={M2_1-1, M2_2-1, M2_3-1, M2_4-1, M2_5-1, M2_6-1, M2_7-1, M2_8-1, M2_9-1, M2_10-1, M2_11-1, M2_12-1, M2_13-1, M2_14-1, M2_15-1, M2_16-1, M2_17-1, M2_18-1, M2_19-1, M2_20-1, M2_21-1, M2_22-1, M2_23-1, M2_24-1, M2_L, M2_R};
	if(zero){
		dl1[ptr[0]]=dl2[ptr[1]]=dl3[ptr[2]]=dl4[ptr[3]]=dl5[ptr[4]]=dl6[ptr[5]]=dl7[ptr[6]]=dl8[ptr[7]]=dl9[ptr[8]]=dl10[ptr[9]]=dl11[ptr[10]]=dl12[ptr[11]]=dl13[ptr[12]]=dl14[ptr[13]]=dl15[ptr[14]]=dl16[ptr[15]]=dl17[ptr[16]]=dl18[ptr[17]]=dl19[ptr[18]]=dl20[ptr[19]]=dl21[ptr[20]]=dl22[ptr[21]]=dl23[ptr[22]]=dl24[ptr[23]]=0;
	}
	else{
		/* olvasas a kesleltelovonalak "vegerol", es a szurok lefuttatasa */
		temp[0]=P2[0]*temp[0]+G2[0]*(1.0-P2[0])*dl1[ptr[0]];
		temp[1]=P2[1]*temp[1]+G2[1]*(1.0-P2[1])*dl2[ptr[1]];
		temp[2]=P2[2]*temp[2]+G2[2]*(1.0-P2[2])*dl3[ptr[2]];
		temp[3]=P2[3]*temp[3]+G2[3]*(1.0-P2[3])*dl4[ptr[3]];
		temp[4]=P2[4]*temp[4]+G2[4]*(1.0-P2[4])*dl5[ptr[4]];
		temp[5]=P2[5]*temp[5]+G2[5]*(1.0-P2[5])*dl6[ptr[5]];
		temp[6]=P2[6]*temp[6]+G2[6]*(1.0-P2[6])*dl7[ptr[6]];
		temp[7]=P2[7]*temp[7]+G2[7]*(1.0-P2[7])*dl8[ptr[7]];
		temp[8]=P2[8]*temp[8]+G2[8]*(1.0-P2[8])*dl9[ptr[8]];
		temp[9]=P2[9]*temp[9]+G2[9]*(1.0-P2[9])*dl10[ptr[9]];
		temp[10]=P2[10]*temp[10]+G2[10]*(1.0-P2[10])*dl11[ptr[10]];
		temp[11]=P2[11]*temp[11]+G2[11]*(1.0-P2[11])*dl12[ptr[11]];
		temp[12]=P2[12]*temp[12]+G2[12]*(1.0-P2[12])*dl13[ptr[12]];
		temp[13]=P2[13]*temp[13]+G2[13]*(1.0-P2[13])*dl14[ptr[13]];
		temp[14]=P2[14]*temp[14]+G2[14]*(1.0-P2[14])*dl15[ptr[14]];
		temp[15]=P2[15]*temp[15]+G2[15]*(1.0-P2[15])*dl16[ptr[15]];
		temp[16]=P2[16]*temp[16]+G2[16]*(1.0-P2[16])*dl17[ptr[16]];
		temp[17]=P2[17]*temp[17]+G2[17]*(1.0-P2[17])*dl18[ptr[17]];
		temp[18]=P2[18]*temp[18]+G2[18]*(1.0-P2[18])*dl19[ptr[18]];
		temp[19]=P2[19]*temp[19]+G2[19]*(1.0-P2[19])*dl20[ptr[19]];
		temp[20]=P2[20]*temp[20]+G2[20]*(1.0-P2[20])*dl21[ptr[20]];
		temp[21]=P2[21]*temp[21]+G2[21]*(1.0-P2[21])*dl22[ptr[21]];
		temp[22]=P2[22]*temp[22]+G2[22]*(1.0-P2[22])*dl23[ptr[22]];
		temp[23]=P2[23]*temp[23]+G2[23]*(1.0-P2[23])*dl24[ptr[23]];
		/* iras a kesleltetovonalak "elejere" */
		dl1[ptr[0]]=B2L[0]*leftIn+B2R[0]*rightIn+A2[0][0]*temp[0]+A2[0][1]*temp[1]+A2[0][2]*temp[2]+A2[0][3]*temp[3]+A2[0][4]*temp[4]+A2[0][5]*temp[5]+A2[0][6]*temp[6]+A2[0][7]*temp[7]+A2[0][8]*temp[8]+A2[0][9]*temp[9]+A2[0][10]*temp[10]+A2[0][11]*temp[11]+A2[0][12]*temp[12]+A2[0][13]*temp[13]+A2[0][14]*temp[14]+A2[0][15]*temp[15]+A2[0][16]*temp[16]+A2[0][17]*temp[17]+A2[0][18]*temp[18]+A2[0][19]*temp[19]+A2[0][20]*temp[20]+A2[0][21]*temp[21]+A2[0][22]*temp[22]+A2[0][23]*temp[23];
		dl2[ptr[1]]=B2L[1]*leftIn+B2R[1]*rightIn+A2[1][0]*temp[0]+A2[1][1]*temp[1]+A2[1][2]*temp[2]+A2[1][3]*temp[3]+A2[1][4]*temp[4]+A2[1][5]*temp[5]+A2[1][6]*temp[6]+A2[1][7]*temp[7]+A2[1][8]*temp[8]+A2[1][9]*temp[9]+A2[1][10]*temp[10]+A2[1][11]*temp[11]+A2[1][12]*temp[12]+A2[1][13]*temp[13]+A2[1][14]*temp[14]+A2[1][15]*temp[15]+A2[1][16]*temp[16]+A2[1][17]*temp[17]+A2[1][18]*temp[18]+A2[1][19]*temp[19]+A2[1][20]*temp[20]+A2[1][21]*temp[21]+A2[1][22]*temp[22]+A2[1][23]*temp[23];
		dl3[ptr[2]]=B2L[2]*leftIn+B2R[2]*rightIn+A2[2][0]*temp[0]+A2[2][1]*temp[1]+A2[2][2]*temp[2]+A2[2][3]*temp[3]+A2[2][4]*temp[4]+A2[2][5]*temp[5]+A2[2][6]*temp[6]+A2[2][7]*temp[7]+A2[2][8]*temp[8]+A2[2][9]*temp[9]+A2[2][10]*temp[10]+A2[2][11]*temp[11]+A2[2][12]*temp[12]+A2[2][13]*temp[13]+A2[2][14]*temp[14]+A2[2][15]*temp[15]+A2[2][16]*temp[16]+A2[2][17]*temp[17]+A2[2][18]*temp[18]+A2[2][19]*temp[19]+A2[2][20]*temp[20]+A2[2][21]*temp[21]+A2[2][22]*temp[22]+A2[2][23]*temp[23];
		dl4[ptr[3]]=B2L[3]*leftIn+B2R[3]*rightIn+A2[3][0]*temp[0]+A2[3][1]*temp[1]+A2[3][2]*temp[2]+A2[3][3]*temp[3]+A2[3][4]*temp[4]+A2[3][5]*temp[5]+A2[3][6]*temp[6]+A2[3][7]*temp[7]+A2[3][8]*temp[8]+A2[3][9]*temp[9]+A2[3][10]*temp[10]+A2[3][11]*temp[11]+A2[3][12]*temp[12]+A2[3][13]*temp[13]+A2[3][14]*temp[14]+A2[3][15]*temp[15]+A2[3][16]*temp[16]+A2[3][17]*temp[17]+A2[3][18]*temp[18]+A2[3][19]*temp[19]+A2[3][20]*temp[20]+A2[3][21]*temp[21]+A2[3][22]*temp[22]+A2[3][23]*temp[23];
		dl5[ptr[4]]=B2L[4]*leftIn+B2R[4]*rightIn+A2[4][0]*temp[0]+A2[4][1]*temp[1]+A2[4][2]*temp[2]+A2[4][3]*temp[3]+A2[4][4]*temp[4]+A2[4][5]*temp[5]+A2[4][6]*temp[6]+A2[4][7]*temp[7]+A2[4][8]*temp[8]+A2[4][9]*temp[9]+A2[4][10]*temp[10]+A2[4][11]*temp[11]+A2[4][12]*temp[12]+A2[4][13]*temp[13]+A2[4][14]*temp[14]+A2[4][15]*temp[15]+A2[4][16]*temp[16]+A2[4][17]*temp[17]+A2[4][18]*temp[18]+A2[4][19]*temp[19]+A2[4][20]*temp[20]+A2[4][21]*temp[21]+A2[4][22]*temp[22]+A2[4][23]*temp[23];
		dl6[ptr[5]]=B2L[5]*leftIn+B2R[5]*rightIn+A2[5][0]*temp[0]+A2[5][1]*temp[1]+A2[5][2]*temp[2]+A2[5][3]*temp[3]+A2[5][4]*temp[4]+A2[5][5]*temp[5]+A2[5][6]*temp[6]+A2[5][7]*temp[7]+A2[5][8]*temp[8]+A2[5][9]*temp[9]+A2[5][10]*temp[10]+A2[5][11]*temp[11]+A2[5][12]*temp[12]+A2[5][13]*temp[13]+A2[5][14]*temp[14]+A2[5][15]*temp[15]+A2[5][16]*temp[16]+A2[5][17]*temp[17]+A2[5][18]*temp[18]+A2[5][19]*temp[19]+A2[5][20]*temp[20]+A2[5][21]*temp[21]+A2[5][22]*temp[22]+A2[5][23]*temp[23];
		dl7[ptr[6]]=B2L[6]*leftIn+B2R[6]*rightIn+A2[6][0]*temp[0]+A2[6][1]*temp[1]+A2[6][2]*temp[2]+A2[6][3]*temp[3]+A2[6][4]*temp[4]+A2[6][5]*temp[5]+A2[6][6]*temp[6]+A2[6][7]*temp[7]+A2[6][8]*temp[8]+A2[6][9]*temp[9]+A2[6][10]*temp[10]+A2[6][11]*temp[11]+A2[6][12]*temp[12]+A2[6][13]*temp[13]+A2[6][14]*temp[14]+A2[6][15]*temp[15]+A2[6][16]*temp[16]+A2[6][17]*temp[17]+A2[6][18]*temp[18]+A2[6][19]*temp[19]+A2[6][20]*temp[20]+A2[6][21]*temp[21]+A2[6][22]*temp[22]+A2[6][23]*temp[23];
		dl8[ptr[7]]=B2L[7]*leftIn+B2R[7]*rightIn+A2[7][0]*temp[0]+A2[7][1]*temp[1]+A2[7][2]*temp[2]+A2[7][3]*temp[3]+A2[7][4]*temp[4]+A2[7][5]*temp[5]+A2[7][6]*temp[6]+A2[7][7]*temp[7]+A2[7][8]*temp[8]+A2[7][9]*temp[9]+A2[7][10]*temp[10]+A2[7][11]*temp[11]+A2[7][12]*temp[12]+A2[7][13]*temp[13]+A2[7][14]*temp[14]+A2[7][15]*temp[15]+A2[7][16]*temp[16]+A2[7][17]*temp[17]+A2[7][18]*temp[18]+A2[7][19]*temp[19]+A2[7][20]*temp[20]+A2[7][21]*temp[21]+A2[7][22]*temp[22]+A2[7][23]*temp[23];
		dl9[ptr[8]]=B2L[8]*leftIn+B2R[8]*rightIn+A2[8][0]*temp[0]+A2[8][1]*temp[1]+A2[8][2]*temp[2]+A2[8][3]*temp[3]+A2[8][4]*temp[4]+A2[8][5]*temp[5]+A2[8][6]*temp[6]+A2[8][7]*temp[7]+A2[8][8]*temp[8]+A2[8][9]*temp[9]+A2[8][10]*temp[10]+A2[8][11]*temp[11]+A2[8][12]*temp[12]+A2[8][13]*temp[13]+A2[8][14]*temp[14]+A2[8][15]*temp[15]+A2[8][16]*temp[16]+A2[8][17]*temp[17]+A2[8][18]*temp[18]+A2[8][19]*temp[19]+A2[8][20]*temp[20]+A2[8][21]*temp[21]+A2[8][22]*temp[22]+A2[8][23]*temp[23];
		dl10[ptr[9]]=B2L[9]*leftIn+B2R[9]*rightIn+A2[9][0]*temp[0]+A2[9][1]*temp[1]+A2[9][2]*temp[2]+A2[9][3]*temp[3]+A2[9][4]*temp[4]+A2[9][5]*temp[5]+A2[9][6]*temp[6]+A2[9][7]*temp[7]+A2[9][8]*temp[8]+A2[9][9]*temp[9]+A2[9][10]*temp[10]+A2[9][11]*temp[11]+A2[9][12]*temp[12]+A2[9][13]*temp[13]+A2[9][14]*temp[14]+A2[9][15]*temp[15]+A2[9][16]*temp[16]+A2[9][17]*temp[17]+A2[9][18]*temp[18]+A2[9][19]*temp[19]+A2[9][20]*temp[20]+A2[9][21]*temp[21]+A2[9][22]*temp[22]+A2[9][23]*temp[23];
		dl11[ptr[10]]=B2L[10]*leftIn+B2R[10]*rightIn+A2[10][0]*temp[0]+A2[10][1]*temp[1]+A2[10][2]*temp[2]+A2[10][3]*temp[3]+A2[10][4]*temp[4]+A2[10][5]*temp[5]+A2[10][6]*temp[6]+A2[10][7]*temp[7]+A2[10][8]*temp[8]+A2[10][9]*temp[9]+A2[10][10]*temp[10]+A2[10][11]*temp[11]+A2[10][12]*temp[12]+A2[10][13]*temp[13]+A2[10][14]*temp[14]+A2[10][15]*temp[15]+A2[10][16]*temp[16]+A2[10][17]*temp[17]+A2[10][18]*temp[18]+A2[10][19]*temp[19]+A2[10][20]*temp[20]+A2[10][21]*temp[21]+A2[10][22]*temp[22]+A2[10][23]*temp[23];
		dl12[ptr[11]]=B2L[11]*leftIn+B2R[11]*rightIn+A2[11][0]*temp[0]+A2[11][1]*temp[1]+A2[11][2]*temp[2]+A2[11][3]*temp[3]+A2[11][4]*temp[4]+A2[11][5]*temp[5]+A2[11][6]*temp[6]+A2[11][7]*temp[7]+A2[11][8]*temp[8]+A2[11][9]*temp[9]+A2[11][10]*temp[10]+A2[11][11]*temp[11]+A2[11][12]*temp[12]+A2[11][13]*temp[13]+A2[11][14]*temp[14]+A2[11][15]*temp[15]+A2[11][16]*temp[16]+A2[11][17]*temp[17]+A2[11][18]*temp[18]+A2[11][19]*temp[19]+A2[11][20]*temp[20]+A2[11][21]*temp[21]+A2[11][22]*temp[22]+A2[11][23]*temp[23];
		dl13[ptr[12]]=B2L[12]*leftIn+B2R[12]*rightIn+A2[12][0]*temp[0]+A2[12][1]*temp[1]+A2[12][2]*temp[2]+A2[12][3]*temp[3]+A2[12][4]*temp[4]+A2[12][5]*temp[5]+A2[12][6]*temp[6]+A2[12][7]*temp[7]+A2[12][8]*temp[8]+A2[12][9]*temp[9]+A2[12][10]*temp[10]+A2[12][11]*temp[11]+A2[12][12]*temp[12]+A2[12][13]*temp[13]+A2[12][14]*temp[14]+A2[12][15]*temp[15]+A2[12][16]*temp[16]+A2[12][17]*temp[17]+A2[12][18]*temp[18]+A2[12][19]*temp[19]+A2[12][20]*temp[20]+A2[12][21]*temp[21]+A2[12][22]*temp[22]+A2[12][23]*temp[23];
		dl14[ptr[13]]=B2L[13]*leftIn+B2R[13]*rightIn+A2[13][0]*temp[0]+A2[13][1]*temp[1]+A2[13][2]*temp[2]+A2[13][3]*temp[3]+A2[13][4]*temp[4]+A2[13][5]*temp[5]+A2[13][6]*temp[6]+A2[13][7]*temp[7]+A2[13][8]*temp[8]+A2[13][9]*temp[9]+A2[13][10]*temp[10]+A2[13][11]*temp[11]+A2[13][12]*temp[12]+A2[13][13]*temp[13]+A2[13][14]*temp[14]+A2[13][15]*temp[15]+A2[13][16]*temp[16]+A2[13][17]*temp[17]+A2[13][18]*temp[18]+A2[13][19]*temp[19]+A2[13][20]*temp[20]+A2[13][21]*temp[21]+A2[13][22]*temp[22]+A2[13][23]*temp[23];
		dl15[ptr[14]]=B2L[14]*leftIn+B2R[14]*rightIn+A2[14][0]*temp[0]+A2[14][1]*temp[1]+A2[14][2]*temp[2]+A2[14][3]*temp[3]+A2[14][4]*temp[4]+A2[14][5]*temp[5]+A2[14][6]*temp[6]+A2[14][7]*temp[7]+A2[14][8]*temp[8]+A2[14][9]*temp[9]+A2[14][10]*temp[10]+A2[14][11]*temp[11]+A2[14][12]*temp[12]+A2[14][13]*temp[13]+A2[14][14]*temp[14]+A2[14][15]*temp[15]+A2[14][16]*temp[16]+A2[14][17]*temp[17]+A2[14][18]*temp[18]+A2[14][19]*temp[19]+A2[14][20]*temp[20]+A2[14][21]*temp[21]+A2[14][22]*temp[22]+A2[14][23]*temp[23];
		dl16[ptr[15]]=B2L[15]*leftIn+B2R[15]*rightIn+A2[15][0]*temp[0]+A2[15][1]*temp[1]+A2[15][2]*temp[2]+A2[15][3]*temp[3]+A2[15][4]*temp[4]+A2[15][5]*temp[5]+A2[15][6]*temp[6]+A2[15][7]*temp[7]+A2[15][8]*temp[8]+A2[15][9]*temp[9]+A2[15][10]*temp[10]+A2[15][11]*temp[11]+A2[15][12]*temp[12]+A2[15][13]*temp[13]+A2[15][14]*temp[14]+A2[15][15]*temp[15]+A2[15][16]*temp[16]+A2[15][17]*temp[17]+A2[15][18]*temp[18]+A2[15][19]*temp[19]+A2[15][20]*temp[20]+A2[15][21]*temp[21]+A2[15][22]*temp[22]+A2[15][23]*temp[23];
		dl17[ptr[16]]=B2L[16]*leftIn+B2R[16]*rightIn+A2[16][0]*temp[0]+A2[16][1]*temp[1]+A2[16][2]*temp[2]+A2[16][3]*temp[3]+A2[16][4]*temp[4]+A2[16][5]*temp[5]+A2[16][6]*temp[6]+A2[16][7]*temp[7]+A2[16][8]*temp[8]+A2[16][9]*temp[9]+A2[16][10]*temp[10]+A2[16][11]*temp[11]+A2[16][12]*temp[12]+A2[16][13]*temp[13]+A2[16][14]*temp[14]+A2[16][15]*temp[15]+A2[16][16]*temp[16]+A2[16][17]*temp[17]+A2[16][18]*temp[18]+A2[16][19]*temp[19]+A2[16][20]*temp[20]+A2[16][21]*temp[21]+A2[16][22]*temp[22]+A2[16][23]*temp[23];
		dl18[ptr[17]]=B2L[17]*leftIn+B2R[17]*rightIn+A2[17][0]*temp[0]+A2[17][1]*temp[1]+A2[17][2]*temp[2]+A2[17][3]*temp[3]+A2[17][4]*temp[4]+A2[17][5]*temp[5]+A2[17][6]*temp[6]+A2[17][7]*temp[7]+A2[17][8]*temp[8]+A2[17][9]*temp[9]+A2[17][10]*temp[10]+A2[17][11]*temp[11]+A2[17][12]*temp[12]+A2[17][13]*temp[13]+A2[17][14]*temp[14]+A2[17][15]*temp[15]+A2[17][16]*temp[16]+A2[17][17]*temp[17]+A2[17][18]*temp[18]+A2[17][19]*temp[19]+A2[17][20]*temp[20]+A2[17][21]*temp[21]+A2[17][22]*temp[22]+A2[17][23]*temp[23];
		dl19[ptr[18]]=B2L[18]*leftIn+B2R[18]*rightIn+A2[18][0]*temp[0]+A2[18][1]*temp[1]+A2[18][2]*temp[2]+A2[18][3]*temp[3]+A2[18][4]*temp[4]+A2[18][5]*temp[5]+A2[18][6]*temp[6]+A2[18][7]*temp[7]+A2[18][8]*temp[8]+A2[18][9]*temp[9]+A2[18][10]*temp[10]+A2[18][11]*temp[11]+A2[18][12]*temp[12]+A2[18][13]*temp[13]+A2[18][14]*temp[14]+A2[18][15]*temp[15]+A2[18][16]*temp[16]+A2[18][17]*temp[17]+A2[18][18]*temp[18]+A2[18][19]*temp[19]+A2[18][20]*temp[20]+A2[18][21]*temp[21]+A2[18][22]*temp[22]+A2[18][23]*temp[23];
		dl20[ptr[19]]=B2L[19]*leftIn+B2R[19]*rightIn+A2[19][0]*temp[0]+A2[19][1]*temp[1]+A2[19][2]*temp[2]+A2[19][3]*temp[3]+A2[19][4]*temp[4]+A2[19][5]*temp[5]+A2[19][6]*temp[6]+A2[19][7]*temp[7]+A2[19][8]*temp[8]+A2[19][9]*temp[9]+A2[19][10]*temp[10]+A2[19][11]*temp[11]+A2[19][12]*temp[12]+A2[19][13]*temp[13]+A2[19][14]*temp[14]+A2[19][15]*temp[15]+A2[19][16]*temp[16]+A2[19][17]*temp[17]+A2[19][18]*temp[18]+A2[19][19]*temp[19]+A2[19][20]*temp[20]+A2[19][21]*temp[21]+A2[19][22]*temp[22]+A2[19][23]*temp[23];
		dl21[ptr[20]]=B2L[20]*leftIn+B2R[20]*rightIn+A2[20][0]*temp[0]+A2[20][1]*temp[1]+A2[20][2]*temp[2]+A2[20][3]*temp[3]+A2[20][4]*temp[4]+A2[20][5]*temp[5]+A2[20][6]*temp[6]+A2[20][7]*temp[7]+A2[20][8]*temp[8]+A2[20][9]*temp[9]+A2[20][10]*temp[10]+A2[20][11]*temp[11]+A2[20][12]*temp[12]+A2[20][13]*temp[13]+A2[20][14]*temp[14]+A2[20][15]*temp[15]+A2[20][16]*temp[16]+A2[20][17]*temp[17]+A2[20][18]*temp[18]+A2[20][19]*temp[19]+A2[20][20]*temp[20]+A2[20][21]*temp[21]+A2[20][22]*temp[22]+A2[20][23]*temp[23];
		dl22[ptr[21]]=B2L[21]*leftIn+B2R[21]*rightIn+A2[21][0]*temp[0]+A2[21][1]*temp[1]+A2[21][2]*temp[2]+A2[21][3]*temp[3]+A2[21][4]*temp[4]+A2[21][5]*temp[5]+A2[21][6]*temp[6]+A2[21][7]*temp[7]+A2[21][8]*temp[8]+A2[21][9]*temp[9]+A2[21][10]*temp[10]+A2[21][11]*temp[11]+A2[21][12]*temp[12]+A2[21][13]*temp[13]+A2[21][14]*temp[14]+A2[21][15]*temp[15]+A2[21][16]*temp[16]+A2[21][17]*temp[17]+A2[21][18]*temp[18]+A2[21][19]*temp[19]+A2[21][20]*temp[20]+A2[21][21]*temp[21]+A2[21][22]*temp[22]+A2[21][23]*temp[23];
		dl23[ptr[22]]=B2L[22]*leftIn+B2R[22]*rightIn+A2[22][0]*temp[0]+A2[22][1]*temp[1]+A2[22][2]*temp[2]+A2[22][3]*temp[3]+A2[22][4]*temp[4]+A2[22][5]*temp[5]+A2[22][6]*temp[6]+A2[22][7]*temp[7]+A2[22][8]*temp[8]+A2[22][9]*temp[9]+A2[22][10]*temp[10]+A2[22][11]*temp[11]+A2[22][12]*temp[12]+A2[22][13]*temp[13]+A2[22][14]*temp[14]+A2[22][15]*temp[15]+A2[22][16]*temp[16]+A2[22][17]*temp[17]+A2[22][18]*temp[18]+A2[22][19]*temp[19]+A2[22][20]*temp[20]+A2[22][21]*temp[21]+A2[22][22]*temp[22]+A2[22][23]*temp[23];
		dl24[ptr[23]]=B2L[23]*leftIn+B2R[23]*rightIn+A2[23][0]*temp[0]+A2[23][1]*temp[1]+A2[23][2]*temp[2]+A2[23][3]*temp[3]+A2[23][4]*temp[4]+A2[23][5]*temp[5]+A2[23][6]*temp[6]+A2[23][7]*temp[7]+A2[23][8]*temp[8]+A2[23][9]*temp[9]+A2[23][10]*temp[10]+A2[23][11]*temp[11]+A2[23][12]*temp[12]+A2[23][13]*temp[13]+A2[23][14]*temp[14]+A2[23][15]*temp[15]+A2[23][16]*temp[16]+A2[23][17]*temp[17]+A2[23][18]*temp[18]+A2[23][19]*temp[19]+A2[23][20]*temp[20]+A2[23][21]*temp[21]+A2[23][22]*temp[22]+A2[23][23]*temp[23];
	}
	/* a pointerek leptetese */
	ptr[0]=ptr[0]?(ptr[0]-1):(M2_1-1);
	ptr[1]=ptr[1]?(ptr[1]-1):(M2_2-1);
	ptr[2]=ptr[2]?(ptr[2]-1):(M2_3-1);
	ptr[3]=ptr[3]?(ptr[3]-1):(M2_4-1);
	ptr[4]=ptr[4]?(ptr[4]-1):(M2_5-1);
	ptr[5]=ptr[5]?(ptr[5]-1):(M2_6-1);
	ptr[6]=ptr[6]?(ptr[6]-1):(M2_7-1);
	ptr[7]=ptr[7]?(ptr[7]-1):(M2_8-1);
	ptr[8]=ptr[8]?(ptr[8]-1):(M2_9-1);
	ptr[9]=ptr[9]?(ptr[9]-1):(M2_10-1);
	ptr[10]=ptr[10]?(ptr[10]-1):(M2_11-1);
	ptr[11]=ptr[11]?(ptr[11]-1):(M2_12-1);
	ptr[12]=ptr[12]?(ptr[12]-1):(M2_13-1);
	ptr[13]=ptr[13]?(ptr[13]-1):(M2_14-1);
	ptr[14]=ptr[14]?(ptr[14]-1):(M2_15-1);
	ptr[15]=ptr[15]?(ptr[15]-1):(M2_16-1);
	ptr[16]=ptr[16]?(ptr[16]-1):(M2_17-1);
	ptr[17]=ptr[17]?(ptr[17]-1):(M2_18-1);
	ptr[18]=ptr[18]?(ptr[18]-1):(M2_19-1);
	ptr[19]=ptr[19]?(ptr[19]-1):(M2_20-1);
	ptr[20]=ptr[20]?(ptr[20]-1):(M2_21-1);
	ptr[21]=ptr[21]?(ptr[21]-1):(M2_22-1);
	ptr[22]=ptr[22]?(ptr[22]-1):(M2_23-1);
	ptr[23]=ptr[23]?(ptr[23]-1):(M2_24-1);	
	/* a kimenet osszeallitasa */
	if(zero) return;
	leftOut4=dlL[ptr[24]]+C2L[0]*temp[0]+C2L[1]*temp[1]+C2L[2]*temp[2]+C2L[3]*temp[3]+C2L[4]*temp[4]+C2L[5]*temp[5]+C2L[6]*temp[6]+C2L[7]*temp[7]+C2L[8]*temp[8]+C2L[9]*temp[9]+C2L[10]*temp[10]+C2L[11]*temp[11]+C2L[12]*temp[12]+C2L[13]*temp[13]+C2L[14]*temp[14]+C2L[15]*temp[15]+C2L[16]*temp[16]+C2L[17]*temp[17]+C2L[18]*temp[18]+C2L[19]*temp[19]+C2L[20]*temp[20]+C2L[21]*temp[21]+C2L[22]*temp[22]+C2L[23]*temp[23];
	leftOut4=0.01*wet*leftOut4+(1.0-0.01*wet)*leftIn;
	rightOut4=dlR[ptr[25]]+C2R[0]*temp[0]+C2R[1]*temp[1]+C2R[2]*temp[2]+C2R[3]*temp[3]+C2R[4]*temp[4]+C2R[5]*temp[5]+C2R[6]*temp[6]+C2R[7]*temp[7]+C2R[8]*temp[8]+C2R[9]*temp[9]+C2R[10]*temp[10]+C2R[11]*temp[11]+C2R[12]*temp[12]+C2R[13]*temp[13]+C2R[14]*temp[14]+C2R[15]*temp[15]+C2R[16]*temp[16]+C2R[17]*temp[17]+C2R[18]*temp[18]+C2R[19]*temp[19]+C2R[20]*temp[20]+C2R[21]*temp[21]+C2R[22]*temp[22]+C2R[23]*temp[23];
	rightOut4=0.01*wet*rightOut4+(1.0-0.01*wet)*rightIn;
	dlL[ptr[24]]=D2L*leftIn;
	dlR[ptr[25]]=D2R*rightIn;
	ptr[24]=ptr[24]?(ptr[24]-1):(M2_L-1);
	ptr[25]=ptr[25]?(ptr[25]-1):(M2_R-1);
}

/* itt tortenik a lenyegi feldolgozas (futasido merheto a DAIP15 labon)
   amig az egyik fut, addig a masikat nullakkal toltjuk fel, hogy ne legyen atkapcsolasi tranziens */
void process(){
	if(currentEffect){
		computeSmallRoom(false);
		computeBigHall(true);
	}
	else{
		computeBigHall(false);
		computeSmallRoom(true);
	}
}
