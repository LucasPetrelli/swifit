/**
 * @file spiffs_impl.c
 * @author Lucas
 * @date Oct 24, 2017
 *
 * @brief
 */


#include "spiffs_impl.h"


static int erases[FS1_FLASH_SIZE/SECTOR_SIZE];
static char _path[256];
static u32_t bytes_rd = 0;
static u32_t bytes_wr = 0;
static u32_t reads = 0;
static u32_t writes = 0;
static u32_t error_after_bytes_written = 0;
static u32_t error_after_bytes_read = 0;
static char error_after_bytes_written_once_only = 0;
static char error_after_bytes_read_once_only = 0;
static char log_flash_ops = 1;
static u32_t fs_check_fixes = 0;

spiffs __fs;
static u8_t _work[LOG_PAGE * 2];
static u8_t _fds[FD_BUF_SIZE * 2];
static u8_t _cache[CACHE_BUF_SIZE];

#define FLASH_UNIT_SIZE 4
static s32_t esp_spiffs_readwrite(u32_t addr, u32_t size, u8_t *p, int write)
{
    /*
     * With proper configurarion spiffs never reads or writes more than
     * LOG_PAGE_SIZE
     */

    if (size > __fs.cfg.log_page_size) {
        printf("Invalid size provided to read/write (%d)\n\r", (int) size);
        return SPIFFS_ERR_NOT_CONFIGURED;
    }

    char tmp_buf[__fs.cfg.log_page_size + FLASH_UNIT_SIZE * 2];
    u32_t aligned_addr = addr & (-FLASH_UNIT_SIZE);
    u32_t aligned_size =
        ((size + (FLASH_UNIT_SIZE - 1)) & -FLASH_UNIT_SIZE) + FLASH_UNIT_SIZE;

    int res = spi_flash_read(aligned_addr, (u32_t *) tmp_buf, aligned_size);

    if (res != 0) {
        printf("spi_flash_read failed: %d (%d, %d)\n\r", res, (int) aligned_addr,
               (int) aligned_size);
        return res;
    }

    if (!write) {
        memcpy(p, tmp_buf + (addr - aligned_addr), size);
        return SPIFFS_OK;
    }

    memcpy(tmp_buf + (addr - aligned_addr), p, size);

    res = spi_flash_write(aligned_addr, (u32_t *) tmp_buf, aligned_size);

    if (res != 0) {
//	    printf("spi_flash_write failed: %d (%d, %d)\n\r", res,
//	              (int) aligned_addr, (int) aligned_size);
        return res;
    }

    return SPIFFS_OK;
}

static s32_t esp_spiffs_read(u32_t addr, u32_t size, u8_t *dst)
{
    return esp_spiffs_readwrite(addr, size, dst, 0);
}

static s32_t esp_spiffs_write(u32_t addr, u32_t size, u8_t *src)
{
    return esp_spiffs_readwrite(addr, size, src, 1);
}

static s32_t esp_spiffs_erase(u32_t addr, u32_t size)
{
    /*
     * With proper configurarion spiffs always
     * provides here sector address & sector size
     */
    if (size != __fs.cfg.phys_erase_block || addr % __fs.cfg.phys_erase_block != 0) {
        printf("Invalid size provided to esp_spiffs_erase (%d, %d)\n\r",
               (int) addr, (int) size);
        return SPIFFS_ERR_NOT_CONFIGURED;
    }
    return spi_flash_erase_sector(addr / __fs.cfg.phys_erase_block);
}

static s32_t _erase(u32_t addr, u32_t size)
{
  if (addr & (__fs.cfg.phys_erase_block-1)) {
    printf("trying to erase at addr %08x, out of boundary\n", addr);
    return -1;
  }
  if (size & (__fs.cfg.phys_erase_block-1)) {
    printf("trying to erase at with size %08x, out of boundary\n", size);
    return -1;
  }
  erases[(addr-__fs.cfg.phys_addr)/__fs.cfg.phys_erase_block]++;
  return esp_spiffs_erase(addr, size);
}

static s32_t _write(u32_t addr, u32_t size, u8_t *src)
{
  int i;
  if (log_flash_ops) {
    bytes_wr += size;
    writes++;
    if (error_after_bytes_written > 0 && bytes_wr >= error_after_bytes_written) {
      if (error_after_bytes_written_once_only) {
        error_after_bytes_written = 0;
      }
      return SPIFFS_ERR_TEST;
    }
  }

  if (addr < __fs.cfg.phys_addr) {
    printf("FATAL write addr too low %08x < %08x\n", addr, FS1_FLASH_ADDR);
    exit(0);
  }
  if (addr + size > __fs.cfg.phys_addr + __fs.cfg.phys_size) {
    printf("FATAL write addr too high %08x + %08x > %08x\n", addr, size, FS1_FLASH_ADDR + FS1_FLASH_SIZE);
    exit(0);
  }
  return esp_spiffs_write(addr, size, src);
}

static s32_t _read(u32_t addr, u32_t size, u8_t *dst)
{
  if (log_flash_ops) {
    bytes_rd += size;
    reads++;
    if (error_after_bytes_read > 0 && bytes_rd >= error_after_bytes_read) {
      if (error_after_bytes_read_once_only) {
        error_after_bytes_read = 0;
      }
      return SPIFFS_ERR_TEST;
    }
  }
  if (addr < __fs.cfg.phys_addr) {
    printf("FATAL read addr too low %08x < %08x\n", addr, FS1_FLASH_ADDR);
    exit(0);
  }
  if (addr + size > __fs.cfg.phys_addr + __fs.cfg.phys_size) {
    printf("FATAL read addr too high %08x + %08x > %08x\n", addr, size, FS1_FLASH_ADDR + FS1_FLASH_SIZE);
    exit(0);
  }

  return esp_spiffs_read(addr, size, dst);
}

static void spiffs_check_cb_f(spiffs_check_type type, spiffs_check_report report,
    u32_t arg1, u32_t arg2)
{
  if (report != SPIFFS_CHECK_PROGRESS) {
    if (report != SPIFFS_CHECK_ERROR) fs_check_fixes++;
    printf("   check: ");
    switch (type) {
    case SPIFFS_CHECK_INDEX:
      printf("INDEX  "); break;
    case SPIFFS_CHECK_LOOKUP:
      printf("LOOKUP "); break;
    case SPIFFS_CHECK_PAGE:
      printf("PAGE   "); break;
    default:
      printf("????   "); break;
    }
    if (report == SPIFFS_CHECK_ERROR) {
      printf("ERROR %i", arg1);
    } else if (report == SPIFFS_CHECK_DELETE_BAD_FILE) {
      printf("DELETE BAD FILE %04x", arg1);
    } else if (report == SPIFFS_CHECK_DELETE_ORPHANED_INDEX) {
      printf("DELETE ORPHANED INDEX %04x", arg1);
    } else if (report == SPIFFS_CHECK_DELETE_PAGE) {
      printf("DELETE PAGE %04x", arg1);
    } else if (report == SPIFFS_CHECK_FIX_INDEX) {
      printf("FIX INDEX %04x:%04x", arg1, arg2);
    } else if (report == SPIFFS_CHECK_FIX_LOOKUP) {
      printf("FIX INDEX %04x:%04x", arg1, arg2);
    } else {
      printf("??");
    }
    printf("\n");
  }
}

s32_t fs_mount_specific(u32_t phys_addr, u32_t phys_size,
    u32_t phys_sector_size,
    u32_t log_block_size, u32_t log_page_size)
{
  spiffs_config c;
  c.hal_erase_f = _erase;
  c.hal_read_f = _read;
  c.hal_write_f = _write;
  c.log_block_size = log_block_size;
  c.log_page_size = log_page_size;
  c.phys_addr = phys_addr;
  c.phys_erase_block = phys_sector_size;
  c.phys_size = phys_size;

  if(SPIFFS_mount(&__fs, &c, _work, _fds, sizeof(_fds), _cache, sizeof(_cache), spiffs_check_cb_f))
  {
	  SPIFFS_format(&__fs);
	  printf("formatting spiffs\n\n");
	  return SPIFFS_mount(&__fs, &c, _work, _fds, sizeof(_fds), _cache, sizeof(_cache), spiffs_check_cb_f);
  }
  else
  {
	  return 0;
  }
}

void spiffs_fs1_init(void)
{
	struct esp_spiffs_config config;
	config.phys_size = FS1_FLASH_SIZE;
	config.phys_addr = FS1_FLASH_ADDR;
	config.phys_erase_block = SECTOR_SIZE;
	config.log_block_size = LOG_BLOCK;
	config.log_page_size = LOG_PAGE;
	config.fd_buf_size = FD_BUF_SIZE * 2;
	config.cache_buf_size = CACHE_BUF_SIZE;
	esp_spiffs_init(&config);
}

spiffs* psGetFilesystemHandle()
{
	return &__fs;
}

