#include <xc.h>
#include "config_bits.h"
#include "config.h"
#include <sys/attribs.h>

void init_timer3();
void init_leds();

void init_timer3(){
      // Close Timer1
    T3CONbits.ON = 0;
    
    // Use 1:256 prescaler on Timer1
    T3CONbits.TCKPS = 0b111;
    
    // Timer1 clock source is peripheral clock
    T3CONbits.TCS = 0;
    
    // Gated time accumulation increments an amount proportionnal to 
    // HIGH time of CLK signal
    T3CONbits.TGATE = 0;
    
    // Continue Timer1 operation during IDLE
    T3CONbits.SIDL = 0;
    
    //TMR1 is instanciated at 0
    TMR3 = 0;
    
    // Reset Timer1 after PR1 cycles
    PR3 = 1874;
    
    
    // Configuration of interrupts
    
    // Current interrupt flag status is cleared
    IFS0bits.T3IF = 0;  
    
    // Primary and secondary priority group
    IPC3bits.T3IP = 2;
    IPC3bits.T3IS = 0;
    
    // Enable flag 
    IEC0bits.T3IE = 1;
    
    // Start Timer1. Don't want to start timer before associated interrupt
    // is configured
    T3CONbits.ON = 1;
}

void init_RGB(){
    // RD3 as digital output initialized at LOW
    TRISDbits.TRISD3 = 0;
    LATDbits.LATD3 = 0;
    ANSELDbits.ANSD3 = 0;
 
    // Remap RD3 to Output Compare 4 (OC4)
    RPD3Rbits.RPD3R = 0b1011;
    
    // Turn off OC4
    OC4CONbits.ON = 0;

    // Stay open at IDLE
    OC4CONbits.SIDL = 0; 
    
    //16 bit timer source not 32 bit
    OC4CONbits.OC32 = 0;
    
    // Timer3 is used as reference, not Timer2  
    OC4CONbits.OCTSEL = 1;

    // PWM mode selected and fault pin disabled   
    OC4CONbits.OCM = 0b110;
    
    // HIGH Logic level until Timer3 reaches OC4R value
    OC4R = 4;
    OC4RS = 4;

    // Turn on OC4
    OC4CONbits.ON = 1;   
}



int main(){
    init_timer3();
    init_RGB();
    // Need to be activated to allow interrupt routines. 
    macro_enable_interrupts();
    
    while(1);
    
}

// Interrupt routine macros defined in attribs.h
void __ISR(_TIMER_3_VECTOR, IPL2AUTO)Timer3_ISR(void){
    PORTAINV = 1;
    IFS0bits.T3IF = 0;  
}
