#include "ata.h"
#include "atadef.h"
#include "../../x86.h"
#include "../../error.h"
#include "../../def.h"

static uint16_t identity_buf[SECTOR_SIZE/2];

ata_identity_struct ata_identity;

static void wait(void)
{
    while (inb(ATA_STATUS) & ATA_ST_BSY || !(inb(ATA_STATUS & ATA_ST_DRDY))) {}
}

static void identify(void)
{
    outb(ATA_DRIVE, 0xf0);
    outb(ATA_COMMAND, ATA_CMD_IDENTIFY_DRIVE);
    wait();
    rep_insw(ATA_DATA, identity_buf, SECTOR_SIZE/2);

    ata_identity.sector_count = (uint32_t)((uint32_t)identity_buf[60] | ((uint32_t)identity_buf[61] << 16));
}

void ata_init(void)
{
    outb(ATA_DRIVE, 0xf0); // slave
    //enable interrupts
    outb(ATA_DEVICE_CONTROL, 0x08);
    identify();
}

int ata_pio_read_sectors(uint32_t lba, void *dst, uint8_t count)
{
    // lba
    outb(ATA_LBA_LOW, (uint8_t)lba);
    outb(ATA_LBA_MID, (uint8_t)(lba >> 8));
    outb(ATA_LBA_HIGH, (uint8_t)(lba >> 16));
    outb(ATA_DRIVE, (uint8_t)(((lba >> 24) & 0xf) | 0xf0)); // lba, slave
    // count
    outb(ATA_SECTOR_COUNT, count);
    outb(ATA_COMMAND, ATA_CMD_READ_SECTORS);

    char *_dst = dst;
    for (int i = 0; i < (int)count; i++) {
        wait();
        if (inb(ATA_STATUS) & ATA_ST_ERR) {
            return -EIO;
        }
        rep_insw(ATA_DATA, _dst, SECTOR_SIZE/2);
        _dst += SECTOR_SIZE;
    }
    return 0;
}


int ata_pio_read(size_t offset, void *dst, size_t count)
{
    int err;
    size_t start_sector = offset / SECTOR_SIZE;
    size_t start_sector_offset = offset % SECTOR_SIZE;
    size_t end_sector = (offset + count) / SECTOR_SIZE;
    size_t end_sector_offset = (offset + count) % SECTOR_SIZE;

    char buffer[SECTOR_SIZE];

    char *_dst = dst;
    if ((err = ata_pio_read_sectors((uint32_t)start_sector, buffer, 1))) {
        goto error;
    }
    for (size_t i = start_sector_offset; i < (start_sector == end_sector ? end_sector_offset : SECTOR_SIZE); i++) {
        *_dst++ = buffer[i];
    }
    if (start_sector + 1 < end_sector) {
        size_t middle_sectors_cnt = end_sector - (start_sector+1);

        for (size_t i = 0; middle_sectors_cnt > 0;) {
            size_t cnt = MIN(middle_sectors_cnt, UINT8_MAX);
            if ((err = ata_pio_read_sectors((uint32_t)(start_sector+1+i), dst, (uint8_t)cnt))) {
                goto error;
            }
            _dst += cnt * SECTOR_SIZE;
            middle_sectors_cnt -= cnt;
            i += cnt;
        }
    }
    if (start_sector != end_sector) {
        if ((err = ata_pio_read_sectors((uint32_t)end_sector, buffer, 1))) {
            goto error;
        }
        for (size_t i = 0; i < end_sector_offset; i++) {
            *_dst++ = buffer[i];
        }
    }

    return 0;
error:
    return err;
}