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

#ifndef __itktubePDFSegmenterRandomForest_h
#define __itktubePDFSegmenterRandomForest_h

#include "itktubePDFSegmenterBase.h"

#include "andres/marray.hxx"
#include "andres/ml/decision-trees.hxx"

#include <itkImage.h>
#include <itkListSample.h>

#include <vector>

namespace itk
{

namespace tube
{

template< class TImage, class TLabelMap >
class PDFSegmenterRandomForest
: public PDFSegmenterBase< TImage, TLabelMap >
{
public:

  using Self = PDFSegmenterRandomForest;
  using Superclass = PDFSegmenterBase< TImage, TLabelMap >;
  using Pointer = SmartPointer< Self >;
  using ConstPointer = SmartPointer< const Self >;

  itkTypeMacro( PDFSegmenterRandomForest, PDFSegmenterBase );

  itkNewMacro( Self );

  //
  // Template Args Typesdefs
  //
  using InputImageType = TImage;
  using LabelMapType = TLabelMap;

  static constexpr unsigned int ImageDimension = TImage::ImageDimension;

  //
  // Superclass Typedefs
  //
  typedef typename Superclass::FeatureVectorGeneratorType
                                                 FeatureVectorGeneratorType;
  using FeatureValueType = typename Superclass::FeatureValueType;
  using FeatureVectorType = typename Superclass::FeatureVectorType;
  using FeatureImageType = typename Superclass::FeatureImageType;

  using LabelMapPixelType = typename Superclass::LabelMapPixelType;

  using ObjectIdType = typename Superclass::ObjectIdType;
  using ObjectIdListType = typename Superclass::ObjectIdListType;

  typedef typename Superclass::ProbabilityPixelType
                                                 ProbabilityPixelType;

  typedef typename Superclass::ProbabilityVectorType
                                                 ProbabilityVectorType;
  typedef typename Superclass::ProbabilityImageType
                                                 ProbabilityImageType;

  using VectorDoubleType = typename Superclass::VectorDoubleType;
  using VectorIntType = typename Superclass::VectorIntType;
  using VectorUIntType = typename Superclass::VectorUIntType;

  //
  // Custom Typedefs
  //
  using DecisionForestType = andres::ml::DecisionForest< FeatureValueType, unsigned int,
    ProbabilityPixelType >;

  //
  // Methods
  //
  itkGetMacro( TrainingDataStride, unsigned int );
  itkSetMacro( TrainingDataStride, unsigned int );

  DecisionForestType & GetModel( void );
  void SetModel( DecisionForestType & model );

  itkGetMacro( NumberOfDecisionTrees, unsigned int );
  itkSetMacro( NumberOfDecisionTrees, unsigned int );

  //
  // Must overwrite
  //
  virtual ProbabilityVectorType GetProbabilityVector( const
    FeatureVectorType & fv ) const;

protected:

  PDFSegmenterRandomForest( void );
  virtual ~PDFSegmenterRandomForest( void );

  //
  // Must overwrite
  //
  virtual void GeneratePDFs( void );

  void PrintSelf( std::ostream & os, Indent indent ) const;

private:

  PDFSegmenterRandomForest( const Self & ); // Purposely not implemented
  void operator = ( const Self & );         // Purposely not implemented

  // Superclass type alias
  using ListVectorType = std::vector< ProbabilityPixelType >;
  using ListSampleType = std::vector< ListVectorType >;
  using ClassListSampleType = std::vector< ListSampleType >;

  // Custom type alias
  DecisionForestType            m_Model;

  unsigned int                  m_TrainingDataStride;

  unsigned int                  m_NumberOfDecisionTrees;

}; // End class PDFSegmenterRandomForest

} // End namespace tube

} // End namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itktubePDFSegmenterRandomForest.hxx"
#endif

#endif // End !defined( __itktubePDFSegmenterRandomForest_h )
