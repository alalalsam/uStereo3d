#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <3ds.h>

#include "tvmgen_default.h"
#include "sample_person.h"
#include "sample_not_person.h"
#include "../include/tvm/runtime/crt/platform.h"

//tvm runtime error logging (IDK how this works)
/*
void TVMLogf(const char* msg, ...) {
	char buffer[128];
	int size;
	va_list args;
	va_start(args, msg);
	size = TVMPlatformFormatMessage(buffer, 128, msg, args);
	va_end(args);
	printf("\x1b[3;1H %c", &hlpuart1);
}
*/

int main()
{
	//tvm model init
	TVMPlatformInitialize();
	
	//3ds init
	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);
	
	//set variables for displaying program run time
	float program_run_time = 0;
	TickCounter program_run_timer = {
		.elapsed = 0,
		.reference = 0,
	};
	osTickCounterStart(&program_run_timer);
	
	//set variables for recording image inference time
	TickCounter inference_timer = {
		.elapsed = 0,
		.reference = 0,
	};
	osTickCounterStart(&inference_timer);


	
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
		
		//print the total elapsed time
		osTickCounterUpdate(&program_run_timer);
		program_run_time += osTickCounterRead(&program_run_timer);
		printf("\x1b[3;1Hprogram time elapsed (ms): %f\n", program_run_time);
		
		
		//printf("current tick: %llu", program_timer.value_ms);
		

		
		printf("Evaluating VWW model using microTVM\n");
		
		//run inference on both images and print result 
		if (sample == 0)
			inputs.input_1_int8 = (void*)&sample_person;
		else
			inputs.input_1_int8 = (void*)&sample_not_person;

		osTickCounterUpdate(&inference_timer);
		tvmgen_default_run(&inputs, &outputs);
		osTickCounterUpdate(&inference_timer);
		timer_val = osTickCounterRead(&inference_timer);
		if (output[0] > output[1])
			printf( "Person not detected, inference time = %lu ms\n", timer_val);
		else
			printf( "\nPerson detected, inference time = %lu ms\n", timer_val);

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
