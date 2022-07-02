#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int delay = 0;
ISR (TIMER0_OVF_vect){
    // Give the voltage a little time to adjust
    if((delay++) % 600 != 0){
        return;
    }

    if(!(ACSR & (1 << ACO))){
        PORTB |= (1 << PB2);
        OCR0B--;
    } else {
        PORTB &= ~(1 << PB2);
        OCR0B++;
    }
    // It is OK to update OCR0B during PWM because in PWM mode, OCR0B is double
    // buffered, and will be updated at either the top or bottom of the timer
}

void init_pwm(){
    // Use OC0B
    DDRB |= (1 << PB1);

    // Clar OC0B on compare match, then clean are bottom (non-inverting mode)
    // Set to Fast PWM
    TCCR0A = (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);

    // Enable timer overflow interrupt
    TIMSK |= (1 << TOIE0);

    // Low initial PWM to prevent high voltages
    OCR0B = 20;

    // No prescaler
    TCCR0B = (1 << CS00);

    // Enable interrupts
    sei();
}

void init_comparator(){
    // Select ADC2
    ADMUX = (1 << MUX0) | (1 << MUX1);

    // Use one of the ADC pins as an input
    ADCSRB |= (1 << ACME);
}

int main(void){
    DDRB |= (1 << PB2);

    //init_adc();
    init_comparator();
    init_pwm();

    while(1){
    }
}
