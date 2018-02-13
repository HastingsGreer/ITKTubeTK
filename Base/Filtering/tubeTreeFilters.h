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

#ifndef __tubeTreeFilters_h
#define __tubeTreeFilters_h

#include <itkImageFileReader.h>
#include "itkGroupSpatialObject.h"

namespace tube
{
template< unsigned int VDimension >
class TreeFilters
{
public:
  //type alias
  using TubeGroupType = itk::GroupSpatialObject< VDimension >;
  using TubeListPointerType = typename TubeGroupType::ChildrenListPointer;
  using TubeType = itk::VesselTubeSpatialObject< VDimension >;
  using TubePointerType = typename TubeType::Pointer;
  using TubePointType = typename TubeType::TubePointType;
  using PositionType = typename TubeType::PointType;
  using TubeIdType = itk::IndexValueType;
  using TubePointListType = typename TubeType::PointListType;

  /** Run Fill Gap on the tube-tree. */
  static void FillGap( typename TubeGroupType::Pointer & pTubeGroup,
    char InterpolationMethod );

  static void InterpolatePath(
  typename TubeType::TubePointType * parentNearestPoint,
  typename TubeType::TubePointType * itkNotUsed( childEndPoint ),
  typename TubeType::PointListType & newTubePoints,
  char InterpolationMethod );

private:
  TreeFilters();
  ~TreeFilters();

}; // End class ImageFilters

} // End namespace tube

#ifndef ITK_MANUAL_INSTANTIATION
#include "tubeTreeFilters.hxx"
#endif

#endif // End !defined( __tubeTreeFilters_h )
