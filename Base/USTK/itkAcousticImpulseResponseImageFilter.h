/*=========================================================================

Library:   TubeTK

Copyright 2010 Kitware Inc. 28 Corporate Drive,
Clifton Park, NY, 12065, USA.

All rights reserved.

Licensed under the Apache License, Version 2.0 ( the "License" );
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

=========================================================================*/

#ifndef __itkAcousticImpulseResponseImageFilter_h
#define __itkAcousticImpulseResponseImageFilter_h

#include <itkCastImageFilter.h>
#include <itkCovariantVector.h>
#include <itkImageToImageFilter.h>

namespace itk
{
/** \class AcousticImpulseResponseImageFilter
 *
 * \brief Compute the acoustic impulse response along the beam direction.
 *
 * This filter computes the acoustic pressure impulse response along a beam
 * direction.
 *
 * \f[
 * T( \mathbf{x} ) =
 *   \cos^n \theta \left( \frac{|\nabla Z( \mathbf{x} )|}
 *     {2 * Z( \mathbf{x} )} \right )
 * \f]
 *
 * where:
 *
 * \f{eqnarray*}
 *   T( \mathbf{x} ) &=& \mbox{acoustic pressure impulse response}
 *   n             &=& \mbox{specifies angle dependence}
 *   Z             &=& \mbox{acoustic impedance}
 *   \cos \theta   &=& \mbox{angle of incidence}
 * \f}
 *
 * This filter requires two inputs.  The first input is the input acoustic
 * impedance image.  The second input is an angle of incidence image.
 * \f$n\f$ is specified with the \c AngleDependence parameter, and
 * defaults to one.
 *
 * It is possible to specify the filter used to calculate the gradient
 * magnitude with \c SetGradientMagnitudeFilter.
 *
 * \ingroup ImageToImageFilter
 */
template< class TInputImage, class TOutputImage,
  class TOperatorValue = float >
class AcousticImpulseResponseImageFilter
  : public ImageToImageFilter< TInputImage, TOutputImage >
{
public:
  /** Standard class type alias. */
  using Self = AcousticImpulseResponseImageFilter;
  using Superclass = ImageToImageFilter< TInputImage, TOutputImage >;
  using Pointer = SmartPointer< Self >;
  using ConstPointer = SmartPointer< const Self >;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information ( and related methods ). */
  itkTypeMacro( AcousticImpulseResponseImageFilter, ImageToImageFilter );

  /** Some convenient type alias. */
  using InputImageType = TInputImage;
  using OriginType = typename InputImageType::PointType;
  using OutputImageType = TOutputImage;
  using OutputImageRegionType = typename OutputImageType::RegionType;

  static constexpr unsigned int ImageDimension = InputImageType::ImageDimension;

  typedef TOperatorValue
    OperatorValueType;
  using OperatorImageType = Image< OperatorValueType, ImageDimension >;
  using GradientMagnitudeFilterType = ImageToImageFilter< OperatorImageType, OperatorImageType >;

  /** Set/Get the angle dependence term \c n in the class documentation. */
  itkSetMacro( AngleDependence, double );
  itkGetConstMacro( AngleDependence, double );

  /** Set/Get the filter used to compute the gradient magnitude. */
  itkSetObjectMacro( GradientMagnitudeFilter, GradientMagnitudeFilterType );
  itkGetObjectMacro( GradientMagnitudeFilter, GradientMagnitudeFilterType );

protected:
  AcousticImpulseResponseImageFilter( void );
  virtual ~AcousticImpulseResponseImageFilter( void ) {}

  virtual void PrintSelf( std::ostream & os, Indent indent ) const;

  virtual void BeforeThreadedGenerateData( void );
  virtual void ThreadedGenerateData( const OutputImageRegionType &
    outputRegionForThread, ThreadIdType threadId );

private:
  // purposely not implemented
  AcousticImpulseResponseImageFilter( const Self & );
  // purposely not implemented
  void operator=( const Self & );

  typename GradientMagnitudeFilterType::Pointer m_GradientMagnitudeFilter;

  double m_AngleDependence;

  using CastImageFilterType = CastImageFilter< InputImageType, OperatorImageType >;
  typename CastImageFilterType::Pointer m_CastImageFilter;

}; // End class AcousticImpulseResponseImageFilter

} // End namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkAcousticImpulseResponseImageFilter.hxx"
#endif

#endif // End !defined( __itkAcousticImpulseResponseImageFilter_h )
