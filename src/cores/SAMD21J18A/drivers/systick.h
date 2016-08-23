#ifndef DRIVERS_SYSTICK_H
#define DRIVERS_SYSTICK_H

#include <stdint.h>
#include <variant.h>

namespace core
{
    namespace systick
    {
        void Initialize();

        uint32_t Ticks_ms();
        uint32_t Ticks_us();

        void Delay_ms( uint32_t msIn );

        inline void Delay_us( uint32_t usIn )
        {
            // From delay.h in Arduino's SAMD core

            if( usIn == 0 )
            {
                return;
            }

            /*
            *  The following loop:
            *
            *    for (; ul; ul--) {
            *      __asm__ volatile("");
            *    }
            *
            *  produce the following assembly code:
            *
            *    loop:
            *      subs r3, #1        // 1 Core cycle
            *      bne.n loop         // 1 Core cycle + 1 if branch is taken
            */

            // VARIANT_MCK / 1000000 == cycles needed to delay 1uS
            //                     3 == cycles used in a loop
            uint32_t n = usIn * (VARIANT_MCK / 1000000) / 3;
            __asm__ __volatile__(
                "1:              \n"
                "   sub %0, #1   \n" // substract 1 from %0 (n)
                "   bne 1b       \n" // if result is not 0 jump to 1
                : "+r" (n)           // '%0' is n variable with RW constraints
                :                    // no input
                :                    // no clobber
            );
            // https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html
            // https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html#Volatile
        }
    }   
}

#endif