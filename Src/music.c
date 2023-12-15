/*
 * music.c
 *
 *  Created on: Dec 12, 2023
 *      Author: Tomas
 */

#include "music.h"

char keys[] = {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'u', 'i', 'o'};
int frequencies[] = {2093, 2217, 2349, 2489, 2637, 2793, 2959, 3135, 3322, 3520, 3729, 3951};


const size_t SINE_SAMPLES = 32;
const uint16_t SINE_WAVE[] = {
  _AMP(2048), _AMP(2447), _AMP(2831), _AMP(3185),
  _AMP(3495), _AMP(3750), _AMP(3939), _AMP(4056),
  _AMP(4095), _AMP(4056), _AMP(3939), _AMP(3750),
  _AMP(3495), _AMP(3185), _AMP(2831), _AMP(2447),
  _AMP(2048), _AMP(1649), _AMP(1265), _AMP(911),
  _AMP(601),  _AMP(346),  _AMP(157),  _AMP(40),
  _AMP(0),    _AMP(40),   _AMP(157),  _AMP(346),
  _AMP(601),  _AMP(911),  _AMP(1265), _AMP(1649)
};

void MUSinit() {
	// Nastavenie hodinových signálov
	RCC->AHBENR   |= (RCC_AHBENR_GPIOAEN | RCC_AHBENR_DMA1EN);
	RCC->APB1ENR  |= (RCC_APB1ENR_DAC1EN | RCC_APB1ENR_TIM6EN);
	RCC->APB2ENR  |= RCC_APB2ENR_SYSCFGEN;

	// Nastavenie GPIO A4: analog mode
	GPIOA->MODER    &= ~(0x3 << (4 * 2));
	GPIOA->MODER    |= (0x3 << (4 * 2));

	// Nastavenie 'TIM6/DAC1 remap' bitu
	SYSCFG->CFGR1 |= SYSCFG_CFGR1_TIM6DAC1Ch1_DMA_RMP;

	// DMA konfigurácia
	DMA1_Channel3->CCR &= ~(DMA_CCR_MEM2MEM | DMA_CCR_PL | DMA_CCR_MSIZE | DMA_CCR_PSIZE | DMA_CCR_PINC | DMA_CCR_EN);
	DMA1_Channel3->CCR |= ((0x2 << DMA_CCR_PL_Pos) | (0x1 << DMA_CCR_MSIZE_Pos) | (0x1 << DMA_CCR_PSIZE_Pos) | DMA_CCR_MINC | DMA_CCR_CIRC | DMA_CCR_DIR);
	DMA1_Channel3->CMAR  = (uint32_t)&SINE_WAVE;
	DMA1_Channel3->CPAR  = (uint32_t)&(DAC1->DHR12R1);
	DMA1_Channel3->CNDTR = SINE_SAMPLES;

	// DAC konfigurácia
	DAC1->CR &= ~(DAC_CR_TSEL1);
	DAC1->CR |= (DAC_CR_DMAEN1 | DAC_CR_EN1);

	// Krátky delay na stabilizáciu
	LL_mDelay(100);
}

void playTone(uint16_t frequency, uint32_t time){
	uint32_t SystemCoreCloc = 8000000;
    // Nastavenie časovača TIM6 pre danú frekvenciu
    TIM6->PSC = 0x0000;
    TIM6->ARR = SystemCoreCloc / (frequency * SINE_SAMPLES);

    // Spustenie časovača a triggeru pre DAC
    TIM6->CR2 |= (0x2 << TIM_CR2_MMS_Pos);
    TIM6->CR1 |= TIM_CR1_CEN;
    DAC1->CR |= DAC_CR_TEN1;

    // Spustenie DMA
    DMA1_Channel3->CCR |= DMA_CCR_EN;

    // Prehrávanie tónu na x sekund
    LL_mDelay(time); // Funkcia na oneskorenie

    // Zastavenie DMA a časovača
    DMA1_Channel3->CCR &= ~DMA_CCR_EN;
    TIM6->CR1 &= ~TIM_CR1_CEN;
}

void playString(uint8_t *string, uint32_t timePlay, uint32_t pause){

	while (*string != '\0') {

	        if (returnFreguency(*string) != 0){
	        	playTone(returnFreguency(*string),timePlay);
	        	LL_mDelay(pause);
	        }

	        string++;
	    }

}


int returnFreguency(char a){

	for (uint8_t j = 0 ; sizeof(keys)>= j ; j++){
		if(a == keys[j]){
			return frequencies[j];
		}
	}

	return 0;
}


void playNWA(int num){
	for (uint8_t replay = 0; replay < num ;replay++)
	  {
		  playTone(2093,200);
		  LL_mDelay(100);
		  playTone(1567,200);
		  LL_mDelay(500);
		  playTone(1567,200);
		  LL_mDelay(100);
		  playTone(2093,200);
		  LL_mDelay(100);
		  playTone(1567,200);
		  LL_mDelay(700);
	  }
}
