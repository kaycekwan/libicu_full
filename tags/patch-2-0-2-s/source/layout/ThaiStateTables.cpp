/*
 * @(#)ThaiStateTables.cpp	1.8 00/03/15
 *
 * (C) Copyright IBM Corp. 1999, 2000 - All Rights Reserved
 *
 * WARNING: THIS FILE IS MACHINE GENERATED. DO NOT HAND EDIT IT UNLESS
 * YOU REALLY KNOW WHAT YOU'RE DOING.
 *
 */

#include "LETypes.h"
#include "ThaiShaping.h"

U_NAMESPACE_BEGIN

	le_uint8 ThaiShaping::classTable[] = {
	//       0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
	//       -------------------------------------------------------------------------------
	/*0E00*/ NON, CON, CON, CON, CON, CON, CON, CON, CON, CON, CON, CON, CON, COD, COD, COD, 
	/*0E10*/ COD, CON, CON, CON, CON, CON, CON, CON, CON, CON, CON, COA, CON, COA, CON, COA, 
	/*0E20*/ CON, CON, CON, CON, FV3, CON, FV3, CON, CON, CON, CON, CON, CON, CON, CON, NON, 
	/*0E30*/ FV1, AV2, FV1, FV1, AV1, AV3, AV2, AV3, BV1, BV2, BDI, NON, NON, NON, NON, NON, 
	/*0E40*/ LVO, LVO, LVO, LVO, LVO, FV2, NON, AD2, TON, TON, TON, TON, AD1, NIK, AD3, NON, 
	/*0E50*/ NON, NON, NON, NON, NON, NON, NON, NON, NON, NON, NON, NON
	};

	ThaiShaping::StateTransition ThaiShaping::thaiStateTable[][ThaiShaping::classCount] = {
	//+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
	//|         N         C         C         C         L         F         F         F         B         B         B         T         A         A         A         N         A         A         A    |
	//|         O         O         O         O         V         V         V         V         V         V         D         O         D         D         D         I         V         V         V    |
	//|         N         N         A         D         O         1         2         3         1         2         I         N         1         2         3         K         1         2         3    |
	//+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
	/*00*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _S}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*01*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _A}, { 0, _S}, { 0, _A}, { 2, _C}, { 6, _C}, { 0, _C}, { 8, _E}, { 0, _E}, { 0, _E}, { 0, _C}, { 9, _E}, {11, _C}, {14, _C}, {16, _C}},
	/*02*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _S}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 3, _E}, { 0, _E}, { 0, _R}, { 0, _R}, { 4, _E}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*03*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _A}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*04*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _S}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 5, _C}, { 0, _C}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*05*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _A}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*06*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _S}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 7, _E}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*07*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _A}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*08*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _A}, { 0, _A}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*09*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _S}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, {10, _C}, { 0, _C}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*10*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _A}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*11*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _S}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, {12, _C}, { 0, _C}, { 0, _R}, { 0, _R}, {13, _C}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*12*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _A}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*13*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _S}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*14*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _S}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, {15, _C}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*15*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _A}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*16*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _S}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, {17, _C}, { 0, _R}, { 0, _C}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*17*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _A}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*18*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _A}, { 0, _S}, { 0, _A}, {19, _C}, {23, _C}, { 0, _C}, {25, _F}, { 0, _F}, { 0, _F}, { 0, _D}, {26, _F}, {28, _D}, {31, _D}, {33, _D}},
	/*19*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _S}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, {20, _F}, { 0, _F}, { 0, _R}, { 0, _R}, {21, _F}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*20*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _A}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*21*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _S}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, {22, _C}, { 0, _C}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*22*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _A}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*23*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _S}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, {24, _F}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*24*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _A}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*25*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _A}, { 0, _A}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*26*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _S}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, {27, _G}, { 0, _G}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*27*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _A}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*28*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _S}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, {29, _G}, { 0, _G}, { 0, _R}, { 0, _R}, {30, _G}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*29*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _A}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*30*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _S}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*31*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _S}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, {32, _G}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*32*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _A}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*33*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _S}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, {34, _G}, { 0, _R}, { 0, _G}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*34*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _A}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*35*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _A}, { 0, _S}, { 0, _A}, {36, _H}, {40, _H}, { 0, _H}, {42, _E}, { 0, _E}, { 0, _E}, { 0, _C}, {43, _E}, {45, _C}, {48, _C}, {50, _C}},
	/*36*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _S}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, {37, _E}, { 0, _E}, { 0, _R}, { 0, _R}, {38, _E}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*37*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _A}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*38*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _S}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, {39, _C}, { 0, _C}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*39*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _A}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*40*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _S}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, {41, _E}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*41*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _A}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*42*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _A}, { 0, _A}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*43*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _S}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, {44, _C}, { 0, _C}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*44*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _A}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*45*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _S}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, {46, _C}, { 0, _C}, { 0, _R}, { 0, _R}, {47, _C}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*46*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _A}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*47*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _S}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*48*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _S}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, {49, _C}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*49*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _A}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*50*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _S}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, {51, _C}, { 0, _R}, { 0, _C}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}},
	/*51*/ {{ 0, _A}, { 1, _A}, {18, _A}, {35, _A}, { 0, _A}, { 0, _S}, { 0, _A}, { 0, _A}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}, { 0, _R}}
	};

U_NAMESPACE_END
