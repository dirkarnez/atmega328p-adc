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
	// ADCSRA = 0x87; // make ADC enable and select ck/128
	// ADMUX = 0xC0;  // 2.56V Vref, ADC0 single ended input, data will be right-justified

	// ADMUX = 0b01000000;
	/* set input channel to read */
	// ADMUX = 0x40 | (channel & 0x07); // 0100 0000 | (channel & 0000 0100)
	// ADCSRA |= (1 << ADEN) | (0 << ADPS2) | (1 << ADPS1) | (0 << ADPS0);
	ADMUX =
		// Reference voltage
		(0 << REFS1) | // 7
		(1 << REFS0) | // 6

		(0 << ADLAR) | // 5, nothing on 4 right-justified

		(0 << MUX3) |  // 3 // ADC0
		(0 << MUX2) |  // 2	// ADC0
		(0 << MUX1) |  // 1	// ADC0
		(0 << MUX0);   // 0	// ADC0
		

	ADCSRA =
		(1 << ADEN) |  // 7, enable ADC
		(0 << ADSC) |  // 6
		(0 << ADATE) | // 5
		(0 << ADIF) |  // 4
		(0 << ADIE) |  // 3
		// Prescaler, use 1 1 1 if not time critcal
		(1 << ADPS2) | // 2
		(1 << ADPS1) | // 1
		(1 << ADPS0);  // 0
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main(void)
{
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
