#include <stdint.h>
#include "../drivers/ata/ata.h"
#include "../drivers/ata/atadef.h"
#include "../x86.h"
#include "../def.h"
#include <string.h>
#include <limits.h>

static void ata_wait(void)
{
    while (inb(ATA_STATUS) & ATA_ST_BSY || !(inb(ATA_STATUS & ATA_ST_DRDY))) {}
}

static void ata_read_sectors(uint32_t lba, void *dst, uint8_t count)
{
    // lba
    outb(ATA_LBA_LOW, (uint8_t)lba);
    outb(ATA_LBA_MID, (uint8_t)(lba >> 8));
    outb(ATA_LBA_HIGH, (uint8_t)(lba >> 16));
    outb(ATA_DRIVE, (uint8_t)(((lba >> 24) & 0xf) | 0xe0)); // lba, master
    // count
    outb(ATA_SECTOR_COUNT, count);
    outb(ATA_COMMAND, ATA_CMD_READ_SECTORS);

    char *_dst = dst;
    for (int i = 0; i < (int)count; i++) {
        ata_wait();
        rep_insw(ATA_DATA, _dst, SECTOR_SIZE/2);
        _dst += SECTOR_SIZE;
    }
}

extern void kmain(void);

void start(void)
{
    // round up kernel size to sectors and load
    ata_read_sectors(1, (char *)KERNEL_START, (uint8_t)((KERNEL_SIZE + SECTOR_SIZE-1) / SECTOR_SIZE));

    // clear bss
    // no memset at this point, so use a loop
    for (size_t i = 0; i < BSS_SIZE; i++) {
        *(char *)(BSS_START+i) = 0;
    }
    kmain();
}