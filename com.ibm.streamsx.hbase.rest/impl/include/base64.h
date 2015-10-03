#ifndef STREAMSX_BASE64
#define STREAMSX_BASE64
#include <SPL/Runtime/Type/Blob.h>
#include <SPL/Runtime/Type/List.h>

namespace com_ibm_streamsx_base64 {


// We're just writing bytes.
    SPL::rstring base64Encode(const SPL::rstring & inputString,SPL::int32 & error);

    SPL::rstring base64DecodeToRstring(const SPL::rstring & encoded,SPL::int32 & error);

}
#endif
