#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <3ds.h>

#include "tvmgen_default.h"
#include "sample_person.h"
#include "sample_not_person.h"
#include "../include/tvm/runtime/crt/platform.h"




int main()
{
	//tvm model init
	TVMPlatformInitialize();
	
	//3ds init
	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);
	
	//set clock for timing image inference
	TickCounter clock = {
		.elapsed = 0,
		.reference = 0,
	};
	osTickCounterStart(&clock);
	
	//defines and sets input and output for model
	signed char output[2];
	struct tvmgen_default_inputs inputs = {
	.input_1_int8 = (void*)&sample_person,
	};
	struct tvmgen_default_outputs outputs = {
	.Identity_int8 = (void*)&output,
	};
	uint8_t sample = 0;
	uint32_t timer_val;
	char buf[50];
	uint16_t buf_len;
	
	
	// Main loop
	while (aptMainLoop())
	{
		gspWaitForVBlank();
		gfxSwapBuffers();
		hidScanInput();

		// Your code goes here
		
		printf("\x1b[3;1HEvaluating VWW model using microTVM\n");

		//run inference on both images and print result 
		if (sample == 0)
			inputs.input_1_int8 = (void*)&sample_person;
		else
			inputs.input_1_int8 = (void*)&sample_not_person;

		timer_val = osTickCounterRead(&clock);
		tvmgen_default_run(&inputs, &outputs);
		timer_val = osTickCounterRead(&clock) - timer_val;
		if (output[0] > output[1])
			printf( "Person not detected, inference time = %lu ms\r\n", timer_val);
		else
			printf( "Person detected, inference time = %lu ms\r\n", timer_val);

		sample++;
		if (sample == 2)
			sample = 0;		

		
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu
	}

	gfxExit();
	return 0;
}
