#include "keccak.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>

int ReadCC = 0;
int WriteCC = 0;
int LeftShiftInst = 0;
int RightShiftInst = 0;
int LeftShift = 0;
int RightShift = 0;
int OrCC = 0;
int AndCC = 0;
int XorCC = 0;
int NotCC = 0;

const unsigned char plaintext[44] = "The quick brown fox jumps over the lazy dog";
const unsigned char expected[512] = {
    0xf4, 0x20, 0x2e, 0x3c, 0x58, 0x52, 0xf9, 0x18, 0x2a, 0x04, 0x30, 0xfd,
    0x81, 0x44, 0xf0, 0xa7, 0x4b, 0x95, 0xe7, 0x41, 0x7e, 0xca, 0xe1, 0x7d,
    0xb0, 0xf8, 0xcf, 0xee, 0xd0, 0xe3, 0xe6, 0x6e, 0xb5, 0x58, 0x5e, 0xc6,
    0xf8, 0x60, 0x21, 0xca, 0xcf, 0x27, 0x2c, 0x79, 0x8b, 0xcf, 0x97, 0xd3,
    0x68, 0xb8, 0x86, 0xb1, 0x8f, 0xec, 0x3a, 0x57, 0x1f, 0x09, 0x60, 0x86,
    0xa5, 0x23, 0x71, 0x7a, 0x37, 0x32, 0xd5, 0x0d, 0xb2, 0xb0, 0xb7, 0x99,
    0x8b, 0x41, 0x17, 0xae, 0x66, 0xa7, 0x61, 0xcc, 0xf1, 0x84, 0x7a, 0x16,
    0x16, 0xf4, 0xc0, 0x7d, 0x51, 0x78, 0xd0, 0xd9, 0x65, 0xf9, 0xfe, 0xba,
    0x35, 0x14, 0x20, 0xf8, 0xbf, 0xb6, 0xf5, 0xab, 0x9a, 0x0c, 0xb1, 0x02,
    0x56, 0x8e, 0xab, 0xf3, 0xdf, 0xa4, 0xe2, 0x22, 0x79, 0xf8, 0x08, 0x2d,
    0xce, 0x81, 0x43, 0xeb, 0x78, 0x23, 0x5a, 0x1a, 0x54, 0x91, 0x4a, 0xb7,
    0x1a, 0xbb, 0x07, 0xf2, 0xf3, 0x64, 0x84, 0x68, 0x37, 0x0b, 0x9f, 0xbb,
    0x07, 0x1e, 0x07, 0x4f, 0x1c, 0x03, 0x0a, 0x40, 0x30, 0x22, 0x5f, 0x40,
    0xc3, 0x94, 0x80, 0x33, 0x9f, 0x3d, 0xc7, 0x1d, 0x0f, 0x04, 0xf7, 0x13,
    0x26, 0xde, 0x13, 0x81, 0x67, 0x4c, 0xc8, 0x9e, 0x25, 0x9e, 0x21, 0x99,
    0x27, 0xfa, 0xe8, 0xea, 0x27, 0x99, 0xa0, 0x3d, 0xa8, 0x62, 0xa5, 0x5a,
    0xfa, 0xfe, 0x67, 0x09, 0x57, 0xa2, 0xaf, 0x33, 0x18, 0xd9, 0x19, 0xd0,
    0xa3, 0x35, 0x8f, 0x3b, 0x89, 0x12, 0x36, 0xd6, 0xa8, 0xe8, 0xd1, 0x99,
    0x99, 0xd1, 0x07, 0x6b, 0x52, 0x99, 0x68, 0xfa, 0xef, 0xbd, 0x88, 0x0d,
    0x77, 0xbb, 0x30, 0x08, 0x29, 0xdc, 0xa8, 0x7e, 0x9c, 0x8e, 0x4c, 0x28,
    0xe0, 0x80, 0x0f, 0xf3, 0x74, 0x90, 0xa5, 0xbd, 0x8c, 0x36, 0xc0, 0xb0,
    0xbd, 0xb2, 0x70, 0x1a, 0x5d, 0x58, 0xd0, 0x33, 0x78, 0xb9, 0xdb, 0xd3,
    0x84, 0x38, 0x9e, 0x3e, 0xf0, 0xfd, 0x40, 0x03, 0xb0, 0x89, 0x98, 0xfd,
    0x3f, 0x32, 0xfe, 0x1a, 0x08, 0x10, 0xfc, 0x0e, 0xcc, 0xaa, 0xd9, 0x4b,
    0xca, 0x8d, 0xd8, 0x3b, 0x34, 0x55, 0x9c, 0x33, 0x3f, 0x0b, 0x16, 0xdf,
    0xc2, 0x89, 0x6e, 0xd8, 0x7b, 0x30, 0xba, 0x14, 0xc8, 0x1f, 0x87, 0xcd,
    0x8b, 0x4b, 0xb6, 0x31, 0x7d, 0xb8, 0x9b, 0x0e, 0x7e, 0x94, 0xc0, 0x61,
    0x6f, 0x9a, 0x66, 0x5f, 0xba, 0x5b, 0x0e, 0x6f, 0xb3, 0x54, 0x9c, 0x9d,
    0x7b, 0x68, 0xe6, 0x6d, 0x08, 0xa8, 0x6e, 0xb2, 0xfa, 0xec, 0x05, 0xcc,
    0x46, 0x2a, 0x77, 0x18, 0x06, 0xb9, 0x3c, 0xc3, 0x8b, 0x0a, 0x4f, 0xeb,
    0x99, 0x35, 0xc6, 0xc8, 0x94, 0x5d, 0xa6, 0xa5, 0x89, 0x89, 0x1b, 0xa5,
    0xee, 0x99, 0x75, 0x3c, 0xfd, 0xd3, 0x8e, 0x1a, 0xbc, 0x71, 0x47, 0xfd,
    0x74, 0xb7, 0xc7, 0xd1, 0xce, 0x06, 0x09, 0xb6, 0x68, 0x0a, 0x2e, 0x18,
    0x88, 0x8d, 0x84, 0x94, 0x9b, 0x6e, 0x6c, 0xf6, 0xa2, 0xaa, 0x41, 0x13,
    0x53, 0x5a, 0xae, 0xe0, 0x79, 0x45, 0x9e, 0x3f, 0x25, 0x7b, 0x56, 0x9a,
    0x94, 0x50, 0x52, 0x3c, 0x41, 0xf5, 0xb5, 0xba, 0x4b, 0x79, 0xb3, 0xba,
    0x59, 0x49, 0x14, 0x0a, 0x74, 0xbb, 0x04, 0x8d, 0xe0, 0x65, 0x7d, 0x04,
    0x95, 0x4b, 0xdd, 0x71, 0xda, 0xe7, 0x6f, 0x61, 0xe2, 0xa1, 0xf8, 0x8a,
    0xec, 0xb9, 0x1c, 0xfa, 0x5b, 0x36, 0xc1, 0xbf, 0x33, 0x50, 0xa7, 0x98,
    0xdc, 0x4d, 0xcf, 0x48, 0x62, 0x8e, 0xff, 0xe3, 0xa0, 0xc5, 0x34, 0x0c,
    0x75, 0x6b, 0xd9, 0x22, 0xf7, 0x8d, 0x0e, 0x36, 0xef, 0x7d, 0xf1, 0x2c,
    0xe7, 0x8c, 0x17, 0x9c, 0xc7, 0x21, 0xad, 0x08, 0x7e, 0x15, 0xea, 0x49,
    0x6b, 0xf5, 0xf6, 0x0b, 0x21, 0xb5, 0x82, 0x2d
};

// static int test_sha3_256_incremental(void) {
//     unsigned char input[512];
//     unsigned char check[32];
//     unsigned char output[32];
//     sha3_256incctx state;
//     int i;
//     int absorbed;
//     int returncode = 0;

//     for (i = 0; i < 512; i++) {
//         input[i] = i;
//     }

//     sha3_256(check, input, 512);

//     sha3_256_inc_init(&state);

//     absorbed = 0;
//     for (i = 0; i < 512 && absorbed + i <= 512; i++) {
//         sha3_256_inc_absorb(&state, input + absorbed, i);
//         absorbed += i;
//     }
//     sha3_256_inc_absorb(&state, input + absorbed, 512 - absorbed);

//     sha3_256_inc_finalize(output, &state);

//     if (memcmp(check, output, 32) != 0) {
//         printf("ERROR sha3_256 incremental did not match sha3_256.\n");
//         printf("  Expected: ");
//         for (i = 0; i < 32; i++) {
//             printf("%02X", check[i]);
//         }
//         printf("\n");
//         printf("  Received: ");
//         for (i = 0; i < 32; i++) {
//             printf("%02X", output[i]);
//         }
//         printf("\n");
//         returncode = 1;
//     }

//     return returncode;
// }



// static int test_shake128_incremental(void) {
//     unsigned char input[512];
//     unsigned char check[512];
//     unsigned char output[512];
//     shake128incctx state_absorb;
//     shake128incctx state_squeeze;
//     shake128incctx state_squeeze_all;
//     shake128incctx state_both;
//     shake128ctx state_combined;
//     int i;
//     int absorbed;
//     int squeezed;
//     int returncode = 0;

//     for (i = 0; i < 512; i++) {
//         input[i] = i;
//     }

//     shake128(check, 512, input, 512);

//     shake128_inc_init(&state_absorb);

//     absorbed = 0;
//     for (i = 0; i < 512 && absorbed + i <= 512; i++) {
//         shake128_inc_absorb(&state_absorb, input + absorbed, i);
//         absorbed += i;
//     }
//     shake128_inc_absorb(&state_absorb, input + absorbed, 512 - absorbed);

//     shake128_inc_finalize(&state_absorb);

//     shake128_absorb(&state_combined, input, 512);

//     if (memcmp(state_absorb.ctx, state_combined.ctx, PQC_SHAKECTX_BYTES) != 0) {
//         printf("ERROR shake128 state after incremental absorb did not match all-at-once absorb.\n");
//         returncode = 1;
//     }

//     shake128_inc_ctx_clone(&state_both, &state_absorb);

//     shake128_squeezeblocks(output, 3, (shake128ctx*)&state_absorb);

//     if (memcmp(check, output, 3*SHAKE128_RATE)) {
//         printf("ERROR shake128 incremental absorb did not match shake128.\n");
//         printf("  Expected: ");
//         for (i = 0; i < 3*SHAKE128_RATE; i++) {
//             printf("%02X", check[i]);
//         }
//         printf("\n");
//         printf("  Received: ");
//         for (i = 0; i < 3*SHAKE128_RATE; i++) {
//             printf("%02X", output[i]);
//         }
//         printf("\n");
//         returncode = 1;
//     }

//     shake128_absorb((shake128ctx*)&state_squeeze, input, 512);
//     state_squeeze.ctx = realloc(state_squeeze.ctx, PQC_SHAKEINCCTX_BYTES);
//     if (state_squeeze.ctx == NULL) {
//         exit(111);
//     }
//     state_squeeze.ctx[25] = 0;

//     shake128_inc_ctx_clone(&state_squeeze_all, &state_squeeze);

//     shake128_inc_squeeze(output, 512, &state_squeeze_all);

//     if (memcmp(check, output, 512) != 0) {
//         printf("ERROR shake128 incremental squeeze-all did not match shake128.\n");
//         printf("  Expected: ");
//         for (i = 0; i < 512; i++) {
//             printf("%02X", check[i]);
//         }
//         printf("\n");
//         printf("  Received: ");
//         for (i = 0; i < 512; i++) {
//             printf("%02X", output[i]);
//         }
//         printf("\n");
//         returncode = 1;
//     }

//     squeezed = 0;
//     memset(output, 0, 512);
//     for (i = 0; i < 512 && squeezed + i <= 512; i++) {
//         shake128_inc_squeeze(output + squeezed, i, &state_squeeze);
//         squeezed += i;
//     }
//     shake128_inc_squeeze(output + squeezed, 512 - squeezed, &state_squeeze);

//     if (memcmp(check, output, 512) != 0) {
//         printf("ERROR shake128 incremental squeeze did not match shake128.\n");
//         printf("  Expected: ");
//         for (i = 0; i < 512; i++) {
//             printf("%02X", check[i]);
//         }
//         printf("\n");
//         printf("  Received: ");
//         for (i = 0; i < 512; i++) {
//             printf("%02X", output[i]);
//         }
//         printf("\n");
//         returncode = 1;
//     }

//     squeezed = 0;
//     memset(output, 0, 512);
//     for (i = 0; i < 512 && squeezed + i <= 512; i++) {
//         shake128_inc_squeeze(output + squeezed, i, &state_both);
//         squeezed += i;
//     }
//     shake128_inc_squeeze(output + squeezed, 512 - squeezed, &state_both);

//     if (memcmp(check, output, 512) != 0) {
//         printf("ERROR shake128 incremental absorb + squeeze did not match shake128.\n");
//         printf("  Expected: ");
//         for (i = 0; i < 512; i++) {
//             printf("%02X", check[i]);
//         }
//         printf("\n");
//         printf("  Received: ");
//         for (i = 0; i < 512; i++) {
//             printf("%02X", output[i]);
//         }
//         printf("\n");
//         returncode = 1;
//     }


//     shake128_inc_ctx_release(&state_absorb);
//     shake128_inc_ctx_release(&state_squeeze);
//     shake128_inc_ctx_release(&state_squeeze_all);
//     shake128_inc_ctx_release(&state_both);
//     shake128_ctx_release(&state_combined);

//     return returncode;
// }

static int test_shake128(void) {
    unsigned char output[32];
    int i = 0;

    shake128(output, 32, plaintext, 43);

    if (memcmp(expected, output, 32) != 0) {
        printf("ERROR shake128 output did not match test vector.\n");
        printf("Expected: ");
        for (i = 0; i < 32; i++) {
            printf("%02X", expected[i]);
        }
        printf("\n");
        printf("Received: ");
        for (i = 0; i < 32; i++) {
            printf("%02X", output[i]);
        }
        printf("\n");
        return 1;
    }

    printf("ReadCC: %d\n", ReadCC);
    printf("WriteCC: %d\n", WriteCC);
    printf("LeftShiftInst: %d\n", LeftShiftInst);
    printf("RightShiftInst: %d\n", RightShiftInst);
    printf("OrCC: %d\n", OrCC);
    printf("AndCC: %d\n", AndCC);
    printf("XorCC: %d\n", XorCC);
    printf("NotCC: %d\n", NotCC);
    printf("Total: %d\n", ReadCC + WriteCC + (LeftShiftInst + RightShiftInst) * 2 + (OrCC + AndCC + XorCC + NotCC) * 3); // 1 for operation, 1 for write

    printf("\nReadCC Cycle: %d\n", ReadCC);
    printf("WriteCC Cycle: %d\n", WriteCC);
    printf("LeftShift Cycle: %d\n", LeftShift + LeftShiftInst); // 1 for shift, 1 for write, no write back in the middle of the shifting
    printf("RightShift Cycle: %d\n", RightShift + RightShiftInst); // 1 for shift, 1 for write, no write back in the middle of the shifting
    printf("OrCC Cycle: %d\n", OrCC * (2 + 1)); // 1 for activate, 1 for or, 1 for write
    printf("AndCC Cycle: %d\n", AndCC * (2 + 1)); // 1 for activate, 1 for and, 1 for write
    printf("XorCC Cycle: %d\n", XorCC * (2 + 1)); // 1 for activate, 1 for xor, 1 for write
    printf("NotCC Cycle: %d\n", NotCC * (2 + 1)); // 1 for activate, 1 for xor, 1 for write. Not is implemented by Xoring a constant full of 1s.
    printf("Total Cycle: %d\n", ReadCC + WriteCC + LeftShift + RightShift + LeftShiftInst + RightShiftInst + OrCC * (2 + 1) + AndCC * (2 + 1) + XorCC * (2 + 1) + NotCC * (2 + 1)); 
    
    printf("\nTest end\n");

    char filename[256];
    strcpy(filename, __FILE__);
    // Find the position of the last '.' in the filename
    char *pos = strrchr(filename, '.');
    if (pos != NULL) {
        // Replace everything after the '.' with ".txt"
        strcpy(pos, ".txt");
    } else {
        // If there's no '.', just append ".txt" to the end
        strcat(filename, ".txt");
    }

    FILE *outfile = fopen(filename, "w");
    if (outfile == NULL) {
        fprintf(stderr, "Failed to open file %s for writing.\n", filename);
        return 1;
    }

    // Write output to the file
    fprintf(outfile, "ReadCC: %d\n", ReadCC);
    fprintf(outfile, "WriteCC: %d\n", WriteCC);
    fprintf(outfile, "LeftShiftInst: %d\n", LeftShiftInst);
    fprintf(outfile, "RightShiftInst: %d\n", RightShiftInst);
    fprintf(outfile, "OrCC: %d\n", OrCC);
    fprintf(outfile, "AndCC: %d\n", AndCC);
    fprintf(outfile, "XorCC: %d\n", XorCC);
    fprintf(outfile, "NotCC: %d\n", NotCC);
    fprintf(outfile, "Total: %d\n", ReadCC + WriteCC + (LeftShiftInst + RightShiftInst) * 2 + (OrCC + AndCC + XorCC + NotCC) * 3); // 1 for operation, 1 for write

    fprintf(outfile, "\nReadCC Cycle: %d\n", ReadCC);
    fprintf(outfile, "WriteCC Cycle: %d\n", WriteCC);
    fprintf(outfile, "LeftShift Cycle: %d\n", LeftShift + LeftShiftInst); // 1 for shift, 1 for write, no write back in the middle of the shifting
    fprintf(outfile, "RightShift Cycle: %d\n", RightShift + RightShiftInst); // 1 for shift, 1 for write, no write back in the middle of the shifting
    fprintf(outfile, "OrCC Cycle: %d\n", OrCC * (2 + 1)); // 1 for activate, 1 for or, 1 for write
    fprintf(outfile, "AndCC Cycle: %d\n", AndCC * (2 + 1)); // 1 for activate, 1 for and, 1 for write
    fprintf(outfile, "XorCC Cycle: %d\n", XorCC * (2 + 1)); // 1 for activate, 1 for xor, 1 for write
    fprintf(outfile, "NotCC Cycle: %d\n", NotCC * (2 + 1)); // 1 for activate, 1 for xor, 1 for write. Not is implemented by Xoring a constant full of 1s.
    fprintf(outfile, "Total Cycle: %d\n", ReadCC + WriteCC + LeftShift + RightShift + LeftShiftInst + RightShiftInst + OrCC * (2 + 1) + AndCC * (2 + 1) + XorCC * (2 + 1) + NotCC * (2 + 1));

    fclose(outfile);

    return 0;
}

int main(void) {
    int result = 0;
    result += test_shake128();
    // result += test_shake128_incremental();
    // result += test_sha3_256_incremental();

    if (result != 0) {
        puts("Errors occurred");
    }
    return result;
}
