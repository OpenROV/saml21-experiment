#include "clocks.h"

#include <sam.h>
#include <variant.h>

namespace core
{
namespace clocks
{

// Clock generator IDs used for the various clocks
constexpr uint32_t GENERIC_CLOCK_GENERATOR_MAIN         = 0;
constexpr uint32_t GENERIC_CLOCK_GENERATOR_XOSC32K      = 1;
constexpr uint32_t GENERIC_CLOCK_GENERATOR_OSCULP32K    = 2;
constexpr uint32_t GENERIC_CLOCK_GENERATOR_OSC8M        = 3;

// Clock multiplexer IDs
constexpr uint32_t GENERIC_CLOCK_MULTIPLEXER_DFLL48M    = 0;

void Initialize()
{
    // Enable generic clock in power manager
    PM->APBAMASK.reg |= PM_APBAMASK_GCLK;

    // ----------------------------------------------------------------------------------------------
    // 1) Enable XOSC32K clock (External on-board 32.768Hz oscillator)
    SYSCTRL->XOSC32K.reg = SYSCTRL_XOSC32K_STARTUP( 0x6u )  // 2-second startup time
                            | SYSCTRL_XOSC32K_XTALEN        // Enable crystal
                            | SYSCTRL_XOSC32K_EN32K         // Enable 32khz output (XOUT32)
                            | SYSCTRL_XOSC32K_ENABLE;       // Enable clock

    // Wait for oscillator stabilization
    while( ( SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_XOSC32KRDY ) == 0 ){}

    // Reset the GCLK peripheral
    GCLK->CTRL.reg = GCLK_CTRL_SWRST;

    // Wait for reset to complete
    while( (GCLK->CTRL.reg & GCLK_CTRL_SWRST) && (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) ){}

    // ----------------------------------------------------------------------------------------------
    // 2) Put XOSC32K as source of Generic Clock Generator 1

    // Select clock generator 1
    GCLK->GENDIV.reg = GCLK_GENDIV_ID( GENERIC_CLOCK_GENERATOR_XOSC32K ) ; 

    // Sync
    while( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY ){}

    // Write Generic Clock Generator 1 configuration
    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID( GENERIC_CLOCK_GENERATOR_XOSC32K )  // Generic Clock Generator 1
                        | GCLK_GENCTRL_SRC_XOSC32K                          // Selected source is External 32KHz Oscillator
                        | GCLK_GENCTRL_GENEN;                               // Enable generator

    // Sync
    while( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY ){}

    // ----------------------------------------------------------------------------------------------
    // 3) Put Generic Clock Generator 1 as source for Generic Clock Multiplexer 0 (DFLL48M reference)

    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID( GENERIC_CLOCK_MULTIPLEXER_DFLL48M )    // Select Generic Clock 0
                        | GCLK_CLKCTRL_GEN_GCLK1                                // Set Generic Clock Generator 1 as source generator
                        | GCLK_CLKCTRL_CLKEN;                                   // Enable Clock

    // Sync
    while( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY ){}

    // ----------------------------------------------------------------------------------------------
    // 4) Enable DFLL48M clock in Closed Loop mode 

    // Remove the OnDemand mode - Errata: 40.1.1.4 DFLL48M-1
    SYSCTRL->DFLLCTRL.bit.ONDEMAND = 0;

    // Sync
    while( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0 ){}

    // Configure DFLL to run at the speed specified by the variant
    SYSCTRL->DFLLMUL.reg = SYSCTRL_DFLLMUL_CSTEP( 31 )                                  // Coarse step is 31, half of the max value
                           | SYSCTRL_DFLLMUL_FSTEP( 511 )                               // Fine step is 511, half of the max value
                           | SYSCTRL_DFLLMUL_MUL( ( VARIANT_MCK / VARIANT_MAINOSC ) );  // External 32KHz is the reference

    // Sync
    while( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0 ){}

    // Finish configuring DFLL
    SYSCTRL->DFLLCTRL.reg |= SYSCTRL_DFLLCTRL_MODE          // Enable closed loop mode
                            | SYSCTRL_DFLLCTRL_WAITLOCK     // Output clock once DFLL is locked
                            | SYSCTRL_DFLLCTRL_QLDIS ;      // Disable Quick lock

    // Sync
    while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0 ){}

    // Enable the DFLL
    SYSCTRL->DFLLCTRL.reg |= SYSCTRL_DFLLCTRL_ENABLE ;

    // Wait for DFLL lock flags
    while( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLLCKC) == 0 || (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLLCKF) == 0 ){}

    // Sync
    while( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0 ){}

    // ----------------------------------------------------------------------------------------------
    // 5) Switch Generic Clock Generator 0 to DFLL48M. CPU will run at 48MHz.

    // Select Generic Clock Generator 0
    GCLK->GENDIV.reg = GCLK_GENDIV_ID( GENERIC_CLOCK_GENERATOR_MAIN ) ; 

    // Sync
    while( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY ){}

    // Configure generic clock generator 0
    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID( GENERIC_CLOCK_GENERATOR_MAIN ) // Generic Clock Generator 0
                        | GCLK_GENCTRL_SRC_DFLL48M                      // Select DFLL48M as source
                        | GCLK_GENCTRL_IDC                              // Set 50/50 duty cycle
                        | GCLK_GENCTRL_GENEN;                           // Enable generator

    // Sync
    while( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY ){}

    // ----------------------------------------------------------------------------------------------
    // 6) Modify PRESCaler value of OSC8M to have 8MHz

    
    SYSCTRL->OSC8M.bit.PRESC = SYSCTRL_OSC8M_PRESC_1_Val ;
    SYSCTRL->OSC8M.bit.ONDEMAND = 0 ;

    // ----------------------------------------------------------------------------------------------
    // 7) Put OSC8M as source for Generic Clock Generator 3 (we don't actually use this for anything right now)
    
    // Select clock generator 3
    GCLK->GENDIV.reg = GCLK_GENDIV_ID( GENERIC_CLOCK_GENERATOR_OSC8M );

    // Configure generator
    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID( GENERIC_CLOCK_GENERATOR_OSC8M )    // Generic Clock Generator 3
                        | GCLK_GENCTRL_SRC_OSC8M                            // Selected source is RC OSC 8MHz (already enabled at reset)
                        | GCLK_GENCTRL_GENEN;                               // Enable generator

    // Sync
    while( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY ){}

    // Set CPU and APBx clock speeds
    PM->CPUSEL.reg  = PM_CPUSEL_CPUDIV_DIV1;
    PM->APBASEL.reg = PM_APBASEL_APBADIV_DIV1_Val;
    PM->APBBSEL.reg = PM_APBBSEL_APBBDIV_DIV1_Val;
    PM->APBCSEL.reg = PM_APBCSEL_APBCDIV_DIV1_Val;

    // Set System Core Clock value
    SystemCoreClock = VARIANT_MCK;
}

}
}