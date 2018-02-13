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
#ifndef __tubeSegmentTubes_h
#define __tubeSegmentTubes_h

// ITK Includes
#include "itkProcessObject.h"

// TubeTK Includes
#include "tubeWrappingMacros.h"

#include "itktubeTubeExtractor.h"

namespace tube
{
/** \class SegmentTubes
 *
 *  \ingroup TubeTKITK
 */

template< class TInputImage >
class SegmentTubes:
  public itk::ProcessObject
{
public:
  /** Standard class type alias. */
  using Self = SegmentTubes;
  using Superclass = itk::ProcessObject;
  using Pointer = itk::SmartPointer< Self >;
  using ConstPointer = itk::SmartPointer< const Self >;

  using ImageType = TInputImage;
  using PixelType = typename ImageType::PixelType;
  using IndexType = typename ImageType::IndexType;

  using FilterType = itk::tube::TubeExtractor< ImageType >;
  using TubeMaskImageType = typename FilterType::TubeMaskImageType;
  using TubeType = typename FilterType::TubeType;
  using TubeGroupType = typename FilterType::TubeGroupType;

  using ContinuousIndexType = typename FilterType::ContinuousIndexType;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information ( and related methods ). */
  itkTypeMacro( SegmentTubes, ProcessObject );

  /** Set the source image. */
  tubeWrapSetObjectMacro( InputImage, ImageType, Filter );

  /** Set the radius image. */
  tubeWrapSetObjectMacro( RadiusInputImage, ImageType, Filter );

  /** Set/Get radius value */
  tubeWrapSetMacro( Radius, double, Filter );
  tubeWrapGetMacro( Radius, double, Filter );

  /** Set the tube mask image. */
  tubeWrapSetObjectMacro( TubeMaskImage, TubeMaskImageType, Filter );
  tubeWrapGetObjectMacro( TubeMaskImage, TubeMaskImageType, Filter );

  /** Add a tube */
  bool AddTube( TubeType * tube );

  /** Delete a tube */
  bool DeleteTube( TubeType * tube );

  /** Set/Get debug status */
  tubeWrapSetMacro( Debug, bool, Filter );
  tubeWrapGetMacro( Debug, bool, Filter );

  /** Set ExtractBound Minimum */
  tubeWrapSetMacro( ExtractBoundMin, IndexType, Filter );
  tubeWrapGetMacro( ExtractBoundMin, IndexType, Filter );

  /** Set ExtractBound Maximum */
  tubeWrapSetMacro( ExtractBoundMax, IndexType, Filter );
  tubeWrapGetMacro( ExtractBoundMax, IndexType, Filter );

  /*** Extract the ND tube given the position of the first point
   * and the tube ID */
  typename itk::tube::TubeExtractor< TInputImage >::TubeType *
    ExtractTube( const ContinuousIndexType & x,
    unsigned int tubeID,
    bool verbose = false );

  /** Get the list of tubes that have been extracted */
  tubeWrapGetConstObjectMacro( TubeGroup, TubeGroupType, Filter );

protected:
  SegmentTubes( void );
  ~SegmentTubes() {}
  void PrintSelf( std::ostream & os, itk::Indent indent ) const;

private:
  /** itktubeTubeExtractor parameters **/
  SegmentTubes( const Self & );
  void operator=( const Self & );

  // To remove warning "was hidden [-Woverloaded-virtual]"
  void SetInput( const DataObjectIdentifierType &, itk::DataObject * ) {};

  typename FilterType::Pointer m_Filter;

};
} // End namespace tube

#ifndef ITK_MANUAL_INSTANTIATION
#include "tubeSegmentTubes.hxx"
#endif

#endif // End !defined( __tubeSegmentTubes_h )
