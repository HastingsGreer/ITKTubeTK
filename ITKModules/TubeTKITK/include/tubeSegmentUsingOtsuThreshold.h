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
#ifndef __tubeSegmentUsingOtsuThreshold_h
#define __tubeSegmentUsingOtsuThreshold_h

// ITK includes
#include "itkProcessObject.h"

// TubeTK includes
#include "tubeWrappingMacros.h"

#include "itkOtsuThresholdImageFilter.h"


namespace tube
{
/** \class SegmentUsingOtsuThreshold
 *
 *  \ingroup TubeTKITK
 */

template< class TInputPixel, unsigned int Dimension,
  class TMaskPixel = unsigned char >
class SegmentUsingOtsuThreshold:
  public itk::ProcessObject
{
public:
  /** Standard class type alias. */
  using Self = SegmentUsingOtsuThreshold;
  using Superclass = itk::ProcessObject;
  using Pointer = itk::SmartPointer< Self >;
  using ConstPointer = itk::SmartPointer< const Self >;

  using InputImageType = itk::Image< TInputPixel, Dimension >;
  using MaskImageType = itk::Image< TMaskPixel, Dimension >;
  using OutputImageType = MaskImageType;

  using FilterType = itk::OtsuThresholdImageFilter< InputImageType,
    OutputImageType >;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information ( and related methods ). */
  itkTypeMacro( SegmentUsingOtsuThreshold, ProcessObject );

  /** Set/Get mask image */
  tubeWrapSetConstObjectMacro( MaskImage, MaskImageType, Filter );
  tubeWrapGetConstObjectMacro( MaskImage, MaskImageType, Filter );

  /** Set/Get mask value */
  tubeWrapSetMacro( MaskValue, TInputPixel, Filter );
  tubeWrapGetMacro( MaskValue, TInputPixel, Filter );

  /** Set/Get input image */
  tubeWrapSetConstObjectMacro( Input, InputImageType, Filter );
  tubeWrapGetConstObjectMacro( Input, InputImageType, Filter );

  /** Runs the thresholding algorithm */
  tubeWrapUpdateMacro( Filter );

  /** Get output segmentation mask */
  tubeWrapGetObjectMacro( Output, OutputImageType, Filter );

  /** Get output threshold */
  tubeWrapGetMacro( Threshold, TInputPixel, Filter );

protected:
  SegmentUsingOtsuThreshold( void );
  ~SegmentUsingOtsuThreshold() {}
  void PrintSelf( std::ostream & os, itk::Indent indent ) const;

private:
  /** itkSegmentUsingOtsuThresholdFilter parameters **/
  SegmentUsingOtsuThreshold( const Self & );
  void operator=( const Self & );

  // To remove warning "was hidden [-Woverloaded-virtual]"
  void SetInput( const DataObjectIdentifierType &, itk::DataObject * ) {};

  typename FilterType::Pointer m_Filter;

};

} // End namespace tube


#ifndef ITK_MANUAL_INSTANTIATION
#include "tubeSegmentUsingOtsuThreshold.hxx"
#endif

#endif // End !defined( __tubeSegmentUsingOtsuThreshold_h )
