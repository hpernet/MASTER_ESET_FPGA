#include <stdio.h>

#define HEX_BASE_ADRESS (volatile unsigned int*) 0x10000020
#define SW_ADRESS       (volatile unsigned int*) 0x10000040
#define KEY_ADRESS      (volatile unsigned int*) 0x10000050
#define LEDR_ADRESS     (volatile unsigned int*) 0x10000000
#define LEDG_ADRESS     (volatile unsigned int*) 0x10000010

#define HEX1_SHIFT 8U
#define HEX2_SHIFT 16U
#define HEX3_SHIFT 24U

unsigned char table[16] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d,
0x7d, 0x07, 0x7f, 0x67,0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71 };

// MAIN
void main (void)
{
	unsigned int	sum = 0;
	unsigned int	m_sum;
	unsigned int	unites;
	unsigned int	dizaines;
	unsigned int	centaines;
	unsigned int	milliers;
	unsigned int sw_value;
	unsigned int index;


	while(1)
	{
		// Get SW values
		sw_value = *SW_ADRESS;

		// Affect LEDR
		*LEDR_ADRESS = sw_value;

		sum += sw_value;
		
		// Decompose sum
		milliers	= sum / 1000;
		m_sum 		= sum % 1000;
		
		centaines	= m_sum / 100;
		m_sum 		= m_sum % 100;
		
		dizaines	= m_sum / 10;
		m_sum 		= m_sum % 10;
		
		unites		= m_sum;		
		
		// Affiche
		*HEX_BASE_ADRESS  = (table[milliers]  << HEX3_SHIFT);
		*HEX_BASE_ADRESS |= (table[centaines] << HEX2_SHIFT);
		*HEX_BASE_ADRESS |= (table[dizaines]  << HEX1_SHIFT);
		*HEX_BASE_ADRESS |= table[unites];	
		
		*LEDG_ADRESS = sum;
		
		// END LOOP
	}
}
