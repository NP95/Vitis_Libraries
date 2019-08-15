/*
 * Copyright 2019 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file transpMatB2.hpp
 * @brief datamovers for symmetric matrices and vectors used in BLAS L2 routines.
 *
 * This file is part of XF BLAS Library.
 */

#ifndef XF_BLAS_TRANSPMATB2_HPP
#define XF_BLAS_TRANSPMATB2_HPP

#include "hls_stream.h"
#include "ap_int.h"
#include "ap_shift_reg.h"

namespace xf {
namespace linear_algebra {
namespace blas {

/**
 * @brief transpSymUpMat function that mirros the super-diagonals in a matrix block to sub-diagonals
 *
 * @tparam t_DataType the data type of the matrix entries
 * @tparam t_ParEntries number of parallelly processed entries in the matrix
 *
 * @param p_blocks number of t_ParEntries x t_ParEntries matrix blocks
 * @param p_in input stream of matrix blocks
 * @param p_out output stream of symmetric matrix blocks
 */
template <typename t_DataType, unsigned int t_ParEntries>
void transpSymUpMatBlocks(unsigned int p_blocks,
                          hls::stream<WideType<t_DataType, t_ParEntries> >& p_in,
                          hls::stream<WideType<t_DataType, t_ParEntries> >& p_out) {
    t_DataType l_buf[t_ParEntries][t_ParEntries];
#pragma HLS ARRAY_PARTITION variable = l_buf complete dim = 0
    for (unsigned int l_block = 0; l_block < p_blocks; ++l_block) {
        // shuffle and store
        for (unsigned int i = 0; i < t_ParEntries; ++i) {
#pragma HLS PIPELINE
            WideType<t_DataType, t_ParEntries> l_val;
#pragma HLS ARRAY_PARTITION variable = l_val complete
            l_val = p_in.read();
            for (unsigned int j = 0; j < t_ParEntries; ++j) {
                l_buf[i][j] = l_val[j];
            }
        }

        for (unsigned int i = 0; i < t_ParEntries; ++i) {
#pragma HLS PIPELINE
            WideType<t_DataType, t_ParEntries> l_val;
#pragma HLS ARRAY_PARTITION variable = l_val complete
            for (unsigned int j = 0; j < t_ParEntries; ++j) {
                l_val[j] = (i > j) ? l_buf[j][i] : l_buf[i][j];
            }
            p_out.write(l_val);
        }
    }
}

/**
 * @brief transpSymLoMat function that mirros the sub-diagonals in a matrix block to super-diagonals
 *
 * @tparam t_DataType the data type of the matrix entries
 * @tparam t_ParEntries number of parallelly processed entries in the matrix
 *
 * @param p_blocks number of t_ParEntries x t_ParEntries matrix blocks
 * @param p_in input stream of matrix blocks
 * @param p_out output stream of symmetric matrix blocks
 */
template <typename t_DataType, unsigned int t_ParEntries>
void transpSymLoMatBlocks(unsigned int p_blocks,
                          hls::stream<WideType<t_DataType, t_ParEntries> >& p_in,
                          hls::stream<WideType<t_DataType, t_ParEntries> >& p_out) {
    t_DataType l_buf[t_ParEntries][t_ParEntries];
#pragma HLS ARRAY_PARTITION variable = l_buf complete dim = 0
    for (unsigned int l_block = 0; l_block < p_blocks; ++l_block) {
        // shuffle and store
        for (unsigned int i = 0; i < t_ParEntries; ++i) {
#pragma HLS PIPELINE
            WideType<t_DataType, t_ParEntries> l_val;
#pragma HLS ARRAY_PARTITION variable = l_val complete
            l_val = p_in.read();
            for (unsigned int j = 0; j < t_ParEntries; ++j) {
                l_buf[i][j] = l_val[j];
            }
        }

        for (unsigned int i = 0; i < t_ParEntries; ++i) {
#pragma HLS PIPELINE
            WideType<t_DataType, t_ParEntries> l_val;
#pragma HLS ARRAY_PARTITION variable = l_val complete
            for (unsigned int j = 0; j < t_ParEntries; ++j) {
                l_val[j] = (i < j) ? l_buf[j][i] : l_buf[i][j];
            }
            p_out.write(l_val);
        }
    }
}
/**
 * @brief transpMat function transposes matrix blocks
 *
 * @tparam t_DataType the data type of the matrix entries
 * @tparam t_ParEntries the number of parallely processed entries in the matrix
 *
 * @param p_blocks number of matrix blocks
 * @param p_in input stream of matrix blocks
 * @param p_out output stream of matrix blocks
 */
template <typename t_DataType, unsigned int t_ParEntries>
void transpMatBlocks(unsigned int p_blocks,
                     hls::stream<WideType<t_DataType, t_ParEntries> >& p_in,
                     hls::stream<WideType<t_DataType, t_ParEntries> >& p_out) {
    t_DataType l_buf[t_ParEntries][t_ParEntries];
#pragma HLS ARRAY_PARTITION variable = l_buf complete dim = 0
    for (unsigned int l_block = 0; l_block < p_blocks; ++l_block) {
        // shuffle and store
        for (unsigned int i = 0; i < t_ParEntries; ++i) {
#pragma HLS PIPELINE
            WideType<t_DataType, t_ParEntries> l_val;
#pragma HLS ARRAY_PARTITION variable = l_val complete
            l_val = p_in.read();
            for (unsigned int j = 0; j < t_ParEntries; ++j) {
                l_buf[i][j] = l_val[j];
            }
        }

        for (unsigned int i = 0; i < t_ParEntries; ++i) {
#pragma HLS PIPELINE
            WideType<t_DataType, t_ParEntries> l_val;
#pragma HLS ARRAY_PARTITION variable = l_val complete
            for (unsigned int j = 0; j < t_ParEntries; ++j) {
                l_val[j] = l_buf[j][i];
            }
            p_out.write(l_val);
        }
    }
}

/**
 * @brief fwdMatBlocks function forwards matrix blocks
 *
 * @tparam t_DataType the data type of the matrix entries
 * @tparam t_ParEntries the number of parallely processed entries in the matrix
 *
 * @param p_blocks number of matrix blocks
 * @param p_in input stream of matrix blocks
 * @param p_out output stream of matrix blocks
 */
template <typename t_DataType, unsigned int t_ParEntries>
void fwdMatBlocks(unsigned int p_blocks,
                  hls::stream<WideType<t_DataType, t_ParEntries> >& p_in,
                  hls::stream<WideType<t_DataType, t_ParEntries> >& p_out) {
    for (unsigned int l_block = 0; l_block < p_blocks; ++l_block) {
        for (unsigned int i = 0; i < t_ParEntries; ++i) {
#pragma HLS PIPELINE
            WideType<t_DataType, t_ParEntries> l_val;
            l_val = p_in.read();
            p_out.write(l_val);
        }
    }
}

/**
 * @brief transpMemWordBlocks memWord wise transposer
 *
 * @tparam t_DataType data type of the matrix entries
 * @tparam t_MemWidth number of entries in one memory word
 * @tparam t_Rows number of rows in the block
 * @tparam t_Cols number of cols in the block
 *
 * @param p_blocks number of blocks
 * @param p_in input stream of memory words
 * @param p_out ouput transposed stream of memory words
 */
template <typename t_DataType, unsigned int t_MemWidth, unsigned int t_Rows, unsigned int t_Cols>
void transpMemWordBlocks(unsigned int p_blocks,
                         hls::stream<WideType<t_DataType, t_MemWidth> >& p_in,
                         hls::stream<WideType<t_DataType, t_MemWidth> >& p_out) {
#ifndef __SYNTHESIS__
    assert(t_Cols % t_MemWidth == 0);
#endif

    static const unsigned int t_ColWords = t_Cols / t_MemWidth;
    static const unsigned int t_Size = t_ColWords * t_Rows;

    WideType<t_DataType, t_MemWidth> l_buf[t_Size];
#pragma HLS DATA_PACK variable = l_buf
    for (unsigned int b = 0; b < p_blocks; ++b) {
#pragma HLS DATAFLOW
        for (unsigned int i = 0; i < t_Rows; ++i) {
            for (unsigned int j = 0; j < t_ColWords; ++j) {
#pragma HLS PIPELINE REWIND
                WideType<t_DataType, t_MemWidth> l_val = p_in.read();
                l_buf[j * t_Rows + i] = l_val;
            }
        }
        for (unsigned int i = 0; i < t_Size; ++i) {
#pragma HLS PIPELINE REWIND
            WideType<t_DataType, t_MemWidth> l_val = l_buf[i];
            p_out.write(l_val);
        }
    }
}

/**
 * @brief transpMemBlocks read data from device memory and transpose the memory blcok
 *
 * @tparam t_DataType data type of the matrix entries
 * @tparam t_MemWidth number of entries in one memory word
 * @tparam t_Rows number of rows in the block
 * @tparam t_Cols number of cols in the block
 *
 * @param p_blocks number of blocks
 * @param p_in input stream of memory words
 * @param p_out ouput transposed stream of memory words
 */
template <typename t_DataType, unsigned int t_MemWidth, unsigned int t_Rows, unsigned int t_Cols>
void transpMemBlocks(unsigned int p_blocks,
                     hls::stream<WideType<t_DataType, t_MemWidth> >& p_in,
                     hls::stream<WideType<t_DataType, t_MemWidth> >& p_out) {
#ifndef __SYNTHESIS__
    assert(t_Cols % t_MemWidth == 0);
    assert(t_Rows % t_MemWidth == 0);
#endif

    static const unsigned int t_ColWords = t_Cols / t_MemWidth;
    static const unsigned int t_RowWords = t_Rows / t_MemWidth;

    t_DataType l_buf[t_Rows * t_ColWords][t_MemWidth];
#pragma HLS ARRAY_PARTITION variable = l_buf complete dim = 2
    for (unsigned int b = 0; b < p_blocks; ++b) {
#pragma HLS DATAFLOW
        for (unsigned int i = 0; i < t_Rows; ++i) {
            for (unsigned int j = 0; j < t_ColWords; ++j) {
#pragma HLS PIPELINE REWIND
                WideType<t_DataType, t_MemWidth> l_val = p_in.read();
#pragma HLS ARRAY_PARTITION variable = l_val complete
                for (unsigned int k = 0; k < t_MemWidth; ++k) {
                    l_buf[i * t_ColWords + j][k] = l_val[(t_MemWidth - i + k) % t_MemWidth];
                }
            }
        }
        for (unsigned int i = 0; i < t_Cols; ++i) {
            for (unsigned int j = 0; j < t_RowWords; ++j) {
#pragma HLS PIPELINE REWIND
                WideType<t_DataType, t_MemWidth> l_val;
#pragma HLS ARRAY_PARTITION variable = l_val complete
                WideType<t_DataType, t_MemWidth> l_out;
#pragma HLS ARRAY_PARTITION variable = l_out complete
                for (unsigned int k = 0; k < t_MemWidth; ++k) {
                    l_val[k] = l_buf[j * t_Cols + i / t_MemWidth +
                                     ((k + t_MemWidth - i % t_MemWidth) % t_MemWidth) * t_ColWords][k];
                }
                for (unsigned int k = 0; k < t_MemWidth; ++k) {
                    l_out[k] = l_val[(k + i) % t_MemWidth];
                }
                p_out.write(l_out);
            }
        }
    }
}
} // namespace blas
} // namespace linear_algebra
} // namespace xf
#endif
