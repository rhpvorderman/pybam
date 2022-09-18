/* Generated by generate_conversions.py, do not modify by hand */
#define PY_SSIZE_T_CLEAN
#include <Python.h>
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
     0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  // 0-15
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

static const uint16_t number_to_nucleotide_pair_le[256] = {
    0x3d3d, 0x413d, 0x433d, 0x4d3d, 0x473d, 0x523d, 0x533d, 0x563d,  // 0-7
    0x543d, 0x573d, 0x593d, 0x483d, 0x4b3d, 0x443d, 0x423d, 0x4e3d,  // 8-15
    0x3d41, 0x4141, 0x4341, 0x4d41, 0x4741, 0x5241, 0x5341, 0x5641,  // 16-23
    0x5441, 0x5741, 0x5941, 0x4841, 0x4b41, 0x4441, 0x4241, 0x4e41,  // 24-31
    0x3d43, 0x4143, 0x4343, 0x4d43, 0x4743, 0x5243, 0x5343, 0x5643,  // 32-39
    0x5443, 0x5743, 0x5943, 0x4843, 0x4b43, 0x4443, 0x4243, 0x4e43,  // 40-47
    0x3d4d, 0x414d, 0x434d, 0x4d4d, 0x474d, 0x524d, 0x534d, 0x564d,  // 48-55
    0x544d, 0x574d, 0x594d, 0x484d, 0x4b4d, 0x444d, 0x424d, 0x4e4d,  // 56-63
    0x3d47, 0x4147, 0x4347, 0x4d47, 0x4747, 0x5247, 0x5347, 0x5647,  // 64-71
    0x5447, 0x5747, 0x5947, 0x4847, 0x4b47, 0x4447, 0x4247, 0x4e47,  // 72-79
    0x3d52, 0x4152, 0x4352, 0x4d52, 0x4752, 0x5252, 0x5352, 0x5652,  // 80-87
    0x5452, 0x5752, 0x5952, 0x4852, 0x4b52, 0x4452, 0x4252, 0x4e52,  // 88-95
    0x3d53, 0x4153, 0x4353, 0x4d53, 0x4753, 0x5253, 0x5353, 0x5653,  // 96-103
    0x5453, 0x5753, 0x5953, 0x4853, 0x4b53, 0x4453, 0x4253, 0x4e53,  // 104-111
    0x3d56, 0x4156, 0x4356, 0x4d56, 0x4756, 0x5256, 0x5356, 0x5656,  // 112-119
    0x5456, 0x5756, 0x5956, 0x4856, 0x4b56, 0x4456, 0x4256, 0x4e56,  // 120-127
    0x3d54, 0x4154, 0x4354, 0x4d54, 0x4754, 0x5254, 0x5354, 0x5654,  // 128-135
    0x5454, 0x5754, 0x5954, 0x4854, 0x4b54, 0x4454, 0x4254, 0x4e54,  // 136-143
    0x3d57, 0x4157, 0x4357, 0x4d57, 0x4757, 0x5257, 0x5357, 0x5657,  // 144-151
    0x5457, 0x5757, 0x5957, 0x4857, 0x4b57, 0x4457, 0x4257, 0x4e57,  // 152-159
    0x3d59, 0x4159, 0x4359, 0x4d59, 0x4759, 0x5259, 0x5359, 0x5659,  // 160-167
    0x5459, 0x5759, 0x5959, 0x4859, 0x4b59, 0x4459, 0x4259, 0x4e59,  // 168-175
    0x3d48, 0x4148, 0x4348, 0x4d48, 0x4748, 0x5248, 0x5348, 0x5648,  // 176-183
    0x5448, 0x5748, 0x5948, 0x4848, 0x4b48, 0x4448, 0x4248, 0x4e48,  // 184-191
    0x3d4b, 0x414b, 0x434b, 0x4d4b, 0x474b, 0x524b, 0x534b, 0x564b,  // 192-199
    0x544b, 0x574b, 0x594b, 0x484b, 0x4b4b, 0x444b, 0x424b, 0x4e4b,  // 200-207
    0x3d44, 0x4144, 0x4344, 0x4d44, 0x4744, 0x5244, 0x5344, 0x5644,  // 208-215
    0x5444, 0x5744, 0x5944, 0x4844, 0x4b44, 0x4444, 0x4244, 0x4e44,  // 216-223
    0x3d42, 0x4142, 0x4342, 0x4d42, 0x4742, 0x5242, 0x5342, 0x5642,  // 224-231
    0x5442, 0x5742, 0x5942, 0x4842, 0x4b42, 0x4442, 0x4242, 0x4e42,  // 232-239
    0x3d4e, 0x414e, 0x434e, 0x4d4e, 0x474e, 0x524e, 0x534e, 0x564e,  // 240-247
    0x544e, 0x574e, 0x594e, 0x484e, 0x4b4e, 0x444e, 0x424e, 0x4e4e,  // 248-255
};

/* Conversion functions for Python integers to C integer types.
   The resulting value is stored in the value_store pointer.
   The tag parameter is used to report back errors */


static int StorePyObjectValue_c(PyObject *value, void *value_store, uint8_t *tag){
    long long v = PyLong_AsLongLong(value);
    if ((v == -1L) && PyErr_Occurred()) {
        return 0;
    }
    if ((v < INT8_MIN) || (v > INT8_MAX)) {
        PyErr_Format(
            PyExc_ValueError,
            "Tag '%c%c' with value_type 'c' should have a value "
            "between %ld and %ld.",
            tag[0], tag[1], INT8_MIN, INT8_MAX);
    }
    ((int8_t *)value_store)[0] = (int8_t)v;
    return sizeof(int8_t);
}


static int StorePyObjectValue_C(PyObject *value, void *value_store, uint8_t *tag){
    long long v = PyLong_AsLongLong(value);
    if ((v == -1L) && PyErr_Occurred()) {
        return 0;
    }
    if ((v < 0) || (v > UINT8_MAX)) {
        PyErr_Format(
            PyExc_ValueError,
            "Tag '%c%c' with value_type 'C' should have a value "
            "between %ld and %ld.",
            tag[0], tag[1], 0, UINT8_MAX);
    }
    ((uint8_t *)value_store)[0] = (uint8_t)v;
    return sizeof(uint8_t);
}


static int StorePyObjectValue_s(PyObject *value, void *value_store, uint8_t *tag){
    long long v = PyLong_AsLongLong(value);
    if ((v == -1L) && PyErr_Occurred()) {
        return 0;
    }
    if ((v < INT16_MIN) || (v > INT16_MAX)) {
        PyErr_Format(
            PyExc_ValueError,
            "Tag '%c%c' with value_type 's' should have a value "
            "between %ld and %ld.",
            tag[0], tag[1], INT16_MIN, INT16_MAX);
    }
    ((int16_t *)value_store)[0] = (int16_t)v;
    return sizeof(int16_t);
}


static int StorePyObjectValue_S(PyObject *value, void *value_store, uint8_t *tag){
    long long v = PyLong_AsLongLong(value);
    if ((v == -1L) && PyErr_Occurred()) {
        return 0;
    }
    if ((v < 0) || (v > UINT16_MAX)) {
        PyErr_Format(
            PyExc_ValueError,
            "Tag '%c%c' with value_type 'S' should have a value "
            "between %ld and %ld.",
            tag[0], tag[1], 0, UINT16_MAX);
    }
    ((uint16_t *)value_store)[0] = (uint16_t)v;
    return sizeof(uint16_t);
}


static int StorePyObjectValue_i(PyObject *value, void *value_store, uint8_t *tag){
    long long v = PyLong_AsLongLong(value);
    if ((v == -1L) && PyErr_Occurred()) {
        return 0;
    }
    if ((v < INT32_MIN) || (v > INT32_MAX)) {
        PyErr_Format(
            PyExc_ValueError,
            "Tag '%c%c' with value_type 'i' should have a value "
            "between %ld and %ld.",
            tag[0], tag[1], INT32_MIN, INT32_MAX);
    }
    ((int32_t *)value_store)[0] = (int32_t)v;
    return sizeof(int32_t);
}


static int StorePyObjectValue_I(PyObject *value, void *value_store, uint8_t *tag){
    long long v = PyLong_AsLongLong(value);
    if ((v == -1L) && PyErr_Occurred()) {
        return 0;
    }
    if ((v < 0) || (v > UINT32_MAX)) {
        PyErr_Format(
            PyExc_ValueError,
            "Tag '%c%c' with value_type 'I' should have a value "
            "between %ld and %ld.",
            tag[0], tag[1], 0, UINT32_MAX);
    }
    ((uint32_t *)value_store)[0] = (uint32_t)v;
    return sizeof(uint32_t);
}

