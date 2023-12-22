#ifndef __PACKAGES__
#define __PACKAGES__

// Actuator 0 : Distribution cartons
#define A0_ON 	0x01<<(0+0)
#define A0_OFF 	0x00<<(0+0)
#define A0_Msk 	0x01<<(0+0)

// Actuator 1 : Tapis distribution cartons
#define A1_ON 	0x01<<(1+0)
#define A1_OFF 	0x00<<(1+0)
#define A1_Msk 0x01<<(1+0)

// Actuator 2 : Blocage entr�e palettiseur
#define A2_ON 	0x01<<(2+0)
#define A2_OFF  0x00<<(2+0)
#define A2_Msk 0x01<<(2+0)

// Actuator 3 : Porte
#define A3_ON 	0x01<<(3+0)
#define A3_OFF  0x00<<(3+0)
#define A3_Msk 0x01<<(3+0)

// Actuator 4 : Poussoir
#define A4_ON 	0x01<<(4+0)
#define A4_OFF  0x00<<(4+0)
#define A4_Msk 0x01<<(4+0)

// Actuator 5 : Clamp
#define A5_ON 	0x01<<(5+0)
#define A5_OFF  0x00<<(5+0)
#define A5_Msk 0x01<<(5+0)

// Actuator 6 : Monter ascenseur
#define A6_ON 	0x01<<(6+0)
#define A6_OFF  0x00<<(6+0)
#define A6_Msk 0x01<<(6+0)

// Actuator 7 : Descendre ascenseur
#define A7_ON 	0x01<<(7+1)
#define A7_OFF  0x00<<(7+1)
#define A7_Msk 0x01<<(7+1)

// Actuator 8 : ascenseur to limit
#define A8_ON 	0x01<<(8+1)
#define A8_OFF  0x00<<(8+1)
#define A8_Msk 0x01<<(8+1)

// Actuator 9 : distribution palette
#define A9_ON 	0x01<<(9+1)
#define A9_OFF  0x00<<(9+1)
#define A9_Msk 0x01<<(9+1)

// Actuator 10 : charger palette
#define A10_ON 	 0x01<<(10+1)
#define A10_OFF  0x00<<(10+1)
#define A10_Msk 0x01<<(10+1)

// Actuator 11 : Tapis carton vers palettiseur
#define A11_ON 	 0x01<<(11+1)
#define A11_OFF  0x00<<(11+1)
#define A11_Msk 0x01<<(11+1)

// Actuator 12 : Tourner carton
#define A12_ON 	 0x01<<(12+1)
#define A12_OFF  0x00<<(12+1)
#define A12_Msk 0x01<<(12+1)

// Actuator 13 : D�charger palettiseur
#define A13_ON 	 0x01<<(13+1)
#define A13_OFF  0x00<<(13+1)
#define A13_Msk 0x01<<(13+1)

// Actuator 14 : Charger palettiseur
#define A14_ON 	 0x01<<(14+2)
#define A14_OFF  0x00<<(14+2)
#define A14_Msk 0x01<<(14+2)

// Actuator 15 : D�charger palette
#define A15_ON 	 0x01<<(15+2)
#define A15_OFF  0x00<<(15+2)
#define A15_Msk 0x01<<(15+2)

// Actuator 16 : Tapis palette vers ascenseur
#define A16_ON 	 0x01<<(16+2)
#define A16_OFF  0x00<<(16+2)
#define A16_Msk 0x01<<(16+2)

// Actuator 17 : Tapis distribution palette
#define A17_ON 	 0x01<<(17+2)
#define A17_OFF  0x00<<(17+2)
#define A17_Msk 0x01<<(17+2)

// Actuator 18 : Tapis fin
#define A18_ON 	 0x01<<(18+2)
#define A18_OFF  0x00<<(18+2)
#define A18_Msk 0x01<<(18+2)

// Actuator 19 : Remover
#define A19_ON 	 0x01<<(19+2)
#define A19_OFF  0x00<<(19+2)
#define A19_Msk 0x01<<(19+2)

// Sensors 0 : Carton distribu�
#define S0_Pos 0x01<<(0+0)
#define S0_OFF 0x00<<(0+0)
#define S0_ON  0x01<<(0+0)

// Sensors 1 : Carton envoy�
#define S1_Pos 0x01<<(1+0)
#define S1_OFF 0x00<<(1+0)
#define S1_ON  0x01<<(1+0)

// Sensors 2 : Entr�e palettiseur
#define S2_Pos 0x01<<(2+0)
#define S2_OFF 0x00<<(2+0)
#define S2_ON  0x01<<(2+0)

// Sensors 3 : Porte ouverte
#define S3_Pos 0x01<<(3+0)
#define S3_OFF 0x00<<(3+0)
#define S3_ON  0x01<<(3+0)

// Sensors 4 : Limite poussoir
#define S4_Pos 0x01<<(4+0)
#define S4_OFF 0x00<<(4+0)
#define S4_ON  0x01<<(4+0)

// Sensors 5 : Clamped
#define S5_Pos 0x01<<(5+0)
#define S5_OFF 0x00<<(5+0)
#define S5_ON  0x01<<(5+0)

// Sensors 6 : Ascenseur �tage RDC
#define S6_Pos 0x01<<(6+0)
#define S6_OFF 0x00<<(6+0)
#define S6_ON  0x01<<(6+0)

// Sensors 7 : Ascenseur �tage 1
#define S7_Pos 0x01<<(7+1)
#define S7_OFF 0x00<<(7+1)
#define S7_ON  0x01<<(7+1)

// Sensors 8 : Ascenseur �tage 2
#define S8_Pos 0x01<<(8+1)
#define S8_OFF 0x00<<(8+1)
#define S8_ON  0x01<<(8+1)

// Sensors 9 : Sortie palette
#define S9_Pos 0x01<<(9+1)
#define S9_OFF 0x00<<(9+1)
#define S9_ON  0x01<<(9+1)

// Sensors 10 : Limite porte
#define S10_Pos 0x01<<(10+1)
#define S10_OFF 0x00<<(10+1)
#define S10_ON  0x01<<(10+1)

// Sensors 11 : Ascenseur en mouvement
#define S11_Pos 0x01<<(11+1)
#define S11_OFF 0x00<<(11+1)
#define S11_ON  0x01<<(11+1)

// Sensors 12 : Entree palette
#define S12_Pos 0x01<<(12+1)
#define S12_OFF 0x00<<(12+1)
#define S12_ON  0x01<<(12+1)

#endif
