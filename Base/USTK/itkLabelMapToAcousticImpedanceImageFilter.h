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

#ifndef __itkLabelMapToAcousticImpedanceImageFilter_h
#define __itkLabelMapToAcousticImpedanceImageFilter_h

#include "itkLabelMapToAcousticImpedanceFunctor.h"

#include <itkUnaryFunctorImageFilter.h>

namespace itk
{

/** \class LabelMapToAcousticImpedanceImageFilter
 *
 * \brief Creates an image of approximate acoustic impedance from a label
 * map of classified tissues.
 */
template< class TInputImage, class TOutputImage, class TLookupTable >
class LabelMapToAcousticImpedanceImageFilter
  : public UnaryFunctorImageFilter< TInputImage, TOutputImage,
    Functor::LabelMapToAcousticImpedanceFunctor<
      typename TInputImage::PixelType,
      typename TOutputImage::PixelType, TLookupTable > >
{
public:
  /** Standard class type alias. */
  using Self = LabelMapToAcousticImpedanceImageFilter;
  using Superclass = UnaryFunctorImageFilter< TInputImage,
            TOutputImage,
            Functor::LabelMapToAcousticImpedanceFunctor<
              typename TInputImage::PixelType,
              typename TOutputImage::PixelType,
              TLookupTable > >;
  using Pointer = SmartPointer< Self >;
  using ConstPointer = SmartPointer< const Self >;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Runtime type information. */
  itkTypeMacro( LabelMapToAcousticImpedanceImageFilter,
                UnaryFunctorImageFilter );

  using FunctorType = typename Superclass::FunctorType;

protected:
  LabelMapToAcousticImpedanceImageFilter( void ) {}
  virtual ~LabelMapToAcousticImpedanceImageFilter( void ) {}

  virtual void BeforeThreadedGenerateData( void );

private:
  // purposely not implemented
  LabelMapToAcousticImpedanceImageFilter( const Self & );
  // purposely not implemented
  void operator=( const Self & );

}; // End class LabelMapToAcousticImpedanceImageFilter

} // End namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkLabelMapToAcousticImpedanceImageFilter.hxx"
#endif

#endif // End !defined( __itkLabelMapToAcousticImpedanceImageFilter_h )
