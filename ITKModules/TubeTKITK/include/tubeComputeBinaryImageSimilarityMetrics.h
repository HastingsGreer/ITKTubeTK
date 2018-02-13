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
#ifndef __tubeComputeBinaryImageSimilarityMetrics_h
#define __tubeComputeBinaryImageSimilarityMetrics_h

// ITK Includes
#include "itkProcessObject.h"

// TubeTK Includes
#include "tubeWrappingMacros.h"

#include "itkLabelOverlapMeasuresImageFilter.h"

namespace tube
{
/** \class ComputeBinaryImageSimilarityMetrics
 *
 *  \ingroup TubeTKITK
 */

template< class TInputImage >
class ComputeBinaryImageSimilarityMetrics:
  public itk::ProcessObject
{
public:
  /** Standard class type alias. */
  using Self = ComputeBinaryImageSimilarityMetrics;
  using Superclass = itk::ProcessObject;
  using Pointer = itk::SmartPointer< Self >;
  using ConstPointer = itk::SmartPointer< const Self >;
  using InputImageType = TInputImage;
  using LabelType = typename TInputImage::PixelType;

  using FilterType = itk::LabelOverlapMeasuresImageFilter< InputImageType >;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information ( and related methods ). */
  itkTypeMacro( ComputeBinaryImageSimilarityMetrics, ProcessObject );

  /** Set the source image. */
  tubeWrapSetConstObjectMacro( SourceImage, InputImageType, Filter );

  /** Set the target image. */
  tubeWrapSetConstObjectMacro( TargetImage, InputImageType, Filter );

  /** measures over all labels */
  tubeWrapGetMacro( TotalOverlap, float, Filter );
  tubeWrapGetMacro( UnionOverlap, float, Filter );
  tubeWrapGetMacro( MeanOverlap, float, Filter );
  tubeWrapGetMacro( VolumeSimilarity, float, Filter );
  tubeWrapGetMacro( FalseNegativeError, float, Filter );
  tubeWrapGetMacro( FalsePositiveError, float, Filter );

  /** Compute image similarity */
  tubeWrapUpdateMacro( Filter );

protected:
  ComputeBinaryImageSimilarityMetrics( void );
  ~ComputeBinaryImageSimilarityMetrics() {}
  void PrintSelf( std::ostream & os, itk::Indent indent ) const;

private:
  /** itkLabelOverlapMeasuresImageFilter parameters **/
  ComputeBinaryImageSimilarityMetrics( const Self & );
  void operator=( const Self & );

  // To remove warning "was hidden [-Woverloaded-virtual]"
  void SetInput( const DataObjectIdentifierType &, itk::DataObject * ) {};

  typename FilterType::Pointer m_Filter;

};

} // End namespace tube

#ifndef ITK_MANUAL_INSTANTIATION
#include "tubeComputeBinaryImageSimilarityMetrics.hxx"
#endif

#endif // End !defined( __tubeComputeBinaryImageSimilarityMetrics_h )
