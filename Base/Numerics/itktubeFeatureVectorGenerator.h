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

#ifndef __itktubeFeatureVectorGenerator_h
#define __itktubeFeatureVectorGenerator_h

#include <itkImage.h>
#include <itkLightProcessObject.h>

#include <vnl/vnl_vector.h>
#include <vnl/vnl_matrix.h>

#include <vector>

namespace itk
{

namespace tube
{

template< class TImage >
class FeatureVectorGenerator : public LightProcessObject
{
public:

  using Self = FeatureVectorGenerator;
  using Superclass = LightProcessObject;
  using Pointer = SmartPointer< Self >;
  using ConstPointer = SmartPointer< const Self >;

  itkTypeMacro( FeatureVectorGenerator, LightProcessObject );

  itkNewMacro( Self );

  using ImageType = TImage;
  using ImageListType = std::vector< typename ImageType::ConstPointer >;

  using IndexType = typename TImage::IndexType;

  static constexpr unsigned int ImageDimension = TImage::ImageDimension;

  using FeatureValueType = float;
  using FeatureVectorType = vnl_vector< FeatureValueType >;

  using FeatureImageType = Image< FeatureValueType, TImage::ImageDimension >;

  using ValueType = double;
  using ValueListType = std::vector< ValueType >;

  virtual void SetInput( const ImageType * img );
  virtual void SetInput( unsigned int id, const ImageType * img );
  void AddInput( const ImageType * img );

  typename ImageType::ConstPointer GetInput( unsigned int imageNum );

  unsigned int GetNumberOfInputImages() const;

  void SetUpdateWhitenStatisticsOnUpdate( bool
    updateWhitenStatisticsOnUpdate );
  bool GetUpdateWhitenStatisticsOnUpdate( void );

  void   SetWhitenMeans( const ValueListType & means );
  const  ValueListType & GetWhitenMeans( void ) const;

  void   SetWhitenStdDevs( const ValueListType & stdDevs );
  const  ValueListType & GetWhitenStdDevs( void ) const;

  void   SetWhitenMean( unsigned int num, double mean );
  double GetWhitenMean( unsigned int num ) const;

  void   SetWhitenStdDev( unsigned int num, double stdDev );
  double GetWhitenStdDev( unsigned int num ) const;

  virtual unsigned int GetNumberOfFeatures( void ) const;

  virtual FeatureVectorType GetFeatureVector(
    const IndexType & indx ) const;

  virtual FeatureValueType GetFeatureVectorValue(
    const IndexType & indx, unsigned int fNum ) const;

  virtual typename FeatureImageType::Pointer GetFeatureImage(
    unsigned int num ) const;

  virtual void Update( void );

protected:

  FeatureVectorGenerator( void );
  virtual ~FeatureVectorGenerator( void );

  ImageListType                   m_InputImageList;

  void UpdateWhitenStatistics( void );

  void PrintSelf( std::ostream & os, Indent indent ) const;

private:

  // Purposely not implemented
  FeatureVectorGenerator( const Self & );
  void operator = ( const Self & );      // Purposely not implemented

  //  Data
  bool                            m_UpdateWhitenStatisticsOnUpdate;
  ValueListType                   m_WhitenMean;
  ValueListType                   m_WhitenStdDev;

}; // End class FeatureVectorGenerator

} // End namespace tube

} // End namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itktubeFeatureVectorGenerator.hxx"
#endif

#endif // End !defined( __itktubeFeatureVectorGenerator_h )
