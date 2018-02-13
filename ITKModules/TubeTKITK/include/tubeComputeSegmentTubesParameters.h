/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 ( the "License" );
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
*=========================================================================*/
#ifndef __tubeComputeSegmentTubesParameters_h
#define __tubeComputeSegmentTubesParameters_h

// ITK Includes
#include "itkProcessObject.h"

// TubeTK Includes
#include "tubeWrappingMacros.h"

#include "itktubeComputeSegmentTubesParameters.h"

namespace tube
{
/** \class ComputeSegmentTubesParameters
 *
 *  \ingroup TubeTKITK
 */

template< class TPixel, unsigned int VDimension >
class ComputeSegmentTubesParameters:
  public itk::ProcessObject
{
public:
  /** Standard class type alias. */
  using Self = ComputeSegmentTubesParameters;
  using Superclass = itk::ProcessObject;
  using Pointer = itk::SmartPointer< Self >;
  using ConstPointer = itk::SmartPointer< const Self >;
  using FilterType = itk::tube::ComputeSegmentTubesParameters< TPixel, VDimension >;

  using InputImageType = typename FilterType::InputImageType;
  using MaskImageType = typename FilterType::MaskImageType;
  using ScaleImageType = typename FilterType::ScaleImageType;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information ( and related methods ). */
  itkTypeMacro( ComputeSegmentTubesParameters, ProcessObject );

  /** Set/Get the input image */
  tubeWrapSetObjectMacro( InputImage, InputImageType, Filter );
  tubeWrapGetObjectMacro( InputImage, InputImageType, Filter );

  /** Set/Get the mask input image */
  tubeWrapSetObjectMacro( MaskInputImage, MaskImageType, Filter );
  tubeWrapGetObjectMacro( MaskInputImage, MaskImageType, Filter );

  /** Set/Get the scale input image */
  tubeWrapSetObjectMacro( ScaleInputImage, ScaleImageType, Filter );
  tubeWrapGetObjectMacro( ScaleInputImage, ScaleImageType, Filter );

  /** Set/Get Mask BackGround Id */
  tubeWrapSetMacro( MaskBackGroundId, int, Filter );
  tubeWrapGetMacro( MaskBackGroundId, int, Filter );

  /** Set/Get Mask BackGround Id */
  tubeWrapSetMacro( MaskTubeId, int, Filter );
  tubeWrapGetMacro( MaskTubeId, int, Filter );

  /** Set Parameter File */
  void SetParameterFile( std::string );

  /** Get Seed Data List */
  std::vector< vnl_vector< double > > GetSeedData( void );

  /** Get Tube Data List */
  std::vector< vnl_vector< double > > GetTubeData( void );

  /** Get Bkg Data List */
  std::vector< vnl_vector< double > > GetBkgData( void );

  /** Get Seed Index List */
  std::vector< itk::ContinuousIndex< double, VDimension > >
  GetSeedDataIndexList( void );

  /** Get Tube Index List */
  std::vector< itk::ContinuousIndex< double, VDimension > >
  GetTubeDataIndexList( void );

  /** Get Bkg Index List */
  std::vector< itk::ContinuousIndex< double, VDimension > >
  GetBkgDataIndexList( void );

  /* Runs computation */
  tubeWrapUpdateMacro( Filter );

protected:
  ComputeSegmentTubesParameters( void );
  ~ComputeSegmentTubesParameters() {}
  void PrintSelf( std::ostream & os, itk::Indent indent ) const;

private:
  /** itktubeComputeSegmentTubesParameters parameters **/
  ComputeSegmentTubesParameters( const Self & );
  void operator=( const Self & );

  // To remove warning "was hidden [-Woverloaded-virtual]"
  void SetInput( const DataObjectIdentifierType &, itk::DataObject * ) {};

  typename FilterType::Pointer m_Filter;
};
} // End namespace tube

#ifndef ITK_MANUAL_INSTANTIATION
#include "tubeComputeSegmentTubesParameters.hxx"
#endif

#endif // End !defined( __tubeComputeSegmentTubesParameters_h )
