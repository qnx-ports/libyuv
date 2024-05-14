/*
 *  Copyright 2011 The LibYuv Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS. All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "libyuv/rotate_row.h"
#include "libyuv/row.h"

#include "libyuv/basic_types.h"

#ifdef __cplusplus
namespace libyuv {
extern "C" {
#endif

#if !defined(LIBYUV_DISABLE_NEON) && defined(__ARM_NEON__) && \
    !defined(__aarch64__)

void TransposeWx8_NEON(const uint8_t* src,
                       int src_stride,
                       uint8_t* dst,
                       int dst_stride,
                       int width) {
  const uint8_t* temp;
  asm(
      // loops are on blocks of 8. loop will stop when
      // counter gets to or below 0. starting the counter
      // at w-8 allow for this
      "sub         %[width], #8                           \n"

      "1:                                                 \n"
      "mov         %[temp], %[src]                        \n"
      "vld1.8      {d0}, [%[temp]], %[src_stride]         \n"
      "vld1.8      {d1}, [%[temp]], %[src_stride]         \n"
      "vld1.8      {d2}, [%[temp]], %[src_stride]         \n"
      "vld1.8      {d3}, [%[temp]], %[src_stride]         \n"
      "vld1.8      {d4}, [%[temp]], %[src_stride]         \n"
      "vld1.8      {d5}, [%[temp]], %[src_stride]         \n"
      "vld1.8      {d6}, [%[temp]], %[src_stride]         \n"
      "vld1.8      {d7}, [%[temp]]                        \n"
      "add         %[src], #8                             \n"

      "vtrn.8      d1, d0                                 \n"
      "vtrn.8      d3, d2                                 \n"
      "vtrn.8      d5, d4                                 \n"
      "vtrn.8      d7, d6                                 \n"
      "subs        %[width], #8                           \n"

      "vtrn.16     d1, d3                                 \n"
      "vtrn.16     d0, d2                                 \n"
      "vtrn.16     d5, d7                                 \n"
      "vtrn.16     d4, d6                                 \n"

      "vtrn.32     d1, d5                                 \n"
      "vtrn.32     d0, d4                                 \n"
      "vtrn.32     d3, d7                                 \n"
      "vtrn.32     d2, d6                                 \n"

      "vrev16.8    q0, q0                                 \n"
      "vrev16.8    q1, q1                                 \n"
      "vrev16.8    q2, q2                                 \n"
      "vrev16.8    q3, q3                                 \n"

      "mov         %[temp], %[dst]                        \n"
      "vst1.8      {d1}, [%[temp]], %[dst_stride]         \n"
      "vst1.8      {d0}, [%[temp]], %[dst_stride]         \n"
      "vst1.8      {d3}, [%[temp]], %[dst_stride]         \n"
      "vst1.8      {d2}, [%[temp]], %[dst_stride]         \n"
      "vst1.8      {d5}, [%[temp]], %[dst_stride]         \n"
      "vst1.8      {d4}, [%[temp]], %[dst_stride]         \n"
      "vst1.8      {d7}, [%[temp]], %[dst_stride]         \n"
      "vst1.8      {d6}, [%[temp]]                        \n"
      "add         %[dst], %[dst], %[dst_stride], lsl #3  \n"

      "bge         1b                                     \n"
      : [temp] "=&r"(temp),            // %[temp]
        [src] "+r"(src),               // %[src]
        [dst] "+r"(dst),               // %[dst]
        [width] "+r"(width)            // %[width]
      : [src_stride] "r"(src_stride),  // %[src_stride]
        [dst_stride] "r"(dst_stride)   // %[dst_stride]
      : "memory", "cc", "q0", "q1", "q2", "q3");
}

void TransposeUVWx8_NEON(const uint8_t* src,
                         int src_stride,
                         uint8_t* dst_a,
                         int dst_stride_a,
                         uint8_t* dst_b,
                         int dst_stride_b,
                         int width) {
  const uint8_t* temp;
  asm(
      // loops are on blocks of 8. loop will stop when
      // counter gets to or below 0. starting the counter
      // at w-8 allow for this
      "sub         %[width], #8                                 \n"

      "1:                                                       \n"
      "mov         %[temp], %[src]                              \n"
      "vld2.8      {d0,  d1},  [%[temp]], %[src_stride]         \n"
      "vld2.8      {d2,  d3},  [%[temp]], %[src_stride]         \n"
      "vld2.8      {d4,  d5},  [%[temp]], %[src_stride]         \n"
      "vld2.8      {d6,  d7},  [%[temp]], %[src_stride]         \n"
      "vld2.8      {d16, d17}, [%[temp]], %[src_stride]         \n"
      "vld2.8      {d18, d19}, [%[temp]], %[src_stride]         \n"
      "vld2.8      {d20, d21}, [%[temp]], %[src_stride]         \n"
      "vld2.8      {d22, d23}, [%[temp]]                        \n"
      "add         %[src], #8*2                                 \n"

      "vtrn.8      q1, q0                                       \n"
      "vtrn.8      q3, q2                                       \n"
      "vtrn.8      q9, q8                                       \n"
      "vtrn.8      q11, q10                                     \n"
      "subs        %[width], #8                                 \n"

      "vtrn.16     q1, q3                                       \n"
      "vtrn.16     q0, q2                                       \n"
      "vtrn.16     q9, q11                                      \n"
      "vtrn.16     q8, q10                                      \n"

      "vtrn.32     q1, q9                                       \n"
      "vtrn.32     q0, q8                                       \n"
      "vtrn.32     q3, q11                                      \n"
      "vtrn.32     q2, q10                                      \n"

      "vrev16.8    q0, q0                                       \n"
      "vrev16.8    q1, q1                                       \n"
      "vrev16.8    q2, q2                                       \n"
      "vrev16.8    q3, q3                                       \n"
      "vrev16.8    q8, q8                                       \n"
      "vrev16.8    q9, q9                                       \n"
      "vrev16.8    q10, q10                                     \n"
      "vrev16.8    q11, q11                                     \n"

      "mov         %[temp], %[dst_a]                            \n"
      "vst1.8      {d2},  [%[temp]], %[dst_stride_a]            \n"
      "vst1.8      {d0},  [%[temp]], %[dst_stride_a]            \n"
      "vst1.8      {d6},  [%[temp]], %[dst_stride_a]            \n"
      "vst1.8      {d4},  [%[temp]], %[dst_stride_a]            \n"
      "vst1.8      {d18}, [%[temp]], %[dst_stride_a]            \n"
      "vst1.8      {d16}, [%[temp]], %[dst_stride_a]            \n"
      "vst1.8      {d22}, [%[temp]], %[dst_stride_a]            \n"
      "vst1.8      {d20}, [%[temp]]                             \n"
      "add         %[dst_a], %[dst_a], %[dst_stride_a], lsl #3  \n"

      "mov         %[temp], %[dst_b]                            \n"
      "vst1.8      {d3},  [%[temp]], %[dst_stride_b]            \n"
      "vst1.8      {d1},  [%[temp]], %[dst_stride_b]            \n"
      "vst1.8      {d7},  [%[temp]], %[dst_stride_b]            \n"
      "vst1.8      {d5},  [%[temp]], %[dst_stride_b]            \n"
      "vst1.8      {d19}, [%[temp]], %[dst_stride_b]            \n"
      "vst1.8      {d17}, [%[temp]], %[dst_stride_b]            \n"
      "vst1.8      {d23}, [%[temp]], %[dst_stride_b]            \n"
      "vst1.8      {d21}, [%[temp]]                             \n"
      "add         %[dst_b], %[dst_b], %[dst_stride_b], lsl #3  \n"

      "bge         1b                                           \n"
      : [temp] "=&r"(temp),                // %[temp]
        [src] "+r"(src),                   // %[src]
        [dst_a] "+r"(dst_a),               // %[dst_a]
        [dst_b] "+r"(dst_b),               // %[dst_b]
        [width] "+r"(width)                // %[width]
      : [src_stride] "r"(src_stride),      // %[src_stride]
        [dst_stride_a] "r"(dst_stride_a),  // %[dst_stride_a]
        [dst_stride_b] "r"(dst_stride_b)   // %[dst_stride_b]
      : "memory", "cc", "q0", "q1", "q2", "q3", "q8", "q9", "q10", "q11");
}

#endif  // defined(__ARM_NEON__) && !defined(__aarch64__)

#ifdef __cplusplus
}  // extern "C"
}  // namespace libyuv
#endif
