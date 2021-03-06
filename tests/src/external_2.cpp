/* =========================================================================
   Copyright (c) 2010-2013, Institute for Microelectronics,
                            Institute for Analysis and Scientific Computing,
                            TU Wien.
   Portions of this software are copyright by UChicago Argonne, LLC.

                            -----------------
                  ViennaCL - The Vienna Computing Library
                            -----------------

   Project Head:    Karl Rupp                   rupp@iue.tuwien.ac.at

   (A list of authors and contributors can be found in the PDF manual)

   License:         MIT (X11), see file LICENSE in the base directory
============================================================================= */

//
// A check for the absence of external linkage (otherwise, library is not truly 'header-only')
//


//#define VIENNACL_WITH_EIGEN
#define VIENNACL_WITH_UBLAS

//
// *** System
//
#include <iostream>

//
// *** ViennaCL
//
#include "viennacl/scalar.hpp"
#include "viennacl/vector.hpp"
#include "viennacl/matrix.hpp"
#include "viennacl/compressed_matrix.hpp"
#include "viennacl/coordinate_matrix.hpp"
#include "viennacl/ell_matrix.hpp"
#include "viennacl/hyb_matrix.hpp"
#ifdef VIENNACL_WITH_OPENCL
  #include "viennacl/circulant_matrix.hpp"
  #include "viennacl/hankel_matrix.hpp"
  #include "viennacl/toeplitz_matrix.hpp"
  #include "viennacl/vandermonde_matrix.hpp"
#endif

#include "viennacl/linalg/ilu.hpp"
#include "viennacl/linalg/row_scaling.hpp"
#include "viennacl/linalg/jacobi_precond.hpp"
#include "viennacl/linalg/cg.hpp"
#include "viennacl/linalg/bicgstab.hpp"
#include "viennacl/linalg/gmres.hpp"
#include "viennacl/linalg/direct_solve.hpp"
#include "viennacl/linalg/qr.hpp"

#include "viennacl/misc/bandwidth_reduction.hpp"

#ifdef VIENNACL_WITH_OPENCL
  #include "viennacl/linalg/amg.hpp"
  #include "viennacl/linalg/spai.hpp"
  #include "viennacl/linalg/svd.hpp"
  #include "viennacl/fft.hpp"
  #include "viennacl/io/kernel_parameters.hpp"
  #include "viennacl/generator/generate.hpp"
#endif

#include "viennacl/io/matrix_market.hpp"
#include "viennacl/scheduler/execute.hpp"

void other_func()
{
  typedef float   NumericType;

  //doing nothing but instantiating a few types
  viennacl::scalar<NumericType>  s;
  viennacl::vector<NumericType>  v(10);
  viennacl::matrix<NumericType>  m(10, 10);
  viennacl::compressed_matrix<NumericType>  compr(10, 10);
  viennacl::coordinate_matrix<NumericType>  coord(10, 10);

}
