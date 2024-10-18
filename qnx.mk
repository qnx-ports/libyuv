# This is a  makefile for QNX.
# make -f qnx.mk install

CC=qcc
CFLAGS=-O2 -fomit-frame-pointer -fPIC -Vgcc_ntoaarch64le -Iinclude/

CXX=qcc
CXXFLAGS=-O2 -fomit-frame-pointer -fPIC -Vgcc_ntoaarch64le -Iinclude/

LOCAL_OBJ_FILES := \
	source/compare.o           \
	source/compare_common.o    \
	source/compare_gcc.o       \
	source/compare_mmi.o       \
	source/compare_msa.o       \
	source/compare_neon.o      \
	source/compare_neon64.o    \
	source/compare_win.o       \
	source/convert.o           \
	source/convert_argb.o      \
	source/convert_from.o      \
	source/convert_from_argb.o \
	source/convert_jpeg.o      \
	source/convert_to_argb.o   \
	source/convert_to_i420.o   \
	source/cpu_id.o            \
	source/mjpeg_decoder.o     \
	source/mjpeg_validate.o    \
	source/planar_functions.o  \
	source/rotate.o            \
	source/rotate_any.o        \
	source/rotate_argb.o       \
	source/rotate_common.o     \
	source/rotate_gcc.o        \
	source/rotate_mmi.o        \
	source/rotate_msa.o        \
	source/rotate_neon.o       \
	source/rotate_neon64.o     \
	source/rotate_win.o        \
	source/row_any.o           \
	source/row_common.o        \
	source/row_gcc.o           \
	source/row_mmi.o           \
	source/row_msa.o           \
	source/row_neon.o          \
	source/row_neon64.o        \
	source/row_win.o           \
	source/scale.o             \
	source/scale_any.o         \
	source/scale_argb.o        \
	source/scale_common.o      \
	source/scale_gcc.o         \
	source/scale_mmi.o         \
	source/scale_msa.o         \
	source/scale_neon.o        \
	source/scale_neon64.o      \
	source/scale_uv.o          \
	source/scale_win.o         \
	source/video_common.o

# Hack required to use INSTALL_ROOT_$(OS) and
# INSTALL_ROOT_HDR variables without using
# default QNX Makefile system
MKFILES_ROOT=$(QNX_TARGET)/usr/include/mk
CPU=aarch64
include $(MKFILES_ROOT)/qconf-nto.mk
include $(MKFILES_ROOT)/qmacros.mk

ifndef NO_TARGET_OVERRIDE
.cc.o:
	$(CXX) -c $(CXXFLAGS) $*.cc -o $*.o

.c.o:
	$(CC) -c $(CFLAGS) $*.c -o $*.o

all: libyuv.so i444tonv12_eg yuvconvert yuvconstants cpuid psnr install

libyuv.so: $(LOCAL_OBJ_FILES)
	$(CXX) -shared -Vgcc_ntoaarch64le -o $@ $(LOCAL_OBJ_FILES)

# A C++ test utility that uses libyuv conversion.
yuvconvert: util/yuvconvert.cc libyuv.so
	$(CXX) $(CXXFLAGS) -Iutil/ -o $@ util/yuvconvert.cc libyuv.so

# A C test utility that generates yuvconstants for yuv to rgb.
yuvconstants: util/yuvconstants.c libyuv.so
	$(CXX) $(CXXFLAGS) -Iutil/ -lm -o $@ util/yuvconstants.c libyuv.so

# A standalone test utility
psnr: util/psnr.cc
	$(CXX) $(CXXFLAGS) -Iutil/ -o $@ util/psnr.cc util/psnr_main.cc util/ssim.cc

# A simple conversion example.
i444tonv12_eg: util/i444tonv12_eg.cc libyuv.so
	$(CXX) $(CXXFLAGS) -o $@ util/i444tonv12_eg.cc libyuv.so

install: libyuv.so
	mkdir -p $(INSTALL_ROOT_$(OS))/aarch64le/usr/lib/ $(INSTALL_ROOT_HDR)
	cp libyuv.so $(INSTALL_ROOT_$(OS))/aarch64le/usr/lib/
	cp -r include/* $(INSTALL_ROOT_HDR)

# A C test utility that uses libyuv conversion from C.
# gcc 4.4 and older require -fno-exceptions to avoid link error on __gxx_personality_v0
# CC=gcc-4.4 CXXFLAGS=-fno-exceptions CXX=g++-4.4 make -f linux.mk
cpuid: util/cpuid.c libyuv.so
	$(CC) $(CFLAGS) -o $@ util/cpuid.c libyuv.so -lm

clean:
	/bin/rm -f source/*.o *.ii *.s libyuv.so i444tonv12_eg yuvconvert yuvconstants cpuid psnr
endif
