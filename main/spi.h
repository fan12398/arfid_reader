//
// Created by fantl on 2020/3/7.
//

#ifndef ARFID_READER_SPI_H
#define ARFID_READER_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

int spiReadWriteData(uint8_t channel, uint8_t* tx_data, uint8_t* rx_data, int len);
int spiHalfDuplex(uint8_t channel, uint8_t* tx_data, uint16_t tx_len, uint8_t* rx_data, uint16_t rx_len);
int spiSetupMode(uint8_t channel, int speed, int mode);
int spiSetup(uint8_t channel, int speed);

#ifdef __cplusplus
}
#endif

#endif //ARFID_READER_SPI_H
