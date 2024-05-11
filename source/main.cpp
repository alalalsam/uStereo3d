#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <3ds.h>

#include "tvmgen_default.h"
#include "sample_person.h"
#include "sample_not_person.h"

void TVMPlatformInitialize();

int main()
{
	//tvm model init
	TVMPlatformInitialize();

	
	
	
	
	
	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);

	// Main loop
	while (aptMainLoop())
	{
		gspWaitForVBlank();
		gfxSwapBuffers();
		hidScanInput();

		// Your code goes here
		
		printf("\x1b[3;1HHello, world!\n");
		
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu
	}

	gfxExit();
	return 0;
}
