#ifdef __AVR_ATmega328P__
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#define F_CPU 16000000UL

#define NORMAL_MODE_VALUE(timer_bit, n_seconds, prescaler) ((int)(((1UL) << (timer_bit)) - ((n_seconds) * ((F_CPU) / (prescaler)))))
#define CTC_MODE_VALUE(n_seconds, prescaler) ((int)(((n_seconds) * ((F_CPU) / (prescaler))) - (1UL)))

#define __UBRR_VALUE(USART_BAUDRATE) ((int)((((F_CPU) / ((double)(16UL))) + ((double)(USART_BAUDRATE) / ((double)(2UL)))) / (double)(USART_BAUDRATE) - ((double)(1UL))))

/*
inline __attribute__((always_inline)) int calculate_UBRR() {
	return 123;
}
*/
/*
UBRRL = (uint8_t)( (F_CPU + BAUD_RATE * 4L) / (BAUD_RATE * 8L) - 1 );

  UBRRH = (((F_CPU/BAUD_RATE)/16)-1)>>8; 	// set baud rate
  UBRRL = (((F_CPU/BAUD_RATE)/16)-1);
	UBRR0L = (uint8_t)(F_CPU/(BAUD_RATE*16L)-1);
	UBRR0H = (F_CPU/(BAUD_RATE*16L)-1) >> 8;
https://github.com/search?q=repo%3Aarduino%2FArduinoCore-avr+UBRRL&type=code
*/

void adc_init()
{
	/*
	| REFS1 | REFS0 | Vref            |                               |
	|-------|-------|-----------------|-------------------------------|
	| 0     | 0     | AREF pin        | Set externally                |
	| 0     | 1     | AVCC pin        | Same as VCC                   |
	| 1     | 0     | Reserved        | -                             |
	| 1     | 1     | Internal 2.56 V | Fixed regardless of VCC value |

	MUX4:0
	00000 ADC0
	00001 ADC1
	00010 ADC2
	00011 ADC3
	00100 ADC4
	00101 ADC5
	00110 ADC6
	00111 ADC7
	*/
	ADMUX =
		(0 << REFS1) | // 7, Reference Selection Bits
		(1 << REFS0) | // 6, Reference Selection Bits
		(0 << ADLAR) | // 5, ADC Left Adjust Results, (usually set 0 for right-adjusting)
		(0 << MUX3) |  // 3, Analog Channel and Gain Selection Bits
		(0 << MUX2) |  // 2, Analog Channel and Gain Selection Bits
		(0 << MUX1) |  // 1, Analog Channel and Gain Selection Bits
		(0 << MUX0);   // 0, Analog Channel and Gain Selection Bits
		

	ADCSRA =
		(1 << ADEN) |  // 7, ADC Enable, This bit enables or disables the ADC. Setting this bit to one will enable the ADC, and clearing this bit to zero will disable it even while a conversion is in progress.
		(0 << ADSC) |  // 6, ADC Start Conversion
		(0 << ADATE) | // 5, ADC Auto Trigger Enable
		(0 << ADIF) |  // 4, ADC Interrupt Flag
		(0 << ADIE) |  // 3, ADC Interrupt Enable
		// Prescaler below, use 1 1 1 if not time critcal
		/*
		| ADPS2 | ADPS1 | ADPS0 | ADC Clock |
		|-------|-------|-------|-----------|
		| 0     | 0     | 0     | Reserved  |
		| 0     | 0     | 1     | CK/2      |
		| 0     | 1     | 0     | CK/4      |
		| 0     | 1     | 1     | CK/8      |
		| 1     | 0     | 0     | CK/16     |
		| 1     | 0     | 1     | CK/32     |
		| 1     | 1     | 0     | CK/64     |
		| 1     | 1     | 1     | CK/128    |
		*/
		(1 << ADPS2) | // 2, ADC Prescaler Select Bits
		(1 << ADPS1) | // 1, ADC Prescaler Select Bits
		(1 << ADPS0);  // 0, ADC Prescaler Select Bits
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main(void)
{
	int const UP_BTN = 2;
	int const DOWN_BTN = 4;
	int const LEFT_BTN = 5;
	int const RIGHT_BTN = 3;
	int const E_BTN = 6;
	int const F_BTN = 7;
	int const JOYSTICK_BTN = 8;
	// int const JOYSTICK_AXIS_X = A0;
	// int const JOYSTICK_AXIS_Y = A1;
	int buttons[] = {UP_BTN, DOWN_BTN, LEFT_BTN, RIGHT_BTN, E_BTN, F_BTN, JOYSTICK_BTN};










	DDRB = 0xFF;   // make Port B an output
	DDRD = 0xFF;   // make Port D an output

	DDRC = 0;	   // make Port C an input for ADC input

	adc_init();
	
// |-----|-----|-----|-----|-----|-----|-----|-----|
// | 7   | 6   | 5   | 4   | 3   | 2   | 1   | 0   |
// |-----|-----|-----|-----|-----|-----|-----|-----|
// | PB3 | PB2 | PB1 | PB0 | PD3 | PD2 | PD1 | PD0 |
// |-----|-----|-----|-----|-----|-----|-----|-----|
	while (1)
	{
		ADCSRA |= (1 << ADSC);						// start conversion
		while ((ADCSRA & (1 << ADIF)) == 0);		// wait for conversion to finish
		ADCSRA |= (1 << ADIF);  
		PORTD = ADCL;								// give the low byte to PORTD
		PORTB = ADCH;								// give the high byte to PORTB
	}
	return 0;
}

#else
#include "gtest/gtest.h"
using ::testing::InitGoogleTest;

// Demonstrate some basic assertions.
TEST(MyTest, BasicAssertions)
{
	//   // Expect two strings not to be equal.
	//   EXPECT_STRNE("hello", "world");
	//   // Expect equality.
	//   EXPECT_EQ(7 * 6, 42);
	EXPECT_EQ(UBRR_VALUE(9600), 103);
	EXPECT_EQ(UBRR_VALUE(4800), 207);
}

int main(int argc, char **argv)
{
	InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

#endif
