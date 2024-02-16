## Setting specific pin mask
#### (12.2.8 - PCMSK0 – Pin Change Mask Register 0)

```C++
PCMSK0 |= _BV(PCINT#)
```

## Enabling interrupt
#### (12.2.4 PCICR – Pin Change Interrupt Control Register)

Within the PCICR register, set the PCIE0 bit to 1 to enable that interrupt

```C++
PCICR |= _BV(PCIE0);
```

## SREG - AVR Status Register (6.3.1)
Bit 7 – I: Global Interrupt Enable
*The global interrupt enable bit must be set for the interrupts to be enabled. The individual interrupt enable control is then performed in separate control registers. If the global interrupt enable register is cleared, none of the interrupts are enabled independent of the individual interrupt enable settings. The I-bit is cleared by hardware after an interrupt has occurred, and is set by the RETI instruction to enable subsequent interrupts. The I-bit can also be set and cleared by the application with the SEI and CLI instructions, as described in the instruction set reference.*
``` C++
	SREG |= _BV(7); // Enables global interrupts?
``` 


## Example Code:

```C++
ISR (PCINT0_vect) {
 // handle pin change interrupt for D8 to D13 here
 }  // end of PCINT0_vect

ISR (PCINT1_vect) {
 // handle pin change interrupt for A0 to A5 here
 }  // end of PCINT1_vect

ISR (PCINT2_vect) {
 // handle pin change interrupt for D0 to D7 here
 }  // end of PCINT2_vect


void setup () { 
  // pin change interrupt (example for D9)
  PCMSK0 |= bit (PCINT1);  // want pin 9
  PCIFR  |= bit (PCIF0);   // clear any outstanding interrupts
  PCICR  |= bit (PCIE0);   // enable pin change interrupts for D8 to D13
}
```

### Breakdown of which pins are which
```
D0	  PCINT16 (PCMSK2 / PCIF2 / PCIE2)
D1	  PCINT17 (PCMSK2 / PCIF2 / PCIE2)
D2	  PCINT18 (PCMSK2 / PCIF2 / PCIE2)
D3	  PCINT19 (PCMSK2 / PCIF2 / PCIE2)
D4	  PCINT20 (PCMSK2 / PCIF2 / PCIE2)
D5	  PCINT21 (PCMSK2 / PCIF2 / PCIE2)
D6	  PCINT22 (PCMSK2 / PCIF2 / PCIE2)
D7	  PCINT23 (PCMSK2 / PCIF2 / PCIE2)
D8	  PCINT0  (PCMSK0 / PCIF0 / PCIE0)
D9	  PCINT1  (PCMSK0 / PCIF0 / PCIE0)
D10	  PCINT2  (PCMSK0 / PCIF0 / PCIE0)
D11	  PCINT3  (PCMSK0 / PCIF0 / PCIE0)
D12	  PCINT4  (PCMSK0 / PCIF0 / PCIE0)
D13	  PCINT5  (PCMSK0 / PCIF0 / PCIE0)
A0	  PCINT8  (PCMSK1 / PCIF1 / PCIE1)
A1	  PCINT9  (PCMSK1 / PCIF1 / PCIE1)
A2	  PCINT10 (PCMSK1 / PCIF1 / PCIE1)
A3	  PCINT11 (PCMSK1 / PCIF1 / PCIE1)
A4	  PCINT12 (PCMSK1 / PCIF1 / PCIE1)
A5	  PCINT13 (PCMSK1 / PCIF1 / PCIE1)
```