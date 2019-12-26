#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define SECTOR_SIZE 512

typedef struct {
    uint32_t sector_count;
} ata_identity_struct;

extern ata_identity_struct ata_identity;

void ata_init(void);
int ata_pio_read_sectors(uint32_t lba, void *dst, uint8_t count);
int ata_pio_read(size_t offset, void *dst, size_t count);