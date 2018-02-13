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

#ifndef __itktubePDFSegmenterSVM_h
#define __itktubePDFSegmenterSVM_h

#include "itktubePDFSegmenterBase.h"

#include "svm.h"

#include <itkImage.h>
#include <itkListSample.h>

#include <vector>

namespace itk
{

namespace tube
{

template< class TImage, class TLabelMap >
class PDFSegmenterSVM : public PDFSegmenterBase< TImage, TLabelMap >
{
public:

  using Self = PDFSegmenterSVM;
  using Superclass = PDFSegmenterBase< TImage, TLabelMap >;
  using Pointer = SmartPointer< Self >;
  using ConstPointer = SmartPointer< const Self >;

  itkTypeMacro( PDFSegmenterSVM, PDFSegmenterBase );

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

  //
  // Methods
  //
  itkGetMacro( TrainingDataStride, unsigned int );
  itkSetMacro( TrainingDataStride, unsigned int );

  void   SetSVMClassWeight( unsigned int c, double w );
  void   SetSVMClassWeights( VectorDoubleType & w );
  double GetSVMClassWeight( unsigned int c ) const;
  VectorDoubleType & GetSVMClassWeights( void );

  svm_model * GetModel( void );
  void SetModel( svm_model * model );

  svm_parameter * GetParameter( void );
  void SetParameter( svm_parameter * parameter );

  //
  // Must overwrite
  //
  virtual ProbabilityVectorType GetProbabilityVector( const
    FeatureVectorType & fv ) const;

protected:

  PDFSegmenterSVM( void );
  virtual ~PDFSegmenterSVM( void );

  //
  // Must overwrite
  //
  virtual void GeneratePDFs( void );

  void PrintSelf( std::ostream & os, Indent indent ) const;

private:

  PDFSegmenterSVM( const Self & );       // Purposely not implemented
  void operator = ( const Self & );      // Purposely not implemented

  // Superclass type alias
  using ListSampleType = typename Superclass::ListSampleType;

  // Custom type alias
  svm_model          * m_Model;
  svm_parameter        m_Parameter;

  svm_problem          m_Problem;

  svm_node           * m_Space;

  VectorDoubleType     m_SVMClassWeight;

  unsigned int         m_TrainingDataStride;

}; // End class PDFSegmenterSVM

} // End namespace tube

} // End namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itktubePDFSegmenterSVM.hxx"
#endif

#endif // End !defined( __itktubePDFSegmenterSVM_h )
