#ifndef __GLOBALS_H_
#define __GLOBALS_H_

#include <windows.h>
#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

enum compType
{
	MESH = 0,
	MATERIAL,
	TRANSFORM,
	LIGHT
};

// Deletes a buffer
#define RELEASE( x ) \
    {\
       if( x != nullptr )\
       {\
         delete x;\
	     x = nullptr;\
       }\
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
	{\
       if( x != nullptr )\
       {\
           delete[] x;\
	       x = nullptr;\
		 }\
	 }

#define ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

#define DEFAULT_GO_NAME "PlaceHolder"

// Configuration -----------
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define SCREEN_SIZE 2
#define FULLSCREEN false
#define RESIZABLE_WINDOW true
#define VSYNC true
#define TITLE "Citadel Motor"

#endif // __GLOBALS_H_