/*
 * V31 RDRAM test - readable C equivalent
 *
 * IMPORTANT:
 * V31 was not compiled from this C file.  The test was injected directly into
 * the embedded PS2 BIOS in ps2_emu as EE/MIPS machine code.  This file is a
 * faithful source-level reconstruction of that injected test logic, intended
 * for documentation / GitHub.
 *
 * V31 behavior:
 *   - runs from Sony's q=1 RDRAM failure path
 *   - tests the complete 32 MiB PS2 RDRAM address space
 *   - four patterns: 00000000, FFFFFFFF, AAAAAAAA, 55555555
 *   - classifies failures by physical channel using address bit 4:
 *         bit4=0 -> Channel A
 *         bit4=1 -> Channel B
 *   - classifies each channel into Range 0..7 using address >> 22
 *   - prints channel totals, approximate failure rates, per-range results,
 *     final Channel A/B OK/FAIL status, and Calyps0 branding
 */

#include <stdint.h>
#include <stddef.h>

/* Sony's existing BIOS debug printf routine in the patched ps2_emu image. */
extern int debug_printf(const char *fmt, ...);

#define RDRAM_BASE      ((volatile uint32_t *)0x00000000u)
#define RDRAM_SIZE      0x02000000u
#define WORD_SIZE       4u

#define CHANNEL_WORDS_PER_PATTERN  0x00400000u /* 16 MiB / 4 */
#define CHANNEL_COMPARISONS        16777216u   /* 4,194,304 * 4 patterns */
#define RANGE_COMPARISONS           2097152u   /* 524,288 * 4 patterns */

static const uint32_t patterns[4] = {
    0x00000000u,
    0xFFFFFFFFu,
    0xAAAAAAAAu,
    0x55555555u
};

/*
 * Exact V31 bucket mapping:
 *
 *   index = (address >> 22) + (((address >> 4) & 1) << 3)
 *
 * index 0..7  = Channel A, Range 0..7
 * index 8..15 = Channel B, Range 0..7
 */
static unsigned bucket_index(uint32_t address)
{
    unsigned range   = address >> 22;
    unsigned channel = (address >> 4) & 1u;

    return range + (channel << 3);
}

static void print_percent_approx(uint64_t bad, uint32_t divisor)
{
    /*
     * V31 intentionally used integer approximation:
     *
     * Channel:
     *   tmp = bad / 1677
     *
     * Range:
     *   tmp = bad / 210
     *
     * Then tmp / 100 and tmp % 100 are printed.
     */
    uint64_t hundredths = bad / divisor;

    debug_printf("%ld.%02ld%c",
                 (long)(hundredths / 100),
                 (long)(hundredths % 100),
                 '%');
}

void v31_rdram_test(void)
{
    uint64_t channel_bad[2] = {0, 0};
    uint64_t range_bad[16]  = {0};

    /*
     * V31 writes one complete pattern over all 32 MiB, then reads the complete
     * area back before moving to the next pattern.
     */
    for (unsigned p = 0; p < 4; p++) {
        uint32_t expected = patterns[p];

        /* Write pattern to every 32-bit word in RDRAM. */
        for (uint32_t address = 0; address < RDRAM_SIZE; address += WORD_SIZE) {
            *(volatile uint32_t *)(uintptr_t)address = expected;
        }

        /* Read back and classify every mismatching word. */
        for (uint32_t address = 0; address < RDRAM_SIZE; address += WORD_SIZE) {
            uint32_t actual = *(volatile uint32_t *)(uintptr_t)address;

            if (actual != expected) {
                unsigned channel = (address >> 4) & 1u;
                unsigned bucket  = bucket_index(address);

                channel_bad[channel]++;
                range_bad[bucket]++;
            }
        }
    }

    /* Full-channel statistics. */
    for (unsigned channel = 0; channel < 2; channel++) {
        char name = channel ? 'B' : 'A';

        debug_printf("# Channel %c bad=%ld / 16777216\n",
                     name, (long)channel_bad[channel]);

        debug_printf("# Channel %c fail rate=", name);
        print_percent_approx(channel_bad[channel], 1677);
        debug_printf("\n");
    }

    /*
     * V31 reports Range 0..7 separately for each channel.
     *
     * Range address windows:
     *   0: 00000000-003FFFFF
     *   1: 00400000-007FFFFF
     *   2: 00800000-00BFFFFF
     *   3: 00C00000-00FFFFFF
     *   4: 01000000-013FFFFF
     *   5: 01400000-017FFFFF
     *   6: 01800000-01BFFFFF
     *   7: 01C00000-01FFFFFF
     *
     * Channel A/B selection inside every range is address bit 4.
     */
    for (unsigned channel = 0; channel < 2; channel++) {
        debug_printf(channel == 0
                     ? "# testing Channel A\n"
                     : "# testing Channel B\n");

        for (unsigned range = 0; range < 8; range++) {
            unsigned idx = range + (channel << 3);
            uint64_t bad = range_bad[idx];

            if (bad == 0) {
                debug_printf("# Range %ld = GOOD (0 errors)\n", (long)range);
            } else {
                debug_printf("# Range %ld = %ld bad, ",
                             (long)range, (long)bad);

                print_percent_approx(bad, 210);
                debug_printf("\n");
            }
        }
    }

    /* Final dynamic result. */
    debug_printf(channel_bad[0] == 0
                 ? "# Channel A: OK\n"
                 : "# Channel A: FAIL\n");

    debug_printf(channel_bad[1] == 0
                 ? "# Channel B: OK\n"
                 : "# Channel B: FAIL\n");

    debug_printf("# RDRAM test by Calyps0\n");
}
