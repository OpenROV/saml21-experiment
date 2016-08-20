#pragma once

extern "C"
{
	#include <sam.h>
}

void RTC_Handler(void)
{
	/* Read and mask interrupt flag register */
	uint16_t interrupt_status = RTC->MODE0.INTFLAG.reg;

	if (interrupt_status & RTC_MODE0_INTFLAG_CMP0) 
	{
		/* Clear interrupt flag */
		RTC->MODE0.INTFLAG.reg = RTC_MODE0_INTFLAG_CMP0;
	}
}

class CRTC
{
public:
	CRTC(){}
	~CRTC(){}

	void Initialize()
	{
		// Enable in APBA
		MCLK->APBCMASK.reg |= MCLK_APBAMASK_RTC;

		// Sync
		while(RTC->MODE0.SYNCBUSY.reg & RTC_MODE0_SYNCBUSY_SWRST ) {};

		// Reset RTC
		RTC->MODE0.CTRLA.reg |= RTC_MODE0_CTRLA_SWRST;

		// Sync
		while(RTC->MODE0.SYNCBUSY.reg & RTC_MODE0_SYNCBUSY_SWRST ) {};

		// Sync
		while(RTC->MODE0.SYNCBUSY.reg & ( RTC_MODE0_SYNCBUSY_SWRST|RTC_MODE0_SYNCBUSY_ENABLE|RTC_MODE0_SYNCBUSY_COUNTSYNC ) ) {};

		// Set CTRLA.MODE = 0x0 for 32bit counter mode
		RTC->MODE0.CTRLA.bit.MODE = 0x0;

		// Set CTRLA.Prescaler to 1 (1Khz)
		RTC->MODE0.CTRLA.bit.PRESCALER = 0x1;

		// Set CTRLA.clear on match to true for periodic events
		RTC->MODE0.CTRLA.bit.MATCHCLR = 0x1;

		// Sync
		while(RTC->MODE0.SYNCBUSY.reg & RTC_MODE0_SYNCBUSY_MASK ) {};

		// Set COMP0 to whatever equals 5 secs (5*1024?)
		RTC->MODE0.COMP[0].reg = 15120u;	
		
		// Enable RTC
		while(RTC->MODE0.SYNCBUSY.reg & ( RTC_MODE0_SYNCBUSY_SWRST|RTC_MODE0_SYNCBUSY_ENABLE|RTC_MODE0_SYNCBUSY_COUNTSYNC ) ) {};
		RTC->MODE0.CTRLA.reg |= RTC_MODE0_CTRLA_ENABLE;
	}

	void EnableInterrupt()
	{
		/* enable RTC_IRQn */
		NVIC_DisableIRQ(RTC_IRQn);
		NVIC_EnableIRQ(RTC_IRQn);

		/* enable cmp */
		RTC->MODE0.INTENSET.reg = RTC_MODE0_INTENSET_CMP0;
	}

	void DisableInterrupt()
	{
		/* disable cmp */
		RTC->MODE0.INTENCLR.reg = RTC_MODE0_INTENCLR_CMP0;

		/* disable RTC_IRQn */
		NVIC_DisableIRQ(RTC_IRQn);
	}
};