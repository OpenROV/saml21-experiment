#include "systick.h"

#include <sam.h>

uint32_t SystemCoreClock = 1000000;

namespace core
{
namespace systick
{

volatile uint32_t m_tickCount = 0;

void Initialize()
{
    // Set Systick to 1ms interval, common to all Cortex-M variants
    if ( SysTick_Config( SystemCoreClock / 1000 ) )
    {
        // Capture error
        while ( 1 ) ;
    }
}

void Delay_ms( uint32_t msIn )
{
    // TODO: This probably doesn't behave properly at m_tickCount wraparound (every 49 days)

    if( msIn == 0 )
    {
        return;
    }

    uint32_t start = m_tickCount;

    // Wait for tick count to increment the specified amount
    while ( m_tickCount - start < msIn ) ;
}

uint32_t Ticks_ms()
{
    return m_tickCount;
}

uint32_t Ticks_us()
{
    // From delay.c in Arduino's SAMD core
    uint32_t ticks, ticks2;
    uint32_t pend, pend2;
    uint32_t count, count2;

    ticks2  = SysTick->VAL;
    pend2   = !!( SCB->ICSR & SCB_ICSR_PENDSTSET_Msk )  ;
    count2  = m_tickCount;

    do
    {
        ticks   = ticks2;
        pend    = pend2;
        count   = count2;
        ticks2  = SysTick->VAL;
        pend2   = !!( SCB->ICSR & SCB_ICSR_PENDSTSET_Msk );
        count2  = m_tickCount;
    } while( ( pend != pend2 ) || ( count != count2 ) || ( ticks < ticks2 ) );

    // This is an optimization to turn a runtime division into two compile-time divisions and
    // a runtime multiplication and shift, saving a few cycles
    return ((count+pend) * 1000) + (((SysTick->LOAD  - ticks)*(1048576/(VARIANT_MCK/1000000)))>>20);
}

}
}

void SysTick_DefaultHandler(void)
{
    // Increment the tick count
    core::systick::m_tickCount++;
}