#ifdef __AVR_ATmega328P__
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#define F_CPU 16000000UL

#define NORMAL_MODE_VALUE(timer_bit, n_seconds, prescaler) ((int)(((1UL) << (timer_bit)) - ((n_seconds) * ((F_CPU) / (prescaler)))))
#define CTC_MODE_VALUE(n_seconds, prescaler) ((int)(((n_seconds) * ((F_CPU) / (prescaler))) - (1UL)))


#define __UBRR_VALUE(USART_BAUDRATE) ((int)  ((((F_CPU) / ((double)(16UL))) + ((double)(USART_BAUDRATE) / ((double)(2UL)))) / (double)(USART_BAUDRATE)-((double)(1UL))))

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

int main(void)
{
	DDRB = 1<<0;			// PB0 as Output
	PORTB = 1<<0;			// Initial LOW
    while (1)				// INF Loop
    {
		//Timer_1_Delay();	// Call 1 s Delay
		// PORTB ^= (1<<0);	// Toggle
		PORTB = 0xFF;
    }
}

#else
#include "gtest/gtest.h"
using ::testing::InitGoogleTest;

// Demonstrate some basic assertions.
TEST(MyTest, BasicAssertions) {
//   // Expect two strings not to be equal.
//   EXPECT_STRNE("hello", "world");
//   // Expect equality.
//   EXPECT_EQ(7 * 6, 42);
	EXPECT_EQ(UBRR_VALUE(9600), 103);
	EXPECT_EQ(UBRR_VALUE(4800), 207);
}


int main(int argc, char** argv) {
  	InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#endif
