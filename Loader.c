//ver1
#include <csl.h>
#include <string.h>

#pragma CODE_SECTION ( loader , ".boot_load");
void loader(){
	Uint8* rom = (Uint8*) 0xB0001000;
	Uint8* dst;
	Uint8 a0, a1, a2, a3;
	Uint32 jump, size, i;
	void (*init)();

    Uint32* PLLCTL  = (Uint32*) 0x029A0100;
    Uint32* PLLM  = (Uint32*) 0x029A0110;
    Uint32* PREDIV  = (Uint32*) 0x029A0114;
    Uint32* PLLCMD  = (Uint32*) 0x029A0138;
    Uint32* PLLDIV4  = (Uint32*) 0x029A0160;

	//PLL to become	stable
	for(i = 0; i < 1400; i++){	asm(" nop 5");	}

	*PLLCTL  = (Uint32) 0x0; //In PLLCTL, write PLLENSRC = 0 (enable PLLEN bit).
	*PLLCTL  = (Uint32) 0x0; //In PLLCTL, write PLLEN = 0 (bypass mode).
	asm(" nop 5");
	*PLLCTL  = (Uint32) 0x8; //In PLLCTL, write PLLRST = 1 (PLL is reset).

	*PLLM  	= (Uint32) 0x0000001D;
	*PREDIV  = (Uint32) 0x00008000;
	*PLLDIV4 = (Uint32) 0x00008003;
	*PLLCMD = (Uint32)0x1;
//Wait for PLL to properly reset. See device-specific data manual for PLL reset time.
	for(i = 0; i < 16; i++){	asm(" nop 8");	}

	*PLLCTL  = (Uint32) 0x0;
//Wait for PLL to lock. See device-specific data manual for PLL lock time.
	for(i = 0; i < 400; i++){	asm(" nop 5");	}

	*PLLCTL  = (Uint32) 0x1;

	Uint32* PERLOCK = (Uint32*)0x02AC0004;
	Uint32* PERCFG0 = (Uint32*)0x02AC0008;
	Uint32* PERSTAT0 = (Uint32*)0x02AC0014;

    Uint32* GPIO_DIR = (Uint32*) 0x02B00010;
    Uint32* GPIO_OUT = (Uint32*) 0x02B00014;

    if ((*PERSTAT0 & 0x8000) == 0){
		*PERLOCK = (Uint32) 0x0F0A0B00;
		*PERCFG0 |= (Uint32) 0x400;
		while((*PERSTAT0 & 0x8000) == 0){}
    }
    *GPIO_DIR = (Uint32)0xCF3F;

	a0=*rom++;	a1=*rom++;	a2=*rom++;	a3=*rom++;
	jump = (a3<<24) + (a2<<16) + (a1<<8) + a0;

	// .text
	a0=*rom++;	a1=*rom++;	a2=*rom++;	a3=*rom++;
	size = ((a3<<24) + (a2<<16) + (a1<<8) + a0);

	a0=*rom++;	a1=*rom++;	a2=*rom++;	a3=*rom++;
	dst = (Uint8*)((a3<<24) + (a2<<16) + (a1<<8) + a0);

	for(i = 0; i < size; i++){*dst++ = *rom++;}

    *GPIO_OUT = (Uint32)0x2000;
	// .switch
	a0=*rom++;	a1=*rom++;	a2=*rom++;	a3=*rom++;
	size = ((a3<<24) + (a2<<16) + (a1<<8) + a0);

	a0=*rom++;	a1=*rom++;	a2=*rom++;	a3=*rom++;
	dst = (Uint8*)((a3<<24) + (a2<<16) + (a1<<8) + a0);

	for(i = 0; i < size; i++){*dst++ = *rom++;}

    *GPIO_OUT = (Uint32)0x3000;
	// .cinit

	a0=*rom++;	a1=*rom++;	a2=*rom++;	a3=*rom++;
	size = ((a3<<24) + (a2<<16) + (a1<<8) + a0);

	a0=*rom++;	a1=*rom++;	a2=*rom++;	a3=*rom++;
	dst = (Uint8*)((a3<<24) + (a2<<16) + (a1<<8) + a0);

	for(i = 0; i < size; i++){*dst++ = *rom++;}

    *GPIO_OUT = (Uint32)0x3080;
	// .csl_vect

	a0=*rom++;	a1=*rom++;	a2=*rom++;	a3=*rom++;
	size = ((a3<<24) + (a2<<16) + (a1<<8) + a0);

	a0=*rom++;	a1=*rom++;	a2=*rom++;	a3=*rom++;
	dst = (Uint8*)((a3<<24) + (a2<<16) + (a1<<8) + a0);

	for(i = 0; i < size; i++){*dst++ = *rom++;}

//    for(i=0;i<0x1000;i++){ asm(" nop 9");}

    *GPIO_OUT = (Uint32)0x30C0;

	init = (void*) jump;
	(*init)();
	asm(" nop 9");
	asm(" nop 9");
	asm(" nop 9");
	asm(" nop 9");
}
