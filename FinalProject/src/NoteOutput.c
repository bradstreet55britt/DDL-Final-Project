#include "NoteOutput.h"

/* Usage Guideline
 * When a note is to be played, call setChInterval to output the squarewave of the note's frequency.
 */

struct TimerStruct {
    unsigned int IR;
    unsigned int TCR;
    unsigned int TC;
    unsigned int PR;
    unsigned int PC;
    unsigned int MCR;
    unsigned int MR[4];
    unsigned int CCR;
    unsigned int CR[2];
    unsigned int EMR;
    unsigned int CTCR;
};
#define T ((volatile struct TimerStruct *) 0x40004000)

/* Contains the time intervals for each Match Register
 * This is used to update the Match Register during the interrupt handler for the next interrupt.
 * The values for these intervals are calculated to be (Period/2) / PCLK
 */
int ChInterval[4];

void initNoteSystem(void){
    T[0].IR = 0xF;          //Clear MR Interupt flags
    T[0].TCR |= 1;          //Enable Timer 0 counter
}

// Enables interrupt generation from the specified match register with the specified interval.
void setChInterval(int MR, int interval){
    ChInterval[MR] = interval;
    T[0].MCR |= (1 << MR);
    T[0].MR[0] = T[0].TC + interval;
}

/* Disables interrupt generation from the specified Match Register.
 * Also clears the relevant interrupt flag.
 */
void disableCh(int MR){
    T[0].MCR &= ~(1u << MR);
    T[0].IR = (1 << MR);
}

//These are for implementing the pin toggle for the square wave. These are to be implemented elsewhere.

__attribute__ ((weak)) void togglePin0(void){

};
__attribute__ ((weak)) void togglePin1(void){

};
__attribute__ ((weak)) void togglePin2(void){

};
__attribute__ ((weak)) void togglePin3(void){

};

void TIMER0_IRQHandler(void){
    if (T[0].IR & 1){
        T[0].IR = 1;
        T[0].MR[0] += ChInterval[0];
        togglePin0();
    }
    if (T[0].IR & 2){
        T[0].IR = 2;
        T[0].MR[1] += ChInterval[1];
        togglePin1();
    }
    if (T[0].IR & 4){
        T[0].IR = 4;
        T[0].MR[2] += ChInterval[2];
        togglePin2();
    }
    if (T[0].IR & 8){
        T[0].IR = 8;
        T[0].MR[3] += ChInterval[3];
        togglePin3();
    }
}