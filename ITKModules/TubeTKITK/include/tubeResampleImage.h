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
#ifndef __tubeResampleImage_h
#define __tubeResampleImage_h

// ITK Includes
#include "itkProcessObject.h"

// TubeTK Includes
#include "tubeWrappingMacros.h"

#include "itktubeResampleImageFilter.h"

namespace tube
{
/** \class ResampleImage
 *
 *  \ingroup TubeTKITK
 */

template< class TPixel, unsigned int VDimension >
class ResampleImage:
  public itk::ProcessObject
{
public:
  /** Standard class type alias. */
  using Self = ResampleImage;
  using Superclass = itk::ProcessObject;
  using Pointer = itk::SmartPointer< Self >;
  using ConstPointer = itk::SmartPointer< const Self >;

  using FilterType = itk::tube::ResampleImageFilter< TPixel, VDimension >;

  using ImageType = typename FilterType::ImageType;
  using ConstImagePointer = typename ImageType::ConstPointer;
  using ImagePointer = typename ImageType::Pointer;
  using TransformType = typename FilterType::TransformType;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information ( and related methods ). */
  itkTypeMacro( ResampleImage, ProcessObject );

  /* Set input image */
  tubeWrapSetConstObjectMacro( Input, ImageType, Filter );

  /** Set/Get input Match Image */
  tubeWrapSetObjectMacro( MatchImage, ImageType, Filter );
  tubeWrapGetObjectMacro( MatchImage, ImageType, Filter );

  /** Set/Get whether Output is isotropic or not */
  tubeWrapSetMacro( MakeIsotropic, bool, Filter );
  tubeWrapGetMacro( MakeIsotropic, bool, Filter );

  /** Set/Get whether Output is high resolution isotropic or not */
  tubeWrapSetMacro( MakeHighResIso, bool, Filter );
  tubeWrapGetMacro( MakeHighResIso, bool, Filter );

  /** Set/Get interpolator */
  tubeWrapSetMacro( Interpolator, std::string, Filter );
  tubeWrapGetMacro( Interpolator, std::string, Filter );

  /** Set/Get whether to load transform or not */
  tubeWrapSetMacro( LoadTransform, bool, Filter );
  tubeWrapGetMacro( LoadTransform, bool, Filter );

  tubeWrapGetObjectMacro( Output, ImageType, Filter );

  /** Set Output Transform */
  void SetTransform( TransformType* t );

  /** Set Output Spacing */
  void SetSpacing( std::vector<double> s );

  /** Set Output Origin */
  void SetOrigin( std::vector<double> o );

  /** Set Output Index */
  void SetIndex( std::vector<int> i );

  /** Set Output Resample Factor */
  void SetResampleFactor( std::vector<double> rf );

  /* Runs tubes to image conversion */
  tubeWrapUpdateMacro( Filter );

protected:
  ResampleImage( void );
  ~ResampleImage() {}

  void PrintSelf( std::ostream & os, itk::Indent indent ) const;

private:
  /** itktubeResampleImageFilter parameters **/
  ResampleImage( const Self & );
  void operator=( const Self & );

  // To remove warning "was hidden [-Woverloaded-virtual]"
  void SetInput( const DataObjectIdentifierType &, itk::DataObject * ) {};

  typename FilterType::Pointer  m_Filter;
};
} // End namespace tube

#ifndef ITK_MANUAL_INSTANTIATION
#include "tubeResampleImage.hxx"
#endif

#endif // End !defined( __tubeResampleImage_h )
