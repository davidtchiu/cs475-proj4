/* clkinit.c - clkinit */

#include <xinu.h>
#include <interrupt.h>
#include <clock.h>

uint32	clktime;		/* seconds since boot			*/
uint32	ctr1000 = 0;		/* milliseconds since boot		*/
uint32	preempt;		/* preemption counter			*/

/**
 * Initialize the clock at system start up
 */
void	clkinit(void)
{
	// TODO -- program the timer!
	// 	Use Counter 0, 16-bit binary counter, rate generator mod, read/write
	//	least significant byte first, followed by most significant byte
	outb(CLKCNTL, 0x34);

	// TODO -- set initial value of the countdown!
	//	clock rate is 1.193 Mhz (ticks per second). We to set countdown in such a way that
	//	the timer goes off every 1ms
	uint16	countdown = 1193;

	// TODO -- Now program the initial value for countdown
	// 	must write in two operations
	outb(CLOCK0, (char) (0xff & countdown));	//write least significant byte of countdown
	outb(CLOCK0, (char) (0xff & countdown >> 8));	//write most significant byte of countdown


	// Set interrupt vector for clock to invoke clkint
	set_evec(IRQBASE, (uint32)clkint);

	preempt = QUANTUM;	// initial time quantum

	clktime = 0;		// start counting seconds

	return;
}
