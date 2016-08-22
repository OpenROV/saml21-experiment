#include <core.h>

void SysInit()
{
	// Set 1 Flash Wait State for 48MHz, cf tables 20.9 and 35.27 in SAMD21 Datasheet
  	NVMCTRL->CTRLB.bit.RWS = NVMCTRL_CTRLB_RWS_HALF_Val ;
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