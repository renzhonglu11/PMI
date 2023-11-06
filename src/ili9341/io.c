#include "ili_priv.h"

/* Defines for switching between sequential or lookup table datapin setter */
#define IO_PINS_SETTER_SEQ          1
#define IO_PINS_SETTER_LOOKUPTABLE  2

/* By default use the lookup setter routine because it is a lot faster */
#ifndef IO_PINS_SETTER
    #define IO_PINS_SETTER  IO_PINS_SETTER_LOOKUPTABLE
#endif

#if IO_PINS_SETTER == IO_PINS_SETTER_LOOKUPTABLE
/**
 * @brief Lookup table for faster data transmission
 */
struct data_pins_lookup_table_s
{
    uint32_t bsrr_a;
    uint32_t bsrr_b;
    uint32_t bsrr_c;
};

/**
 * @brief Lookup table to set pins D0-D7
 * @note Used by function @ref io_dpins_set
 */
static const struct data_pins_lookup_table_s dlt[] =
{
    {0x07000000, 0x04380000, 0x00800000}, /* idx:  0 */
    {0x05000200, 0x04380000, 0x00800000}, /* idx:  1 */
    {0x07000000, 0x04380000, 0x00000080}, /* idx:  2 */
    {0x05000200, 0x04380000, 0x00000080}, /* idx:  3 */
    {0x03000400, 0x04380000, 0x00800000}, /* idx:  4 */
    {0x01000600, 0x04380000, 0x00800000}, /* idx:  5 */
    {0x03000400, 0x04380000, 0x00000080}, /* idx:  6 */
    {0x01000600, 0x04380000, 0x00000080}, /* idx:  7 */
    {0x07000000, 0x04300008, 0x00800000}, /* idx:  8 */
    {0x05000200, 0x04300008, 0x00800000}, /* idx:  9 */
    {0x07000000, 0x04300008, 0x00000080}, /* idx: 10 */
    {0x05000200, 0x04300008, 0x00000080}, /* idx: 11 */
    {0x03000400, 0x04300008, 0x00800000}, /* idx: 12 */
    {0x01000600, 0x04300008, 0x00800000}, /* idx: 13 */
    {0x03000400, 0x04300008, 0x00000080}, /* idx: 14 */
    {0x01000600, 0x04300008, 0x00000080}, /* idx: 15 */
    {0x07000000, 0x04180020, 0x00800000}, /* idx: 16 */
    {0x05000200, 0x04180020, 0x00800000}, /* idx: 17 */
    {0x07000000, 0x04180020, 0x00000080}, /* idx: 18 */
    {0x05000200, 0x04180020, 0x00000080}, /* idx: 19 */
    {0x03000400, 0x04180020, 0x00800000}, /* idx: 20 */
    {0x01000600, 0x04180020, 0x00800000}, /* idx: 21 */
    {0x03000400, 0x04180020, 0x00000080}, /* idx: 22 */
    {0x01000600, 0x04180020, 0x00000080}, /* idx: 23 */
    {0x07000000, 0x04100028, 0x00800000}, /* idx: 24 */
    {0x05000200, 0x04100028, 0x00800000}, /* idx: 25 */
    {0x07000000, 0x04100028, 0x00000080}, /* idx: 26 */
    {0x05000200, 0x04100028, 0x00000080}, /* idx: 27 */
    {0x03000400, 0x04100028, 0x00800000}, /* idx: 28 */
    {0x01000600, 0x04100028, 0x00800000}, /* idx: 29 */
    {0x03000400, 0x04100028, 0x00000080}, /* idx: 30 */
    {0x01000600, 0x04100028, 0x00000080}, /* idx: 31 */
    {0x07000000, 0x04280010, 0x00800000}, /* idx: 32 */
    {0x05000200, 0x04280010, 0x00800000}, /* idx: 33 */
    {0x07000000, 0x04280010, 0x00000080}, /* idx: 34 */
    {0x05000200, 0x04280010, 0x00000080}, /* idx: 35 */
    {0x03000400, 0x04280010, 0x00800000}, /* idx: 36 */
    {0x01000600, 0x04280010, 0x00800000}, /* idx: 37 */
    {0x03000400, 0x04280010, 0x00000080}, /* idx: 38 */
    {0x01000600, 0x04280010, 0x00000080}, /* idx: 39 */
    {0x07000000, 0x04200018, 0x00800000}, /* idx: 40 */
    {0x05000200, 0x04200018, 0x00800000}, /* idx: 41 */
    {0x07000000, 0x04200018, 0x00000080}, /* idx: 42 */
    {0x05000200, 0x04200018, 0x00000080}, /* idx: 43 */
    {0x03000400, 0x04200018, 0x00800000}, /* idx: 44 */
    {0x01000600, 0x04200018, 0x00800000}, /* idx: 45 */
    {0x03000400, 0x04200018, 0x00000080}, /* idx: 46 */
    {0x01000600, 0x04200018, 0x00000080}, /* idx: 47 */
    {0x07000000, 0x04080030, 0x00800000}, /* idx: 48 */
    {0x05000200, 0x04080030, 0x00800000}, /* idx: 49 */
    {0x07000000, 0x04080030, 0x00000080}, /* idx: 50 */
    {0x05000200, 0x04080030, 0x00000080}, /* idx: 51 */
    {0x03000400, 0x04080030, 0x00800000}, /* idx: 52 */
    {0x01000600, 0x04080030, 0x00800000}, /* idx: 53 */
    {0x03000400, 0x04080030, 0x00000080}, /* idx: 54 */
    {0x01000600, 0x04080030, 0x00000080}, /* idx: 55 */
    {0x07000000, 0x04000038, 0x00800000}, /* idx: 56 */
    {0x05000200, 0x04000038, 0x00800000}, /* idx: 57 */
    {0x07000000, 0x04000038, 0x00000080}, /* idx: 58 */
    {0x05000200, 0x04000038, 0x00000080}, /* idx: 59 */
    {0x03000400, 0x04000038, 0x00800000}, /* idx: 60 */
    {0x01000600, 0x04000038, 0x00800000}, /* idx: 61 */
    {0x03000400, 0x04000038, 0x00000080}, /* idx: 62 */
    {0x01000600, 0x04000038, 0x00000080}, /* idx: 63 */
    {0x07000000, 0x00380400, 0x00800000}, /* idx: 64 */
    {0x05000200, 0x00380400, 0x00800000}, /* idx: 65 */
    {0x07000000, 0x00380400, 0x00000080}, /* idx: 66 */
    {0x05000200, 0x00380400, 0x00000080}, /* idx: 67 */
    {0x03000400, 0x00380400, 0x00800000}, /* idx: 68 */
    {0x01000600, 0x00380400, 0x00800000}, /* idx: 69 */
    {0x03000400, 0x00380400, 0x00000080}, /* idx: 70 */
    {0x01000600, 0x00380400, 0x00000080}, /* idx: 71 */
    {0x07000000, 0x00300408, 0x00800000}, /* idx: 72 */
    {0x05000200, 0x00300408, 0x00800000}, /* idx: 73 */
    {0x07000000, 0x00300408, 0x00000080}, /* idx: 74 */
    {0x05000200, 0x00300408, 0x00000080}, /* idx: 75 */
    {0x03000400, 0x00300408, 0x00800000}, /* idx: 76 */
    {0x01000600, 0x00300408, 0x00800000}, /* idx: 77 */
    {0x03000400, 0x00300408, 0x00000080}, /* idx: 78 */
    {0x01000600, 0x00300408, 0x00000080}, /* idx: 79 */
    {0x07000000, 0x00180420, 0x00800000}, /* idx: 80 */
    {0x05000200, 0x00180420, 0x00800000}, /* idx: 81 */
    {0x07000000, 0x00180420, 0x00000080}, /* idx: 82 */
    {0x05000200, 0x00180420, 0x00000080}, /* idx: 83 */
    {0x03000400, 0x00180420, 0x00800000}, /* idx: 84 */
    {0x01000600, 0x00180420, 0x00800000}, /* idx: 85 */
    {0x03000400, 0x00180420, 0x00000080}, /* idx: 86 */
    {0x01000600, 0x00180420, 0x00000080}, /* idx: 87 */
    {0x07000000, 0x00100428, 0x00800000}, /* idx: 88 */
    {0x05000200, 0x00100428, 0x00800000}, /* idx: 89 */
    {0x07000000, 0x00100428, 0x00000080}, /* idx: 90 */
    {0x05000200, 0x00100428, 0x00000080}, /* idx: 91 */
    {0x03000400, 0x00100428, 0x00800000}, /* idx: 92 */
    {0x01000600, 0x00100428, 0x00800000}, /* idx: 93 */
    {0x03000400, 0x00100428, 0x00000080}, /* idx: 94 */
    {0x01000600, 0x00100428, 0x00000080}, /* idx: 95 */
    {0x07000000, 0x00280410, 0x00800000}, /* idx: 96 */
    {0x05000200, 0x00280410, 0x00800000}, /* idx: 97 */
    {0x07000000, 0x00280410, 0x00000080}, /* idx: 98 */
    {0x05000200, 0x00280410, 0x00000080}, /* idx: 99 */
    {0x03000400, 0x00280410, 0x00800000}, /* idx:100 */
    {0x01000600, 0x00280410, 0x00800000}, /* idx:101 */
    {0x03000400, 0x00280410, 0x00000080}, /* idx:102 */
    {0x01000600, 0x00280410, 0x00000080}, /* idx:103 */
    {0x07000000, 0x00200418, 0x00800000}, /* idx:104 */
    {0x05000200, 0x00200418, 0x00800000}, /* idx:105 */
    {0x07000000, 0x00200418, 0x00000080}, /* idx:106 */
    {0x05000200, 0x00200418, 0x00000080}, /* idx:107 */
    {0x03000400, 0x00200418, 0x00800000}, /* idx:108 */
    {0x01000600, 0x00200418, 0x00800000}, /* idx:109 */
    {0x03000400, 0x00200418, 0x00000080}, /* idx:110 */
    {0x01000600, 0x00200418, 0x00000080}, /* idx:111 */
    {0x07000000, 0x00080430, 0x00800000}, /* idx:112 */
    {0x05000200, 0x00080430, 0x00800000}, /* idx:113 */
    {0x07000000, 0x00080430, 0x00000080}, /* idx:114 */
    {0x05000200, 0x00080430, 0x00000080}, /* idx:115 */
    {0x03000400, 0x00080430, 0x00800000}, /* idx:116 */
    {0x01000600, 0x00080430, 0x00800000}, /* idx:117 */
    {0x03000400, 0x00080430, 0x00000080}, /* idx:118 */
    {0x01000600, 0x00080430, 0x00000080}, /* idx:119 */
    {0x07000000, 0x00000438, 0x00800000}, /* idx:120 */
    {0x05000200, 0x00000438, 0x00800000}, /* idx:121 */
    {0x07000000, 0x00000438, 0x00000080}, /* idx:122 */
    {0x05000200, 0x00000438, 0x00000080}, /* idx:123 */
    {0x03000400, 0x00000438, 0x00800000}, /* idx:124 */
    {0x01000600, 0x00000438, 0x00800000}, /* idx:125 */
    {0x03000400, 0x00000438, 0x00000080}, /* idx:126 */
    {0x01000600, 0x00000438, 0x00000080}, /* idx:127 */
    {0x06000100, 0x04380000, 0x00800000}, /* idx:128 */
    {0x04000300, 0x04380000, 0x00800000}, /* idx:129 */
    {0x06000100, 0x04380000, 0x00000080}, /* idx:130 */
    {0x04000300, 0x04380000, 0x00000080}, /* idx:131 */
    {0x02000500, 0x04380000, 0x00800000}, /* idx:132 */
    {0x00000700, 0x04380000, 0x00800000}, /* idx:133 */
    {0x02000500, 0x04380000, 0x00000080}, /* idx:134 */
    {0x00000700, 0x04380000, 0x00000080}, /* idx:135 */
    {0x06000100, 0x04300008, 0x00800000}, /* idx:136 */
    {0x04000300, 0x04300008, 0x00800000}, /* idx:137 */
    {0x06000100, 0x04300008, 0x00000080}, /* idx:138 */
    {0x04000300, 0x04300008, 0x00000080}, /* idx:139 */
    {0x02000500, 0x04300008, 0x00800000}, /* idx:140 */
    {0x00000700, 0x04300008, 0x00800000}, /* idx:141 */
    {0x02000500, 0x04300008, 0x00000080}, /* idx:142 */
    {0x00000700, 0x04300008, 0x00000080}, /* idx:143 */
    {0x06000100, 0x04180020, 0x00800000}, /* idx:144 */
    {0x04000300, 0x04180020, 0x00800000}, /* idx:145 */
    {0x06000100, 0x04180020, 0x00000080}, /* idx:146 */
    {0x04000300, 0x04180020, 0x00000080}, /* idx:147 */
    {0x02000500, 0x04180020, 0x00800000}, /* idx:148 */
    {0x00000700, 0x04180020, 0x00800000}, /* idx:149 */
    {0x02000500, 0x04180020, 0x00000080}, /* idx:150 */
    {0x00000700, 0x04180020, 0x00000080}, /* idx:151 */
    {0x06000100, 0x04100028, 0x00800000}, /* idx:152 */
    {0x04000300, 0x04100028, 0x00800000}, /* idx:153 */
    {0x06000100, 0x04100028, 0x00000080}, /* idx:154 */
    {0x04000300, 0x04100028, 0x00000080}, /* idx:155 */
    {0x02000500, 0x04100028, 0x00800000}, /* idx:156 */
    {0x00000700, 0x04100028, 0x00800000}, /* idx:157 */
    {0x02000500, 0x04100028, 0x00000080}, /* idx:158 */
    {0x00000700, 0x04100028, 0x00000080}, /* idx:159 */
    {0x06000100, 0x04280010, 0x00800000}, /* idx:160 */
    {0x04000300, 0x04280010, 0x00800000}, /* idx:161 */
    {0x06000100, 0x04280010, 0x00000080}, /* idx:162 */
    {0x04000300, 0x04280010, 0x00000080}, /* idx:163 */
    {0x02000500, 0x04280010, 0x00800000}, /* idx:164 */
    {0x00000700, 0x04280010, 0x00800000}, /* idx:165 */
    {0x02000500, 0x04280010, 0x00000080}, /* idx:166 */
    {0x00000700, 0x04280010, 0x00000080}, /* idx:167 */
    {0x06000100, 0x04200018, 0x00800000}, /* idx:168 */
    {0x04000300, 0x04200018, 0x00800000}, /* idx:169 */
    {0x06000100, 0x04200018, 0x00000080}, /* idx:170 */
    {0x04000300, 0x04200018, 0x00000080}, /* idx:171 */
    {0x02000500, 0x04200018, 0x00800000}, /* idx:172 */
    {0x00000700, 0x04200018, 0x00800000}, /* idx:173 */
    {0x02000500, 0x04200018, 0x00000080}, /* idx:174 */
    {0x00000700, 0x04200018, 0x00000080}, /* idx:175 */
    {0x06000100, 0x04080030, 0x00800000}, /* idx:176 */
    {0x04000300, 0x04080030, 0x00800000}, /* idx:177 */
    {0x06000100, 0x04080030, 0x00000080}, /* idx:178 */
    {0x04000300, 0x04080030, 0x00000080}, /* idx:179 */
    {0x02000500, 0x04080030, 0x00800000}, /* idx:180 */
    {0x00000700, 0x04080030, 0x00800000}, /* idx:181 */
    {0x02000500, 0x04080030, 0x00000080}, /* idx:182 */
    {0x00000700, 0x04080030, 0x00000080}, /* idx:183 */
    {0x06000100, 0x04000038, 0x00800000}, /* idx:184 */
    {0x04000300, 0x04000038, 0x00800000}, /* idx:185 */
    {0x06000100, 0x04000038, 0x00000080}, /* idx:186 */
    {0x04000300, 0x04000038, 0x00000080}, /* idx:187 */
    {0x02000500, 0x04000038, 0x00800000}, /* idx:188 */
    {0x00000700, 0x04000038, 0x00800000}, /* idx:189 */
    {0x02000500, 0x04000038, 0x00000080}, /* idx:190 */
    {0x00000700, 0x04000038, 0x00000080}, /* idx:191 */
    {0x06000100, 0x00380400, 0x00800000}, /* idx:192 */
    {0x04000300, 0x00380400, 0x00800000}, /* idx:193 */
    {0x06000100, 0x00380400, 0x00000080}, /* idx:194 */
    {0x04000300, 0x00380400, 0x00000080}, /* idx:195 */
    {0x02000500, 0x00380400, 0x00800000}, /* idx:196 */
    {0x00000700, 0x00380400, 0x00800000}, /* idx:197 */
    {0x02000500, 0x00380400, 0x00000080}, /* idx:198 */
    {0x00000700, 0x00380400, 0x00000080}, /* idx:199 */
    {0x06000100, 0x00300408, 0x00800000}, /* idx:200 */
    {0x04000300, 0x00300408, 0x00800000}, /* idx:201 */
    {0x06000100, 0x00300408, 0x00000080}, /* idx:202 */
    {0x04000300, 0x00300408, 0x00000080}, /* idx:203 */
    {0x02000500, 0x00300408, 0x00800000}, /* idx:204 */
    {0x00000700, 0x00300408, 0x00800000}, /* idx:205 */
    {0x02000500, 0x00300408, 0x00000080}, /* idx:206 */
    {0x00000700, 0x00300408, 0x00000080}, /* idx:207 */
    {0x06000100, 0x00180420, 0x00800000}, /* idx:208 */
    {0x04000300, 0x00180420, 0x00800000}, /* idx:209 */
    {0x06000100, 0x00180420, 0x00000080}, /* idx:210 */
    {0x04000300, 0x00180420, 0x00000080}, /* idx:211 */
    {0x02000500, 0x00180420, 0x00800000}, /* idx:212 */
    {0x00000700, 0x00180420, 0x00800000}, /* idx:213 */
    {0x02000500, 0x00180420, 0x00000080}, /* idx:214 */
    {0x00000700, 0x00180420, 0x00000080}, /* idx:215 */
    {0x06000100, 0x00100428, 0x00800000}, /* idx:216 */
    {0x04000300, 0x00100428, 0x00800000}, /* idx:217 */
    {0x06000100, 0x00100428, 0x00000080}, /* idx:218 */
    {0x04000300, 0x00100428, 0x00000080}, /* idx:219 */
    {0x02000500, 0x00100428, 0x00800000}, /* idx:220 */
    {0x00000700, 0x00100428, 0x00800000}, /* idx:221 */
    {0x02000500, 0x00100428, 0x00000080}, /* idx:222 */
    {0x00000700, 0x00100428, 0x00000080}, /* idx:223 */
    {0x06000100, 0x00280410, 0x00800000}, /* idx:224 */
    {0x04000300, 0x00280410, 0x00800000}, /* idx:225 */
    {0x06000100, 0x00280410, 0x00000080}, /* idx:226 */
    {0x04000300, 0x00280410, 0x00000080}, /* idx:227 */
    {0x02000500, 0x00280410, 0x00800000}, /* idx:228 */
    {0x00000700, 0x00280410, 0x00800000}, /* idx:229 */
    {0x02000500, 0x00280410, 0x00000080}, /* idx:230 */
    {0x00000700, 0x00280410, 0x00000080}, /* idx:231 */
    {0x06000100, 0x00200418, 0x00800000}, /* idx:232 */
    {0x04000300, 0x00200418, 0x00800000}, /* idx:233 */
    {0x06000100, 0x00200418, 0x00000080}, /* idx:234 */
    {0x04000300, 0x00200418, 0x00000080}, /* idx:235 */
    {0x02000500, 0x00200418, 0x00800000}, /* idx:236 */
    {0x00000700, 0x00200418, 0x00800000}, /* idx:237 */
    {0x02000500, 0x00200418, 0x00000080}, /* idx:238 */
    {0x00000700, 0x00200418, 0x00000080}, /* idx:239 */
    {0x06000100, 0x00080430, 0x00800000}, /* idx:240 */
    {0x04000300, 0x00080430, 0x00800000}, /* idx:241 */
    {0x06000100, 0x00080430, 0x00000080}, /* idx:242 */
    {0x04000300, 0x00080430, 0x00000080}, /* idx:243 */
    {0x02000500, 0x00080430, 0x00800000}, /* idx:244 */
    {0x00000700, 0x00080430, 0x00800000}, /* idx:245 */
    {0x02000500, 0x00080430, 0x00000080}, /* idx:246 */
    {0x00000700, 0x00080430, 0x00000080}, /* idx:247 */
    {0x06000100, 0x00000438, 0x00800000}, /* idx:248 */
    {0x04000300, 0x00000438, 0x00800000}, /* idx:249 */
    {0x06000100, 0x00000438, 0x00000080}, /* idx:250 */
    {0x04000300, 0x00000438, 0x00000080}, /* idx:251 */
    {0x02000500, 0x00000438, 0x00800000}, /* idx:252 */
    {0x00000700, 0x00000438, 0x00800000}, /* idx:253 */
    {0x02000500, 0x00000438, 0x00000080}, /* idx:254 */
    {0x00000700, 0x00000438, 0x00000080}  /* idx:255 */
};
#endif

/* Index for gpio bank pin corresponding to display pin */
#define D0 9    // Port A
#define D1 7    // Port C
#define D2 10   // Port A
#define D3 3    // Port B
#define D4 5    // Port B
#define D5 4    // Port B
#define D6 10   // Port B
#define D7 8    // Port A

#define RST 1   // Port C
#define CS  0   // Port B
#define RS  4   // Port A
#define WR  1   // Port A
#define RD  0   // Port A

/**
 * @brief Data pin index array
 */
static const uint8_t data_pin_idxs[] =
{
    D0, D1, D2, D3, D4,
    D5, D6, D7
};

#if IO_PINS_SETTER == IO_PINS_SETTER_SEQ
/**
 * @brief Data pin masks array
 */
static const uint32_t data_pin_masks[] =
{
    1 << D0, 1 << D1, 1 << D2, 1 << D3,
    1 << D4, 1 << D5, 1 << D6, 1 << D7,
};
#endif

/* Port definitions */
#define D0_PORT     GPIOA
#define D1_PORT     GPIOC
#define D2_PORT     GPIOA
#define D3_PORT     GPIOB
#define D4_PORT     GPIOB
#define D5_PORT     GPIOB
#define D6_PORT     GPIOB
#define D7_PORT     GPIOA
#define RST_PORT    GPIOC
#define CS_PORT     GPIOB
#define RS_PORT     GPIOA
#define WR_PORT     GPIOA
#define RD_PORT     GPIOA

/**
 * @brief Data port array
 * @note Used to loop over data pins. See e.g. @ref io_dpins_set
 */
static GPIO_TypeDef* data_pin_ports[] =
{
    D0_PORT, D1_PORT, D2_PORT,
    D3_PORT, D4_PORT, D5_PORT,
    D6_PORT, D7_PORT
};

/* Mask definitions */
#define D0_MODER_MODE   GPIO_MODER_MODE9
#define D1_MODER_MODE   GPIO_MODER_MODE7
#define D2_MODER_MODE   GPIO_MODER_MODE10
#define D3_MODER_MODE   GPIO_MODER_MODE3
#define D4_MODER_MODE   GPIO_MODER_MODE5
#define D5_MODER_MODE   GPIO_MODER_MODE4
#define D6_MODER_MODE   GPIO_MODER_MODE10
#define D7_MODER_MODE   GPIO_MODER_MODE8
#define RST_MODER_MODE  GPIO_MODER_MODE1
#define CS_MODER_MODE   GPIO_MODER_MODE0
#define RS_MODER_MODE   GPIO_MODER_MODE4
#define WR_MODER_MODE   GPIO_MODER_MODE1
#define RD_MODER_MODE   GPIO_MODER_MODE0

#define D0_MODER_MODE_OUT   GPIO_MODER_MODE9_0
#define D1_MODER_MODE_OUT   GPIO_MODER_MODE7_0
#define D2_MODER_MODE_OUT   GPIO_MODER_MODE10_0
#define D3_MODER_MODE_OUT   GPIO_MODER_MODE3_0
#define D4_MODER_MODE_OUT   GPIO_MODER_MODE5_0
#define D5_MODER_MODE_OUT   GPIO_MODER_MODE4_0
#define D6_MODER_MODE_OUT   GPIO_MODER_MODE10_0
#define D7_MODER_MODE_OUT   GPIO_MODER_MODE8_0
#define RST_MODER_MODE_OUT  GPIO_MODER_MODE1_0
#define CS_MODER_MODE_OUT   GPIO_MODER_MODE0_0
#define RS_MODER_MODE_OUT   GPIO_MODER_MODE4_0
#define WR_MODER_MODE_OUT   GPIO_MODER_MODE1_0
#define RD_MODER_MODE_OUT   GPIO_MODER_MODE0_0

/* Speed definitions */
#define D0_OSPEEDER_OSPEED  GPIO_OSPEEDER_OSPEED9
#define D1_OSPEEDER_OSPEED  GPIO_OSPEEDER_OSPEED7
#define D2_OSPEEDER_OSPEED  GPIO_OSPEEDER_OSPEED10
#define D3_OSPEEDER_OSPEED  GPIO_OSPEEDER_OSPEED3
#define D4_OSPEEDER_OSPEED  GPIO_OSPEEDER_OSPEED5
#define D5_OSPEEDER_OSPEED  GPIO_OSPEEDER_OSPEED4
#define D6_OSPEEDER_OSPEED  GPIO_OSPEEDER_OSPEED10
#define D7_OSPEEDER_OSPEED  GPIO_OSPEEDER_OSPEED8
#define RST_OSPEEDER_OSPEED GPIO_OSPEEDER_OSPEED1
#define CS_OSPEEDER_OSPEED  GPIO_OSPEEDER_OSPEED0
#define RS_OSPEEDER_OSPEED  GPIO_OSPEEDER_OSPEED4
#define WR_OSPEEDER_OSPEED  GPIO_OSPEEDER_OSPEED1
#define RD_OSPEEDER_OSPEED  GPIO_OSPEEDER_OSPEED0


/* Functional definitions */
/**
 * @brief Setter for reset pin
 * @param state[in] Use @ref IO_STATE_E
 */
inline void io_rst_set(uint8_t state)
{
    if (state == IO_STATE_ACTIVATE)
    {
        RST_PORT->BSRR = 1 << RST;
        return;
    }
    RST_PORT->BRR = 1 << RST;
}

/**
 * @brief Setter for read control pin
 * @param state[in] Use @ref IO_STATE_E
 */
inline void io_rd_set(uint8_t state)
{
    if (state == IO_STATE_ACTIVATE)
    {
        RD_PORT->BRR = 1 << RD;
        return;
    }
    RD_PORT->BSRR = 1 << RD;
}

/**
 * @brief Setter for chip select pin
 * @param state[in] If non-zero, chip select is activated. Use @ref PMI_BOOL_E
 */
inline void io_cs_set(uint8_t state)
{
    if (state == IO_STATE_ACTIVATE)
    {
        CS_PORT->BRR = 1 << CS;
        return;
    }
    CS_PORT->BSRR = 1 << CS;
}

/**
 * @brief Setter for write control pin
 * @param state[in] Use @ref IO_STATE_E
 */
inline void io_wr_set(uint8_t state)
{
    if (state == IO_STATE_ACTIVATE)
    {
        WR_PORT->BRR = 1 << WR;
        return;
    }
    WR_PORT->BSRR = 1 << WR;
}

/**
 * @brief Setter for read or command pin
 * @param cmd[in] If non-zero, pin is set to command. Use @ref IO_RS_E
 */
inline void io_rs_set(uint8_t cmd)
{
    if (cmd == IO_RS_CMD)
    {
        RS_PORT->BRR = 1 << RS;
        return;
    }
    RS_PORT->BSRR = 1 << RS;
}

/**
 * @brief Enable clock for all necessary ports and set every pin to output and
 *  the highest output speed configuration
 */
void io_gpio_init(void)
{
    /* Enable clock */
    RCC->IOPENR |= (RCC_IOPENR_GPIOAEN | RCC_IOPENR_GPIOBEN |
                    RCC_IOPENR_GPIOCEN);

    MODIFY_REG(CS_PORT->MODER, CS_MODER_MODE, CS_MODER_MODE_OUT);
    MODIFY_REG(RS_PORT->MODER, RS_MODER_MODE, RS_MODER_MODE_OUT);
    MODIFY_REG(WR_PORT->MODER, WR_MODER_MODE, WR_MODER_MODE_OUT);
    MODIFY_REG(RD_PORT->MODER, RD_MODER_MODE, RD_MODER_MODE_OUT);
    MODIFY_REG(RST_PORT->MODER, RST_MODER_MODE, RST_MODER_MODE_OUT);

    io_dpins_as_output();

    MODIFY_REG(D0_PORT->OSPEEDR, D0_OSPEEDER_OSPEED, D0_OSPEEDER_OSPEED);
    MODIFY_REG(D1_PORT->OSPEEDR, D1_OSPEEDER_OSPEED, D1_OSPEEDER_OSPEED);
    MODIFY_REG(D2_PORT->OSPEEDR, D2_OSPEEDER_OSPEED, D2_OSPEEDER_OSPEED);
    MODIFY_REG(D3_PORT->OSPEEDR, D3_OSPEEDER_OSPEED, D3_OSPEEDER_OSPEED);
    MODIFY_REG(D4_PORT->OSPEEDR, D4_OSPEEDER_OSPEED, D4_OSPEEDER_OSPEED);
    MODIFY_REG(D5_PORT->OSPEEDR, D5_OSPEEDER_OSPEED, D5_OSPEEDER_OSPEED);
    MODIFY_REG(D6_PORT->OSPEEDR, D6_OSPEEDER_OSPEED, D6_OSPEEDER_OSPEED);
    MODIFY_REG(D7_PORT->OSPEEDR, D7_OSPEEDER_OSPEED, D7_OSPEEDER_OSPEED);
    MODIFY_REG(CS_PORT->OSPEEDR, CS_OSPEEDER_OSPEED, CS_OSPEEDER_OSPEED);
    MODIFY_REG(RS_PORT->OSPEEDR, RS_OSPEEDER_OSPEED, RS_OSPEEDER_OSPEED);
    MODIFY_REG(WR_PORT->OSPEEDR, WR_OSPEEDER_OSPEED, WR_OSPEEDER_OSPEED);
    MODIFY_REG(RD_PORT->OSPEEDR, RD_OSPEEDER_OSPEED, RD_OSPEEDER_OSPEED);
    MODIFY_REG(RST_PORT->OSPEEDR, RST_OSPEEDER_OSPEED, RST_OSPEEDER_OSPEED);

    io_dpins_set(0x00);
}

/**
 * @brief Set all data Pins to output
 */
void io_dpins_as_output(void)
{
    MODIFY_REG(D0_PORT->MODER, D0_MODER_MODE, D0_MODER_MODE_OUT);
    MODIFY_REG(D1_PORT->MODER, D1_MODER_MODE, D1_MODER_MODE_OUT);
    MODIFY_REG(D2_PORT->MODER, D2_MODER_MODE, D2_MODER_MODE_OUT);
    MODIFY_REG(D3_PORT->MODER, D3_MODER_MODE, D3_MODER_MODE_OUT);
    MODIFY_REG(D4_PORT->MODER, D4_MODER_MODE, D4_MODER_MODE_OUT);
    MODIFY_REG(D5_PORT->MODER, D5_MODER_MODE, D5_MODER_MODE_OUT);
    MODIFY_REG(D6_PORT->MODER, D6_MODER_MODE, D6_MODER_MODE_OUT);
    MODIFY_REG(D7_PORT->MODER, D7_MODER_MODE, D7_MODER_MODE_OUT);
}

/**
 * @brief Set all data pins to input
 */
void io_dpins_as_input(void)
{
    D0_PORT->MODER &= ~D0_MODER_MODE;
    D1_PORT->MODER &= ~D1_MODER_MODE;
    D2_PORT->MODER &= ~D2_MODER_MODE;
    D3_PORT->MODER &= ~D3_MODER_MODE;
    D4_PORT->MODER &= ~D4_MODER_MODE;
    D5_PORT->MODER &= ~D5_MODER_MODE;
    D6_PORT->MODER &= ~D6_MODER_MODE;
    D7_PORT->MODER &= ~D7_MODER_MODE;
}

/**
 * @brief Applies byte on data pins
 * @note Set pins to output first. See @ref io_dpins_as_output
 * @param b[in] Data byte, LSB corresponds to D0, MSB corresponds to D7
 */
void io_dpins_set(uint8_t b)
#if IO_PINS_SETTER == IO_PINS_SETTER_SEQ
{
    static uint8_t state = 0xFF;
    uint8_t i;

    /* Skip if pins already set */
    if (b == state)
    {
        return;
    }
    state = b;

    for (i = 0; i < 8; i++)
    {
        if(b & 1)
        {
            data_pin_ports[i]->BSRR = data_pin_masks[i];
        }
        else
        {
            data_pin_ports[i]->BRR = data_pin_masks[i];
        }

        b = b >> 1;
    }
}
#elif IO_PINS_SETTER == IO_PINS_SETTER_LOOKUPTABLE
{
    static uint8_t state = 0xFF;
    const struct data_pins_lookup_table_s *e;

    /* Skip if pins already set */
    if (b == state)
    {
        return;
    }
    state = b;

    /* Lookup */
    e = &dlt[b];

    /* Set registers */
    GPIOA->BSRR = e->bsrr_a;
    GPIOB->BSRR = e->bsrr_b;
    GPIOC->BSRR = e->bsrr_c;
}
#endif

/**
 * @brief Gets current state on parallel bus
 * @note Pins have to be set as inputs by @ref ili9341_set_dir_read
 * @return Data mask where MSB corresponds to D0 and LSB corresponds to D7
 */
uint8_t io_dpins_get(void)
{
    uint8_t data = 0;
    uint8_t i;

    for(i = 0; i < 8; i++)
    {
        if(data_pin_ports[i]->IDR & (1 << data_pin_idxs[i]))
        {
            data += (1 << i);
        }
    }
    return data;
}
