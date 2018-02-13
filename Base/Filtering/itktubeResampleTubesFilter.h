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

#ifndef __itktubeResampleTubesFilter_h
#define __itktubeResampleTubesFilter_h

#include "itkGroupSpatialObject.h"
#include "itkImage.h"
#include <itkDisplacementFieldTransform.h>
#include "itktubeSpatialObjectToSpatialObjectFilter.h"
#include "itkVesselTubeSpatialObject.h"
#include "itktubeTubeToTubeTransformFilter.h"
#include "itkTransformBase.h"
namespace itk
{
namespace tube
{

/** \class ResampleTubesFilter
 * \brief resamples a given tube spatial object.
 *
 */
template< unsigned int VDimension >
class ResampleTubesFilter
  : public SpatialObjectToSpatialObjectFilter<
    GroupSpatialObject< VDimension >, GroupSpatialObject< VDimension > >
{
public:
  /** Standard class type alias. */
  using TubeGroupType = itk::GroupSpatialObject< VDimension >;
  using TubeSpatialObjectType = itk::VesselTubeSpatialObject< VDimension >;

  using Self = ResampleTubesFilter< VDimension >;
  using Superclass = SpatialObjectToSpatialObjectFilter< TubeGroupType, TubeGroupType >;
  using Pointer = SmartPointer< Self >;
  using ConstPointer = SmartPointer< const Self >;

  using PixelType = char;
  using ImageType = itk::Image< PixelType, VDimension >;

  /** Typedefs for Displacement field tranform.    */
  using DisplacementFieldTransformType = itk::DisplacementFieldTransform< double, VDimension >;
  typedef typename DisplacementFieldTransformType::DisplacementFieldType
    DisplacementFieldType;

  /** Typedefs for transform read from a file    */
  using BaseTransformType = itk::TransformBaseTemplate< double >;
  using BaseTransformListType = std::list< BaseTransformType::Pointer >;

  /** Run-time type information ( and related methods ).   */
  itkTypeMacro( ResampleTubesFilter,
                SpatialObjectToSpatialObjectFilter );

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Set/Get match image */
  itkSetObjectMacro( MatchImage, ImageType );
  itkGetObjectMacro( MatchImage, ImageType );

  /** Set/Get sampling factor */
  itkSetMacro( SamplingFactor, int );
  itkGetMacro( SamplingFactor, int );

  /** Set/Get  use Inverse Transform */
  itkSetMacro( UseInverseTransform, bool );
  itkGetMacro( UseInverseTransform, bool );

  itkSetObjectMacro( InputSpatialObject, TubeGroupType );

  void SetDisplacementField( DisplacementFieldType* field );
  void SetReadTransformList( BaseTransformListType* tList );

protected:
  ResampleTubesFilter( void );
  virtual ~ResampleTubesFilter( void );

  virtual void GenerateData( void );
  void PrintSelf( std::ostream & os, Indent indent ) const;

private:
  // purposely not implemented
  ResampleTubesFilter( const Self & );
  // purposely not implemented
  void operator=( const Self & );

  typename ImageType::Pointer             m_MatchImage;
  int                                     m_SamplingFactor;
  bool                                    m_UseInverseTransform;
  BaseTransformListType*                  m_ReadTransformList;
  typename DisplacementFieldType::Pointer m_DisplacementField;
  typename TubeGroupType::Pointer         m_InputSpatialObject;

  void ReadImageTransform
    ( typename TubeGroupType::TransformType::Pointer &outputTransform );
  typename TubeGroupType::Pointer ApplyDisplacementFieldTransform
    ( typename TubeGroupType::TransformType::Pointer outputTransform );
  typename TubeGroupType::Pointer ApplyInputTransform
    ( typename TubeGroupType::TransformType::Pointer outputTransform );
  typename TubeGroupType::Pointer ApplyIdentityAffineTransform
    ( typename TubeGroupType::TransformType::Pointer outputTransform );
}; // End class ResampleTubesFilter

} // End namespace tube
} // End namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itktubeResampleTubesFilter.hxx"
#endif

#endif // End !defined( __itktubeResampleTubesFilter_h )
