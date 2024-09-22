#include "Mykeyboard.h"
#include "usart.h"
#include "stdio.h"
#include "usb_device.h"
#include "usbd_hid.h"
#include "main.h"
#include "usbd_hid.h"
#include "string.h"

extern USBD_HandleTypeDef hUsbDeviceFS;
const int Key_Value_Map[5][4] = {

    0x90, 0x54, 0x55, 0x56, // num,/,*,-
    0x5F, 0x60, 0x61, 0x57, // 7,8,9,+
    0x5C, 0x5D, 0x5E, 0x00, // 4,5,6,k
    0x59, 0x5A, 0x5B, 0x00, // 1,2,3,k
    0x62, 0x63, 0x58, 0x00  // 0,.,E,k

     /* 0x01, 0x02, 0x03, 0x04,
      0x05, 0x06, 0x07, 0x08,
      0x09, 0x0A, 0x0B, 0x0C,
      0x0D, 0x0E, 0x0F, 0x10,
      0x62, 0x12, 0x13, 0x14*/

};

const uint16_t Key_RowN[5] = { // 按键扫描行号

    KEY_R0_Pin,
    KEY_R1_Pin,
    KEY_R2_Pin,
    KEY_R3_Pin,
    KEY_R4_Pin

};

const uint16_t Key_ColN[4] = { // 按键扫描列号

    KEY_C0_Pin,
    KEY_C1_Pin,
    KEY_C2_Pin,
    KEY_C3_Pin

};

typedef struct Mykeyboard
{
    unsigned int Key_Press_Flag; // 键值
    unsigned int Key_Scanf_Flag;
    unsigned int RowN; // 行号
    unsigned int ColN; // 列号
} KeyScanfStructure;

KeyScanfStructure Key_Row_Col_Number[6] = {0, 0, 0, 0, 0, 0}; // 键值存储数组
uint8_t Keyboard_Key_Send[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

unsigned int Keyboard_Change_Flag = 0; // 按键检测更新标志
unsigned int Keyboard_Update_Flag = 0; // 上传标志
unsigned int Keyboard_Value_Flag = 0;  // 扫描到几个按键
volatile uint8_t USB_HID_Report_Ready = 1;

// 调用模板：Mykeyboard_Key_Scanf(ROW_GPIO_Port,Key_RowN,COL_GPIO_Port,Key_ColN);
void Mykeyboard_Key_Scanf(GPIO_TypeDef *ROW_GPIOPort, const uint16_t *ROW_PIN_Number,
                          GPIO_TypeDef *COL_GPIOPort, const uint16_t *Col_PIN_Number)
{

    KeyScanfStructure *Key_Scan = Key_Row_Col_Number; // 初始化Key_Scan指向结构体数组的首地址

    for (int i = 0; i < sizeof(Key_RowN) / sizeof(Key_RowN[0]); i++) // 行
    {

        HAL_GPIO_WritePin(ROW_GPIOPort, *(ROW_PIN_Number + i), GPIO_PIN_SET); // 拉高行GPIO管脚电平

        for (int j = 0; j < sizeof(Key_ColN) / sizeof(Key_ColN[0]); j++) // 列
        {
            if (HAL_GPIO_ReadPin(COL_GPIOPort, *(Col_PIN_Number + j)) && Keyboard_Value_Flag < 6)
            {
                (Key_Scan + Keyboard_Value_Flag)->Key_Scanf_Flag = 0x01;
                (Key_Scan + Keyboard_Value_Flag)->RowN = i;
                (Key_Scan + Keyboard_Value_Flag)->ColN = j;
                Keyboard_Value_Flag++;
            }
            if (Keyboard_Value_Flag == 6)
                break;
        }

        HAL_GPIO_WritePin(ROW_GPIOPort, *(ROW_PIN_Number + i), GPIO_PIN_RESET);

        if (Keyboard_Value_Flag == 6)
            break;
        HAL_Delay(1);
    }

    if (Keyboard_Value_Flag != 0)
    {
        Keyboard_Change_Flag = 1;
    }
}

void Mykeyboard_Key_Filter_Scanf(GPIO_TypeDef *ROW_F_GPIOPort, const uint16_t *ROW_F_PIN_Number,
                                 GPIO_TypeDef *COL_F_GPIOPort, const uint16_t *Col_F_PIN_Number)
{
    int Filter_ROW = 0;
    int Filter_COL = 0;
    KeyScanfStructure *Key_Filter_Scanf = Key_Row_Col_Number;
    for (int i = 0; i < Keyboard_Value_Flag; i++)
    {
        Filter_ROW = (Key_Filter_Scanf + i)->RowN;
        Filter_COL = (Key_Filter_Scanf + i)->ColN;
        HAL_GPIO_WritePin(ROW_F_GPIOPort, *(ROW_F_PIN_Number + Filter_ROW), GPIO_PIN_SET);

        if (HAL_GPIO_ReadPin(COL_F_GPIOPort, *(Col_F_PIN_Number + Filter_COL)))
        {
            (Key_Filter_Scanf + i)->Key_Press_Flag = 0x01;
        }

        HAL_GPIO_WritePin(ROW_F_GPIOPort, *(ROW_F_PIN_Number + Filter_ROW), GPIO_PIN_RESET);
    }
}

void Mykeyboard_Key_Filter_Ready()
{
    int Row_Ready = 0;
    int Col_Ready = 0;
    KeyScanfStructure *Key_Filter_Ready = Key_Row_Col_Number;

    for (int i = 0; i < Keyboard_Value_Flag; i++)
    {
        if (((Key_Filter_Ready + i)->Key_Press_Flag) & ((Key_Filter_Ready + i)->Key_Scanf_Flag))
        {
            Row_Ready = (Key_Filter_Ready + i)->RowN;
            Col_Ready = (Key_Filter_Ready + i)->ColN;
            Keyboard_Key_Send[i + 2] = *(*(Key_Value_Map + Row_Ready) + Col_Ready);
            //Keyboard_Key_Send[i] = *(*(Key_Value_Map + Row_Ready) + Col_Ready);
        }
        else
            Keyboard_Value_Flag--;
    }
}

void Mykeyboard_Key_Send()
{
    Mykeyboard_Key_Scanf(ROW_GPIO_Port, Key_RowN, COL_GPIO_Port, Key_ColN);
    if (Keyboard_Change_Flag)
    {
        delay_us(100);
        Mykeyboard_Key_Filter_Scanf(ROW_GPIO_Port, Key_RowN, COL_GPIO_Port, Key_ColN);
        Mykeyboard_Key_Filter_Ready();
        USBD_HID_SendReport(&hUsbDeviceFS, Keyboard_Key_Send, sizeof(Keyboard_Key_Send));
        Keyboard_Change_Flag = 0; // 置零状态改变标志
        Keyboard_Value_Flag = 0;  // 置零按键检测标志
    }
    memset(Keyboard_Key_Send, 0, sizeof(Keyboard_Key_Send));   // 清零发送数组
    memset(Key_Row_Col_Number, 0, sizeof(Key_Row_Col_Number)); // 清零结构体
    USBD_HID_SendReport(&hUsbDeviceFS, Keyboard_Key_Send, sizeof(Keyboard_Key_Send));
}

void Mykeyboard_Key_Text()
{
    Mykeyboard_Key_Scanf(ROW_GPIO_Port, Key_RowN, COL_GPIO_Port, Key_ColN);
    printf("K%d\n", Keyboard_Value_Flag);
    if (Keyboard_Change_Flag)
    {
        delay_us(100);
        Mykeyboard_Key_Filter_Scanf(ROW_GPIO_Port, Key_RowN, COL_GPIO_Port, Key_ColN);
        Mykeyboard_Key_Filter_Ready();
        for (int i = 0; i < Keyboard_Value_Flag; i++)
        {
            printf("第%d个元素：%d\n", i, Keyboard_Key_Send[i]);
        }
        printf("\n");
    }
    memset(Keyboard_Key_Send, 0, sizeof(Keyboard_Key_Send));   // 清零发送数组
    memset(Key_Row_Col_Number, 0, sizeof(Key_Row_Col_Number)); // 清零结构体
    Keyboard_Change_Flag = 0;
    Keyboard_Value_Flag = 0;
    HAL_Delay(500);
}