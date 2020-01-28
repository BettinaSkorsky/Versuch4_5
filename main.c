#include "main.h"
#include "led.h"
#include "usart.h"

#include "led.h"

void draw_rect_center() {

	uint16_t rectSize = 100;
	uint16_t centerX  = BSP_LCD_GetXSize()/2;
	uint16_t centerY  = BSP_LCD_GetYSize()/2;
	uint16_t rectX    = centerX-rectSize/2;
	uint16_t rectY    = centerY-rectSize/2;
	BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
	BSP_LCD_FillRect(rectX, rectY, rectSize, rectSize); //fill with textcolor
}

void draw_text_center(char *text) {
	uint16_t centerX  = BSP_LCD_GetXSize()/2;
	uint16_t centerY  = BSP_LCD_GetYSize()/2;
	sFONT *textFont   = &Font16;
	uint16_t textH    = textFont->Height;
	uint16_t textW    = textFont->Width*strlen(text);
	BSP_LCD_SetFont(textFont);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_SetBackColor(LCD_COLOR_GREEN);
	BSP_LCD_DisplayStringAt(centerX-textW/2,centerY-textH/2, (uint8_t *)text, LEFT_MODE);
}

void init_ADC() {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 						// GPIOA clock an
	GPIOA->MODER |= GPIO_MODER_MODER0_0 | GPIO_MODER_MODER0_1; 	// Analog Mode

	RCC->APB2ENR |= RCC_APB2ENR_ADC3EN; 						//clock ADC3 an
	ADC->CCR  |= ADC_CCR_ADCPRE_0; 								// Clock auf Teiler vier
	ADC3->CR2 |= ADC_CR2_ADON;    								// ADC an, dauert bis aktiv.
	ADC3->CR1 |= ADC_CR1_RES_1;   								// 0b10 -> 8 Bit Aufloesung
	//ADC3->CR2 |= ADC_CR2_ALIGN;							    // Linksbuendig abspeichern
	ADC3->CR2 |= ADC_CR2_CONT;    								// Staendig wandeln
	ADC3->CR2 |= ADC_CR2_SWSTART; 								// Start der Wandlung(en)
}
int main(void) {
	int counter = 0;

	LCD_LOG_DeInit();
	BSP_LCD_Init();
	BSP_LCD_Clear(LCD_COLOR_YELLOW);
	draw_rect_center();
	draw_text_center("Hallo MIT Labor!");
	init_ADC();
	HAL_Delay(2000);
	BSP_LCD_Clear(LCD_COLOR_BLUE);
	while (1) {
		int adcres = (ADC3->DR&0xff);
		float result = 3.3f*adcres/255.f;
		double dresult = result;
		char buffer[20];
		snprintf(buffer, sizeof(buffer), "%5d:ADC=%4.2f", counter++, result);
		draw_text_center(buffer);

	}
}
