// @HEADER
// ***********************************************************************
//                Copyright message goes here. 
// ***********************************************************************
// @HEADER

/*! \file rcb.cpp
    \brief An example of partitioning coordinates with RCB.
*/

#include <Zoltan2_PartitioningSolution.hpp>
#include <Zoltan2_PartitioningProblem.hpp>
#include <Zoltan2_BasicCoordinateInput.hpp>
#include <Zoltan2_InputTraits.hpp>
#include <vector>
#include <cstdlib>

using namespace std;
using std::vector;

/*! \example rcb.cpp
    An example of the use of the RCB algorithm to partition coordinate data.
*/

// Zoltan2 is templated.  What data types will we use for
// scalars (coordinate values and weights), for local ids, and
// for global ids?
//
// If Zoltan2 was compiled with explicit instantiation, we will
// use the the library's data types.  These macros are defined
// in Zoltan2_config.h.

#ifdef HAVE_ZOLTAN2_INST_FLOAT_INT_LONG
typedef float scalar_t;
typedef int localId_t;
typedef long globalId_t;
#else
  #ifdef HAVE_ZOLTAN2_INST_DOUBLE_INT_LONG
  typedef double scalar_t;
  typedef int localId_t;
  typedef long globalId_t;
  #else
    #ifdef HAVE_ZOLTAN2_INST_FLOAT_INT_INT
    typedef float scalar_t;
    typedef int localId_t;
    typedef int globalId_t;
    #else
      #ifdef HAVE_ZOLTAN2_INST_DOUBLE_INT_INT
      typedef double scalar_t;
      typedef int localId_t;
      typedef int globalId_t;
      #else
      typedef float scalar_t;
      typedef int localId_t;
      typedef int globalId_t;
      #endif
    #endif
  #endif
#endif


int main(int argc, char *argv[])
{
#ifdef HAVE_ZOLTAN2_MPI                   
  MPI_Init(&argc, &argv);
  int rank, nprocs;
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#else
  int rank=0, nprocs=1;
#endif

  // TODO explain
  typedef Zoltan2::BasicUserTypes<scalar_t, globalId_t, localId_t, globalId_t> myTypes;

  // TODO explain
  typedef Zoltan2::BasicCoordinateInput<myTypes> inputAdapter_t;

  ///////////////////////////////////////////////////////////////////////
  // Create input data.

  size_t localCount = 40;
  int dim = 3;

  scalar_t *coords = new scalar_t [dim * localCount];

  scalar_t *x = coords; 
  scalar_t *y = x + localCount; 
  scalar_t *z = y + localCount; 

  // Create coordinates that range from 0 to 10.0

  srand(rank);
  scalar_t scalingFactor = 10.0 / RAND_MAX;

  for (int i=0; i < localCount*dim; i++){
    coords[i] = scalar_t(rand()) * scalingFactor;
  }

  // Create global ids for the coordinates.

  globalId_t *globalIds = new globalId_t [localCount];
  globalId_t offset = rank * localCount;

  for (localId_t i=0; i < localCount; i++)
    globalIds[i] = offset++;
   
  ///////////////////////////////////////////////////////////////////////
  // Create parameters for an RCB problem

  scalar_t tolerance = 1.2;
  if (rank == 0)
    std::cout << "Imbalance tolerance is " << tolerance << std::endl;

  Teuchos::ParameterList params("test params");
  params.set("debug_level", "basic_status");
  params.set("debug_procs", "0");
  params.set("error_check_level", "debug_mode_assertions");

  Teuchos::ParameterList &parParams = params.sublist("partitioning");
  parParams.set("algorithm", "rcb");
  parParams.set("imbalance_tolerance", tolerance);
  parParams.set("num_global_parts", nprocs);

  Teuchos::ParameterList &geoParams = parParams.sublist("geometric");
  geoParams.set("bisection_num_test_cuts", 1);
   
  ///////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////
  // A simple problem with no weights.
  ///////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////

  // Create a Zoltan2 input adapter for this geometry. TODO explain

  inputAdapter_t ia1(localCount, globalIds, x, y, z, 1, 1, 1);

  // Create a Zoltan2 partitioning problem

#ifdef HAVE_ZOLTAN2_MPI                   
  Zoltan2::PartitioningProblem<inputAdapter_t> problem1(&ia1, &params, 
    MPI_COMM_WORLD);
#else
  Zoltan2::PartitioningProblem<inputAdapter_t> problem1(&ia1, &params);
#endif
   
  // Solve the problem

  problem1.solve();
   
  // Obtain the solution

  const Zoltan2::PartitioningSolution<inputAdapter_t> &solution1 = 
    problem1.getSolution();
   
  // Check the solution.

  if (rank == 0)
    solution1.printMetrics(cout);

  if (rank == 0){
    scalar_t imb = solution1.getImbalance();
    if (imb < tolerance)
      std::cout << "PASS: " << imb << std::endl;
    else
      std::cout << "FAIL: " << imb << std::endl;
  }
   
#if 0
  ///////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////
  // Try a problem with weights (1 dimension)
  ///////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////

  scalar_t *weights = new scalar_t [localCount];
  for (int i=0; i < localCount; i++){
    weights[i] = 1.0 + scalar_t(rank) / scalar_t(nprocs);
  }

  // Create a Zoltan2 input adapter that includes weights.

  vector<const scalar_t *>coordVec(2);
  vector<int> coordStrides(2);

  coordVec[0] = x; coordStrides[0] = 1;
  coordVec[1] = y; coordStrides[1] = 1;

  vector<const scalar_t *>weightVec(1);
  vector<int> weightStrides(1);

  weightVec[0] = weights; weightStrides[0] = 1;

  inputAdapter_t ia2(
    localCount, globalIds,  
    coordVec, coordStrides, 
    weightVec, weightStrides);

  // Create a Zoltan2 partitioning problem

#ifdef HAVE_ZOLTAN2_MPI                   
  Zoltan2::PartitioningProblem<inputAdapter_t> problem2(
    &ia2, &params, MPI_COMM_WORLD);
#else
  Zoltan2::PartitioningProblem<inputAdapter_t> problem2(&ia2, &params);
#endif

  // Solve the problem

  problem2.solve();

  // Obtain the solution

  const Zoltan2::PartitioningSolution<inputAdapter_t> &solution2 =
    problem2.getSolution();

  // Check the solution.

  const ArrayRCP<MetricValues<scalar_t> > & metrics2 =
    solution2.getMetrics();

  if (rank == 0)
    Zoltan2::printMetrics<scalar_t>(cout, nprocs, nprocs, nprocs,
      metrics2.view(0,metrics2.size()));

  if (rank == 0){
    scalar_t imb = solution2.getImbalance();
    if (imb < tolerance)
      std::cout << "PASS: " << imb << std::endl;
    else
      std::cout << "FAIL: " << imb << std::endl;
  }

  if (localCount > 0)
    delete [] weights;

  ///////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////
  // Try a problem with multiple weights.
  ///////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////

  // Add to the parameters the multicriteria objective.

  parParams.set("objective", "multicriteria_minimize_total_weight");

  // Create the new weights.

  weights = new scalar_t [localCount*3];
  srand(555);

  for (int i=0; i < localCount*3; i+=3){
    weights[i] = 1.0 + rank / nprocs;      // weight dimension 1
    weights[i+1] = rank<nprocs/2 ? 1 : 2;  // weight dimension 2
    weights[i+2] = rand() +.5;             // weight dimension 3
  }

  // Create a Zoltan2 input adapter with these weights.

  weightVec.resize(3);
  weightStrides.resize(3);

  weightVec[0] = weights;   weightStrides[0] = 3;
  weightVec[1] = weights+1; weightStrides[1] = 3;
  weightVec[2] = weights+2; weightStrides[2] = 3;

  inputAdapter_t ia3(
    localCount, globalIds,  
    coordVec, coordStrides, 
    weightVec, weightStrides);

  // Create a Zoltan2 partitioning problem.

#ifdef HAVE_ZOLTAN2_MPI                   
  Zoltan2::PartitioningProblem<inputAdapter_t> problem3(
    &ia3, &params, MPI_COMM_WORLD);
#else
  Zoltan2::PartitioningProblem<inputAdapter_t> problem3(&ia3, &params);
#endif

  // Solve the problem

  problem3.solve();

  // Obtain the solution

  const Zoltan2::PartitioningSolution<inputAdapter_t> &solution3 =
    problem3.getSolution();

  // Check the solution.

  const ArrayRCP<MetricValues<scalar_t> > & metrics3 =
    solution3.getMetrics();

  if (rank == 0)
    Zoltan2::printMetrics<scalar_t>(cout, nprocs, nprocs, nprocs,
      metrics3.view(0,metrics3.size()));

  if (rank == 0){
    scalar_t imb = solution3.getImbalance();
    if (imb < tolerance)
      std::cout << "PASS: " << imb << std::endl;
    else
      std::cout << "FAIL: " << imb << std::endl;
  }

  if (localCount)
    delete [] weights;

  ///////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////
  // Using part sizes, ask for some parts to be empty.
  ///////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////

  // Change the number of parts to twice the number of processes to
  // ensure that we have more than one global part.

  parParams.set("num_global_parts", nprocs*2);
  parParams.set("num_local_parts", 2);

  // Using the initial problem that did not have any weights, reset
  // parameter list, and give it some part sizes.

  problem1.resetParameterList(&params);

  zoltan2_partId_t *partIds = new zoltan2_partId_t [2];
  scalar_t *partSizes = new scalar_t [2];

  partIds[0] = rank*2;    partSizes[0] = 0;
  partIds[1] = rank*2+1;  partSizes[1] = 1;

  problem1.setPartSizes(2, partIds, partSizes);

  // Solve the problem.  The flag "false" indicates that we
  // have not changed the input data, which allows the problem
  // so skip some work when re-solving.

  problem1.solve(false);

  // Obtain the solution

  const Zoltan2::PartitioningSolution<inputAdapter_t> &solution4 =
    problem1.getSolution();

  // Check the solution.

  const ArrayRCP<MetricValues<scalar_t> > & metrics4 =
    solution4.getMetrics();

  if (rank == 0)
    Zoltan2::printMetrics<scalar_t>(cout, nprocs, nprocs, nprocs,
      metrics4.view(0,metrics4.size()));

  if (rank == 0){
    scalar_t imb = solution4.getImbalance();
    if (imb < tolerance)
      std::cout << "PASS: " << imb << std::endl;
    else
      std::cout << "FAIL: " << imb << std::endl;
  }

  delete [] partIds;
  delete [] partSizes;
#endif

#ifdef HAVE_ZOLTAN2_MPI
  MPI_Finalize();
#endif

}

