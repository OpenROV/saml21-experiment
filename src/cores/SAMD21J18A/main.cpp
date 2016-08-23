#include <core.h>
#include <drivers/clocks.h>
#include <drivers/adc.h>
#include <drivers/systick.h>

void SysInit()
{
	// Set 1 Flash Wait State for 48MHz, cf tables 20.9 and 35.27 in SAMD21 Datasheet
  	NVMCTRL->CTRLB.bit.RWS = NVMCTRL_CTRLB_RWS_HALF_Val;

	// Initialize system clocks
	core::clocks::Initialize();

	// Initialize ADC subsystem
	core::adc::Initialize();

	// Initialize systick subsystem
	core::systick::Initialize();
}

int main()
{
	// Chip and peripheral initialization
	SysInit();

	Setup();

	// Program loop
	while(1)
	{
		Loop();
	}
}