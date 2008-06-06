//@HEADER
/*
************************************************************************

              Isorropia: Partitioning and Load Balancing Package
                Copyright (2006) Sandia Corporation

Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
license for use of this work by or on behalf of the U.S. Government.

This library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation; either version 2.1 of the
License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
USA
Questions? Contact Alan Williams (william@sandia.gov)
                or Erik Boman    (egboman@sandia.gov)

************************************************************************
*/
//@HEADER

#ifndef _Isorropia_Epetra_hpp_
#define _Isorropia_Epetra_hpp_

#include <Isorropia_ConfigDefs.hpp>
#include <Teuchos_RefCountPtr.hpp>
#include <Teuchos_ParameterList.hpp>

#ifdef HAVE_EPETRA
class Epetra_Comm;
class Epetra_Map;
class Epetra_BlockMap;
class Epetra_Import;
class Epetra_Vector;
class Epetra_MultiVector;
class Epetra_RowMatrix;
class Epetra_CrsGraph;
class Epetra_CrsMatrix;
class Epetra_LinearProblem;
#endif

namespace Isorropia {

namespace Epetra {

  class Partitioner;
  class CostDescriber;

#ifdef HAVE_EPETRA

/** Given an input matrix-graph that is to be repartitioned, and a parameter-
    list (possibly specifying things such as the partitioning package/method),
    create an instance of Isorropia::Partitioner. This is a factory
    function, the run-time type of the returned Partitioner is
    Isorropia::EpetraPartitioner.

    If Isorropia has been configured with Zoltan support, then Zoltan will
    be used as the underlying partitioner by default. Otherwise, the simple
    linear built-in partitioner will be used.
    To specify that Zoltan be used as the underlying partitioner, create a
    sublist named "Zoltan" in the ParameterList object. Then, in that sublist,
    put any parameters that should be relayed straight to Zoltan.
    Refer to the Zoltan users guide for specific parameters that Zoltan
    recognizes. A couple of important ones are "LB_METHOD" (valid values
    include "GRAPH", "HYPERGRAPH"), "DEBUG_LEVEL" (valid values are
    0 to 10, default is 1), etc.
*/
Teuchos::RefCountPtr<Partitioner>
create_partitioner(Teuchos::RefCountPtr<const Epetra_CrsGraph> input_graph,
		   const Teuchos::ParameterList& paramlist);

/** Given an input matrix-graph that is to be repartitioned, and a parameter-
    list (possibly specifying the partitioning package/method etc.),
    create an instance of Isorropia::Partitioner. This is a factory
    function, the run-time type of the returned Partitioner is
    Isorropia::EpetraPartitioner.

    If Isorropia has been configured with Zoltan support, then Zoltan will
    be used as the underlying partitioner by default. Otherwise, the simple
    linear built-in partitioner will be used.
    To specify that Zoltan be used as the underlying partitioner, create a
    sublist named "Zoltan" in the ParameterList object. Then, in that sublist,
    put any parameters that should be relayed straight to Zoltan.
    Refer to the Zoltan users guide for specific parameters that Zoltan
    recognizes. A couple of important ones are "LB_METHOD" (valid values
    include "GRAPH", "HYPERGRAPH"), "DEBUG_LEVEL" (valid values are
    0 to 10, default is 1), etc.
*/
Teuchos::RefCountPtr<Partitioner>
create_partitioner(Teuchos::RefCountPtr<const Epetra_CrsGraph> input_graph,
		   Teuchos::RefCountPtr<CostDescriber> costs,
		   const Teuchos::ParameterList& paramlist);

/** Given an input row-matrix that is to be repartitioned, and a parameter-
    list (possibly specifying the partitioning package/method etc.),
    create an instance of Isorropia::Partitioner. This is a factory
    function, the run-time type of the returned Partitioner is
    Isorropia::EpetraPartitioner.

    If Isorropia has been configured with Zoltan support, then Zoltan will
    be used as the underlying partitioner by default. Otherwise, the simple
    linear built-in partitioner will be used.
    To specify that Zoltan be used as the underlying partitioner, create a
    sublist named "Zoltan" in the ParameterList object. Then, in that sublist,
    put any parameters that should be relayed straight to Zoltan.
    Refer to the Zoltan users guide for specific parameters that Zoltan
    recognizes. A couple of important ones are "LB_METHOD" (valid values
    include "GRAPH", "HYPERGRAPH"), "DEBUG_LEVEL" (valid values are
    0 to 10, default is 1), etc.
*/
Teuchos::RefCountPtr<Partitioner>
create_partitioner(Teuchos::RefCountPtr<const Epetra_RowMatrix> input_matrix,
		   const Teuchos::ParameterList& paramlist);

/** Given an input row-matrix that is to be repartitioned, and a parameter-
    list (possibly specifying the partitioning package/method etc.),
    create an instance of Isorropia::Partitioner. This is a factory
    function, the run-time type of the returned Partitioner is
    Isorropia::EpetraPartitioner.

    If Isorropia has been configured with Zoltan support, then Zoltan will
    be used as the underlying partitioner by default. Otherwise, the simple
    linear built-in partitioner will be used.
    To specify that Zoltan be used as the underlying partitioner, create a
    sublist named "Zoltan" in the ParameterList object. Then, in that sublist,
    put any parameters that should be relayed straight to Zoltan.
    Refer to the Zoltan users guide for specific parameters that Zoltan
    recognizes. A couple of important ones are "LB_METHOD" (valid values
    include "GRAPH", "HYPERGRAPH"), "DEBUG_LEVEL" (valid values are
    0 to 10, default is 1), etc.
*/
Teuchos::RefCountPtr<Partitioner>
create_partitioner(Teuchos::RefCountPtr<const Epetra_RowMatrix> input_matrix,
		   Teuchos::RefCountPtr<CostDescriber> costs,
		   const Teuchos::ParameterList& paramlist);

/** Given a Partitioner object, create a target map representing the
   new partitioning.
   This function calls partitioner.compute_partitioning() if it has not
   already been called.
*/
Teuchos::RefCountPtr<Epetra_Map>
create_target_map(const Epetra_Comm& comm, Partitioner& partitioner);

/** Create a balanced copy of an input Epetra_CrsMatrix.

  Isorropia will use Zoltan to perform partitioning if it has been
  configured with Zoltan support.  If Zoltan is not available, it will
  perform its own simple row partitioning.

  Zoltan will perform hypergraph partitioning using its own PHG method.  
  It will assign unit weight to each row (vertex), and unit weight to each 
  column (hypergraph).  It will attempt to balance row weights while 
  minimizing cuts in the hyperedges.

  The non-Zoltan rebalancing is 1-D, row-wise, and attempts to make the number
  of nonzeros equal in each partition. I.e., it is equivalent to specifying
  a weighted rebalance where the weights are the number of nonzeros in
  each row.

*/
Teuchos::RefCountPtr<Epetra_CrsMatrix>
  create_balanced_copy(const Epetra_CrsMatrix& input_matrix);

/** Create a balanced copy of an input Epetra_CrsMatrix.

  Isorropia will use Zoltan to perform partitioning if it has been
  configured with Zoltan support.  If Zoltan is not available, it will
  perform its own simple row partitioning.

  The balancing algorithm (Zoltan or non-Zoltan) will use the row
  weights provided for the matrix rows.

  Zoltan will perform hypergraph partitioning using its own PHG method.  
  It will assign a unit weight to each column (hypergraph).  It will attempt 
  to balance row weights while minimizing cuts in the hyperedges.

  The non-Zoltan rebalancing is a quick 1-D, row-wise balancing.
*/
Teuchos::RefCountPtr<Epetra_CrsMatrix>
  create_balanced_copy(const Epetra_CrsMatrix& input_matrix,
                       const Epetra_Vector &row_weights);

/** Create a balanced copy of an input Epetra_CrsMatrix.

  Isorropia can use Zoltan to perform partitioning if it has been
  configured with Zoltan support.  If Zoltan is not available, it will
  perform its own simple row partitioning.

  In addition, if the parameter "PARTITIONING_METHOD" is set to "SIMPLE_LINEAR",
  Isorropia will perform its own simple row partitioning, even if Zoltan is
  available.  This method balances the rows across processes after assigning 
  each row a weight equal to the number of non zeros it has.

  If Zoltan is called to do the partitioning, any parameters set in a
  "Zoltan" sublist of the paramlist are provided to Zoltan.

  Refer to the Zoltan users guide for specific parameters that Zoltan
  recognizes. A couple of important ones are "LB_METHOD" (valid values
  include "GRAPH", "HYPERGRAPH"), "DEBUG_LEVEL" (valid values are
  0 to 10, default is 1), etc.

  If no Zoltan parameters are provided, Zoltan will perform hypergraph
  partitioning using its own PHG method.  It will assign unit weight to
  each row (vertex), and unit weight to each column (hypergraph).  It will
  attempt to minimize cuts in the hyperedges.
*/
Teuchos::RefCountPtr<Epetra_CrsMatrix>
  create_balanced_copy(const Epetra_CrsMatrix& input_matrix,
                     const Teuchos::ParameterList& paramlist);


/** Create a balanced copy of an input Epetra_CrsMatrix.

  Isorropia can use Zoltan to perform partitioning if it has been
  configured with Zoltan support.  If Zoltan is not available, it will
  perform its own simple row partitioning.

  In addition, if the parameter "PARTITIONING_METHOD" is set to "SIMPLE_LINEAR",
  Isorropia will perform its own simple row partitioning, even if Zoltan is
  available.  This method balances the rows across processes after assigning 
  each row a weight equal to the number of non zeros it has.  It ignores the
  costs object.

  If Zoltan is called to do the partitioning, any parameters set in a
  "Zoltan" sublist of the paramlist are provided to Zoltan.  The weights
  provided in the CostDescriber object will be supplied to Zoltan.

  Refer to the Zoltan users guide for specific parameters that Zoltan
  recognizes. A couple of important ones are "LB_METHOD" (valid values
  include "GRAPH", "HYPERGRAPH"), "DEBUG_LEVEL" (valid values are
  0 to 10, default is 1), etc.

  If no Zoltan parameters are provided, Zoltan will perform hypergraph
  partitioning using its own PHG method.  

  If an empty CostDescriber is supplied, we will assign unit weight to each 
  row (vertex), and unit weight to each column (hypergraph), in the case of 
  hypergraph partitioning.  We will assign unit weight to each row (vertex)
  and each non zero (edge) in the case of graph partitioning.
*/
Teuchos::RefCountPtr<Epetra_CrsMatrix>
  create_balanced_copy(const Epetra_CrsMatrix& input_matrix,
                     CostDescriber &costs,
                     const Teuchos::ParameterList& paramlist);

/** Create a balanced copy of an input Epetra_RowMatrix.

  Isorropia will use Zoltan to perform partitioning if it has been
  configured with Zoltan support.  If Zoltan is not available, it will
  perform its own simple row partitioning.

  Zoltan will perform hypergraph partitioning using its own PHG method.  
  It will assign unit weight to each row (vertex), and unit weight to each 
  column (hypergraph).  It will attempt to minimize cuts in the hyperedges.

  The non-Zoltan rebalancing is 1-D, row-wise, and attempts to make the number
  of nonzeros equal in each partition. I.e., it is equivalent to specifying
  a weighted rebalance where the weights are the number of nonzeros in
  each row.

*/
Teuchos::RefCountPtr<Epetra_RowMatrix>
  create_balanced_copy(const Epetra_RowMatrix& input_matrix);

/** Create a balanced copy of an input Epetra_RowMatrix.

  Isorropia will use Zoltan to perform partitioning if it has been
  configured with Zoltan support.  If Zoltan is not available, it will
  perform its own simple row partitioning.

  The balancing algorithm (Zoltan or non-Zoltan) will use the row
  weights provided for the matrix rows.

  Zoltan will perform hypergraph partitioning using its own PHG method.  
  It will assign a unit weight to each column (hypergraph).  It will attempt 
  to balance row weights while minimizing cuts in the hyperedges.

  The non-Zoltan rebalancing is a quick 1-D, row-wise balancing.
*/
Teuchos::RefCountPtr<Epetra_RowMatrix>
  create_balanced_copy(const Epetra_RowMatrix& input_matrix,
                       const Epetra_Vector &row_weights);

/** Create a balanced copy of an input Epetra_RowMatrix.

  Isorropia can use Zoltan to perform partitioning if it has been
  configured with Zoltan support.  If Zoltan is not available, it will
  perform its own simple row partitioning.

  In addition, if the parameter "PARTITIONING_METHOD" is set to "SIMPLE_LINEAR",
  Isorropia will perform its own simple row partitioning, even if Zoltan is
  available.  This method balances the rows across processes after assigning 
  each row a weight equal to the number of non zeros it has.

  If Zoltan is called to do the partitioning, any parameters set in a
  "Zoltan" sublist of the paramlist are provided to Zoltan.

  Refer to the Zoltan users guide for specific parameters that Zoltan
  recognizes. A couple of important ones are "LB_METHOD" (valid values
  include "GRAPH", "HYPERGRAPH"), "DEBUG_LEVEL" (valid values are
  0 to 10, default is 1), etc.

  If no Zoltan parameters are provided, Zoltan will perform hypergraph
  partitioning using its own PHG method.  It will assign unit weight to
  each row (vertex), and unit weight to each column (hypergraph).  It will
  attempt to minimize cuts in the hyperedges.
*/
Teuchos::RefCountPtr<Epetra_RowMatrix>
  create_balanced_copy(const Epetra_RowMatrix& input_matrix,
                     const Teuchos::ParameterList& paramlist);

/** Create a balanced copy of an input Epetra_RowMatrix.

  Isorropia can use Zoltan to perform partitioning if it has been
  configured with Zoltan support.  If Zoltan is not available, it will
  perform its own simple row partitioning.

  In addition, if the parameter "PARTITIONING_METHOD" is set to "SIMPLE_LINEAR",
  Isorropia will perform its own simple row partitioning, even if Zoltan is
  available.  This method balances the rows across processes after assigning 
  each row a weight equal to the number of non zeros it has.  The costs
  object is ignored in this case.

  If Zoltan is called to do the partitioning, any parameters set in a
  "Zoltan" sublist of the paramlist are provided to Zoltan.  The weights
  provided in the CostDescriber object will be supplied to Zoltan.

  Refer to the Zoltan users guide for specific parameters that Zoltan
  recognizes. A couple of important ones are "LB_METHOD" (valid values
  include "GRAPH", "HYPERGRAPH"), "DEBUG_LEVEL" (valid values are
  0 to 10, default is 1), etc.

  If no Zoltan parameters are provided, Zoltan will perform hypergraph
  partitioning using its own PHG method.  

  If an empty CostDescriber is supplied, we will assign unit weight to each 
  row (vertex), and unit weight to each column (hypergraph), in the case of 
  hypergraph partitioning.  We will assign unit weight to each row (vertex)
  and each non zero (edge) in the case of graph partitioning.
*/
Teuchos::RefCountPtr<Epetra_RowMatrix>
  create_balanced_copy(const Epetra_RowMatrix& input_matrix,
                     CostDescriber &costs,
                     const Teuchos::ParameterList& paramlist);

/** Create a balanced copy of an input Epetra_Graph.

  Isorropia will use Zoltan to perform partitioning if it has been
  configured with Zoltan support.  If Zoltan is not available, it will
  perform its own simple row partitioning.

  Zoltan will perform hypergraph partitioning using its own PHG method.  
  It will assign unit weight to each row (vertex), and unit weight to each 
  column (hypergraph).  It will attempt to minimize cuts in the hyperedges.

  The non-Zoltan rebalancing is 1-D, row-wise, and attempts to make the number
  of nonzeros equal in each partition. I.e., it is equivalent to specifying
  a weighted rebalance where the weights are the number of nonzeros in
  each row.

*/
Teuchos::RefCountPtr<Epetra_CrsGraph>
  create_balanced_copy(const Epetra_CrsGraph& input_graph);

/** Create a balanced copy of an input Epetra_CrsGraph.

  Isorropia will use Zoltan to perform partitioning if it has been
  configured with Zoltan support.  If Zoltan is not available, it will
  perform its own simple row partitioning.

  The balancing algorithm (Zoltan or non-Zoltan) will use the row
  weights provided for the matrix rows.

  Zoltan will perform hypergraph partitioning using its own PHG method.  
  It will assign a unit weight to each column (hypergraph).  It will attempt 
  to balance row weights while minimizing cuts in the hyperedges.

  The non-Zoltan rebalancing is a quick 1-D, row-wise balancing.
*/
Teuchos::RefCountPtr<Epetra_CrsGraph>
  create_balanced_copy(const Epetra_CrsGraph& input_matrix,
                       const Epetra_Vector &row_weights);

/** Create a balanced copy of an input Epetra_CrsGraph.

  Isorropia can use Zoltan to perform partitioning if it has been
  configured with Zoltan support.  If Zoltan is not available, it will
  perform its own simple row partitioning.

  In addition, if the parameter "PARTITIONING_METHOD" is set to "SIMPLE_LINEAR",
  Isorropia will perform its own simple row partitioning, even if Zoltan is
  available.  This method balances the rows across processes after assigning 
  each row a weight equal to the number of non zeros it has.

  If Zoltan is called to do the partitioning, any parameters set in a
  "Zoltan" sublist of the paramlist are provided to Zoltan.

  Refer to the Zoltan users guide for specific parameters that Zoltan
  recognizes. A couple of important ones are "LB_METHOD" (valid values
  include "GRAPH", "HYPERGRAPH"), "DEBUG_LEVEL" (valid values are
  0 to 10, default is 1), etc.

  If no Zoltan parameters are provided, Zoltan will perform hypergraph
  partitioning using its own PHG method.  It will assign unit weight to
  each row (vertex), and unit weight to each column (hypergraph).  It will
  attempt to minimize cuts in the hyperedges.
*/
Teuchos::RefCountPtr<Epetra_CrsGraph>
  create_balanced_copy(const Epetra_CrsGraph& input_graph,
                     const Teuchos::ParameterList& paramlist);

/** Create a balanced copy of an input Epetra_CrsGraph.

  Isorropia can use Zoltan to perform partitioning if it has been
  configured with Zoltan support.  If Zoltan is not available, it will
  perform its own simple row partitioning.

  In addition, if the parameter "PARTITIONING_METHOD" is set to "SIMPLE_LINEAR",
  Isorropia will perform its own simple row partitioning, even if Zoltan is
  available.  This method balances the rows across processes after assigning 
  each row a weight equal to the number of non zeros it has.

  If Zoltan is called to do the partitioning, any parameters set in a
  "Zoltan" sublist of the paramlist are provided to Zoltan.  The weights
  provided in the CostDescriber object will be supplied to Zoltan.

  Refer to the Zoltan users guide for specific parameters that Zoltan
  recognizes. A couple of important ones are "LB_METHOD" (valid values
  include "GRAPH", "HYPERGRAPH"), "DEBUG_LEVEL" (valid values are
  0 to 10, default is 1), etc.

  If no Zoltan parameters are provided, Zoltan will perform hypergraph
  partitioning using its own PHG method.  

  If an empty CostDescriber is supplied, we will assign unit weight to each 
  row (vertex), and unit weight to each column (hypergraph), in the case of 
  hypergraph partitioning.  We will assign unit weight to each row (vertex)
  and each non zero (edge) in the case of graph partitioning.
*/
Teuchos::RefCountPtr<Epetra_CrsGraph>
  create_balanced_copy(const Epetra_CrsGraph& input_graph,
                     CostDescriber &costs,
                     const Teuchos::ParameterList& paramlist);

/** Create a balanced copy of an input Epetra_LinearProblem.

  Isorropia will use Zoltan to perform partitioning if it has been
  configured with Zoltan support.  If Zoltan is not available, it will
  perform its own simple row partitioning.

  Zoltan will perform hypergraph partitioning using its own PHG method.  
  It will assign unit weight to each row (vertex), and unit weight to each 
  column (hypergraph).  It will attempt to minimize cuts in the hyperedges.

  The non-Zoltan rebalancing is 1-D, row-wise, and attempts to make the number
  of nonzeros equal in each partition. I.e., it is equivalent to specifying
  a weighted rebalance where the weights are the number of nonzeros in
  each row.

*/

Teuchos::RefCountPtr<Epetra_LinearProblem>
  create_balanced_copy(const Epetra_LinearProblem & input_problem);

/** Create a balanced copy of an input Epetra_LinearProblem.

  Isorropia will use Zoltan to perform partitioning if it has been
  configured with Zoltan support.  If Zoltan is not available, it will
  perform its own simple row partitioning.

  The balancing algorithm (Zoltan or non-Zoltan) will use the row
  weights provided for the matrix rows.

  Zoltan will perform hypergraph partitioning using its own PHG method.  
  It will assign a unit weight to each column (hypergraph).  It will attempt 
  to balance row weights while minimizing cuts in the hyperedges.

  The non-Zoltan rebalancing is a quick 1-D, row-wise balancing.
*/
Teuchos::RefCountPtr<Epetra_LinearProblem>
  create_balanced_copy(const Epetra_LinearProblem& input_matrix,
                       const Epetra_Vector &row_weights);

/** Create a balanced copy of an input Epetra_LinearProblem.

  Isorropia can use Zoltan to perform partitioning if it has been
  configured with Zoltan support.  If Zoltan is not available, it will
  perform its own simple row partitioning.

  In addition, if the parameter "PARTITIONING_METHOD" is set to "SIMPLE_LINEAR",
  Isorropia will perform its own simple row partitioning, even if Zoltan is
  available.  This method balances the rows across processes after assigning 
  each row a weight equal to the number of non zeros it has.

  If Zoltan is called to do the partitioning, any parameters set in a
  "Zoltan" sublist of the paramlist are provided to Zoltan.

  Refer to the Zoltan users guide for specific parameters that Zoltan
  recognizes. A couple of important ones are "LB_METHOD" (valid values
  include "GRAPH", "HYPERGRAPH"), "DEBUG_LEVEL" (valid values are
  0 to 10, default is 1), etc.

  If no Zoltan parameters are provided, Zoltan will perform hypergraph
  partitioning using its own PHG method.  It will assign unit weight to
  each row (vertex), and unit weight to each column (hypergraph).  It will
  attempt to minimize cuts in the hyperedges.
*/

Teuchos::RefCountPtr<Epetra_LinearProblem>
  create_balanced_copy(const Epetra_LinearProblem& input_problem,
                     const Teuchos::ParameterList& paramlist);

/** Create a balanced copy of an input Epetra_LinearProblem.

  Isorropia can use Zoltan to perform partitioning if it has been
  configured with Zoltan support.  If Zoltan is not available, it will
  perform its own simple row partitioning.

  In addition, if the parameter "PARTITIONING_METHOD" is set to "SIMPLE_LINEAR",
  Isorropia will perform its own simple row partitioning, even if Zoltan is
  available.  This method balances the rows across processes after assigning 
  each row a weight equal to the number of non zeros it has.  This costs
  object is ignored in this case.

  If Zoltan is called to do the partitioning, any parameters set in a
  "Zoltan" sublist of the paramlist are provided to Zoltan.  The weights
  provided in the CostDescriber object will be supplied to Zoltan.

  Refer to the Zoltan users guide for specific parameters that Zoltan
  recognizes. A couple of important ones are "LB_METHOD" (valid values
  include "GRAPH", "HYPERGRAPH"), "DEBUG_LEVEL" (valid values are
  0 to 10, default is 1), etc.

  If no Zoltan parameters are provided, Zoltan will perform hypergraph
  partitioning using its own PHG method.  

  If an empty CostDescriber is supplied, we will assign unit weight to each 
  row (vertex), and unit weight to each column (hypergraph), in the case of 
  hypergraph partitioning.  We will assign unit weight to each row (vertex)
  and each non zero (edge) in the case of graph partitioning.
*/
Teuchos::RefCountPtr<Epetra_LinearProblem>
  create_balanced_copy(const Epetra_LinearProblem& input_problem,
                     CostDescriber &costs,
                     const Teuchos::ParameterList& paramlist);


/** Return a new Epetra_CrsMatrix object constructed with target_rowmap,
  and with the contents of input_matrix imported into it.

  The caller is responsible for deleting the returned object.

  param input_matrix Source/input object.

  param target_rowmap Target rowmap, required to be compatible with
     input_matrix.RowMap() in terms of number-of-elements, etc.

  param importer Optional argument. If importer is supplied, it will be
     used to perform the import operation. Otherwise, a temporary importer
     will be created and used.
*/
Teuchos::RefCountPtr<Epetra_CrsMatrix>
  redistribute_rows(const Epetra_CrsMatrix& input_matrix,
                    const Epetra_Map& target_rowmap,
                    Epetra_Import* importer=0);

/** Return a new Epetra_CrsMatrix object constructed with target_rowmap,
  and with the contents of input_matrix imported into it.

  The caller is responsible for deleting the returned object.

  param input_matrix Source/input object.

  param target_rowmap Target rowmap, required to be compatible with
     input_matrix.RowMatrixRowMap() in terms of number-of-elements, etc.

  param importer Optional argument. If importer is supplied, it will be
     used to perform the import operation. Otherwise, a temporary importer
     will be created and used.
*/
Teuchos::RefCountPtr<Epetra_CrsMatrix>
  redistribute_rows(const Epetra_RowMatrix& input_matrix,
                    const Epetra_Map& target_rowmap,
                    Epetra_Import* importer=0);

/** Return a new Epetra_CrsGraph object constructed with target_rowmap,
  and with the contents of input_graph imported into it.

  param input_graph Source/input object.

  param target_rowmap Target rowmap, required to be compatible with
     input_graph.RowMap() in terms of number-of-elements, etc.

  param importer Optional argument. If importer is supplied, it will be
     used to perform the import operation. Otherwise, a temporary importer
     will be created and used.
*/
Teuchos::RefCountPtr<Epetra_CrsGraph>
  redistribute_rows(const Epetra_CrsGraph& input_graph,
                    const Epetra_Map& target_rowmap,
                    Epetra_Import* importer=0);

/** Return a new Epetra_MultiVector object constructed with target_map,
  and with the contents of 'input' imported into it.

  param input Source/input object.

  param target_map Target map, required to be compatible with
     input.Map() in terms of number-of-elements, etc.

  param importer Optional argument. If importer is supplied, it will be
     used to perform the import operation. Otherwise, a temporary importer
     will be created and used.
*/
Teuchos::RefCountPtr<Epetra_MultiVector>
  redistribute(const Epetra_MultiVector& input,
               const Epetra_BlockMap& target_map,
               Epetra_Import* importer=0);

/** Return a new Epetra_Vector object constructed with target_map,
  and with the contents of 'input' imported into it.

  param input Source/input object.

  param target_map Target map, required to be compatible with
     input.RowMap() in terms of number-of-elements, etc.

  param importer Optional argument. If importer is supplied, it will be
     used to perform the import operation. Otherwise, a temporary importer
     will be created and used.
*/
Teuchos::RefCountPtr<Epetra_Vector>
  redistribute(const Epetra_Vector& input,
               const Epetra_Map& target_map,
               Epetra_Import* importer=0);

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/** Return a vector containing weights that are equal to the number of
  nonzeros per row in the input_matrix. The returned vector will have
  the same size and distribution as input_matrix's row-map.
*/
Epetra_Vector* create_row_weights_nnz(const Epetra_RowMatrix& input_matrix);

/** Return a vector containing weights that are equal to the number of
  nonzeros per row in the input_graph. The returned vector will have
  the same size and distribution as input_graph's row-map.
*/
Epetra_Vector* create_row_weights_nnz(const Epetra_CrsGraph& input_graph);

/** Calculate a new partitioning, and fill output containers with new
    elements for the local partition, as well as export and import lists.
    This is a simple linear partitioning that does not use Zoltan.

    \param input_map Input map describing the existing or 'old' partitioning.

    \param weights Input vector giving a weight for each element in input_map.
    weights.Map() is required to be the same size and layout as input_map.

    \param myNewElements Output vector containing all elements that will
    reside on the local partition in the new partitioning.

    \param exports Output map contains set of export elements, and maps them
    to the processors that they are to be exported to.

    \param imports Output map contains set of import elements, and maps them
    to the processors that they are to be imported from.

    \return Error-code, 0 if successful. This probably should be a void
    function, since a serious error will result in an exception-throw
    rather than an integer-code error-return.
*/
int
repartition(const Epetra_BlockMap& input_map,
	    const Epetra_Vector& weights,
            std::vector<int>& myNewElements,
            std::map<int,int>& exports,
            std::map<int,int>& imports);

/** Given an Epetra_BlockMap object, fill a vector of length numprocs+1
  with each processor's starting offset into the Epetra_BlockMap's global
  set of elements (the last position will contain num-global-elements).
  Gather the vector of offsets onto all processors.
*/
void gather_all_proc_global_offsets(const Epetra_BlockMap& blkmap,
                                    std::vector<int>& all_proc_offsets);


/** to do
 */
double compute_imbalance(int nprocs, std::vector<int> &offsets, 
                         double *wgts, double target);

#endif //DOXYGEN_SHOULD_SKIP_THIS
#endif //HAVE_EPETRA

}//namespace Epetra
}//namespace Isorropia

#endif

