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

#ifndef __itktubeRidgeSeedFilter_h
#define __itktubeRidgeSeedFilter_h

#include "itktubeBasisFeatureVectorGenerator.h"
#include "itktubePDFSegmenterBase.h"
#include "itktubePDFSegmenterParzen.h"
#include "itktubeRidgeFFTFeatureVectorGenerator.h"

#include <itkImage.h>

#include <vnl/vnl_matrix.h>
#include <vnl/vnl_vector.h>

#include <vector>

namespace itk
{

namespace tube
{

template< class TImage, class TLabelMap >
class RidgeSeedFilter : public ImageToImageFilter< TImage, TLabelMap >
{
public:

  using Self = RidgeSeedFilter;
  using Superclass = ImageToImageFilter< TImage, TLabelMap >;
  using Pointer = SmartPointer< Self >;
  using ConstPointer = SmartPointer< const Self >;

  itkTypeMacro( RidgeSeedFilter, ImageToImageFilter );

  itkNewMacro( Self );

  using InputImageType = TImage;
  using OutputImageType = Image< float, TImage::ImageDimension >;

  using LabelMapType = TLabelMap;
  using LabelMapPixelType = typename LabelMapType::PixelType;

  static constexpr unsigned int ImageDimension = TImage::ImageDimension;

  using RidgeFeatureGeneratorType = RidgeFFTFeatureVectorGenerator< InputImageType >;

  typedef typename RidgeFeatureGeneratorType::FeatureValueType
    FeatureValueType;
  typedef typename RidgeFeatureGeneratorType::FeatureVectorType
    FeatureVectorType;
  typedef typename RidgeFeatureGeneratorType::FeatureImageType
    FeatureImageType;

  typedef typename RidgeFeatureGeneratorType::IndexType
    IndexType;
  typedef typename RidgeFeatureGeneratorType::RidgeScalesType
    RidgeScalesType;
  typedef typename RidgeFeatureGeneratorType::ValueListType
    WhitenMeansType;
  typedef typename RidgeFeatureGeneratorType::ValueListType
    WhitenStdDevsType;

  using SeedFeatureGeneratorType = BasisFeatureVectorGenerator< InputImageType, LabelMapType >;

  using ObjectIdType = typename SeedFeatureGeneratorType::ObjectIdType;
  using VectorType = typename SeedFeatureGeneratorType::VectorType;
  using MatrixType = typename SeedFeatureGeneratorType::MatrixType;

  using PDFSegmenterType = PDFSegmenterBase< InputImageType, LabelMapType >;
  using PDFSegmenterParzenType = PDFSegmenterParzen< InputImageType, LabelMapType >;
  typedef typename  PDFSegmenterType::ProbabilityPixelType
    ProbabilityPixelType;
  typedef typename  PDFSegmenterType::ProbabilityImageType
    ProbabilityImageType;

  virtual void SetInput( const InputImageType * img );
  virtual void SetInput( unsigned int id, const InputImageType * img );

  using Superclass::AddInput;
  virtual void AddInput( const InputImageType * img );

  void SetLabelMap( LabelMapType * img );

  typename SeedFeatureGeneratorType::Pointer
    GetSeedFeatureGenerator( void );
  typename RidgeFeatureGeneratorType::Pointer
    GetRidgeFeatureGenerator( void );

  typename PDFSegmenterType::Pointer GetPDFSegmenter( void );
  void SetPDFSegmenter( PDFSegmenterType * pdfSegmenter );

  void            SetScales( const RidgeScalesType & Scales );
  RidgeScalesType GetScales( void ) const;

  // Basis
  void         SetInputWhitenMeans( const WhitenMeansType & means );
  void         SetInputWhitenStdDevs( const WhitenStdDevsType & stdDevs );
  const WhitenMeansType &   GetInputWhitenMeans( void ) const;
  const WhitenStdDevsType & GetInputWhitenStdDevs( void ) const;
  void         SetOutputWhitenMeans( const WhitenMeansType & means );
  void         SetOutputWhitenStdDevs( const WhitenStdDevsType & stdDevs );
  const WhitenMeansType &   GetOutputWhitenMeans( void ) const;
  const WhitenStdDevsType & GetOutputWhitenStdDevs( void ) const;

  unsigned int GetNumberOfBasis( void ) const;

  double       GetBasisValue( unsigned int basisNum ) const;
  VectorType   GetBasisVector( unsigned int basisNum ) const;
  MatrixType   GetBasisMatrix( void ) const;
  VectorType   GetBasisValues( void ) const;

  typename FeatureImageType::Pointer GetBasisImage( unsigned int num = 0 )
    const;

  void   SetBasisValue( unsigned int basisNum, double value );
  void   SetBasisVector( unsigned int basisNum, const VectorType & vec );
  void   SetBasisMatrix( const MatrixType & mat );
  void   SetBasisValues( const VectorType & values );

  // PDFSegmenter
  typename ProbabilityImageType::Pointer
    GetClassProbabilityImage( unsigned int objectNum ) const;

  typename ProbabilityImageType::Pointer
    GetClassLikelihoodRatioImage( unsigned int objectNum ) const;

  // Ridge, Basis, and PDFSegmenter
  itkSetMacro( RidgeId, ObjectIdType );
  itkGetMacro( RidgeId, ObjectIdType );
  itkSetMacro( BackgroundId, ObjectIdType );
  itkGetMacro( BackgroundId, ObjectIdType );
  itkSetMacro( UnknownId, ObjectIdType );
  itkGetMacro( UnknownId, ObjectIdType );

  itkSetMacro( SeedTolerance, double );
  itkGetMacro( SeedTolerance, double );

  itkSetMacro( Skeletonize, bool );
  itkGetMacro( Skeletonize, bool );

  itkSetMacro( UseIntensityOnly, bool );
  itkGetMacro( UseIntensityOnly, bool );

  itkSetMacro( TrainClassifier, bool );
  itkGetMacro( TrainClassifier, bool );

  // Local
  void   Update();
  void   ClassifyImages();

  typename LabelMapType::Pointer GetOutput( void );

  typename OutputImageType::Pointer GetOutputSeedScales( void );

protected:

  RidgeSeedFilter( void );
  virtual ~RidgeSeedFilter( void );

  void PrintSelf( std::ostream & os, Indent indent ) const;

private:

  RidgeSeedFilter( const Self & );    // Purposely not implemented
  void operator = ( const Self & );      // Purposely not implemented

  // To remove warning "was hidden [-Woverloaded-virtual]"
  void SetInput( const typename Superclass::DataObjectIdentifierType &,
    itk::DataObject * ) {};

  typename RidgeFeatureGeneratorType::Pointer     m_RidgeFeatureGenerator;
  typename SeedFeatureGeneratorType::Pointer      m_SeedFeatureGenerator;
  typename PDFSegmenterType::Pointer              m_PDFSegmenter;

  ObjectIdType   m_RidgeId;
  ObjectIdType   m_BackgroundId;
  ObjectIdType   m_UnknownId;

  double         m_SeedTolerance;

  bool           m_Skeletonize;

  bool           m_UseIntensityOnly;

  bool           m_TrainClassifier;

  typename LabelMapType::Pointer m_LabelMap;

}; // End class RidgeSeedFilter

} // End namespace tube

} // End namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itktubeRidgeSeedFilter.hxx"
#endif

#endif // End !defined( __itktubeRidgeSeedFilter_h )
