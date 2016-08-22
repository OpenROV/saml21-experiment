#include <sam.h>

// Macros
#define DUMMY __attribute__ ((weak, alias ("Dummy_Handler")))

// Sections
extern uint32_t _sfixed;		// Start fixed
extern uint32_t _efixed;		// End fixed
extern uint32_t _etext;			// End text
extern uint32_t _srelocate;		// Start Relocate
extern uint32_t _erelocate;		// End Relocate
extern uint32_t _szero;			// Start Zero
extern uint32_t _ezero;			// End Zero
extern uint32_t _sstack;		// Stack Start
extern uint32_t _estack;		// Stack End

int main();
void __libc_init_array( void );

// Default handler
void Dummy_Handler( void );

// Cortex-M0+ core handlers
DUMMY	void Reset_Handler		( void );
DUMMY 	void NMI_Handler		( void );
DUMMY 	void HardFault_Handler	( void );
DUMMY 	void SVC_Handler		( void );
DUMMY 	void PendSV_Handler		( void );
DUMMY 	void SysTick_Handler	( void );

/* Peripheral handlers */
DUMMY 	void PM_Handler			( void );
DUMMY 	void SYSCTRL_Handler	( void );
DUMMY 	void WDT_Handler		( void );
DUMMY 	void RTC_Handler		( void );
DUMMY 	void EIC_Handler		( void );
DUMMY 	void NVMCTRL_Handler	( void );
DUMMY 	void DMAC_Handler		( void );
DUMMY 	void USB_Handler		( void );
DUMMY 	void EVSYS_Handler		( void );
DUMMY 	void SERCOM0_Handler	( void );
DUMMY 	void SERCOM1_Handler	( void );
DUMMY 	void SERCOM2_Handler	( void );
DUMMY 	void SERCOM3_Handler	( void );
DUMMY 	void SERCOM4_Handler	( void );
DUMMY 	void SERCOM5_Handler	( void );
DUMMY 	void TCC0_Handler		( void );
DUMMY 	void TCC1_Handler		( void );
DUMMY 	void TCC2_Handler		( void );
DUMMY 	void TC0_Handler		( void );
DUMMY 	void TC1_Handler		( void );
DUMMY 	void TC2_Handler		( void );
DUMMY 	void TC3_Handler		( void );
DUMMY 	void TC4_Handler		( void );
DUMMY	void TC5_Handler		( void );
DUMMY	void TC6_Handler		( void );
DUMMY	void TC7_Handler		( void );
DUMMY 	void ADC_Handler		( void );
DUMMY 	void AC_Handler			( void );
DUMMY 	void DAC_Handler		( void );
DUMMY 	void PTC_Handler		( void );
DUMMY 	void I2S_Handler		( void );

// Exception table
__attribute__ ((section(".vectors")))
const DeviceVectors exception_table = 
{
	// Configure Initial Stack Pointer, using linker-generated symbols
	(void*) (&_estack),

	(void*) Reset_Handler,
	(void*) NMI_Handler,
	(void*) HardFault_Handler,
	(void*) (0UL),                  /* Reserved */
	(void*) (0UL),                  /* Reserved */
	(void*) (0UL),                  /* Reserved */
	(void*) (0UL),                  /* Reserved */
	(void*) (0UL),                  /* Reserved */
	(void*) (0UL),                  /* Reserved */
	(void*) (0UL),                  /* Reserved */
	(void*) SVC_Handler,
	(void*) (0UL),                  /* Reserved */
	(void*) (0UL),                  /* Reserved */
	(void*) PendSV_Handler,
	(void*) SysTick_Handler,

	/* Configurable interrupts */
	(void*) PM_Handler,             /*  0 Power Manager */
	(void*) SYSCTRL_Handler,        /*  1 System Control */
	(void*) WDT_Handler,            /*  2 Watchdog Timer */
	(void*) RTC_Handler,            /*  3 Real-Time Counter */
	(void*) EIC_Handler,            /*  4 External Interrupt Controller */
	(void*) NVMCTRL_Handler,        /*  5 Non-Volatile Memory Controller */
	(void*) DMAC_Handler,           /*  6 Direct Memory Access Controller */
	(void*) USB_Handler,            /*  7 Universal Serial Bus */
	(void*) EVSYS_Handler,          /*  8 Event System Interface */
	(void*) SERCOM0_Handler,        /*  9 Serial Communication Interface 0 */
	(void*) SERCOM1_Handler,        /* 10 Serial Communication Interface 1 */
	(void*) SERCOM2_Handler,        /* 11 Serial Communication Interface 2 */
	(void*) SERCOM3_Handler,        /* 12 Serial Communication Interface 3 */
	(void*) SERCOM4_Handler,        /* 13 Serial Communication Interface 4 */
	(void*) SERCOM5_Handler,        /* 14 Serial Communication Interface 5 */
	(void*) TCC0_Handler,           /* 15 Timer Counter Control 0 */
	(void*) TCC1_Handler,           /* 16 Timer Counter Control 1 */
	(void*) TCC2_Handler,           /* 17 Timer Counter Control 2 */
	(void*) TC3_Handler,            /* 18 Basic Timer Counter 0 */
	(void*) TC4_Handler,            /* 19 Basic Timer Counter 1 */
	(void*) TC5_Handler,            /* 20 Basic Timer Counter 2 */
	(void*) TC6_Handler,            /* 21 Basic Timer Counter 3 */
	(void*) TC7_Handler,            /* 22 Basic Timer Counter 4 */
	(void*) ADC_Handler,            /* 23 Analog Digital Converter */
	(void*) AC_Handler,             /* 24 Analog Comparators */
	(void*) DAC_Handler,            /* 25 Digital Analog Converter */
	(void*) PTC_Handler,            /* 26 Peripheral Touch Controller */
	(void*) I2S_Handler             /* 27 Inter-IC Sound Interface */
};

// Called on system resets
void Reset_Handler( void )
{
	uint32_t *pSrc
	uint32_t *pDest;

	// Initialize the relocate segment
	pSrc    = &_etext;
	pDest   = &_srelocate;

	if( pSrc != pDest ) 
	{
		for( ; pDest < &_erelocate; ) 
		{
			*pDest++ = *pSrc++;
		}
	}

	// Clear the zero segment
	for( pDest = &_szero; pDest < &_ezero; ) 
	{
		*pDest++ = 0;
	}

	// Set the vector table base address
	pSrc 		= (uint32_t *)&_sfixed;
	SCB->VTOR 	= ((uint32_t) pSrc & SCB_VTOR_TBLOFF_Msk);

	// Initialize arrays and run constructors
	__libc_init_array();

	// Branch to main function
	main();

	// Infinite loop
	while(1){}
}

// Default interrupt handler for unused IRQs.
void Dummy_Handler(void)
{
	while(1){}
}

// TODO: Implement sys::time to replace delay and set the systick handler