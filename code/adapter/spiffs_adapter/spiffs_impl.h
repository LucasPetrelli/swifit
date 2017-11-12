/**
 * @file spiffs_impl.h
 * @author Lucas
 * @date Oct 24, 2017
 *
 * @brief ESP8266 implementation of a SPIFFS
 * as seen in ESP8266_RTOS_SDK\examples\spiffs_test
 */

#ifndef CODE_ADAPTER_SPIFFS_ADAPTER_SPIFFS_IMPL_H_
#define CODE_ADAPTER_SPIFFS_ADAPTER_SPIFFS_IMPL_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "esp_common.h"

#define FS1_FLASH_SIZE      (1*1024*1024)

#define FS1_FLASH_ADDR      (2*1024*1024)


#define SECTOR_SIZE         (4*1024)
#define LOG_BLOCK           (SECTOR_SIZE)
#define LOG_PAGE            (128)

#define FD_BUF_SIZE         32*4
#define CACHE_BUF_SIZE      (LOG_PAGE + 32)*8

static s32_t esp_spiffs_readwrite(u32_t addr, u32_t size, u8_t *p, int write);

static s32_t esp_spiffs_read(u32_t addr, u32_t size, u8_t *dst);

static s32_t esp_spiffs_write(u32_t addr, u32_t size, u8_t *src);

static s32_t esp_spiffs_erase(u32_t addr, u32_t size);

static s32_t _erase(u32_t addr, u32_t size);

static s32_t _write(u32_t addr, u32_t size, u8_t *src);

static s32_t _read(u32_t addr, u32_t size, u8_t *dst);

static void spiffs_check_cb_f(spiffs_check_type type, spiffs_check_report report, u32_t arg1, u32_t arg2);

s32_t fs_mount_specific(u32_t phys_addr, u32_t phys_size, u32_t phys_sector_size, u32_t log_block_size, u32_t log_page_size);

spiffs* psGetFilesystemHandle();

void spiffs_fs1_init(void);
#endif /* CODE_ADAPTER_SPIFFS_ADAPTER_SPIFFS_IMPL_H_ */
