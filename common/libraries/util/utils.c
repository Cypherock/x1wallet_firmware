/**
 * @file    utils.c
 * @author  Cypherock X1 Team
 * @brief   Title of the file.
 *          Short description of the file
 * @copyright Copyright (c) 2022 HODL TECH PTE LTD
 * <br/> You may obtain a copy of license at <a href="https://mitcc.org/" target=_blank>https://mitcc.org/</a>
 * 
 ******************************************************************************
 * @attention
 *
 * (c) Copyright 2022 by HODL TECH PTE LTD
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *  
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *  
 *  
 * "Commons Clause" License Condition v1.0
 *  
 * The Software is provided to you by the Licensor under the License,
 * as defined below, subject to the following condition.
 *  
 * Without limiting other conditions in the License, the grant of
 * rights under the License will not include, and the License does not
 * grant to you, the right to Sell the Software.
 *  
 * For purposes of the foregoing, "Sell" means practicing any or all
 * of the rights granted to you under the License to provide to third
 * parties, for a fee or other consideration (including without
 * limitation fees for hosting or consulting/ support services related
 * to the Software), a product or service whose value derives, entirely
 * or substantially, from the functionality of the Software. Any license
 * notice or attribution required by the License must also include
 * this Commons Clause License Condition notice.
 *  
 * Software: All X1Wallet associated files.
 * License: MIT
 * Licensor: HODL TECH PTE LTD
 *
 ******************************************************************************
 */
#include "utils.h"
#include "bip32.h"
#include "bip39.h"
#include "sha2.h"
#include "curves.h"
#include "wallet.h"
#include "crypto_random.h"
#include "cryptoauthlib.h"
#include "assert_conf.h"
#include "logger.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief struct for
 * @details
 *
 * @see
 * @since v1.0.0
 *
 * @note
 */
typedef struct {
    void *mem;
    size_t mem_size;
} memory_list_t;

/**
 * @brief struct for
 * @details
 *
 * @see
 * @since v1.0.0
 *
 * @note
 */
typedef struct cy_linked_list {
    struct cy_linked_list *next;
    memory_list_t mem_entry;
} cy_linked_list_t;

static cy_linked_list_t *memory_list = NULL;

void * cy_malloc(size_t mem_size) {
    cy_linked_list_t *new_entry = (cy_linked_list_t *) malloc(sizeof(cy_linked_list_t));
    new_entry->mem_entry.mem = malloc(mem_size);
    ASSERT(new_entry != NULL && new_entry->mem_entry.mem != NULL);

    new_entry->next = memory_list;
    new_entry->mem_entry.mem_size = mem_size;
    memory_list = new_entry;
    memzero(new_entry->mem_entry.mem, mem_size);
    return new_entry->mem_entry.mem;
}

void cy_free() {
    cy_linked_list_t *temp = memory_list;
    while ((temp = memory_list) != NULL) {
        memzero(temp->mem_entry.mem, temp->mem_entry.mem_size);
        free(temp->mem_entry.mem);
        memory_list = memory_list->next;
        memzero(temp, sizeof(cy_linked_list_t));
        free(temp);
    }
}

int is_zero(const uint8_t *bytes, const uint8_t len)
{
    if (len == 0)
        return 1;

    for (int i = 0; i < len; i++) {
        if (bytes[i])
            return 0;
    }

    return 1;
}

/// Array of hex characters
uint8_t map[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                 '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

uint32_t byte_array_to_hex_string(const uint8_t *bytes, const uint32_t byte_len, char *hex_str, size_t str_len)
{
    if (bytes == NULL || hex_str == NULL || byte_len < 1 || str_len < 2) return 0;
    if ((2 * byte_len + 1) > str_len) return 0;
    uint32_t i = 0;
    for (i = 0; i < byte_len; i++) {
        hex_str[i * 2] = map[(bytes[i] & 0xf0) >> 4];
        hex_str[i * 2 + 1] = map[bytes[i] & 0x0f];
    }
    hex_str[i * 2] = '\0';
    return (i * 2 + 1);
}

void __single_to_multi_line(const char* input, const uint16_t input_len, char output[24][15])
{
    if (input == NULL || output == NULL) return;
    uint16_t i = 0U;
    uint16_t j;
    uint16_t l = 0U;
    while (1) {
        j = 0U;
        while (input[l] != ' ' && l < input_len) {
            output[i][j] = input[l];
            l++;
            j++;
        }
        output[i][j] = 0U;
        i++;
        if (l >= input_len) {
            return;
        }
        l++;
    }
}

void __multi_to_single_line(const char input[24][15], const uint8_t number_of_mnemonics, char* output)
{
    if (input == NULL || output == NULL) return;
    uint8_t word_len;
    uint16_t offset = 0U;
    uint16_t i = 0U;
    for (; i < number_of_mnemonics; i++) {
        word_len = strlen(input[i]);
        memcpy(output + offset, input[i], word_len);
        offset += word_len;
        memcpy(output + offset, " ", 1);
        offset++;
    }
    offset--;
    uint8_t zero=0;
    memcpy(output + offset, &zero, 1);
}

void hex_string_to_byte_array(const char* hex_string, const uint32_t string_length, uint8_t* byte_array)
{
    char hex[3] = {'\0'};

    for (int i = 0; i < string_length; i += 2) {
        hex[0] = hex_string[i];
        hex[1] = hex_string[i + 1];
        byte_array[i / 2] = (uint8_t)strtol(hex, NULL, 16);
    }
}

void print_hex_array(const char text[], const uint8_t* arr, const uint8_t length)
{
    printf("%s %d\n", text, length);

    for (uint8_t i = 0U; i < length; i++) {
        printf("%02X ", arr[i]);
    }
    printf("\n");
}

void byte_array_re(uint8_t* input_output, uint16_t len)
{
    uint8_t* temp_output = (uint8_t*)malloc(len);
    ASSERT(temp_output != NULL);
    uint16_t output_array_index = 0U;
    while (output_array_index < len) {
        temp_output[output_array_index] = input_output[len - 1 - output_array_index];
        output_array_index++;
    }
    memcpy(input_output, temp_output, len);
    free(temp_output);
}

uint8_t decode_card_number(const uint8_t encoded_card_number)
{
    switch (encoded_card_number) {
    case 1U:
        return 1;
    case 2U:
        return 2;
    case 4U:
        return 3;
    case 8U:
        return 4;
    default:
        return 1;
    }
}

uint8_t encode_card_number(const uint8_t decoded_card_number)
{
    uint8_t output = 1U;

    for (uint8_t i = 0U; i < (decoded_card_number - 1U); i++) {
        output = output << 1U;
    }

    return output;
}
void get_firmaware_version(uint16_t pid, const char *product_hash , char message[20]){
    uint16_t len=0;
    char pid_array[4] = {'0','0','0','0'};
    while(pid > 0 && len < sizeof(pid_array))
    { 
       pid_array[3-len] = (char)('0' + pid%10);
       pid = pid / 10;
       len = len + 1;  
    }
    message[0] = pid_array[0],message[1] = '.',message[2] = pid_array[1] , message[3] = '.' ;
    if(pid_array[2]!='0'){
      message[4] = pid_array[2] , message[5] = pid_array[3];  
      strcpy(&message[6], product_hash);
      message[16] = 0;
    }else{
      message[4] = pid_array[3];  
      strcpy(&message[5], product_hash);
      message[15] = 0;
    }
}

void random_generate(uint8_t* arr,int len){
    if(len > 32) return ;

     crypto_random_generate(arr,len);

     //using atecc
     uint8_t temp[32] = {0};
     ATCAIfaceCfg *atca_cfg;
     atca_cfg = cfg_atecc608a_iface;
     ATCA_STATUS status = atcab_init(atca_cfg);
     status = atcab_random(temp);

     for(int i=0; i<len; ++i){
        arr[i]^=temp[i];
     }
}

int check_digit(uint64_t value)
{
    if(value<100)return 1;
    if(value%10!=0)return 1;
    value/=10;
    if(value%10!=0)return 1;

    return 0;

}

void der_to_sig(const uint8_t *der, uint8_t *sig)
{
    if (!der || !sig)
        return;
    uint8_t off = 4 + (der[3] & 1);
    memcpy(sig, der + off, 32);
    off += 34 + (der[off + 33] & 1);
    memcpy(sig + 32, der + off, 32);
}
/**
 * @brief hex char to integer.
 *
 * @param ch char
 * @return int output.
 */
static int hexchartoint(const uint8_t ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    else if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    else if(ch >= 'C' && ch <= 'F'){
        return ch - 'A' + 10;
    }
    return '\0';
}

/**
 * @brief intermediate step to add the last hex
 * in the partial input.
 * @details
 *
 * @param Out output reference
 * @param NewHex attached hex.
 *
 * @return
 * @retval
 *
 * @see
 * @since v1.0.0
 *
 * @note
 */
static void updateDec(uint8_t *Out, const uint8_t NewHex, const uint8_t size){
  if(NULL == Out){
      return;
  }
  int power = NewHex;
  for(int i = size - 1; i >=0;i--){
    int temp = Out[i]*16 + power;
    Out[i] = temp%10;
    power = temp/10;
  }
}

void convertbase16tobase10(const uint8_t size_inp, const char *u_Inp, uint8_t *Out, const uint8_t size_out){
  if(NULL == u_Inp || NULL == Out){
      return;
  }
  memzero(Out, size_out);
  for(int i=0; i<size_inp; i++){
    updateDec(Out, hexchartoint(u_Inp[i]), size_out);
  }
}

uint8_t dec_to_hex(const uint64_t dec, uint8_t *bytes, uint8_t len)
{
    uint8_t hex[8];
    uint8_t size = 0;

    hex[0] = (dec & 0xff00000000000000) >> 56;
    size = size == 0 && hex[0] > 0 ? 8 : size;
    hex[1] = (dec & 0x00ff000000000000) >> 48;
    size = size == 0 && hex[1] > 0 ? 7 : size;
    hex[2] = (dec & 0x0000ff0000000000) >> 40;
    size = size == 0 && hex[2] > 0 ? 6 : size;
    hex[3] = (dec & 0x000000ff00000000) >> 32;
    size = size == 0 && hex[3] > 0 ? 5 : size;
    hex[4] = (dec & 0x00000000ff000000) >> 24;
    size = size == 0 && hex[4] > 0 ? 4 : size;
    hex[5] = (dec & 0x0000000000ff0000) >> 16;
    size = size == 0 && hex[5] > 0 ? 3 : size;
    hex[6] = (dec & 0x000000000000ff00) >> 8;
    size = size == 0 && hex[6] > 0 ? 2 : size;
    hex[7] = dec & 0x00000000000000ff;
    size = size == 0 && hex[7] > 0 ? 1 : size;

    if (bytes)
        memcpy(bytes, hex + 8 - len, len);

    return size;
}
