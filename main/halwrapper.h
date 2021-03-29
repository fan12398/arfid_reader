//
// Created by fantl on 2020/4/14.
//

#ifndef ARFID_READER_HALWRAPPER_H
#define ARFID_READER_HALWRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void spi0gpio24gpio27Init(void);
void spi0Trans(uint8_t* txbuff, uint16_t txlen, uint8_t* rxbuff, uint16_t rxlen);
uint8_t readGPIO24(void);
uint8_t readGPIO27(void);

void spi1gpio25gpio22Init(void);
void spi1Trans(uint8_t* txbuff, uint16_t txlen, uint8_t* rxbuff, uint16_t rxlen);
uint8_t readGPIO25(void);
uint8_t readGPIO22(void);

void sleep1ms(uint16_t cnt);
void sleep10us(uint16_t cnt);

#ifdef __cplusplus
}
#endif

#endif //ARFID_READER_HALWRAPPER_H
