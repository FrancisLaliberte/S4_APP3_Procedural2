#include <xc.h>
#include "config_bits.h"
#include "config.h"
#include <sys/attribs.h>

void init_timer2();
void init_timer3();
void init_leds();
void init_adc();


void init_timer2(){
    // Close Timer2
    T2CONbits.ON = 0;
    
    // Use 1:1 prescaler on Timer1
    T2CONbits.TCKPS = 0b000;
    
    // Timer2 clock source is peripheral clock
    T2CONbits.TCS = 0;
    
    // Gated time accumulation increments an amount proportionnal to 
    // HIGH time of CLK signal
    T2CONbits.TGATE = 0;
    
    // Continue Timer2 operation during IDLE
    T2CONbits.SIDL = 0;
    
    //Timer2 is instanciated at 0
    TMR2 = 0;
    
    // Reset Timer2 after PR2 cycles
    PR2 = 999;
    
    
    // Configuration of interrupts
    
    // Current interrupt flag status is cleared
    IFS0bits.T2IF = 0;  
    
    // Enable flag 
    IEC0bits.T2IE = 1;
    
    // Primary and secondary priority group
    IPC2bits.T2IP = 2;
    IPC2bits.T2IS = 0;
    

    
    // Start Timer1. Don't want to start timer before associated interrupt
    // is configured
    T2CONbits.ON = 1;
}
void init_timer3(){
      // Close Timer3
    T3CONbits.ON = 0;
    
    // Use 1:256 prescaler on Timer3
    T3CONbits.TCKPS = 0b000;
    
    // Timer3 clock source is peripheral clock
    T3CONbits.TCS = 0;
    
    // Disable gated time accumulation
    T3CONbits.TGATE = 0;
    
    // Continue Timer3 operation during IDLE
    T3CONbits.SIDL = 0;
    
    //TMR3 is instanciated at 0
    TMR3 = 0;
    
    // Reset Timer1 after PR3 cycles
    PR3 = 11999;
    
    
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
    
    // Timer2 is used as reference, not Timer3  
    OC4CONbits.OCTSEL = 0;

    // PWM mode selected and fault pin disabled   
    OC4CONbits.OCM = 0b110;
    
    // HIGH Logic level until Timer3 reaches OC4R value
    OC4R = 4;
    OC4RS = 4;

    // Turn on OC4
    OC4CONbits.ON = 1;   
}
void init_adc(){
        // Configure Analog pin
    
    // Configure AIC as input
    TRISBbits.TRISB2 = 1;
    
    // Configure pin as analog
     ANSELBbits.ANSB2 = 1;  
     
     
     // Configure ADC
     
     // Close ADC
     AD1CON1bits.ON = 0;
     
     // Continue operation in IDLE
     AD1CON1bits.SIDL = 0;
     
      // Store value as positive 16 bit integer
     AD1CON1bits.FORM = 0b000;    
  
      // Stop sampling and start conversion triggered by TIMER3 period match
     AD1CON1bits.SSRC = 0b010;   
     
      // Buffer is overwritten by next conversion
     AD1CON1bits.CLRASAM = 0;   
     
     // Sampling begins automatically after last conversion completes
     AD1CON1bits.ASAM = 1; 
     
     // Select analog channel 2 for AN2  
     AD1CHSbits.CH0SA = 2;
     
      // No scan among all analog inputs
     AD1CON2bits.CSCNA = 0; 
     
      // Interrupt after each sample/convert sequence
     AD1CON2bits.SMPI = 0; 
     
      // Buffer configured as 1 16bit block
     AD1CON2bits.BUFM = 0;  
     
      // Always use Sample A input multiplexer for sampling
     AD1CON2bits.ALTS = 0;  
     
     // Use vref instead of AN1 as reference
     AD1CHSbits.CH0NA = 0;
     
     
     //Interrupt routines 
     
     // Clear flag
     IFS0bits.AD1IF = 0;

     // Flag enabling
     IEC0bits.AD1IE = 1;
          
     // Priority group and subgroup
     IPC5bits.AD1IP = 5;
     IPC5bits.AD1IS = 0;
     
     // Open ADC
     AD1CON1bits.ON = 1;
}



int main(){
    
    init_timer2();
    init_timer3();
    init_RGB();
    init_adc();
    // Need to be activated to allow interrupt routines. 
    macro_enable_interrupts();
    
    while(1);
    
}

// Interrupt routine macros defined in attribs.h
void __ISR(_TIMER_3_VECTOR, IPL2AUTO)Timer3_ISR(void){
    PORTAINV = 1;
    IFS0bits.T3IF = 0;  
}
