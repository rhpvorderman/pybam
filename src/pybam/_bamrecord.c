#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"         // PyMemberDef
#include <stdint.h>

# define BAM_PROPERTIES_STRUCT_SIZE 36  // The combined size of all integers in BamRecord

typedef struct {
    PyObject_HEAD
    uint32_t block_size;
    int32_t refID; 
    int32_t pos; 
    uint8_t l_read_name;
    uint8_t mapq;
    uint16_t bin;
    uint16_t n_cigar_op;
    uint16_t flag; 
    uint32_t l_seq;
    int32_t next_refID;
    int32_t next_pos;
    int32_t tlen;
    PyObject * read_name;
    PyObject * cigar;
    PyObject * seq;
    PyObject * qual;
    PyObject * tags;
} BamRecord;

# define BAM_PROPERTIES_STRUCT_START offsetof(BamRecord, block_size)

static void
BamRecord_dealloc(BamRecord *self) {
    Py_CLEAR(self->read_name);
    Py_CLEAR(self->cigar);
    Py_CLEAR(self->seq);
    Py_CLEAR(self->qual);
    Py_CLEAR(self->tags);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyMemberDef BamRecord_members[] = {
    {"pos", T_INT, offsetof(BamRecord, pos), READONLY},
    {"mapq", T_UBYTE, offsetof(BamRecord, mapq), READONLY},
    {"flag", T_USHORT, offsetof(BamRecord, flag), READONLY},
    {"next_pos", T_INT, offsetof(BamRecord, next_pos), READONLY},
    {"pnext", T_INT, offsetof(BamRecord, next_pos), READONLY}, // SAM name for next pos.
    {"tlen", T_INT, offsetof(BamRecord, tlen), READONLY},
    {"cigar", T_OBJECT_EX, offsetof(BamRecord, cigar), READONLY},
    {"seq", T_OBJECT_EX, offsetof(BamRecord, seq), READONLY},
    {"qual", T_OBJECT_EX, offsetof(BamRecord, qual), READONLY},
    {"tags", T_OBJECT_EX, offsetof(BamRecord, tags), 0},
    {NULL}
};


static inline Py_ssize_t BamRecord_size(BamRecord * self) {
    // self->l_read_name in the struct should be updated as users can assign 
    // another object to self->read_name
    Py_ssize_t tags_length = PyBytes_Size(self->tags);
    if (tags_length == -1){
        PyErr_SetString(PyExc_TypeError, "tags should be a bytes object.");
        return -1;
    }
    // self->l_read_name is guaranteed to be correct by the getter and setter.
    // The rest of the attributes are readonly so the sizes in the stuct are correct.
    return BAM_PROPERTIES_STRUCT_SIZE +           // All struct integer sizes
           self->l_read_name +                    // Length of read_name + 1 (NUL)
           self->n_cigar_op * sizeof(uint32_t) +  // Length of cigar string
           (self->l_seq + 1 / 2) +                // Length of encoded seq
           (self->l_seq) +                        // Length of qualities
           tags_length;                           // Length of tags
}

// PROPERTIES

PyDoc_STRVAR(BamRecord_qname_doc,
"The name of the aligned read as a string.\n"
"WARNING: this attribute is a property that converts 'read_name' \n"
"To ASCII For faster access use the 'read_name' attribute which \n"
"is an ASCII-encoded bytes object.");

static PyObject * BamRecord_get_qname(BamRecord * self, void* closure) {
    return PyUnicode_FromEncodedObject(self->read_name, "ascii", "strict");
}

static int BamRecord_set_qname(BamRecord * self, PyObject * new_qname, void* closure) {
    PyObject * new_read_name = PyUnicode_AsASCIIString(new_qname);
    if (new_read_name == NULL)
        return -1;
    Py_ssize_t read_name_size = PyBytes_GET_SIZE(new_read_name);
    if (read_name_size > 254) {
        PyErr_SetString(PyExc_ValueError, 
            "read_name may not be larger than 254 characters.");
        Py_DecRef(new_read_name);
        return -1;
    }
    PyObject * old_read_name = self->read_name;
    self->read_name = new_read_name;
    Py_DECREF(old_read_name);
    uint8_t old_l_read_name = self->l_read_name;
    self->l_read_name = (uint8_t)read_name_size + 1;
    self->block_size = self->block_size + self->l_read_name - old_l_read_name;
    return 0;
}

PyDoc_STRVAR(BamRecord_read_name_doc,
"The name of the aligned read as an ASCII encoded bytes object.\n");

static PyObject * BamRecord_get_read_name(BamRecord * self, void* closure) {
    Py_INCREF(self->read_name);
    return self->read_name;
}

static int BamRecord_set_read_name(BamRecord * self, PyObject * new_read_name, void* closure) {
    if (!PyBytes_CheckExact(new_read_name)){
        PyErr_SetString(PyExc_TypeError, "read_name must be a bytes object");
        return -1;
    }
    Py_ssize_t read_name_size = PyBytes_GET_SIZE(new_read_name);
    if (read_name_size > 254) {
        PyErr_SetString(PyExc_ValueError, 
            "read_name may not be larger than 254 characters.");
        return -1;
    }
    PyObject * old_read_name = self->read_name;
    Py_INCREF(new_read_name);
    self->read_name = new_read_name;
    Py_DECREF(old_read_name);
    // Make sure the internal sizes are correct after updating.
    uint8_t old_l_read_name = self->l_read_name;
    self->l_read_name = (uint8_t)read_name_size + 1;
    self->block_size = self->block_size + self->l_read_name - old_l_read_name;
    return 0;
}

PyDoc_STRVAR(BamRecord_tags_doc,
"The raw tags as a bytes object.");

static PyObject * BamRecord_get_tags(BamRecord * self, void* closure) {
    Py_INCREF(self->tags);
    return self->tags;
}

static int BamRecord_set_tags(BamRecord * self, PyObject * new_tags, void* closure) {
    if (!PyBytes_CheckExact(new_tags)){
        PyErr_SetString(PyExc_TypeError, "tags must be a bytes object");
        return -1;
    }
    Py_ssize_t new_tags_size = PyBytes_GET_SIZE(new_tags);
    Py_ssize_t old_tags_size = PyBytes_GET_SIZE(self->tags);
    PyObject * old_tags = self->tags;
    Py_INCREF(new_tags);
    self->tags = new_tags;
    Py_DECREF(old_tags);
    // Make sure the internal sizes are correct after updating.
    self->block_size = self->block_size + new_tags_size - old_tags_size;
    return 0;
}

static PyGetSetDef BamRecord_properties[] = {
    {"qname", (getter)BamRecord_get_qname, (setter)BamRecord_set_qname, 
     BamRecord_qname_doc, NULL},
    {"read_name", (getter)BamRecord_get_read_name, (setter)BamRecord_set_read_name,
     BamRecord_read_name_doc, NULL},
    {"tags", (getter)BamRecord_get_tags, (setter)BamRecord_set_tags,
     BamRecord_tags_doc, NULL},
    {NULL}
};

// METHODS
PyDoc_STRVAR(BamRecord_as_bytes__doc__,
"Return the BAM record as a bytesobject that can be written into a file.");

#define BAMRECORD_AS_BYTES_METHODDEF    \
    {"as_bytes", (PyCFunction)(void(*)(void))BamRecord_as_bytes, METH_NOARGS, BamRecord_as_bytes__doc__}

static PyObject *
BamRecord_as_bytes(BamRecord *self, PyObject *NoArgs){
   return NULL;
}


static PyMethodDef BamRecord_methods[] = {
    BAMRECORD_AS_BYTES_METHODDEF,
    {NULL}
};

PyDoc_STRVAR(BamRecord__doc__, 
"An object that represents an alignment record from a BAM file.");

static PyTypeObject BamRecord_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "_bamrecord.BamRecord",             /* tp_name */
    sizeof(BamRecord),                  /* tp_basicsize */
    0,                                  /* tp_itemsize */
    (destructor)BamRecord_dealloc,      /* tp_dealloc */
    0,                                  /* tp_vectorcall_offset */
    0,                                  /* tp_getattr */
    0,                                  /* tp_setattr */
    0,                                  /* tp_as_async */
    0,                                  /* tp_repr */
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    0,                                  /* tp_hash  */
    0,                                  /* tp_call */
    0,                                  /* tp_str */
    0,                                  /* tp_getattro */
    0,                                  /* tp_setattro */
    0,                                  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,                 /* tp_flags */
    BamRecord__doc__,                   /* tp_doc */
    0,                                  /* tp_traverse */
    0,                                  /* tp_clear */
    0,                                  /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    0,                                  /* tp_iter */
    0,                                  /* tp_iternext */
    BamRecord_methods,                  /* tp_methods */
    BamRecord_members,                  /* tp_members */
    BamRecord_properties,               /* tp_getset */
    0,                                  /* tp_base */
    0,                                  /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    0,                                  /* tp_init */
    0,                                  /* tp_alloc */
    0,                                  /* tp_new */
};

typedef struct {
    PyObject_HEAD 
    Py_buffer view; 
    char * buf;
    Py_ssize_t pos;
    Py_ssize_t len; 
} BamIterator;

static void
BamIterator_dealloc(BamIterator *self) {
    PyBuffer_Release(&(self->view));
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static BamIterator *
BamIterator_iter(BamIterator *self){
    Py_INCREF(self);
    return self;
}

static PyObject *
BamIterator_iternext(BamIterator *self){
    if (self->pos >= self->len){
        PyErr_SetNone(PyExc_StopIteration);
        return NULL;
    }
    Py_ssize_t start_pos = self->pos;

    BamRecord * bam_record = PyObject_New(BamRecord, &BamRecord_Type);
    bam_record->read_name = NULL;
    bam_record->seq = NULL;
    bam_record->cigar = NULL;
    bam_record->qual = NULL;
    bam_record->tags = NULL;

    if ((self->len - self->pos) < BAM_PROPERTIES_STRUCT_SIZE) {
        PyErr_SetString(PyExc_EOFError, "Truncated BAM record");
        Py_DECREF(bam_record);
        return NULL;
    }
    // Copy the bam file data directly into the struct.
    memcpy((void *)bam_record + BAM_PROPERTIES_STRUCT_START, 
            self->buf + self->pos,
            BAM_PROPERTIES_STRUCT_SIZE);

    if (self->pos + bam_record->block_size > self->len) {
        PyErr_SetString(PyExc_EOFError, "Truncated BAM record");
        Py_DECREF(bam_record);
        return NULL;
    }
    self->pos += BAM_PROPERTIES_STRUCT_SIZE;
    bam_record->read_name = PyBytes_FromStringAndSize(
        self->buf + self->pos, bam_record->l_read_name -1);
    if (bam_record->read_name == NULL) {
        Py_DECREF(bam_record);
        return PyErr_NoMemory();
    }
    self->pos += bam_record->l_read_name;

    Py_ssize_t cigar_length = bam_record->n_cigar_op * sizeof(uint32_t);
    bam_record->cigar = PyBytes_FromStringAndSize(
        self->buf + self->pos, cigar_length);
    if (bam_record->cigar == NULL) {
        Py_DECREF(bam_record);
        return PyErr_NoMemory();
    }
    self->pos += cigar_length;

    Py_ssize_t seq_length = (bam_record->l_seq + 1) / 2;
    bam_record->seq = PyBytes_FromStringAndSize(
        self->buf + self->pos, seq_length);
    if (bam_record->seq == NULL) {
        Py_DECREF(bam_record);
        return PyErr_NoMemory();
    }
    self->pos += seq_length;

    bam_record->qual = PyBytes_FromStringAndSize(
        self->buf + self->pos, bam_record->l_seq);
    if (bam_record->qual == NULL) {
        Py_DECREF(bam_record);
        return PyErr_NoMemory();
    }
    self->pos += bam_record->l_seq;

    // Tags are in the remaining block of data.
    Py_ssize_t tags_length = start_pos + bam_record-> block_size - self->pos;
    bam_record->tags = PyBytes_FromStringAndSize(
        self->buf + self-> pos, tags_length);
    if (bam_record->tags == NULL) {
        Py_DECREF(bam_record);
        return PyErr_NoMemory();
    }

    // Should be equal to start_pos + bam->record.block_size
    self->pos += tags_length;

    return (PyObject *)bam_record;
}

static PyTypeObject BamIterator_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "_bamrecord.BamIterator", 
    .tp_basicsize = sizeof(BamIterator),
    .tp_dealloc =(destructor)BamIterator_dealloc,  
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_iter = (PyObject * (*)(PyObject *))BamIterator_iter,
    .tp_iternext = (PyObject * (*)(PyObject *))BamIterator_iternext
};

PyDoc_STRVAR(bam_iterator_doc,
"bam_iterator($module, data, /)\n"
"--\n"
"\n"
"Return an iterator that yields BamRecord objects.\n"
"\n"
"  data\n"
"    A block of raw BAM Record data. May be any object\n"
"    that supports the buffer protocol: bytes, bytearray, memoryview.\n"
);
static PyObject * 
bam_iterator(PyObject *module, PyObject * obj) {
    BamIterator *self = PyObject_New(BamIterator, &BamIterator_Type);
    if (!PyObject_GetBuffer(obj, &(self->view), PyBUF_SIMPLE) == 0) {
        Py_DECREF(self);
        return NULL;
    }
    self->buf = self->view.buf;
    self->pos = 0;
    self->len = self->view.len;
    return (PyObject *)self;
}

static PyMethodDef _bamrecord_methods[] = {
    {"bam_iterator", (PyCFunction)(void(*)(void))bam_iterator,
     METH_O, bam_iterator_doc},
    {NULL}
};

static struct PyModuleDef _bamrecord_module = {
    PyModuleDef_HEAD_INIT,
    "_bamrecord",   /* name of module */
    NULL, /* module documentation, may be NULL */
    -1,
    _bamrecord_methods  /* module methods */
};


PyMODINIT_FUNC
PyInit__bamrecord(void)
{
    PyObject *m;

    m = PyModule_Create(&_bamrecord_module);
    if (m == NULL)
        return NULL;

    if (PyType_Ready(&BamIterator_Type) < 0)
        return NULL;
    PyObject * BamiteratorType = (PyObject *)&BamIterator_Type;
    Py_INCREF(BamiteratorType);
    if (PyModule_AddObject(m, "BamIterator", BamiteratorType) < 0)
        return NULL;

    if (PyType_Ready(&BamRecord_Type) < 0)
        return NULL;
    PyObject * BamRecordType = (PyObject *)&BamRecord_Type;
    Py_INCREF(BamRecordType);
    if (PyModule_AddObject(m, "BamRecord", BamRecordType) < 0) {
        return NULL;
    }
    return m;
}
