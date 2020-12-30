package main

import (
	"bytes"
	"encoding/binary"
	"github.com/jdeng/jbig2dec"
	"io/ioutil"
	"log"
)

type bmpHeader struct {
	sigBM           [2]byte
	fileSize        uint32
	resverved       [2]uint16
	pixOffset       uint32
	dibHeaderSize   uint32
	width           uint32
	height          uint32
	colorPlane      uint16
	bpp             uint16
	compression     uint32
	imageSize       uint32
	xPixelsPerMeter uint32
	yPixelsPerMeter uint32
	colorUse        uint32
	colorImportant  uint32
}

func saveBitmap(fn string, width, height int, data []byte) error {
	h := &bmpHeader{
		sigBM:         [2]byte{'B', 'M'},
		fileSize:      14 + 40,
		pixOffset:     14 + 40,
		dibHeaderSize: 40,
		width:         uint32(width),
		height:        uint32(height),
		colorPlane:    1,
	}
	pal := []byte{0, 0, 0, 0, 255, 255, 255, 0}

	bpp := 1
	rowsize := (width*bpp + 31) / 32 * 4

	h.bpp = uint16(bpp)
	h.colorUse = 1 << bpp
	h.imageSize = uint32(height * rowsize)
	h.fileSize += h.imageSize + uint32(len(pal))
	h.pixOffset += uint32(len(pal))

	var w bytes.Buffer
	binary.Write(&w, binary.LittleEndian, h)
	w.Write(pal)

	//reverse order
	for i := height - 1; i >= 0; i-- {
		w.Write(data[rowsize*i : rowsize*(i+1)])
	}

	return ioutil.WriteFile(fn, w.Bytes(), 0644)
}

func main() {
	width, height := 2242, 3136
	data, err := ioutil.ReadFile("1.jb2")
	if err != nil {
		log.Fatal(err)
	}

	out, err := jbig2dec.Decode(width, height, data)
	if err != nil {
		log.Fatal(err)
	}

	saveBitmap("1.bmp", width, height, out)
}
