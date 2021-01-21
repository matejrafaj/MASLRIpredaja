#ifndef OV7670_H
#define OV7670_H

#include <xil_types.h>

#include "auto_contrast_state.h"

int Camera_I2C_Write(u8 addr, u8 data);
int Camera_I2C_Read(u8 addr, u8 *buffer);

void Camera_determineImageOrientation(u8 vertical, u8 horizontal);

void Camera_TestBarMode();	// Configures camera to return the test bar
void Camera_ImageMode();	// Configures camera to take pictures

// Must be called before any other functions from this library
int Camera_InitI2c();

unsigned char *Camera_getRGBImage();
int Camera_getJPEG(unsigned char **data, auto_contrast_state acs, u8 enc);

#endif


