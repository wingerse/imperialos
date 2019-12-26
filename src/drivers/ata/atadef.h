#pragma once

#define ATA_DATA             0x1f0
#define ATA_ERROR            0x1f1
#define ATA_FEATURES         0x1f1
#define ATA_SECTOR_COUNT     0x1f2
#define ATA_LBA_LOW          0x1f3
#define ATA_LBA_MID          0x1f4
#define ATA_LBA_HIGH         0x1f5
#define ATA_DRIVE            0x1f6
#define ATA_STATUS           0x1f7
#define ATA_COMMAND          0x1f7
#define ATA_ALTERNATE_STATUS 0x3f6
#define ATA_DEVICE_CONTROL   0x3f6
#define ATA_DRIVE_ADDRESS    0x3f7

#define ATA_DC_SRST (1 << 2)
#define ATA_DC_nIEN (1 << 1)

#define ATA_ST_BSY   (1 << 7)
#define ATA_ST_DRDY  (1 << 6)
#define ATA_ST_DWF   (1 << 5)
#define ATA_ST_DSC   (1 << 4)
#define ATA_ST_DRQ   (1 << 3)
#define ATA_ST_CORR  (1 << 2)
#define ATA_ST_IDX   (1 << 1)
#define ATA_ST_ERR   (1 << 0)

#define ATA_ERR_BBK   (1 << 7)
#define ATA_ERR_UNC   (1 << 6)
#define ATA_ERR_MC    (1 << 5)
#define ATA_ERR_IDNF  (1 << 4)
#define ATA_ERR_MCR   (1 << 3)
#define ATA_ERR_ABRT  (1 << 2)
#define ATA_ERR_TK0NF (1 << 1)
#define ATA_ERR_AMNF  (1 << 0)

#define ATA_MASTER 0
#define ATA_SLAVE  1

#define ATA_CMD_READ_SECTORS   0x20
#define ATA_CMD_IDENTIFY_DRIVE 0xec