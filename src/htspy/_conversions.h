#include "stdint.h"
#include "htslib/sam.h"

static const char bam_cigar_table[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 0-15
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 16-31
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 32-47
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, BAM_CEQUAL, -1, -1,  // 48-63
    -1, -1, BAM_CBACK, -1, BAM_CDEL, -1, -1, -1, BAM_CHARD_CLIP, BAM_CINS, -1, -1, -1, BAM_CMATCH, BAM_CREF_SKIP, -1,  // 64-79
    BAM_CPAD, -1, -1, BAM_CSOFT_CLIP, -1, -1, -1, -1, BAM_CDIFF, -1, -1, -1, -1, -1, -1, -1,  // 80-95
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 96-111
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 112-127
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 128-143
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 144-159
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 160-175
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 176-191
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 192-207
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 208-223
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 224-239
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 240-255
};

static const char nucleotide_to_number[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 0-15
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 16-31
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 32-47
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  0, -1, -1,  // 48-63
    -1,  1, 14,  2, 13, -1, -1,  4, 11, -1, -1, 12, -1,  3, 15, -1,  // 64-79
    -1, -1,  5,  6,  8, -1,  7,  9, -1, 10, -1, -1, -1, -1, -1, -1,  // 80-95
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 96-111
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 112-127
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 128-143
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 144-159
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 160-175
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 176-191
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 192-207
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 208-223
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 224-239
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 240-255
};

static const uint16_t number_to_nucleotide_pair[256] = {
    '==', '=A', '=C', '=M', '=G', '=R', '=S', '=V', '=T', '=W', '=Y', '=H',  // 0-11
    '=K', '=D', '=B', '=N', 'A=', 'AA', 'AC', 'AM', 'AG', 'AR', 'AS', 'AV',  // 12-23
    'AT', 'AW', 'AY', 'AH', 'AK', 'AD', 'AB', 'AN', 'C=', 'CA', 'CC', 'CM',  // 24-35
    'CG', 'CR', 'CS', 'CV', 'CT', 'CW', 'CY', 'CH', 'CK', 'CD', 'CB', 'CN',  // 36-47
    'M=', 'MA', 'MC', 'MM', 'MG', 'MR', 'MS', 'MV', 'MT', 'MW', 'MY', 'MH',  // 48-59
    'MK', 'MD', 'MB', 'MN', 'G=', 'GA', 'GC', 'GM', 'GG', 'GR', 'GS', 'GV',  // 60-71
    'GT', 'GW', 'GY', 'GH', 'GK', 'GD', 'GB', 'GN', 'R=', 'RA', 'RC', 'RM',  // 72-83
    'RG', 'RR', 'RS', 'RV', 'RT', 'RW', 'RY', 'RH', 'RK', 'RD', 'RB', 'RN',  // 84-95
    'S=', 'SA', 'SC', 'SM', 'SG', 'SR', 'SS', 'SV', 'ST', 'SW', 'SY', 'SH',  // 96-107
    'SK', 'SD', 'SB', 'SN', 'V=', 'VA', 'VC', 'VM', 'VG', 'VR', 'VS', 'VV',  // 108-119
    'VT', 'VW', 'VY', 'VH', 'VK', 'VD', 'VB', 'VN', 'T=', 'TA', 'TC', 'TM',  // 120-131
    'TG', 'TR', 'TS', 'TV', 'TT', 'TW', 'TY', 'TH', 'TK', 'TD', 'TB', 'TN',  // 132-143
    'W=', 'WA', 'WC', 'WM', 'WG', 'WR', 'WS', 'WV', 'WT', 'WW', 'WY', 'WH',  // 144-155
    'WK', 'WD', 'WB', 'WN', 'Y=', 'YA', 'YC', 'YM', 'YG', 'YR', 'YS', 'YV',  // 156-167
    'YT', 'YW', 'YY', 'YH', 'YK', 'YD', 'YB', 'YN', 'H=', 'HA', 'HC', 'HM',  // 168-179
    'HG', 'HR', 'HS', 'HV', 'HT', 'HW', 'HY', 'HH', 'HK', 'HD', 'HB', 'HN',  // 180-191
    'K=', 'KA', 'KC', 'KM', 'KG', 'KR', 'KS', 'KV', 'KT', 'KW', 'KY', 'KH',  // 192-203
    'KK', 'KD', 'KB', 'KN', 'D=', 'DA', 'DC', 'DM', 'DG', 'DR', 'DS', 'DV',  // 204-215
    'DT', 'DW', 'DY', 'DH', 'DK', 'DD', 'DB', 'DN', 'B=', 'BA', 'BC', 'BM',  // 216-227
    'BG', 'BR', 'BS', 'BV', 'BT', 'BW', 'BY', 'BH', 'BK', 'BD', 'BB', 'BN',  // 228-239
    'N=', 'NA', 'NC', 'NM', 'NG', 'NR', 'NS', 'NV', 'NT', 'NW', 'NY', 'NH',  // 240-251
    'NK', 'ND', 'NB', 'NN',  // 252-255
};
