PDFIUM_ROOT=/home/jack/Source/pdfium/pdfium

mkdir -p pdfium/core/fxcodec pdfium/third_party pdfium/build
cp -r $PDFIUM_ROOT/third_party/base pdfium/third_party/
rm -rf pdfium/third_party/base/allocator
cp -r $PDFIUM_ROOT/core/fxcrt pdfium/core
rm -rf pdfium/core/fxcrt/xml
rm -rf pdfium/core/fxcrt/css
cp -r $PDFIUM_ROOT/core/fxcodec/jbig2 pdfium/core/fxcodec
cp -r $PDFIUM_ROOT/core/fxcodec/fax pdfium/core/fxcodec
cp $PDFIUM_ROOT/core/fxcodec/fx_codec_def.h \
	$PDFIUM_ROOT/core/fxcodec/fx_codec.h \
	$PDFIUM_ROOT/core/fxcodec/scanlinedecoder.h \
	$PDFIUM_ROOT/core/fxcodec/scanlinedecoder.cpp pdfium/core/fxcodec
cp build_config.h pdfium/build

echo "Syncing pdfium done"


