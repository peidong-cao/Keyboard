#include "main.h"
#include "gpio.h"
#include "usbd_hid.h"

void Mykeyboard_Key_Scanf(GPIO_TypeDef *ROW_GPIOPort, const uint16_t *ROW_PIN_Number,
                          GPIO_TypeDef *COL_GPIOPort, const uint16_t *Col_PIN_Number);

void Mykeyboard_Key_Filter_Scanf(GPIO_TypeDef *ROW_F_GPIOPort, const uint16_t *ROW_F_PIN_Number,
                                 GPIO_TypeDef *COL_F_GPIOPort, const uint16_t *Col_F_PIN_Number);

void Mykeyboard_Key_Filter_Ready();

void Mykeyboard_Key_Text();

void Mykeyboard_Key_Send();

void USBD_HID_DataInCallback(USBD_HandleTypeDef *pdev, uint8_t epnum);
