#include "soft_aes.h"
#include "hal_aes_api.h"
//#include "trace.h"

uint32_t HAL_ADD_PKCS7Padding(uint8_t* buffer, uint32_t sz)
{
	unsigned int i = 0;
	uint8_t *paddingBuf = buffer;
	uint8_t  padding = MAX_PADDING_SIZE - (sz % MAX_PADDING_SIZE);
	
	for(i = sz; i < sz + padding; i++)
		paddingBuf[i] = padding;	
	return i;
}

// return 0:err other:ok
uint32_t HAL_GET_PKCS7Padding(uint8_t* buffer, uint32_t sz)
{
	uint8_t padding_len = 0, i=0;
	uint32_t outsz = 0;
	
	padding_len = buffer[sz-1];
	if( sz > padding_len ){
		for(i=0; i<padding_len; i++ ){
			if(padding_len != buffer[sz-1-i]){
				break;
			}
		}
		if( i == padding_len ){
			outsz = sz - padding_len;
		}
	}
	
	return outsz;
}



int HAL_AesEcbEncrypt(const char* key, uint8_t* in, uint8_t* out, uint32_t sz)
{
	int len = 0;
    aes_context aes_t;
	memset( &aes_t, 0, sizeof(aes_context) );
	aes_setkey_enc(&aes_t, (unsigned char*)key, 128);
	while(len < sz) {
		aes_crypt_ecb( &aes_t, AES_ENCRYPT, in+len, out+len );
		len += 16;
	}
	return 0;
}

int HAL_AesEcbDecrypt(const char* key, uint8_t* in, uint8_t* out, uint32_t sz)
{
	int len = 0;
    aes_context aes_t;
	memset( &aes_t, 0, sizeof(aes_context) );
	aes_setkey_dec(&aes_t, (unsigned char*)key, 128);
	while(len < sz) {
		aes_crypt_ecb( &aes_t, AES_DECRYPT, in+len, out+len );
		len += 16;
	}
	return 0;
}



#if TEST_AES128_ECB_PKCS7==1
//#include "mesh_log.h"
uint8_t testindata[256],testoutdata[256];
void hal_test_aes128_ecb_pkcs7(uint16_t scrlen)
{
	uint16_t i=0,destlen,tmp;
	for(i=0;i<256;i++){
		testindata[i] = i;
		testoutdata[i] = 0;
	}
	
	destlen = HAL_ADD_PKCS7Padding(testindata, scrlen);	
	HAL_AesEcbEncrypt(PACKET_PRIVATE_KEY, testindata, testoutdata, destlen);
	
//	 DBG_DIRECT("HAL_AesEcbEncrypt srclen%d %d\n",scrlen,destlen);
    for (i = 0; i < destlen; i++) {
//        DBG_DIRECT("%04d %02X, %02X", i, testindata[i], testoutdata[i]);
    }
	
	for(i=0;i<256;i++){
		testindata[i] = 0;
	}
	HAL_AesEcbDecrypt(PACKET_PRIVATE_KEY, testoutdata, testindata, destlen);
	tmp = HAL_GET_PKCS7Padding(testindata, destlen);
//	 DBG_DIRECT("HAL_AesEcbDecrypt srclen%d %d\n",tmp,destlen);
    for (i = 0; i < destlen; i++) {
//        DBG_DIRECT("%02X, %02X", testindata[i], testoutdata[i]);
    }
}
#endif














