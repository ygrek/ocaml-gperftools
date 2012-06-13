
extern "C" {
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/fail.h>
#include <caml/alloc.h>
#include <caml/callback.h>

#include <assert.h>
}

#define UNUSED(x) (void)(x)
#define ML_EXTERN extern "C" value

#include <google/malloc_extension.h>
#include <google/heap-profiler.h>

ML_EXTERN ml_gpt_ReleaseFreeMemory(value unit)
{
  UNUSED(unit);
  MallocExtension::instance()->ReleaseFreeMemory();
  return Val_unit;
}

ML_EXTERN ml_gpt_HeapProfilerStart(value prefix)
{
  HeapProfilerStart(String_val(prefix));
  return Val_unit;
}

ML_EXTERN ml_gpt_IsHeapProfilerRunning(value unit)
{
  UNUSED(unit);
  return Val_bool(IsHeapProfilerRunning());
}

ML_EXTERN ml_gpt_HeapProfilerStop(value unit)
{
  UNUSED(unit);
  HeapProfilerStop();
  return Val_unit;
}

ML_EXTERN ml_gpt_HeapProfilerDump(value reason)
{
  HeapProfilerDump(String_val(reason));
  return Val_unit;
}

ML_EXTERN ml_gpt_GetHeapProfile(value unit)
{
  CAMLparam1(unit);
  CAMLlocal1(v);
  char * s = NULL;

  s = GetHeapProfile();
  if (NULL == s)
    caml_failwith("GetHeapProfile");

  v = caml_copy_string(s);
  free(s); s = NULL;

  CAMLreturn(v);
}

ML_EXTERN ml_gpt_SetMemoryReleaseRate(value v_rate)
{
  MallocExtension::instance()->SetMemoryReleaseRate(Double_val(v_rate));
  return Val_unit;
}

ML_EXTERN ml_gpt_GetMemoryReleaseRate(value unit)
{
  UNUSED(unit);
  double d = MallocExtension::instance()->GetMemoryReleaseRate();
  return caml_copy_double(d);
}

static void invalid_property(value message) 
{
  CAMLparam1(message);
  static value* exn = NULL;
  if (NULL == exn)
  {
    exn = caml_named_value("Gperftools.Invalid_property");
    assert(NULL != exn);
  }
  caml_raise_with_arg(*exn, message);
  CAMLnoreturn;
}

ML_EXTERN ml_gpt_SetNumericProperty(value v_name, value v_value)
{
  if (false == MallocExtension::instance()->SetNumericProperty(String_val(v_name), Long_val(v_value)))
  {
    invalid_property(v_name);
  }
  return Val_unit;
}

ML_EXTERN ml_gpt_GetNumericProperty(value v_name)
{
  size_t x;
  if (false == MallocExtension::instance()->GetNumericProperty(String_val(v_name), &x))
  {
    invalid_property(v_name);
  }
  return Val_long(x);
}

