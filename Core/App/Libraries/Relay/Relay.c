#include "Relay.h"
#include"main.h"
void KhoiTaoCacChan(Relay_t *r)
{
    HAL_GPIO_WritePin(ST_RELAY1_GPIO_Port, ST_RELAY1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ST_RELAY2_GPIO_Port, ST_RELAY2_Pin, GPIO_PIN_RESET);

    r->K1   = _OFF;
    r->K2   = _OFF;
    r->MODE = _MANUAL;
}

void OFFCacRole(Relay_t *r)
{
    HAL_GPIO_WritePin(ST_RELAY1_GPIO_Port, ST_RELAY1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ST_RELAY2_GPIO_Port, ST_RELAY2_Pin, GPIO_PIN_RESET);

    r->K1 = _OFF;
    r->K2 = _OFF;
}

void ONCacRole(Relay_t *r)
{
    HAL_GPIO_WritePin(ST_RELAY1_GPIO_Port, ST_RELAY1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ST_RELAY2_GPIO_Port, ST_RELAY2_Pin, GPIO_PIN_SET);

    r->K1 = _ON;
    r->K2 = _ON;
}

bool ONOFFBangTayK1(Relay_t *r, int v)
{
    r->K1 = (v == _ON ? _ON : _OFF);

    if (r->MODE == _MANUAL)
    {
        HAL_GPIO_WritePin(ST_RELAY1_GPIO_Port,
                          ST_RELAY1_Pin,
                          (r->K1 == _ON ? GPIO_PIN_SET : GPIO_PIN_RESET));
    }

    return true;
}

bool ONOFFBangTayK2(Relay_t *r, int v)
{
    r->K2 = (v == _ON ? _ON : _OFF);

    if (r->MODE == _MANUAL)
    {
        HAL_GPIO_WritePin(ST_RELAY2_GPIO_Port,
                          ST_RELAY2_Pin,
                          (r->K2 == _ON ? GPIO_PIN_SET : GPIO_PIN_RESET));
    }

    return true;
}
