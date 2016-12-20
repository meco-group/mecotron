#ifndef MICROOS_H
#define MICROOS_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Print.h>
//#include <SPI.h>
//#include "eeprom_m.h"
#include "hal_base.h"
#include "communicator_interface.h"
#include "mavlink_communicator.h"
#include "thread.h"

/*Defines with respect to the automatic threads: onboard and communication */

#define MICROOS_SLOW_THREAD_ID				100
#define MICROOS_SLOW_THREAD_PERIOD			100000	//10Hz
#define MICROOS_FAST_THREAD_ID				101
#define MICROOS_FAST_THREAD_PERIOD			10000	//100Hz

#if defined(__AVR_ATmega328P__)
	#define MICROOS_LIGHT
#endif
	
#ifdef MICROOS_LIGHT
	#define MICROOS_PRINT_BUFFER_SIZE			32
	#define MICROOS_DEBUG_FLOAT_SIZE			4
	#define MICROOS_DEBUG_INT_SIZE				2
	#define MICROOS_NOPRINT
	//#define MICROOS_NOPRINTBUFFER
#else
	#define MICROOS_PRINT_BUFFER_SIZE			128
	#define MICROOS_DEBUG_FLOAT_SIZE			8
	#define MICROOS_DEBUG_INT_SIZE				4
#endif

#define MICROOS_I2C_ENABLE		1
#define MICROOS_SPI_ENABLE		1<<1
#define MICROOS_SLOW_DISABLE	1<<2
#define MICROOS_FAST_DISABLE	1<<3


typedef enum system_info_t {
	NOINFO = 0, 		//nothing particular to report
	BOOTING,			//state at startup
	RUNNING,			//state of normal operation
	HWFAIL,				//error when initializing the hardware
	OVERLOAD, 			//latency of the threads is to large
	ERROR = 255
} system_info_t;

typedef enum system_request_t {
	NOREQUEST = 0,
	THREADINFO,
	NEXTTHREADINFO,
	HWINFO
} system_request_t;

typedef enum system_run_t {
	PRIORITIZED = 0,
	RESCHEDULED
} system_run_t;

/*
	All static members to ensure there is only one copy of the OS!
*/
#ifndef MICROOS_NOPRINT
	class MicroOS : public Print
#else
	class MicroOS
#endif
{
private:
	system_info_t			_system_info;
	uint8_t					_system_request;
	float					_gpin_float[MICROOS_DEBUG_FLOAT_SIZE];
	int32_t					_gpin_int[MICROOS_DEBUG_INT_SIZE];
	float					_gpout_float[MICROOS_DEBUG_FLOAT_SIZE];
	int32_t					_gpout_int[MICROOS_DEBUG_INT_SIZE];
	
#ifndef MICROOS_NOPRINT
	char					_print_buffer[MICROOS_PRINT_BUFFER_SIZE];
	uint8_t					_print_buffer_head;
	uint8_t					_print_buffer_tail;
    size_t write(const uint8_t);
    size_t write(const uint8_t *buffer, size_t size);
#endif

	uint8_t					_thread_count; 
	uint8_t					_next_thread;	
	uint8_t					_scheduled_thread;
	Thread**				_threads;
	
	HALBase*				_hal;
	CommunicatorInterface*	_communicator;
	uint8_t					_config;
	uint8_t					_slowhook_splitcounter;
	
	int findThread(uint8_t ID);
	
public:
	MicroOS();		
	
	void setHAL(HALBase* hal);
	void setCommunicator(CommunicatorInterface* communicator);
	void configure(uint8_t config);
	void start(system_start_t mode = REGULAR);
	void run(system_run_t mode = RESCHEDULED);
	
	HALBase*				hal();
	CommunicatorInterface*	communicator();
	
	uint8_t addThread(priority_t priority, uint32_t period, int (*Fcn)(), bool start, uint8_t ID=0);
	uint8_t addThread(Thread *thread);
	Thread *thread(uint8_t ID);
	//TODO: deleteThread();

	void sendSystemRequest(uint8_t system_request);
	void handleSystemRequest(void);
    void sendNextThreadInfo(void);
    void sendAllThreadInfo(void);

#ifndef MICROOS_NOPRINT
	void write();
#else
	void println(const char *text);
	void println(const __FlashStringHelper *text);
#endif
	
	float* getGPinFloat(void);
	int32_t* getGPinInt(void);
	float getGPinFloat(uint8_t index);
	int32_t getGPinInt(uint8_t index);
	void setGPinFloat(uint8_t index, float value);
	void setGPinInt(uint8_t index, int32_t value);
	float* getGPoutFloat(void);
	int32_t* getGPoutInt(void);
	float getGPoutFloat(uint8_t index);
	int32_t getGPoutInt(uint8_t index);
	void setGPoutFloat(uint8_t index, float value);
	void setGPoutInt(uint8_t index, int32_t value);
};

extern MicroOS System;

#endif //MICROOS_H
