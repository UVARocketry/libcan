
#include <Python.h>
#include <stddef.h>

/* this block of #ifs should be kept exactly identical between
   c/_cffi_backend.c, cffi/vengine_cpy.py, cffi/vengine_gen.py
   and cffi/_cffi_include.h */
#if defined(_MSC_VER)
# include <malloc.h>   /* for alloca() */
# if _MSC_VER < 1600   /* MSVC < 2010 */
   typedef __int8 int8_t;
   typedef __int16 int16_t;
   typedef __int32 int32_t;
   typedef __int64 int64_t;
   typedef unsigned __int8 uint8_t;
   typedef unsigned __int16 uint16_t;
   typedef unsigned __int32 uint32_t;
   typedef unsigned __int64 uint64_t;
   typedef __int8 int_least8_t;
   typedef __int16 int_least16_t;
   typedef __int32 int_least32_t;
   typedef __int64 int_least64_t;
   typedef unsigned __int8 uint_least8_t;
   typedef unsigned __int16 uint_least16_t;
   typedef unsigned __int32 uint_least32_t;
   typedef unsigned __int64 uint_least64_t;
   typedef __int8 int_fast8_t;
   typedef __int16 int_fast16_t;
   typedef __int32 int_fast32_t;
   typedef __int64 int_fast64_t;
   typedef unsigned __int8 uint_fast8_t;
   typedef unsigned __int16 uint_fast16_t;
   typedef unsigned __int32 uint_fast32_t;
   typedef unsigned __int64 uint_fast64_t;
   typedef __int64 intmax_t;
   typedef unsigned __int64 uintmax_t;
# else
#  include <stdint.h>
# endif
# if _MSC_VER < 1800   /* MSVC < 2013 */
#  ifndef __cplusplus
    typedef unsigned char _Bool;
#  endif
# endif
#else
# include <stdint.h>
# if (defined (__SVR4) && defined (__sun)) || defined(_AIX) || defined(__hpux)
#  include <alloca.h>
# endif
#endif

#if PY_MAJOR_VERSION < 3
# undef PyCapsule_CheckExact
# undef PyCapsule_GetPointer
# define PyCapsule_CheckExact(capsule) (PyCObject_Check(capsule))
# define PyCapsule_GetPointer(capsule, name) \
    (PyCObject_AsVoidPtr(capsule))
#endif

#if PY_MAJOR_VERSION >= 3
# define PyInt_FromLong PyLong_FromLong
#endif

#define _cffi_from_c_double PyFloat_FromDouble
#define _cffi_from_c_float PyFloat_FromDouble
#define _cffi_from_c_long PyInt_FromLong
#define _cffi_from_c_ulong PyLong_FromUnsignedLong
#define _cffi_from_c_longlong PyLong_FromLongLong
#define _cffi_from_c_ulonglong PyLong_FromUnsignedLongLong
#define _cffi_from_c__Bool PyBool_FromLong

#define _cffi_to_c_double PyFloat_AsDouble
#define _cffi_to_c_float PyFloat_AsDouble

#define _cffi_from_c_int_const(x)                                        \
    (((x) > 0) ?                                                         \
        ((unsigned long long)(x) <= (unsigned long long)LONG_MAX) ?      \
            PyInt_FromLong((long)(x)) :                                  \
            PyLong_FromUnsignedLongLong((unsigned long long)(x)) :       \
        ((long long)(x) >= (long long)LONG_MIN) ?                        \
            PyInt_FromLong((long)(x)) :                                  \
            PyLong_FromLongLong((long long)(x)))

#define _cffi_from_c_int(x, type)                                        \
    (((type)-1) > 0 ? /* unsigned */                                     \
        (sizeof(type) < sizeof(long) ?                                   \
            PyInt_FromLong((long)x) :                                    \
         sizeof(type) == sizeof(long) ?                                  \
            PyLong_FromUnsignedLong((unsigned long)x) :                  \
            PyLong_FromUnsignedLongLong((unsigned long long)x)) :        \
        (sizeof(type) <= sizeof(long) ?                                  \
            PyInt_FromLong((long)x) :                                    \
            PyLong_FromLongLong((long long)x)))

#define _cffi_to_c_int(o, type)                                          \
    ((type)(                                                             \
     sizeof(type) == 1 ? (((type)-1) > 0 ? (type)_cffi_to_c_u8(o)        \
                                         : (type)_cffi_to_c_i8(o)) :     \
     sizeof(type) == 2 ? (((type)-1) > 0 ? (type)_cffi_to_c_u16(o)       \
                                         : (type)_cffi_to_c_i16(o)) :    \
     sizeof(type) == 4 ? (((type)-1) > 0 ? (type)_cffi_to_c_u32(o)       \
                                         : (type)_cffi_to_c_i32(o)) :    \
     sizeof(type) == 8 ? (((type)-1) > 0 ? (type)_cffi_to_c_u64(o)       \
                                         : (type)_cffi_to_c_i64(o)) :    \
     (Py_FatalError("unsupported size for type " #type), (type)0)))

#define _cffi_to_c_i8                                                    \
                 ((int(*)(PyObject *))_cffi_exports[1])
#define _cffi_to_c_u8                                                    \
                 ((int(*)(PyObject *))_cffi_exports[2])
#define _cffi_to_c_i16                                                   \
                 ((int(*)(PyObject *))_cffi_exports[3])
#define _cffi_to_c_u16                                                   \
                 ((int(*)(PyObject *))_cffi_exports[4])
#define _cffi_to_c_i32                                                   \
                 ((int(*)(PyObject *))_cffi_exports[5])
#define _cffi_to_c_u32                                                   \
                 ((unsigned int(*)(PyObject *))_cffi_exports[6])
#define _cffi_to_c_i64                                                   \
                 ((long long(*)(PyObject *))_cffi_exports[7])
#define _cffi_to_c_u64                                                   \
                 ((unsigned long long(*)(PyObject *))_cffi_exports[8])
#define _cffi_to_c_char                                                  \
                 ((int(*)(PyObject *))_cffi_exports[9])
#define _cffi_from_c_pointer                                             \
    ((PyObject *(*)(char *, CTypeDescrObject *))_cffi_exports[10])
#define _cffi_to_c_pointer                                               \
    ((char *(*)(PyObject *, CTypeDescrObject *))_cffi_exports[11])
#define _cffi_get_struct_layout                                          \
    ((PyObject *(*)(Py_ssize_t[]))_cffi_exports[12])
#define _cffi_restore_errno                                              \
    ((void(*)(void))_cffi_exports[13])
#define _cffi_save_errno                                                 \
    ((void(*)(void))_cffi_exports[14])
#define _cffi_from_c_char                                                \
    ((PyObject *(*)(char))_cffi_exports[15])
#define _cffi_from_c_deref                                               \
    ((PyObject *(*)(char *, CTypeDescrObject *))_cffi_exports[16])
#define _cffi_to_c                                                       \
    ((int(*)(char *, CTypeDescrObject *, PyObject *))_cffi_exports[17])
#define _cffi_from_c_struct                                              \
    ((PyObject *(*)(char *, CTypeDescrObject *))_cffi_exports[18])
#define _cffi_to_c_wchar_t                                               \
    ((wchar_t(*)(PyObject *))_cffi_exports[19])
#define _cffi_from_c_wchar_t                                             \
    ((PyObject *(*)(wchar_t))_cffi_exports[20])
#define _cffi_to_c_long_double                                           \
    ((long double(*)(PyObject *))_cffi_exports[21])
#define _cffi_to_c__Bool                                                 \
    ((_Bool(*)(PyObject *))_cffi_exports[22])
#define _cffi_prepare_pointer_call_argument                              \
    ((Py_ssize_t(*)(CTypeDescrObject *, PyObject *, char **))_cffi_exports[23])
#define _cffi_convert_array_from_object                                  \
    ((int(*)(char *, CTypeDescrObject *, PyObject *))_cffi_exports[24])
#define _CFFI_NUM_EXPORTS 25

typedef struct _ctypedescr CTypeDescrObject;

static void *_cffi_exports[_CFFI_NUM_EXPORTS];
static PyObject *_cffi_types, *_cffi_VerificationError;

static int _cffi_setup_custom(PyObject *lib);   /* forward */

static PyObject *_cffi_setup(PyObject *self, PyObject *args)
{
    PyObject *library;
    int was_alive = (_cffi_types != NULL);
    (void)self; /* unused */
    if (!PyArg_ParseTuple(args, "OOO", &_cffi_types, &_cffi_VerificationError,
                                       &library))
        return NULL;
    Py_INCREF(_cffi_types);
    Py_INCREF(_cffi_VerificationError);
    if (_cffi_setup_custom(library) < 0)
        return NULL;
    return PyBool_FromLong(was_alive);
}

union _cffi_union_alignment_u {
    unsigned char m_char;
    unsigned short m_short;
    unsigned int m_int;
    unsigned long m_long;
    unsigned long long m_longlong;
    float m_float;
    double m_double;
    long double m_longdouble;
};

struct _cffi_freeme_s {
    struct _cffi_freeme_s *next;
    union _cffi_union_alignment_u alignment;
};

#ifdef __GNUC__
  __attribute__((unused))
#endif
static int _cffi_convert_array_argument(CTypeDescrObject *ctptr, PyObject *arg,
                                        char **output_data, Py_ssize_t datasize,
                                        struct _cffi_freeme_s **freeme)
{
    char *p;
    if (datasize < 0)
        return -1;

    p = *output_data;
    if (p == NULL) {
        struct _cffi_freeme_s *fp = (struct _cffi_freeme_s *)PyObject_Malloc(
            offsetof(struct _cffi_freeme_s, alignment) + (size_t)datasize);
        if (fp == NULL)
            return -1;
        fp->next = *freeme;
        *freeme = fp;
        p = *output_data = (char *)&fp->alignment;
    }
    memset((void *)p, 0, (size_t)datasize);
    return _cffi_convert_array_from_object(p, ctptr, arg);
}

#ifdef __GNUC__
  __attribute__((unused))
#endif
static void _cffi_free_array_arguments(struct _cffi_freeme_s *freeme)
{
    do {
        void *p = (void *)freeme;
        freeme = freeme->next;
        PyObject_Free(p);
    } while (freeme != NULL);
}

static int _cffi_init(void)
{
    PyObject *module, *c_api_object = NULL;

    module = PyImport_ImportModule("_cffi_backend");
    if (module == NULL)
        goto failure;

    c_api_object = PyObject_GetAttrString(module, "_C_API");
    if (c_api_object == NULL)
        goto failure;
    if (!PyCapsule_CheckExact(c_api_object)) {
        PyErr_SetNone(PyExc_ImportError);
        goto failure;
    }
    memcpy(_cffi_exports, PyCapsule_GetPointer(c_api_object, "cffi"),
           _CFFI_NUM_EXPORTS * sizeof(void *));

    Py_DECREF(module);
    Py_DECREF(c_api_object);
    return 0;

  failure:
    Py_XDECREF(module);
    Py_XDECREF(c_api_object);
    return -1;
}

#define _cffi_type(num) ((CTypeDescrObject *)PyList_GET_ITEM(_cffi_types, num))

/**********/




static int _cffi_const_HEALTH_CRITICAL(PyObject *lib)
{
  PyObject *o;
  int res;
  if ((HEALTH_CRITICAL) <= 0 || (unsigned long)(HEALTH_CRITICAL) != 2UL) {
    char buf[64];
    if ((HEALTH_CRITICAL) <= 0)
        snprintf(buf, 63, "%ld", (long)(HEALTH_CRITICAL));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(HEALTH_CRITICAL));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "", "HEALTH_CRITICAL", buf, "2");
    return -1;
  }
  o = _cffi_from_c_int_const(HEALTH_CRITICAL);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "HEALTH_CRITICAL", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return ((void)lib,0);
}

static int _cffi_const_HEALTH_NOMINAL(PyObject *lib)
{
  PyObject *o;
  int res;
  if ((HEALTH_NOMINAL) > 0 || (long)(HEALTH_NOMINAL) != 0L) {
    char buf[64];
    if ((HEALTH_NOMINAL) <= 0)
        snprintf(buf, 63, "%ld", (long)(HEALTH_NOMINAL));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(HEALTH_NOMINAL));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "", "HEALTH_NOMINAL", buf, "0");
    return -1;
  }
  o = _cffi_from_c_int_const(HEALTH_NOMINAL);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "HEALTH_NOMINAL", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_HEALTH_CRITICAL(lib);
}

static int _cffi_const_HEALTH_WARNING(PyObject *lib)
{
  PyObject *o;
  int res;
  if ((HEALTH_WARNING) <= 0 || (unsigned long)(HEALTH_WARNING) != 1UL) {
    char buf[64];
    if ((HEALTH_WARNING) <= 0)
        snprintf(buf, 63, "%ld", (long)(HEALTH_WARNING));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(HEALTH_WARNING));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "", "HEALTH_WARNING", buf, "1");
    return -1;
  }
  o = _cffi_from_c_int_const(HEALTH_WARNING);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "HEALTH_WARNING", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_HEALTH_NOMINAL(lib);
}

static int _cffi_const_ID_DEBUG_PRINT(PyObject *lib)
{
  PyObject *o;
  int res;
  if ((ID_DEBUG_PRINT) <= 0 || (unsigned long)(ID_DEBUG_PRINT) != 1056UL) {
    char buf[64];
    if ((ID_DEBUG_PRINT) <= 0)
        snprintf(buf, 63, "%ld", (long)(ID_DEBUG_PRINT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(ID_DEBUG_PRINT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "", "ID_DEBUG_PRINT", buf, "1056");
    return -1;
  }
  o = _cffi_from_c_int_const(ID_DEBUG_PRINT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "ID_DEBUG_PRINT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_HEALTH_WARNING(lib);
}

static int _cffi_const_ID_ENGINE_SENSORS_A(PyObject *lib)
{
  PyObject *o;
  int res;
  if ((ID_ENGINE_SENSORS_A) <= 0 || (unsigned long)(ID_ENGINE_SENSORS_A) != 272UL) {
    char buf[64];
    if ((ID_ENGINE_SENSORS_A) <= 0)
        snprintf(buf, 63, "%ld", (long)(ID_ENGINE_SENSORS_A));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(ID_ENGINE_SENSORS_A));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "", "ID_ENGINE_SENSORS_A", buf, "272");
    return -1;
  }
  o = _cffi_from_c_int_const(ID_ENGINE_SENSORS_A);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "ID_ENGINE_SENSORS_A", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_ID_DEBUG_PRINT(lib);
}

static int _cffi_const_ID_ENGINE_SENSORS_B(PyObject *lib)
{
  PyObject *o;
  int res;
  if ((ID_ENGINE_SENSORS_B) <= 0 || (unsigned long)(ID_ENGINE_SENSORS_B) != 288UL) {
    char buf[64];
    if ((ID_ENGINE_SENSORS_B) <= 0)
        snprintf(buf, 63, "%ld", (long)(ID_ENGINE_SENSORS_B));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(ID_ENGINE_SENSORS_B));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "", "ID_ENGINE_SENSORS_B", buf, "288");
    return -1;
  }
  o = _cffi_from_c_int_const(ID_ENGINE_SENSORS_B);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "ID_ENGINE_SENSORS_B", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_ID_ENGINE_SENSORS_A(lib);
}

static int _cffi_const_ID_ERROR(PyObject *lib)
{
  PyObject *o;
  int res;
  if ((ID_ERROR) <= 0 || (unsigned long)(ID_ERROR) != 1040UL) {
    char buf[64];
    if ((ID_ERROR) <= 0)
        snprintf(buf, 63, "%ld", (long)(ID_ERROR));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(ID_ERROR));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "", "ID_ERROR", buf, "1040");
    return -1;
  }
  o = _cffi_from_c_int_const(ID_ERROR);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "ID_ERROR", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_ID_ENGINE_SENSORS_B(lib);
}

static int _cffi_const_ID_FUEL_PRESS_MOTOR_STATUS(PyObject *lib)
{
  PyObject *o;
  int res;
  if ((ID_FUEL_PRESS_MOTOR_STATUS) <= 0 || (unsigned long)(ID_FUEL_PRESS_MOTOR_STATUS) != 528UL) {
    char buf[64];
    if ((ID_FUEL_PRESS_MOTOR_STATUS) <= 0)
        snprintf(buf, 63, "%ld", (long)(ID_FUEL_PRESS_MOTOR_STATUS));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(ID_FUEL_PRESS_MOTOR_STATUS));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "", "ID_FUEL_PRESS_MOTOR_STATUS", buf, "528");
    return -1;
  }
  o = _cffi_from_c_int_const(ID_FUEL_PRESS_MOTOR_STATUS);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "ID_FUEL_PRESS_MOTOR_STATUS", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_ID_ERROR(lib);
}

static int _cffi_const_ID_HEARTBEAT(PyObject *lib)
{
  PyObject *o;
  int res;
  if ((ID_HEARTBEAT) <= 0 || (unsigned long)(ID_HEARTBEAT) != 1024UL) {
    char buf[64];
    if ((ID_HEARTBEAT) <= 0)
        snprintf(buf, 63, "%ld", (long)(ID_HEARTBEAT));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(ID_HEARTBEAT));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "", "ID_HEARTBEAT", buf, "1024");
    return -1;
  }
  o = _cffi_from_c_int_const(ID_HEARTBEAT);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "ID_HEARTBEAT", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_ID_FUEL_PRESS_MOTOR_STATUS(lib);
}

static int _cffi_const_ID_OX_MAIN_MOTOR_STATUS(PyObject *lib)
{
  PyObject *o;
  int res;
  if ((ID_OX_MAIN_MOTOR_STATUS) <= 0 || (unsigned long)(ID_OX_MAIN_MOTOR_STATUS) != 512UL) {
    char buf[64];
    if ((ID_OX_MAIN_MOTOR_STATUS) <= 0)
        snprintf(buf, 63, "%ld", (long)(ID_OX_MAIN_MOTOR_STATUS));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(ID_OX_MAIN_MOTOR_STATUS));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "", "ID_OX_MAIN_MOTOR_STATUS", buf, "512");
    return -1;
  }
  o = _cffi_from_c_int_const(ID_OX_MAIN_MOTOR_STATUS);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "ID_OX_MAIN_MOTOR_STATUS", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_ID_HEARTBEAT(lib);
}

static int _cffi_const_ID_POWER_CONTROL(PyObject *lib)
{
  PyObject *o;
  int res;
  if ((ID_POWER_CONTROL) <= 0 || (unsigned long)(ID_POWER_CONTROL) != 560UL) {
    char buf[64];
    if ((ID_POWER_CONTROL) <= 0)
        snprintf(buf, 63, "%ld", (long)(ID_POWER_CONTROL));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(ID_POWER_CONTROL));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "", "ID_POWER_CONTROL", buf, "560");
    return -1;
  }
  o = _cffi_from_c_int_const(ID_POWER_CONTROL);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "ID_POWER_CONTROL", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_ID_OX_MAIN_MOTOR_STATUS(lib);
}

static int _cffi_const_ID_POWER_STATUS(PyObject *lib)
{
  PyObject *o;
  int res;
  if ((ID_POWER_STATUS) <= 0 || (unsigned long)(ID_POWER_STATUS) != 560UL) {
    char buf[64];
    if ((ID_POWER_STATUS) <= 0)
        snprintf(buf, 63, "%ld", (long)(ID_POWER_STATUS));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(ID_POWER_STATUS));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "", "ID_POWER_STATUS", buf, "560");
    return -1;
  }
  o = _cffi_from_c_int_const(ID_POWER_STATUS);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "ID_POWER_STATUS", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_ID_POWER_CONTROL(lib);
}

static int _cffi_const_ID_PYRO_CONTROL(PyObject *lib)
{
  PyObject *o;
  int res;
  if ((ID_PYRO_CONTROL) <= 0 || (unsigned long)(ID_PYRO_CONTROL) != 80UL) {
    char buf[64];
    if ((ID_PYRO_CONTROL) <= 0)
        snprintf(buf, 63, "%ld", (long)(ID_PYRO_CONTROL));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(ID_PYRO_CONTROL));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "", "ID_PYRO_CONTROL", buf, "80");
    return -1;
  }
  o = _cffi_from_c_int_const(ID_PYRO_CONTROL);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "ID_PYRO_CONTROL", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_ID_POWER_STATUS(lib);
}

static int _cffi_const_ID_PYRO_STATUS(PyObject *lib)
{
  PyObject *o;
  int res;
  if ((ID_PYRO_STATUS) <= 0 || (unsigned long)(ID_PYRO_STATUS) != 544UL) {
    char buf[64];
    if ((ID_PYRO_STATUS) <= 0)
        snprintf(buf, 63, "%ld", (long)(ID_PYRO_STATUS));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(ID_PYRO_STATUS));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "", "ID_PYRO_STATUS", buf, "544");
    return -1;
  }
  o = _cffi_from_c_int_const(ID_PYRO_STATUS);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "ID_PYRO_STATUS", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_ID_PYRO_CONTROL(lib);
}

static int _cffi_const_ID_RESTART(PyObject *lib)
{
  PyObject *o;
  int res;
  if ((ID_RESTART) <= 0 || (unsigned long)(ID_RESTART) != 1072UL) {
    char buf[64];
    if ((ID_RESTART) <= 0)
        snprintf(buf, 63, "%ld", (long)(ID_RESTART));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(ID_RESTART));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "", "ID_RESTART", buf, "1072");
    return -1;
  }
  o = _cffi_from_c_int_const(ID_RESTART);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "ID_RESTART", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_ID_PYRO_STATUS(lib);
}

static int _cffi_const_ID_VALVE_CONTROL(PyObject *lib)
{
  PyObject *o;
  int res;
  if ((ID_VALVE_CONTROL) <= 0 || (unsigned long)(ID_VALVE_CONTROL) != 256UL) {
    char buf[64];
    if ((ID_VALVE_CONTROL) <= 0)
        snprintf(buf, 63, "%ld", (long)(ID_VALVE_CONTROL));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(ID_VALVE_CONTROL));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "", "ID_VALVE_CONTROL", buf, "256");
    return -1;
  }
  o = _cffi_from_c_int_const(ID_VALVE_CONTROL);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "ID_VALVE_CONTROL", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_ID_RESTART(lib);
}

static int _cffi_const_RCU_ID_ENGINE_SENSOR_RCU(PyObject *lib)
{
  PyObject *o;
  int res;
  if ((RCU_ID_ENGINE_SENSOR_RCU) <= 0 || (unsigned long)(RCU_ID_ENGINE_SENSOR_RCU) != 3UL) {
    char buf[64];
    if ((RCU_ID_ENGINE_SENSOR_RCU) <= 0)
        snprintf(buf, 63, "%ld", (long)(RCU_ID_ENGINE_SENSOR_RCU));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(RCU_ID_ENGINE_SENSOR_RCU));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "", "RCU_ID_ENGINE_SENSOR_RCU", buf, "3");
    return -1;
  }
  o = _cffi_from_c_int_const(RCU_ID_ENGINE_SENSOR_RCU);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "RCU_ID_ENGINE_SENSOR_RCU", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_ID_VALVE_CONTROL(lib);
}

static int _cffi_const_RCU_ID_ENGINE_VALVE_RCU(PyObject *lib)
{
  PyObject *o;
  int res;
  if ((RCU_ID_ENGINE_VALVE_RCU) <= 0 || (unsigned long)(RCU_ID_ENGINE_VALVE_RCU) != 2UL) {
    char buf[64];
    if ((RCU_ID_ENGINE_VALVE_RCU) <= 0)
        snprintf(buf, 63, "%ld", (long)(RCU_ID_ENGINE_VALVE_RCU));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(RCU_ID_ENGINE_VALVE_RCU));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "", "RCU_ID_ENGINE_VALVE_RCU", buf, "2");
    return -1;
  }
  o = _cffi_from_c_int_const(RCU_ID_ENGINE_VALVE_RCU);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "RCU_ID_ENGINE_VALVE_RCU", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_RCU_ID_ENGINE_SENSOR_RCU(lib);
}

static int _cffi_const_RCU_ID_MAIN_RCU(PyObject *lib)
{
  PyObject *o;
  int res;
  if ((RCU_ID_MAIN_RCU) <= 0 || (unsigned long)(RCU_ID_MAIN_RCU) != 1UL) {
    char buf[64];
    if ((RCU_ID_MAIN_RCU) <= 0)
        snprintf(buf, 63, "%ld", (long)(RCU_ID_MAIN_RCU));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(RCU_ID_MAIN_RCU));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "", "RCU_ID_MAIN_RCU", buf, "1");
    return -1;
  }
  o = _cffi_from_c_int_const(RCU_ID_MAIN_RCU);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "RCU_ID_MAIN_RCU", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_RCU_ID_ENGINE_VALVE_RCU(lib);
}

static int _cffi_const_RCU_ID_POWER_PYRO_RCU(PyObject *lib)
{
  PyObject *o;
  int res;
  if ((RCU_ID_POWER_PYRO_RCU) <= 0 || (unsigned long)(RCU_ID_POWER_PYRO_RCU) != 4UL) {
    char buf[64];
    if ((RCU_ID_POWER_PYRO_RCU) <= 0)
        snprintf(buf, 63, "%ld", (long)(RCU_ID_POWER_PYRO_RCU));
    else
        snprintf(buf, 63, "%lu", (unsigned long)(RCU_ID_POWER_PYRO_RCU));
    PyErr_Format(_cffi_VerificationError,
                 "%s%s has the real value %s, not %s",
                 "", "RCU_ID_POWER_PYRO_RCU", buf, "4");
    return -1;
  }
  o = _cffi_from_c_int_const(RCU_ID_POWER_PYRO_RCU);
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "RCU_ID_POWER_PYRO_RCU", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_RCU_ID_MAIN_RCU(lib);
}

static void _cffi_check_struct_DebugPrint_t(struct DebugPrint_t *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  { char *(*tmp)[8] = &p->text; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_DebugPrint_t(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct DebugPrint_t y; };
  static Py_ssize_t nums[] = {
    sizeof(struct DebugPrint_t),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct DebugPrint_t, text),
    sizeof(((struct DebugPrint_t *)0)->text),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_DebugPrint_t(0);
}

static void _cffi_check_struct_EngineSensorsA_t(struct EngineSensorsA_t *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->fuel_tank_press_raw) << 1);
  (void)((p->chamber_press_raw) << 1);
  (void)((p->fuel_inj_press_raw) << 1);
  (void)((p->ox_inj_press_raw) << 1);
}
static PyObject *
_cffi_layout_struct_EngineSensorsA_t(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct EngineSensorsA_t y; };
  static Py_ssize_t nums[] = {
    sizeof(struct EngineSensorsA_t),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct EngineSensorsA_t, fuel_tank_press_raw),
    sizeof(((struct EngineSensorsA_t *)0)->fuel_tank_press_raw),
    offsetof(struct EngineSensorsA_t, chamber_press_raw),
    sizeof(((struct EngineSensorsA_t *)0)->chamber_press_raw),
    offsetof(struct EngineSensorsA_t, fuel_inj_press_raw),
    sizeof(((struct EngineSensorsA_t *)0)->fuel_inj_press_raw),
    offsetof(struct EngineSensorsA_t, ox_inj_press_raw),
    sizeof(((struct EngineSensorsA_t *)0)->ox_inj_press_raw),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_EngineSensorsA_t(0);
}

static void _cffi_check_struct_EngineSensorsB_t(struct EngineSensorsB_t *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->thrust_raw) << 1);
  (void)((p->aux_1_raw) << 1);
  (void)((p->aux_2_raw) << 1);
  (void)((p->aux_3_raw) << 1);
}
static PyObject *
_cffi_layout_struct_EngineSensorsB_t(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct EngineSensorsB_t y; };
  static Py_ssize_t nums[] = {
    sizeof(struct EngineSensorsB_t),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct EngineSensorsB_t, thrust_raw),
    sizeof(((struct EngineSensorsB_t *)0)->thrust_raw),
    offsetof(struct EngineSensorsB_t, aux_1_raw),
    sizeof(((struct EngineSensorsB_t *)0)->aux_1_raw),
    offsetof(struct EngineSensorsB_t, aux_2_raw),
    sizeof(((struct EngineSensorsB_t *)0)->aux_2_raw),
    offsetof(struct EngineSensorsB_t, aux_3_raw),
    sizeof(((struct EngineSensorsB_t *)0)->aux_3_raw),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_EngineSensorsB_t(0);
}

static void _cffi_check_struct_Error_t(struct Error_t *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->error_no) << 1);
  { uint8_t(*tmp)[7] = &p->error_data; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_Error_t(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct Error_t y; };
  static Py_ssize_t nums[] = {
    sizeof(struct Error_t),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct Error_t, error_no),
    sizeof(((struct Error_t *)0)->error_no),
    offsetof(struct Error_t, error_data),
    sizeof(((struct Error_t *)0)->error_data),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_Error_t(0);
}

static void _cffi_check_struct_Heartbeat_t(struct Heartbeat_t *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->health) << 1);
  (void)((p->uptime_s) << 1);
}
static PyObject *
_cffi_layout_struct_Heartbeat_t(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct Heartbeat_t y; };
  static Py_ssize_t nums[] = {
    sizeof(struct Heartbeat_t),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct Heartbeat_t, health),
    sizeof(((struct Heartbeat_t *)0)->health),
    offsetof(struct Heartbeat_t, uptime_s),
    sizeof(((struct Heartbeat_t *)0)->uptime_s),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_Heartbeat_t(0);
}

static void _cffi_check_struct_LimitSwitchStatus_t(struct LimitSwitchStatus_t *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->limit_switch) << 1);
  (void)((p->has_homed) << 1);
}
static PyObject *
_cffi_layout_struct_LimitSwitchStatus_t(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct LimitSwitchStatus_t y; };
  static Py_ssize_t nums[] = {
    sizeof(struct LimitSwitchStatus_t),
    offsetof(struct _cffi_aligncheck, y),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_LimitSwitchStatus_t(0);
}

static void _cffi_check_struct_MotorStatus_t(struct MotorStatus_t *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->pos) << 1);
  (void)((p->speed) << 1);
  { struct LimitSwitchStatus_t *tmp = &p->homing; (void)tmp; }
  { struct MotorValveFaults_t *tmp = &p->faults; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_MotorStatus_t(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct MotorStatus_t y; };
  static Py_ssize_t nums[] = {
    sizeof(struct MotorStatus_t),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct MotorStatus_t, pos),
    sizeof(((struct MotorStatus_t *)0)->pos),
    offsetof(struct MotorStatus_t, speed),
    sizeof(((struct MotorStatus_t *)0)->speed),
    offsetof(struct MotorStatus_t, homing),
    sizeof(((struct MotorStatus_t *)0)->homing),
    offsetof(struct MotorStatus_t, faults),
    sizeof(((struct MotorStatus_t *)0)->faults),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_MotorStatus_t(0);
}

static void _cffi_check_struct_MotorValveFaults_t(struct MotorValveFaults_t *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->motor_stuck) << 1);
  (void)((p->homing_timeout) << 1);
  (void)((p->limit_switch_stuck) << 1);
  (void)((p->motor_slip_encoder_drift) << 1);
  (void)((p->encoder_error) << 1);
}
static PyObject *
_cffi_layout_struct_MotorValveFaults_t(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct MotorValveFaults_t y; };
  static Py_ssize_t nums[] = {
    sizeof(struct MotorValveFaults_t),
    offsetof(struct _cffi_aligncheck, y),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_MotorValveFaults_t(0);
}

static void _cffi_check_struct_PowerControl_t(struct PowerControl_t *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->shutdown_request) << 1);
}
static PyObject *
_cffi_layout_struct_PowerControl_t(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct PowerControl_t y; };
  static Py_ssize_t nums[] = {
    sizeof(struct PowerControl_t),
    offsetof(struct _cffi_aligncheck, y),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_PowerControl_t(0);
}

static void _cffi_check_struct_PowerStatus_t(struct PowerStatus_t *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->voltage_12V_mV) << 1);
  (void)((p->current_12V_mA) << 1);
  (void)((p->current_5V_mA) << 1);
  /* cannot generate 'struct $1' in field 'flags': unknown type name */
}
static PyObject *
_cffi_layout_struct_PowerStatus_t(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct PowerStatus_t y; };
  static Py_ssize_t nums[] = {
    sizeof(struct PowerStatus_t),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct PowerStatus_t, voltage_12V_mV),
    sizeof(((struct PowerStatus_t *)0)->voltage_12V_mV),
    offsetof(struct PowerStatus_t, current_12V_mA),
    sizeof(((struct PowerStatus_t *)0)->current_12V_mA),
    offsetof(struct PowerStatus_t, current_5V_mA),
    sizeof(((struct PowerStatus_t *)0)->current_5V_mA),
    offsetof(struct PowerStatus_t, flags),
    sizeof(((struct PowerStatus_t *)0)->flags),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_PowerStatus_t(0);
}

static void _cffi_check_struct_PyroControl_t(struct PyroControl_t *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->fire_ignitor) << 1);
  (void)((p->fire_main) << 1);
  (void)((p->fire_drogue) << 1);
}
static PyObject *
_cffi_layout_struct_PyroControl_t(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct PyroControl_t y; };
  static Py_ssize_t nums[] = {
    sizeof(struct PyroControl_t),
    offsetof(struct _cffi_aligncheck, y),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_PyroControl_t(0);
}

static void _cffi_check_struct_PyroStatus_t(struct PyroStatus_t *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->ignitor_cont) << 1);
  (void)((p->main_cont) << 1);
  (void)((p->drogue_cont) << 1);
}
static PyObject *
_cffi_layout_struct_PyroStatus_t(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct PyroStatus_t y; };
  static Py_ssize_t nums[] = {
    sizeof(struct PyroStatus_t),
    offsetof(struct _cffi_aligncheck, y),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_PyroStatus_t(0);
}

static void _cffi_check_struct_Restart_t(struct Restart_t *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->rcu_id) << 1);
}
static PyObject *
_cffi_layout_struct_Restart_t(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct Restart_t y; };
  static Py_ssize_t nums[] = {
    sizeof(struct Restart_t),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct Restart_t, rcu_id),
    sizeof(((struct Restart_t *)0)->rcu_id),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_Restart_t(0);
}

static void _cffi_check_struct_SolenoidsControl_t(struct SolenoidsControl_t *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->engine_vent_valve_close) << 1);
  (void)((p->main_fuel_valve_open) << 1);
  (void)((p->aux_1_energize) << 1);
  (void)((p->aux_2_energize) << 1);
}
static PyObject *
_cffi_layout_struct_SolenoidsControl_t(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct SolenoidsControl_t y; };
  static Py_ssize_t nums[] = {
    sizeof(struct SolenoidsControl_t),
    offsetof(struct _cffi_aligncheck, y),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_SolenoidsControl_t(0);
}

static void _cffi_check_struct_ValveControl_t(struct ValveControl_t *p)
{
  /* only to generate compile-time warnings or errors */
  (void)p;
  (void)((p->main_ox_valve_goal_pos) << 1);
  (void)((p->fuel_pres_valve_goal_pos) << 1);
  { struct SolenoidsControl_t *tmp = &p->solenoids; (void)tmp; }
}
static PyObject *
_cffi_layout_struct_ValveControl_t(PyObject *self, PyObject *noarg)
{
  struct _cffi_aligncheck { char x; struct ValveControl_t y; };
  static Py_ssize_t nums[] = {
    sizeof(struct ValveControl_t),
    offsetof(struct _cffi_aligncheck, y),
    offsetof(struct ValveControl_t, main_ox_valve_goal_pos),
    sizeof(((struct ValveControl_t *)0)->main_ox_valve_goal_pos),
    offsetof(struct ValveControl_t, fuel_pres_valve_goal_pos),
    sizeof(((struct ValveControl_t *)0)->fuel_pres_valve_goal_pos),
    offsetof(struct ValveControl_t, solenoids),
    sizeof(((struct ValveControl_t *)0)->solenoids),
    -1
  };
  (void)self; /* unused */
  (void)noarg; /* unused */
  return _cffi_get_struct_layout(nums);
  /* the next line is not executed, but compiled */
  _cffi_check_struct_ValveControl_t(0);
}

static int _cffi_setup_custom(PyObject *lib)
{
  return _cffi_const_RCU_ID_POWER_PYRO_RCU(lib);
}

static PyMethodDef _cffi_methods[] = {
  {"_cffi_layout_struct_DebugPrint_t", _cffi_layout_struct_DebugPrint_t, METH_NOARGS, NULL},
  {"_cffi_layout_struct_EngineSensorsA_t", _cffi_layout_struct_EngineSensorsA_t, METH_NOARGS, NULL},
  {"_cffi_layout_struct_EngineSensorsB_t", _cffi_layout_struct_EngineSensorsB_t, METH_NOARGS, NULL},
  {"_cffi_layout_struct_Error_t", _cffi_layout_struct_Error_t, METH_NOARGS, NULL},
  {"_cffi_layout_struct_Heartbeat_t", _cffi_layout_struct_Heartbeat_t, METH_NOARGS, NULL},
  {"_cffi_layout_struct_LimitSwitchStatus_t", _cffi_layout_struct_LimitSwitchStatus_t, METH_NOARGS, NULL},
  {"_cffi_layout_struct_MotorStatus_t", _cffi_layout_struct_MotorStatus_t, METH_NOARGS, NULL},
  {"_cffi_layout_struct_MotorValveFaults_t", _cffi_layout_struct_MotorValveFaults_t, METH_NOARGS, NULL},
  {"_cffi_layout_struct_PowerControl_t", _cffi_layout_struct_PowerControl_t, METH_NOARGS, NULL},
  {"_cffi_layout_struct_PowerStatus_t", _cffi_layout_struct_PowerStatus_t, METH_NOARGS, NULL},
  {"_cffi_layout_struct_PyroControl_t", _cffi_layout_struct_PyroControl_t, METH_NOARGS, NULL},
  {"_cffi_layout_struct_PyroStatus_t", _cffi_layout_struct_PyroStatus_t, METH_NOARGS, NULL},
  {"_cffi_layout_struct_Restart_t", _cffi_layout_struct_Restart_t, METH_NOARGS, NULL},
  {"_cffi_layout_struct_SolenoidsControl_t", _cffi_layout_struct_SolenoidsControl_t, METH_NOARGS, NULL},
  {"_cffi_layout_struct_ValveControl_t", _cffi_layout_struct_ValveControl_t, METH_NOARGS, NULL},
  {"_cffi_setup", _cffi_setup, METH_VARARGS, NULL},
  {NULL, NULL, 0, NULL}    /* Sentinel */
};

#if PY_MAJOR_VERSION >= 3

static struct PyModuleDef _cffi_module_def = {
  PyModuleDef_HEAD_INIT,
  "_cffi__x7cf0e87bx8148f6f",
  NULL,
  -1,
  _cffi_methods,
  NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC
PyInit__cffi__x7cf0e87bx8148f6f(void)
{
  PyObject *lib;
  lib = PyModule_Create(&_cffi_module_def);
  if (lib == NULL)
    return NULL;
  if (((void)lib,0) < 0 || _cffi_init() < 0) {
    Py_DECREF(lib);
    return NULL;
  }
  return lib;
}

#else

PyMODINIT_FUNC
init_cffi__x7cf0e87bx8148f6f(void)
{
  PyObject *lib;
  lib = Py_InitModule("_cffi__x7cf0e87bx8148f6f", _cffi_methods);
  if (lib == NULL)
    return;
  if (((void)lib,0) < 0 || _cffi_init() < 0)
    return;
  return;
}

#endif
