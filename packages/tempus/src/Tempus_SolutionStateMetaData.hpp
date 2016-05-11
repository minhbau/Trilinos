#ifndef TEMPUS_SOLUTIONSTATEMETADATA_HPP
#define TEMPUS_SOLUTIONSTATEMETADATA_HPP

// Teuchos
#include "Teuchos_VerboseObject.hpp"
#include "Teuchos_Describable.hpp"
// Tempus
#include "Tempus_StepType.hpp"


namespace Tempus {

/** \brief Solution state meta data.
 */
template<class Scalar>
class SolutionStateMetaData :
  public Teuchos::Describable,
  public Teuchos::VerboseObject<Tempus::SolutionStateMetaData<Scalar> >
{
public:

  /// Default constructor.
  SolutionStateMetaData();

  /// Constructor
  SolutionStateMetaData(
    const Scalar time_,
    const int    iStep_,
    const Scalar dt_,
    const Scalar suggestedDt_,
    const Scalar errorAbs_,
    const Scalar errorRel_,
    const int    order_,
    const int    nFailures_,
    const int    nConsecutiveFailures_,
    const SolutionStatus status_,
    const bool   output_,
    const bool   isAccepted_,
    const bool   isRestartable_,
    const bool   isInterpolated_,
    const Scalar accuracy_);

  /// Copy constructor
  SolutionStateMetaData(const SolutionStateMetaData<Scalar>& ssmd_);

  /// Clone
  Teuchos::RCP<SolutionStateMetaData<Scalar> > clone();

  /// Destructor
  virtual ~SolutionStateMetaData() {};

  Scalar time;            ///< Time of solution
  int    iStep;           ///< Time step index for this solution
  Scalar dt;              ///< Time step for this solution
  Scalar suggestedDt;     ///< Time step suggested to the Stepper
  Scalar errorAbs;        ///< Absolute local truncation error
  Scalar errorRel;        ///< Relative local truncation error
  int order;              ///< Order of this solution
  int nFailures;          ///< Total number of stepper failures
  int nConsecutiveFailures; ///< Consecutive number of stepper failures
  SolutionStatus status;  ///< Status of SolutionState (passing, failed)
  bool   output;          ///< SolutionState should be or has been outputted
  bool   isAccepted;      ///< SolutionState accepted (i.e, no more changes)
  bool   isRestartable;   ///< T - soln can be used as a restart
  bool   isInterpolated;  ///< F - soln is time integrated; T - soln is interpolated
  Scalar accuracy;        ///< Interpolation accuracy of solution

  /// \name Overridden from Teuchos::Describable
  //@{
    virtual std::string description() const;
    virtual void describe(Teuchos::FancyOStream          &out,
                          const Teuchos::EVerbosityLevel verbLevel) const;
  //@}

};
} // namespace Tempus

#include "Tempus_SolutionStateMetaData_impl.hpp"

#endif // TEMPUS_SOLUTIONSTATEMETADATA_HPP
