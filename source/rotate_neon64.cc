/*
 *  Copyright 2014 The LibYuv Project Authors. All rights reserved.
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

// This module is for GCC Neon armv8 64 bit.
#if !defined(LIBYUV_DISABLE_NEON) && defined(__aarch64__)

void TransposeWx16_NEON(const uint8_t* src,
                        int src_stride,
                        uint8_t* dst,
                        int dst_stride,
                        int width) {
  const uint8_t* src_temp;
  asm("1:                                              \n"
      "mov   %[src_temp], %[src]                       \n"

      "ld1   {v0.16b}, [%[src_temp]], %[src_stride]    \n"
      "ld1   {v1.16b}, [%[src_temp]], %[src_stride]    \n"
      "ld1   {v2.16b}, [%[src_temp]], %[src_stride]    \n"
      "ld1   {v3.16b}, [%[src_temp]], %[src_stride]    \n"
      "ld1   {v4.16b}, [%[src_temp]], %[src_stride]    \n"
      "ld1   {v5.16b}, [%[src_temp]], %[src_stride]    \n"
      "ld1   {v6.16b}, [%[src_temp]], %[src_stride]    \n"
      "ld1   {v7.16b}, [%[src_temp]], %[src_stride]    \n"
      "ld1   {v8.16b}, [%[src_temp]], %[src_stride]    \n"
      "ld1   {v9.16b}, [%[src_temp]], %[src_stride]    \n"
      "ld1   {v10.16b}, [%[src_temp]], %[src_stride]   \n"
      "ld1   {v11.16b}, [%[src_temp]], %[src_stride]   \n"
      "ld1   {v12.16b}, [%[src_temp]], %[src_stride]   \n"
      "ld1   {v13.16b}, [%[src_temp]], %[src_stride]   \n"
      "ld1   {v14.16b}, [%[src_temp]], %[src_stride]   \n"
      "ld1   {v15.16b}, [%[src_temp]], %[src_stride]   \n"

      "add   %[src], %[src], #16                       \n"

      // Transpose 8x8-byte blocks.
      "trn1  v16.2d, v0.2d, v8.2d                      \n"
      "trn1  v17.2d, v1.2d, v9.2d                      \n"
      "trn1  v18.2d, v2.2d, v10.2d                     \n"
      "trn1  v19.2d, v3.2d, v11.2d                     \n"
      "trn1  v20.2d, v4.2d, v12.2d                     \n"
      "trn1  v21.2d, v5.2d, v13.2d                     \n"
      "trn1  v22.2d, v6.2d, v14.2d                     \n"
      "trn1  v23.2d, v7.2d, v15.2d                     \n"
      "trn2  v24.2d, v0.2d, v8.2d                      \n"
      "trn2  v25.2d, v1.2d, v9.2d                      \n"
      "trn2  v26.2d, v2.2d, v10.2d                     \n"
      "trn2  v27.2d, v3.2d, v11.2d                     \n"
      "trn2  v28.2d, v4.2d, v12.2d                     \n"
      "trn2  v29.2d, v5.2d, v13.2d                     \n"
      "trn2  v30.2d, v6.2d, v14.2d                     \n"
      "trn2  v31.2d, v7.2d, v15.2d                     \n"

      "subs  %w[width], %w[width], #16                 \n"

      // Transpose 4x4-byte blocks within each 8x8 block.
      "trn1  v0.4s, v16.4s, v20.4s                     \n"
      "trn1  v1.4s, v17.4s, v21.4s                     \n"
      "trn1  v2.4s, v18.4s, v22.4s                     \n"
      "trn1  v3.4s, v19.4s, v23.4s                     \n"
      "trn2  v4.4s, v16.4s, v20.4s                     \n"
      "trn2  v5.4s, v17.4s, v21.4s                     \n"
      "trn2  v6.4s, v18.4s, v22.4s                     \n"
      "trn2  v7.4s, v19.4s, v23.4s                     \n"
      "trn1  v8.4s, v24.4s, v28.4s                     \n"
      "trn1  v9.4s, v25.4s, v29.4s                     \n"
      "trn1  v10.4s, v26.4s, v30.4s                    \n"
      "trn1  v11.4s, v27.4s, v31.4s                    \n"
      "trn2  v12.4s, v24.4s, v28.4s                    \n"
      "trn2  v13.4s, v25.4s, v29.4s                    \n"
      "trn2  v14.4s, v26.4s, v30.4s                    \n"
      "trn2  v15.4s, v27.4s, v31.4s                    \n"

      // Transpose 2x2-byte blocks within each 4x4 block.
      "trn1  v16.8h, v0.8h, v2.8h                      \n"
      "trn1  v17.8h, v1.8h, v3.8h                      \n"
      "trn2  v18.8h, v0.8h, v2.8h                      \n"
      "trn2  v19.8h, v1.8h, v3.8h                      \n"
      "trn1  v20.8h, v4.8h, v6.8h                      \n"
      "trn1  v21.8h, v5.8h, v7.8h                      \n"
      "trn2  v22.8h, v4.8h, v6.8h                      \n"
      "trn2  v23.8h, v5.8h, v7.8h                      \n"
      "trn1  v24.8h, v8.8h, v10.8h                     \n"
      "trn1  v25.8h, v9.8h, v11.8h                     \n"
      "trn2  v26.8h, v8.8h, v10.8h                     \n"
      "trn2  v27.8h, v9.8h, v11.8h                     \n"
      "trn1  v28.8h, v12.8h, v14.8h                    \n"
      "trn1  v29.8h, v13.8h, v15.8h                    \n"
      "trn2  v30.8h, v12.8h, v14.8h                    \n"
      "trn2  v31.8h, v13.8h, v15.8h                    \n"

      // Transpose bytes within each 2x2 block.
      "trn1  v0.16b, v16.16b, v17.16b                  \n"
      "trn2  v1.16b, v16.16b, v17.16b                  \n"
      "trn1  v2.16b, v18.16b, v19.16b                  \n"
      "trn2  v3.16b, v18.16b, v19.16b                  \n"
      "trn1  v4.16b, v20.16b, v21.16b                  \n"
      "trn2  v5.16b, v20.16b, v21.16b                  \n"
      "trn1  v6.16b, v22.16b, v23.16b                  \n"
      "trn2  v7.16b, v22.16b, v23.16b                  \n"
      "trn1  v8.16b, v24.16b, v25.16b                  \n"
      "trn2  v9.16b, v24.16b, v25.16b                  \n"
      "trn1  v10.16b, v26.16b, v27.16b                 \n"
      "trn2  v11.16b, v26.16b, v27.16b                 \n"
      "trn1  v12.16b, v28.16b, v29.16b                 \n"
      "trn2  v13.16b, v28.16b, v29.16b                 \n"
      "trn1  v14.16b, v30.16b, v31.16b                 \n"
      "trn2  v15.16b, v30.16b, v31.16b                 \n"

      "st1   {v0.16b}, [%[dst]], %[dst_stride]         \n"
      "st1   {v1.16b}, [%[dst]], %[dst_stride]         \n"
      "st1   {v2.16b}, [%[dst]], %[dst_stride]         \n"
      "st1   {v3.16b}, [%[dst]], %[dst_stride]         \n"
      "st1   {v4.16b}, [%[dst]], %[dst_stride]         \n"
      "st1   {v5.16b}, [%[dst]], %[dst_stride]         \n"
      "st1   {v6.16b}, [%[dst]], %[dst_stride]         \n"
      "st1   {v7.16b}, [%[dst]], %[dst_stride]         \n"
      "st1   {v8.16b}, [%[dst]], %[dst_stride]         \n"
      "st1   {v9.16b}, [%[dst]], %[dst_stride]         \n"
      "st1   {v10.16b}, [%[dst]], %[dst_stride]        \n"
      "st1   {v11.16b}, [%[dst]], %[dst_stride]        \n"
      "st1   {v12.16b}, [%[dst]], %[dst_stride]        \n"
      "st1   {v13.16b}, [%[dst]], %[dst_stride]        \n"
      "st1   {v14.16b}, [%[dst]], %[dst_stride]        \n"
      "st1   {v15.16b}, [%[dst]], %[dst_stride]        \n"

      "b.gt  1b                                        \n"
      : [src] "+r"(src),                          // %[src]
        [src_temp] "=&r"(src_temp),               // %[src_temp]
        [dst] "+r"(dst),                          // %[dst]
        [width] "+r"(width)                       // %[width]
      : [src_stride] "r"((ptrdiff_t)src_stride),  // %[src_stride]
        [dst_stride] "r"((ptrdiff_t)dst_stride)   // %[dst_stride]
      : "cc", "memory", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8",
        "v9", "v10", "v11", "v12", "v13", "v14", "v15", "v16", "v17", "v18",
        "v19", "v20", "v21", "v22", "v23", "v24", "v25", "v26", "v27", "v28",
        "v29", "v30", "v31");
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
      "sub         %w[width], %w[width], #8                      \n"

      "1:                                                        \n"
      "mov         %[temp], %[src]                               \n"
      "ld1         {v0.16b}, [%[temp]], %[src_stride]            \n"
      "ld1         {v1.16b}, [%[temp]], %[src_stride]            \n"
      "ld1         {v2.16b}, [%[temp]], %[src_stride]            \n"
      "ld1         {v3.16b}, [%[temp]], %[src_stride]            \n"
      "ld1         {v4.16b}, [%[temp]], %[src_stride]            \n"
      "ld1         {v5.16b}, [%[temp]], %[src_stride]            \n"
      "ld1         {v6.16b}, [%[temp]], %[src_stride]            \n"
      "ld1         {v7.16b}, [%[temp]]                           \n"
      "add         %[src], %[src], #16                           \n"

      "trn1        v16.16b, v0.16b, v1.16b                       \n"
      "trn2        v17.16b, v0.16b, v1.16b                       \n"
      "trn1        v18.16b, v2.16b, v3.16b                       \n"
      "trn2        v19.16b, v2.16b, v3.16b                       \n"
      "trn1        v20.16b, v4.16b, v5.16b                       \n"
      "trn2        v21.16b, v4.16b, v5.16b                       \n"
      "trn1        v22.16b, v6.16b, v7.16b                       \n"
      "trn2        v23.16b, v6.16b, v7.16b                       \n"

      "subs        %w[width], %w[width],  #8                     \n"

      "trn1        v0.8h, v16.8h, v18.8h                         \n"
      "trn2        v1.8h, v16.8h, v18.8h                         \n"
      "trn1        v2.8h, v20.8h, v22.8h                         \n"
      "trn2        v3.8h, v20.8h, v22.8h                         \n"
      "trn1        v4.8h, v17.8h, v19.8h                         \n"
      "trn2        v5.8h, v17.8h, v19.8h                         \n"
      "trn1        v6.8h, v21.8h, v23.8h                         \n"
      "trn2        v7.8h, v21.8h, v23.8h                         \n"

      "trn1        v16.4s, v0.4s, v2.4s                          \n"
      "trn2        v17.4s, v0.4s, v2.4s                          \n"
      "trn1        v18.4s, v1.4s, v3.4s                          \n"
      "trn2        v19.4s, v1.4s, v3.4s                          \n"
      "trn1        v20.4s, v4.4s, v6.4s                          \n"
      "trn2        v21.4s, v4.4s, v6.4s                          \n"
      "trn1        v22.4s, v5.4s, v7.4s                          \n"
      "trn2        v23.4s, v5.4s, v7.4s                          \n"

      "mov         %[temp], %[dst_a]                             \n"
      "st1         {v16.d}[0], [%[temp]], %[dst_stride_a]        \n"
      "st1         {v18.d}[0], [%[temp]], %[dst_stride_a]        \n"
      "st1         {v17.d}[0], [%[temp]], %[dst_stride_a]        \n"
      "st1         {v19.d}[0], [%[temp]], %[dst_stride_a]        \n"
      "st1         {v16.d}[1], [%[temp]], %[dst_stride_a]        \n"
      "st1         {v18.d}[1], [%[temp]], %[dst_stride_a]        \n"
      "st1         {v17.d}[1], [%[temp]], %[dst_stride_a]        \n"
      "st1         {v19.d}[1], [%[temp]]                         \n"
      "add         %[dst_a], %[dst_a], %[dst_stride_a], lsl #3   \n"

      "mov         %[temp], %[dst_b]                             \n"
      "st1         {v20.d}[0], [%[temp]], %[dst_stride_b]        \n"
      "st1         {v22.d}[0], [%[temp]], %[dst_stride_b]        \n"
      "st1         {v21.d}[0], [%[temp]], %[dst_stride_b]        \n"
      "st1         {v23.d}[0], [%[temp]], %[dst_stride_b]        \n"
      "st1         {v20.d}[1], [%[temp]], %[dst_stride_b]        \n"
      "st1         {v22.d}[1], [%[temp]], %[dst_stride_b]        \n"
      "st1         {v21.d}[1], [%[temp]], %[dst_stride_b]        \n"
      "st1         {v23.d}[1], [%[temp]]                         \n"
      "add         %[dst_b], %[dst_b], %[dst_stride_b], lsl #3   \n"

      "b.ge        1b                                            \n"
      : [temp] "=&r"(temp),                           // %[temp]
        [src] "+r"(src),                              // %[src]
        [dst_a] "+r"(dst_a),                          // %[dst_a]
        [dst_b] "+r"(dst_b),                          // %[dst_b]
        [width] "+r"(width)                           // %[width]
      : [src_stride] "r"((ptrdiff_t)src_stride),      // %[src_stride]
        [dst_stride_a] "r"((ptrdiff_t)dst_stride_a),  // %[dst_stride_a]
        [dst_stride_b] "r"((ptrdiff_t)dst_stride_b)   // %[dst_stride_b]
      : "memory", "cc", "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v16",
        "v17", "v18", "v19", "v20", "v21", "v22", "v23", "v30", "v31");
}

#endif  // !defined(LIBYUV_DISABLE_NEON) && defined(__aarch64__)

#ifdef __cplusplus
}  // extern "C"
}  // namespace libyuv
#endif
