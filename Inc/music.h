/*
 * music.h
 *
 *  Created on: Dec 12, 2023
 *      Author: Tomas
 */

#include "stm32f3xx.h"
#include "led.h"
#include <stdint.h>
#include <stdlib.h>

#define _AMP(x) ( x / 2 )


void MUSinit();

void playTone(uint16_t frequency, uint32_t time);

void playString(uint8_t *string, uint32_t timePlay, uint32_t pause);
void playNWA(int num);

int returnFreguency(char a);



#ifndef INC_MUSIC_H_
#define INC_MUSIC_H_



#endif /* INC_MUSIC_H_ */
