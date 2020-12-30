package jbig2dec

import (
	"fmt"
	"unsafe"
)

//#cgo CXXFLAGS: -Ipdfium
//#include <stdlib.h>
//typedef unsigned char uint8_t;
//extern int jbig2Decode(int width, int height, const uint8_t *data, size_t len, uint8_t *buf);
import "C"

func Decode(width, height int, data []byte) (out []byte, err error) {
	pitch := (width + 31) / 32 * 4
	buf := C.malloc(C.size_t(height * pitch))
	defer C.free(buf)

	ret := C.jbig2Decode(C.int(width), C.int(height), (*C.uint8_t)(unsafe.Pointer(&data[0])), C.size_t(len(data)), (*C.uint8_t)(buf))
	if ret != 0 {
		err = fmt.Errorf("failed to decode")
		return
	}

	out = C.GoBytes(buf, C.int(height*pitch))
	return
}
