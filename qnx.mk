# This is a makefile for QNX.
# make -f qnx.mk install

CC=qcc
CFLAGS=-O2 -fomit-frame-pointer -fPIC -Iinclude/

CXX=qcc
CXXFLAGS=-O2 -fomit-frame-pointer -fPIC -Iinclude/

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

all: libyuv_x86_64.so libyuv_aarch64le.so i444tonv12_eg_x86_64 i444tonv12_eg_aarch64le yuvconvert_x86_64 yuvconvert_aarch64le yuvconstants_x86_64 yuvconstants_aarch64le cpuid_x86_64 cpuid_aarch64le psnr_x86_64 psnr_aarch64le install

libyuv_x86_64.so: $(LOCAL_OBJ_FILES:.o=_x86_64.o)
	$(CXX) -shared -Vgcc_ntox86_64 -o $@ $(LOCAL_OBJ_FILES:.o=_x86_64.o)

libyuv_aarch64le.so: $(LOCAL_OBJ_FILES:.o=_aarch64le.o)
	$(CXX) -shared -Vgcc_ntoaarch64le -o $@ $(LOCAL_OBJ_FILES:.o=_aarch64le.o)

source/%_x86_64.o: source/%.cc
	$(CXX) -c $(CXXFLAGS) -Vgcc_ntox86_64 -o $@ $<

source/%_aarch64le.o: source/%.cc
	$(CXX) -c $(CXXFLAGS) -Vgcc_ntoaarch64le -o $@ $<

# A C++ test utility that uses libyuv conversion.
yuvconvert_x86_64: util/yuvconvert.cc libyuv_x86_64.so
	$(CXX) $(CXXFLAGS) -Vgcc_ntox86_64 -Iutil/ -o $@ util/yuvconvert.cc libyuv_x86_64.so

yuvconvert_aarch64le: util/yuvconvert.cc libyuv_aarch64le.so
	$(CXX) $(CXXFLAGS) -Vgcc_ntoaarch64le -Iutil/ -o $@ util/yuvconvert.cc libyuv_aarch64le.so

# A C test utility that generates yuvconstants for yuv to rgb.
yuvconstants_x86_64: util/yuvconstants.c libyuv_x86_64.so
	$(CXX) $(CXXFLAGS) -Vgcc_ntox86_64 -Iutil/ -lm -o $@ util/yuvconstants.c libyuv_x86_64.so

yuvconstants_aarch64le: util/yuvconstants.c libyuv_aarch64le.so
	$(CXX) $(CXXFLAGS) -Vgcc_ntoaarch64le -Iutil/ -lm -o $@ util/yuvconstants.c libyuv_aarch64le.so

# A standalone test utility
psnr_x86_64: util/psnr.cc
	$(CXX) $(CXXFLAGS) -Vgcc_ntox86_64 -Iutil/ -o $@ util/psnr.cc util/psnr_main.cc util/ssim.cc

psnr_aarch64le: util/psnr.cc
	$(CXX) $(CXXFLAGS) -Vgcc_ntoaarch64le -Iutil/ -o $@ util/psnr.cc util/psnr_main.cc util/ssim.cc

# A simple conversion example.
i444tonv12_eg_x86_64: util/i444tonv12_eg.cc libyuv_x86_64.so
	$(CXX) $(CXXFLAGS) -Vgcc_ntox86_64 -o $@ util/i444tonv12_eg.cc libyuv_x86_64.so

i444tonv12_eg_aarch64le: util/i444tonv12_eg.cc libyuv_aarch64le.so
	$(CXX) $(CXXFLAGS) -Vgcc_ntoaarch64le -o $@ util/i444tonv12_eg.cc libyuv_aarch64le.so

install: libyuv_x86_64.so libyuv_aarch64le.so
	mkdir -p $(INSTALL_ROOT_$(OS))/aarch64le/usr/lib/ $(INSTALL_ROOT_HDR)
	cp libyuv_x86_64.so $(INSTALL_ROOT_$(OS))/x86_64/usr/lib/libyuv.so
	cp libyuv_aarch64le.so $(INSTALL_ROOT_$(OS))/aarch64le/usr/lib/libyuv.so
	cp -r include/* $(INSTALL_ROOT_HDR)

# A C test utility that uses libyuv conversion from C.
# gcc 4.4 and older require -fno-exceptions to avoid link error on __gxx_personality_v0
# CC=gcc-4.4 CXXFLAGS=-fno-exceptions CXX=g++-4.4 make -f linux.mk
cpuid_x86_64: util/cpuid.c libyuv_x86_64.so
	$(CC) $(CFLAGS) -Vgcc_ntox86_64 -o $@ util/cpuid.c libyuv_x86_64.so -lm

cpuid_aarch64le: util/cpuid.c libyuv_aarch64le.so
	$(CC) $(CFLAGS) -Vgcc_ntoaarch64le -o $@ util/cpuid.c libyuv_aarch64le.so -lm

clean:
	/bin/rm -f source/*_x86_64.o source/*_aarch64le.o *.ii *.s libyuv_x86_64.so libyuv_aarch64le.so i444tonv12_eg_x86_64 i444tonv12_eg_aarch64le yuvconvert_x86_64 yuvconvert_aarch64le yuvconstants_x86_64 yuvconstants_aarch64le cpuid_x86_64 cpuid_aarch64le psnr_x86_64 psnr_aarch64le
endif
