/*
//@HEADER
// ************************************************************************
//
//               KokkosKernels: Linear Algebra and Graph Kernels
//                 Copyright 2016 Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact Siva Rajamanickam (srajama@sandia.gov)
//
// ************************************************************************
//@HEADER
*/

#ifndef MKL_SPMV_HPP_
#define MKL_SPMV_HPP_

#ifdef HAVE_MKL
#include <mkl.h>

template<typename AType, typename XType, typename YType>
void mkl_matvec(AType A, XType x, YType y, int rows_per_thread, int team_size, int vector_length) {

  double s_a = 1.0;
  double s_b = 0.0;

  char matdescra[6] = "GLNC0";
  char transa = 'N';
  int m = A.numRows();
  int n = x.dimension_1();
  int k = A.numCols();
  double* x_ptr = (double*)x.ptr_on_device();
  double* y_ptr = (double*)y.ptr_on_device();

  mkl_dcsrmv(&transa,
             &m, &k,
             &s_a,
             matdescra,
             A.values.ptr_on_device(),
             A.graph.entries.ptr_on_device(),
             (int*) &A.graph.row_map(0),
             (int*) &A.graph.row_map(1),
             x_ptr,
             &s_b,
             y_ptr);
}
#endif

#endif /* MKL_SPMV_HPP_ */
