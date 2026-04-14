#include "07_Relay.h"


void MyRelay_Init(MyRelay_t *me,GPIO_TypeDef *RelayPort,uint16_t RelayPin)
{
    me->Port = RelayPort;
    me->Pin  = RelayPin;
    HAL_GPIO_WritePin(me->Port,me->Pin,GPIO_PIN_RESET);
    me -> run = false;
    me -> mocthoigian = HAL_GetTick();
}
void MyRelay_Toggle(MyRelay_t *me)
{
	HAL_GPIO_TogglePin(me->Port,me->Pin);
if(HAL_GPIO_ReadPin(me->Port,me->Pin)==GPIO_PIN_SET)
{
    me->run=true;
    me->mocthoigian = HAL_GetTick();
}
else me->run=false;
}
void MyRelay_Time(MyRelay_t *me)
{
    uint32_t hientai=HAL_GetTick();
    if(me->run==true){
        GPIO_PinState trangthai=HAL_GPIO_ReadPin(me->Port,me->Pin);
    if(trangthai==GPIO_PIN_SET){

    if( hientai-me -> mocthoigian >= 5000)
    {
        HAL_GPIO_WritePin(me->Port,me->Pin,GPIO_PIN_RESET);
        me->mocthoigian=hientai;
    }
    }

else
{
    if(hientai-me->mocthoigian >= 1000)
    {
        HAL_GPIO_WritePin(me->Port,me->Pin,GPIO_PIN_SET);
        me ->mocthoigian=hientai;
    }
}
}}

const Khunggio_t KhunggioList[]= {
		{6,55,9,32},
		{18,0,18,50}
};
bool MyRelay_TimeControll(MyRelay_t *me,Timer_t currenttime )
{
	uint16_t current=currenttime.Gio * 60 + currenttime.Phut ;
	for (uint8_t i = 0; i < sizeof(KhunggioList) / sizeof(Khunggio_t); i++)
	{
		 uint16_t start = KhunggioList[i].giobatdau*60 + KhunggioList[i].phutbatdau;
		 uint16_t end = KhunggioList[i].gioketthuc*60 + KhunggioList[i].phutketthuc;
		 if(current >= start && current <= end) return true;
	}
	return 0;

}
