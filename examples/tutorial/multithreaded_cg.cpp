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

/*
*
*   Tutorial: Using ViennaCL with multiple threads for a conjugate gradient solver, one thread per GPU
*
*/

#ifndef VIENNACL_WITH_OPENCL
  #define VIENNACL_WITH_OPENCL
#endif

// include necessary system headers
#include <iostream>

//
// ublas includes
//
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/operation.hpp>
#include <boost/numeric/ublas/operation_sparse.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>

// Must be set if you want to use ViennaCL algorithms on ublas objects
#define VIENNACL_WITH_UBLAS 1


//include basic scalar and vector types of ViennaCL
#include "viennacl/scalar.hpp"
#include "viennacl/vector.hpp"
#include "viennacl/compressed_matrix.hpp"
#include "viennacl/io/matrix_market.hpp"

#include "viennacl/ocl/device.hpp"
#include "viennacl/ocl/platform.hpp"
#include "viennacl/ocl/backend.hpp"

//include the generic inner product functions of ViennaCL
#include "viennacl/linalg/norm_2.hpp"
#include "viennacl/linalg/cg.hpp"

// Some helper functions for this tutorial:
#include "Random.hpp"
#include "vector-io.hpp"

using namespace boost::numeric;

#include <boost/thread.hpp>

template <typename T>
void thread_func(std::string * message, std::size_t thread_id) //Note: using references instead of pointers leads to some troubles with boost.thread
{
  //
  // Set up some ublas objects
  //
  ublas::vector<T> rhs;
  ublas::vector<T> ref_result;
  ublas::compressed_matrix<T> ublas_matrix;

  //
  // Read system from file
  //
  if (!viennacl::io::read_matrix_market_file(ublas_matrix, "../examples/testdata/mat65k.mtx"))
  {
    std::cout << "Error reading Matrix file" << std::endl;
    return;
  }

  if (!readVectorFromFile("../examples/testdata/rhs65025.txt", rhs))
  {
    std::cout << "Error reading RHS file" << std::endl;
    return;
  }

  if (!readVectorFromFile("../examples/testdata/result65025.txt", ref_result))
  {
    std::cout << "Error reading Result file" << std::endl;
    return;
  }

  //
  // Set up some ViennaCL objects in the respective context
  //
  viennacl::context ctx(viennacl::ocl::get_context(thread_id));

  std::size_t vcl_size = rhs.size();
  viennacl::compressed_matrix<T> vcl_compressed_matrix(ctx);
  viennacl::vector<T> vcl_rhs(vcl_size, ctx);
  viennacl::vector<T> vcl_ref_result(vcl_size, ctx);

  viennacl::copy(rhs.begin(), rhs.end(), vcl_rhs.begin());
  viennacl::copy(ref_result.begin(), ref_result.end(), vcl_ref_result.begin());


  //
  // Transfer ublas-matrix to GPU:
  //
  viennacl::copy(ublas_matrix, vcl_compressed_matrix);

  viennacl::vector<T> vcl_result = viennacl::linalg::solve(vcl_compressed_matrix, vcl_rhs, viennacl::linalg::cg_tag());

  std::stringstream ss;
  ss << "Result of thread " << thread_id << " on device " << viennacl::ocl::get_context(thread_id).devices()[0].name() << ": " << vcl_result[0] << ", should: " << ref_result[0] << std::endl;
  *message = ss.str();
}


int main()
{
  //Change this type definition to double if your gpu supports that
  typedef float       ScalarType;

  if (viennacl::ocl::get_platforms().size() == 0)
  {
    std::cerr << "Error: No platform found!" << std::endl;
    return EXIT_FAILURE;
  }

  //
  // Part 1: Setup first device for first context, second device for second context:
  //
  viennacl::ocl::platform pf = viennacl::ocl::get_platforms()[0];
  std::vector<viennacl::ocl::device> const & devices = pf.devices();

  // Set first device to first context:
  viennacl::ocl::setup_context(0, devices[0]);

  // Set second device for second context (use the same device for the second context if only one device available):
  if (devices.size() > 1)
    viennacl::ocl::setup_context(1, devices[1]);
  else
    viennacl::ocl::setup_context(1, devices[0]);

  //
  // Part 2: Now let two threads operate on two GPUs in parallel
  //

  std::string message0;
  std::string message1;
  boost::thread worker_0(thread_func<ScalarType>, &message0, 0);
  boost::thread worker_1(thread_func<ScalarType>, &message1, 1);

  worker_0.join();
  worker_1.join();

  std::cout << message0 << std::endl;
  std::cout << message1 << std::endl;

  std::cout << "!!!! TUTORIAL COMPLETED SUCCESSFULLY !!!!" << std::endl;

  return EXIT_SUCCESS;
}

