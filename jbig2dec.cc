#include "pdfium/core/fxcodec/jbig2/JBig2_ArithDecoder.cpp"
#include "pdfium/core/fxcodec/jbig2/JBig2_ArithIntDecoder.cpp"
#include "pdfium/core/fxcodec/jbig2/JBig2_BitStream.cpp"
#include "pdfium/core/fxcodec/jbig2/JBig2_Context.cpp"
#include "pdfium/core/fxcodec/jbig2/JBig2_DocumentContext.cpp"
#include "pdfium/core/fxcodec/jbig2/JBig2_GrdProc.cpp"
#include "pdfium/core/fxcodec/jbig2/JBig2_GrrdProc.cpp"
#include "pdfium/core/fxcodec/jbig2/JBig2_HtrdProc.cpp"
#include "pdfium/core/fxcodec/jbig2/JBig2_HuffmanDecoder.cpp"
#include "pdfium/core/fxcodec/jbig2/JBig2_HuffmanTable.cpp"
#include "pdfium/core/fxcodec/jbig2/JBig2_Image.cpp"
#include "pdfium/core/fxcodec/jbig2/JBig2_PatternDict.cpp"
#include "pdfium/core/fxcodec/jbig2/JBig2_PddProc.cpp"
#include "pdfium/core/fxcodec/jbig2/JBig2_SddProc.cpp"
#include "pdfium/core/fxcodec/jbig2/JBig2_Segment.cpp"
#include "pdfium/core/fxcodec/jbig2/JBig2_SymbolDict.cpp"
#include "pdfium/core/fxcodec/jbig2/JBig2_TrdProc.cpp"
#include "pdfium/core/fxcodec/jbig2/jbig2_decoder.cpp"

#include "pdfium/core/fxcodec/scanlinedecoder.cpp"
#include "pdfium/core/fxcodec/fax/faxmodule.cpp"
#include "pdfium/core/fxcrt/cfx_binarybuf.cpp"

namespace pdfium
{
  namespace internal
  {
    void *AllocOrDie(size_t num_members, size_t member_size)
    {
      void *result = malloc(num_members * member_size);
      if (!result)
        abort(); // Never returns.

      return result;
    }

    void *CallocOrDie(size_t num_members, size_t member_size)
    {
      void *result = calloc(num_members, member_size);
      if (!result)
        abort(); // Never returns.

      return result;
    }

    void *CallocOrDie2D(size_t w, size_t h, size_t member_size)
    {
      return CallocOrDie(w * h, member_size);
    }

    void *ReallocOrDie(void *ptr, size_t num_members, size_t member_size)
    {
      void *result = realloc(ptr, num_members * member_size);
      if (!result)
        abort(); // Never returns.

      return result;
    }
  } // namespace internal
} // namespace pdfium

namespace fxcodec
{
  FX_SAFE_UINT32 CalculatePitch32(int bpp, int width)
  {
    FX_SAFE_UINT32 pitch = bpp;
    pitch *= width;
    pitch += 31;
    pitch /= 32; // quantized to number of 32-bit words.
    pitch *= 4;  // and then back to bytes, (not just /8 in one step).
    return pitch;
  }
} // namespace fxcodec

void FX_Free(void *ptr)
{
  free(ptr);
}

extern "C" int jbig2Decode(int width, int height, uint8_t *data, size_t sz, uint8_t *buf)
{
  int pitch = (width + 31) / 32 * 4;
  auto ctx = std::make_unique<Jbig2Context>();

  auto status = Jbig2Decoder::StartDecode(
      ctx.get(), nullptr, width, height,
      pdfium::span<uint8_t>{data, sz}, 0, pdfium::span<uint8_t>{}, 0,
      buf, pitch, nullptr);
  if (status != FXCODEC_STATUS_DECODE_FINISH)
    return -1;
  return 0;
}
