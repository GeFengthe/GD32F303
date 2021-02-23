
#ifndef HAL_AES_API_H
#define HAL_AES_API_H


#include <stdint.h>
#include <string.h>

#define MAX_PADDING_SIZE   (16) /*never changed*/
#define PACKET_PRIVATE_KEY "28D21314EB80A1C3" /*never changed*/
#define MAX_UDP_DGRAM_SIZE (1024)




extern uint32_t HAL_ADD_PKCS7Padding(uint8_t* buffer, uint32_t sz);
extern uint32_t HAL_GET_PKCS7Padding(uint8_t* buffer, uint32_t sz);
extern int HAL_AesEcbEncrypt(const char* key, uint8_t* in, uint8_t* out, uint32_t sz);
extern int HAL_AesEcbDecrypt(const char* key, uint8_t* in, uint8_t* out, uint32_t sz);


#define TEST_AES128_ECB_PKCS7  1
#if TEST_AES128_ECB_PKCS7==1
void hal_test_aes128_ecb_pkcs7(uint16_t scrlen);
#endif
	

#endif














