// @HEADER
// ************************************************************************
//
//                           Intrepid Package
//                 Copyright (2007) Sandia Corporation
//
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
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
// Questions? Contact Kyungjoo Kim  (kyukim@sandia.gov), or
//                    Mauro Perego  (mperego@sandia.gov)
//
// ************************************************************************
// @HEADER

/** \file   Intrepid_ArrayToolsDefCloneScale.hpp
    \brief  Definition file for clone / scale operations of the array tools interface.
    \author Created by P. Bochev and D. Ridzal.
            Kokkorized by Kyungjoo Kim
*/

#ifndef __INTREPID2_ARRAYTOOLS_DEF_CLONESCALE_HPP__
#define __INTREPID2_ARRAYTOOLS_DEF_CLONESCALE_HPP__

namespace Intrepid2 {
  
  namespace FunctorArrayTools {
    template <typename outputFieldViewType, 
              typename inputFieldViewType>
    struct F_cloneFields{
      outputFieldViewType _outputFields;
      inputFieldViewType _inputFields;

      KOKKOS_INLINE_FUNCTION
      F_cloneFields(outputFieldViewType outputFields_,
                    inputFieldViewType inputFields_)
        : _outputFields(outputFields_), _inputFields(inputFields_) {}

      KOKKOS_INLINE_FUNCTION
      void operator()(const ordinal_type cl,
                      const ordinal_type bf,
                      const ordinal_type pt) const {
        const ordinal_type 
          iend = _outputFields.dimension(3),
          jend = _outputFields.dimension(4);

        for (ordinal_type i=0;i<iend;++i)
          for (ordinal_type j=0;j<jend;++j)
            _outputFields(cl, bf, pt, i, j) = _inputFields(bf, pt, i, j);        
      }
    };
  } //namespace

  template<typename SpT>
  template<typename outputFieldValueType, class ...outputFieldProperties,
           typename inputFieldValueType,  class ...inputFieldProperties>
  void ArrayTools<SpT>::
  cloneFields( /**/  Kokkos::DynRankView<outputFieldValueType,outputFieldProperties...> outputFields,
               const Kokkos::DynRankView<inputFieldValueType, inputFieldProperties...>  inputFields ) {

    //static_assert
    // - two memory space of input and output fields should be matched

#ifdef HAVE_INTREPID2_DEBUG
    {
      INTREPID2_TEST_FOR_EXCEPTION( ( inputFields.rank() < 2 || inputFields.rank() > 4 ), std::invalid_argument,
                                      ">>> ERROR (ArrayTools::cloneFields): Input fields container must have rank 2, 3, or 4.");
      INTREPID2_TEST_FOR_EXCEPTION( ( outputFields.rank() != (inputFields.rank()+1) ), std::invalid_argument,
                                      ">>> ERROR (ArrayTools::cloneFields): The rank of the input fields container must be one less than the rank of the output fields container.");
      for (size_type i=0;i<inputFields.rank();++i) {
        INTREPID2_TEST_FOR_EXCEPTION( (inputFields.dimension(i) != outputFields.dimension(i+1)), std::invalid_argument,
                                        ">>> ERROR (ArrayTools::cloneFields): Dimensions of input and output fields containers do not match.");
      }
    }
#endif

    typedef Kokkos::DynRankView<outputFieldValueType,outputFieldProperties...> outputFieldViewType;
    typedef Kokkos::DynRankView<inputFieldValueType, inputFieldProperties...>  inputFieldViewType; 
    typedef typename ExecSpace< typename inputFieldViewType::execution_space , SpT >::ExecSpaceType ExecSpaceType;

    typedef FunctorArrayTools::F_cloneFields<outputFieldViewType, inputFieldViewType> FunctorType; 
    using range_policy_type = Kokkos::Experimental::MDRangePolicy
      < ExecSpaceType, Kokkos::Experimental::Rank<3>, Kokkos::IndexType<ordinal_type> >;
    range_policy_type policy( { 0, 0, 0 }, 
                              { outputFields.dimension(0), outputFields.dimension(1), outputFields.dimension(2) } );
    Kokkos::Experimental::md_parallel_for( policy, FunctorType(outputFields, inputFields) );
  }
  
  namespace FunctorArrayTools {
    template < typename outputFieldViewType , typename inputFactorsViewType , typename inputFieldViewType >
    struct F_cloneScaleFields {
      outputFieldViewType _outputFields;
      inputFactorsViewType _inputFactors;
      inputFieldViewType _inputFields;
      
      KOKKOS_INLINE_FUNCTION
      F_cloneScaleFields(outputFieldViewType outputFields_,
                         inputFactorsViewType inputFactors_,
                         inputFieldViewType inputFields_)
        : _outputFields(outputFields_), _inputFactors(inputFactors_), _inputFields(inputFields_) {}
      
      KOKKOS_INLINE_FUNCTION
      void operator()(const ordinal_type cl,
                      const ordinal_type bf,
                      const ordinal_type pt) const {
        const ordinal_type iend  = _outputFields.dimension(3);
        const ordinal_type jend  = _outputFields.dimension(4);
        
        const auto val = _inputFactors( cl, bf );
        for (ordinal_type i=0;i<iend;++i)
          for (ordinal_type j=0;j<jend;++j)
            _outputFields( cl, bf, pt, i, j) = _inputFields(bf, pt, i, j)*val;
      }
    };
  } //namespace
  
  template<typename SpT>
  template<typename outputFieldValueType, class ...outputFieldProperties,
           typename inputFactorValueType, class ...inputFactorProperties,
           typename inputFieldValueType,  class ...inputFieldProperties>

  void ArrayTools<SpT>::
  cloneScaleFields( /**/  Kokkos::DynRankView<outputFieldValueType,outputFieldProperties...> outputFields,
                    const Kokkos::DynRankView<inputFactorValueType,inputFactorProperties...> inputFactors,
                    const Kokkos::DynRankView<inputFieldValueType, inputFieldProperties...> inputFields ) {
    
    // static assert
#ifdef HAVE_INTREPID2_DEBUG
    { 
      INTREPID2_TEST_FOR_EXCEPTION( inputFactors.rank() != 2, std::invalid_argument,
                                      ">>> ERROR (ArrayTools::cloneScaleFields): The rank of the input factors container must be 2.");
      INTREPID2_TEST_FOR_EXCEPTION( inputFields.rank() < 2 || inputFields.rank() > 4, std::invalid_argument,
                                      ">>> ERROR (ArrayTools::cloneScaleFields): Input fields container must have rank 2, 3, or 4.");
      INTREPID2_TEST_FOR_EXCEPTION( outputFields.rank() != (inputFields.rank()+1), std::invalid_argument,
                                      ">>> ERROR (ArrayTools::cloneScaleFields): The rank of the input fields container must be one less than the rank of the output fields container.");
      INTREPID2_TEST_FOR_EXCEPTION( inputFactors.dimension(0) != outputFields.dimension(0), std::invalid_argument,
                                      ">>> ERROR (ArrayTools::cloneScaleFields): Zeroth dimensions of input factors container and output fields container (numbers of integration domains) must agree!");
      INTREPID2_TEST_FOR_EXCEPTION( inputFactors.dimension(1) != outputFields.dimension(1), std::invalid_argument,
                                      ">>> ERROR (ArrayTools::cloneScaleFields): First dimensions of input factors container and output fields container (numbers of fields) must agree!");
      for (size_type i=0;i<inputFields.rank();++i) {
        INTREPID2_TEST_FOR_EXCEPTION( (inputFields.dimension(i) != outputFields.dimension(i+1)), std::invalid_argument,
                                        ">>> ERROR (ArrayTools::cloneScaleFields): Dimensions of input and output fields containers do not match.");
      }
    }
#endif

    typedef Kokkos::DynRankView<outputFieldValueType,outputFieldProperties...> outputFieldViewType;
    typedef Kokkos::DynRankView<inputFactorValueType,inputFactorProperties...> inputFactorsViewType;
    typedef Kokkos::DynRankView<inputFieldValueType, inputFieldProperties...>  inputFieldViewType;
    typedef FunctorArrayTools::F_cloneScaleFields<outputFieldViewType, inputFactorsViewType, inputFieldViewType> FunctorType;
    typedef typename ExecSpace<typename inputFieldViewType::execution_space , SpT>::ExecSpaceType ExecSpaceType;

    using range_policy_type = Kokkos::Experimental::MDRangePolicy
      < ExecSpaceType, Kokkos::Experimental::Rank<3>, Kokkos::IndexType<ordinal_type> >;
    range_policy_type policy( { 0, 0, 0 }, 
                              { outputFields.dimension(0), outputFields.dimension(1), outputFields.dimension(2) } );
    Kokkos::Experimental::md_parallel_for( policy, FunctorType(outputFields, inputFactors, inputFields) );
  }

  namespace FunctorArrayTools {
    template < typename inoutFieldViewType , typename inputFactorsViewType >
    struct F_scaleFields {
      inoutFieldViewType _inoutFields;
      inputFactorsViewType _inputFactors;


      F_scaleFields(inoutFieldViewType inoutFields_,
                    inputFactorsViewType inputFactors_)
        : _inoutFields(inoutFields_), _inputFactors(inputFactors_) {}
      
      KOKKOS_INLINE_FUNCTION
      void operator()(const ordinal_type cl,
                      const ordinal_type bf,
                      const ordinal_type pt) const {
        const ordinal_type iend  = _inoutFields.dimension(3);
        const ordinal_type jend  = _inoutFields.dimension(4);

        const auto val = _inputFactors( cl, bf );
        for (ordinal_type i=0;i<iend;++i)
          for (ordinal_type j=0;j<jend;++j)
            _inoutFields(cl, bf, pt, i, j) *= val;
      }
    };
    } //namespace

  template<typename SpT>
  template<typename inoutFieldValueType,  class ...inoutFieldProperties,
           typename inputFactorValueType, class ...inputFactorProperties>

  void ArrayTools<SpT>::
  scaleFields( /**/  Kokkos::DynRankView<inoutFieldValueType, inoutFieldProperties...>  inoutFields,
               const Kokkos::DynRankView<inputFactorValueType,inputFactorProperties...> inputFactors ) {

#ifdef HAVE_INTREPID2_DEBUG
    {
      INTREPID2_TEST_FOR_EXCEPTION( inputFactors.rank() != 2, std::invalid_argument,
                                      ">>> ERROR (ArrayTools::scaleFields): The rank of the input factors container must be 2.");
      INTREPID2_TEST_FOR_EXCEPTION( inoutFields.rank() < 3 || inoutFields.rank() > 5, std::invalid_argument,
                                      ">>> ERROR (ArrayTools::scaleFields): Input/output fields container must have rank 3, 4, or 5.");
      INTREPID2_TEST_FOR_EXCEPTION( inputFactors.dimension(0) != inoutFields.dimension(0), std::invalid_argument,
                                      ">>> ERROR (ArrayTools::scaleFields): Zeroth dimensions of input factors container and input/output fields container (numbers of integration domains) must agree!");
      INTREPID2_TEST_FOR_EXCEPTION( inputFactors.dimension(1) != inoutFields.dimension(1), std::invalid_argument,
                                      ">>> ERROR (ArrayTools::scaleFields): First dimensions (number of fields) of input factors and input/output fields containers must agree!");
    }
#endif

    typedef Kokkos::DynRankView<inoutFieldValueType, inoutFieldProperties...>  inoutFieldViewType;
    typedef Kokkos::DynRankView<inputFactorValueType,inputFactorProperties...> inputFactorsViewType;
    typedef FunctorArrayTools::F_scaleFields<inoutFieldViewType, inputFactorsViewType> FunctorType;
    typedef typename ExecSpace< typename inoutFieldViewType::execution_space , SpT >::ExecSpaceType ExecSpaceType;
    
    using range_policy_type = Kokkos::Experimental::MDRangePolicy
      < ExecSpaceType, Kokkos::Experimental::Rank<3>, Kokkos::IndexType<ordinal_type> >;
    range_policy_type policy( { 0, 0, 0 }, 
                              { inoutFields.dimension(0), inoutFields.dimension(1), inoutFields.dimension(2) } );
    Kokkos::Experimental::md_parallel_for( policy, FunctorType(inoutFields, inputFactors) );
  }

} // end namespace Intrepid2

#endif
