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

#ifndef __itkAngleOfIncidenceImageFilter_h
#define __itkAngleOfIncidenceImageFilter_h

#include "itktubeSymmetricEigenVectorAnalysisImageFilter.h"

#include <itkHessianRecursiveGaussianImageFilter.h>
#include <itkImageRegionIteratorWithIndex.h>
#include <itkImageToImageFilter.h>
#include <itkSymmetricEigenAnalysisImageFilter.h>
#include <itkVectorImage.h>

namespace itk
{
/** \class AngleOfIncidenceImageFilter
 * \brief Computes angle of incidence
 *
 * The angle of incidence is defined as the angle between the beam
 * direction at a organ boundary and the normal to the boundary.
 *
 * \ingroup ImageToImageFilter
 */
template< class TInputImage, class TOutputImage >
class AngleOfIncidenceImageFilter
  : public ImageToImageFilter< TInputImage, TOutputImage >
{
public:
  /** Standard class type alias. */
  using Self = AngleOfIncidenceImageFilter;
  using Superclass = ImageToImageFilter< TInputImage, TOutputImage >;
  using Pointer = SmartPointer< Self >;
  using ConstPointer = SmartPointer< const Self >;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information ( and related methods ). */
  itkTypeMacro( AngleOfIncidenceImageFilter, ImageToImageFilter );

  static constexpr unsigned int ImageDimension = TInputImage::ImageDimension;

  /** Some convenient type alias for input image */
  using InputImageType = TInputImage;
  using InputImagePointer = typename InputImageType::ConstPointer;
  using InputImageRegionType = typename InputImageType::RegionType;
  using InputImagePixelType = typename InputImageType::PixelType;


  /** type alias for the origin type */
  using VectorType = Vector< double, ImageDimension >;

  /* type alias for the output image */
  using OutputImageType = TOutputImage;
  using OutputImagePointer = typename OutputImageType::Pointer;
  using OutputImageRegionType = typename OutputImageType::RegionType;
  using OutputImagePixelType = typename OutputImageType::PixelType;

  /** type alias to generate surface normal vector using eigen analysis */
  using HessianFilterType = typename itk::HessianRecursiveGaussianImageFilter< InputImageType >;

  using SymmetricSecondRankTensorType = SymmetricSecondRankTensor< double, ImageDimension >;
  using SymmetricSecondRankTensorImageType = Image< SymmetricSecondRankTensorType, ImageDimension >;
  using EigenVectorMatrixType = Matrix< double, ImageDimension, ImageDimension >;
  using EigenVectorMatrixImageType = Image< EigenVectorMatrixType, ImageDimension >;
  using EigenValueArrayType = FixedArray< double, ImageDimension >;
  using EigenValueImageType = Image< EigenValueArrayType, ImageDimension >;

  using EigenVectorImageType = itk::VectorImage< double, ImageDimension >;

  using EigenValueAnalysisFilterType = itk::SymmetricEigenAnalysisImageFilter
    <SymmetricSecondRankTensorImageType, EigenValueImageType>;

  using EigenVectorAnalysisFilterType = itk::tube::SymmetricEigenVectorAnalysisImageFilter
        <SymmetricSecondRankTensorImageType, EigenValueImageType,
         EigenVectorMatrixImageType>;

  /** Set/Get Ultrasound origin vector */
  itkSetMacro( UltrasoundProbeOrigin, VectorType );
  itkGetConstMacro( UltrasoundProbeOrigin, VectorType );

protected:
  AngleOfIncidenceImageFilter( void );
  virtual ~AngleOfIncidenceImageFilter( void ) {}
  void PrintSelf( std::ostream & os, Indent indent ) const;

  /* Generate Data */
  void GenerateData( void );

  void ComputeNormalVectorImage( void );
private:
  AngleOfIncidenceImageFilter( const Self & ); //purposely not implemented
  void operator=( const Self & );          //purposely not implemented

  /* Ultrasound origin*/
  VectorType m_UltrasoundProbeOrigin;

  /* Hessian analysis filter */
  typename HessianFilterType::Pointer m_HessianFilter;

  /* eigenvalue analysis filter */
  typename EigenValueAnalysisFilterType::Pointer m_EigenValueAnalysisFilter;

  /* eigenvector analysis filter */
  typename EigenVectorAnalysisFilterType::Pointer m_EigenVectorAnalysisFilter;

  // Primary eigenvector image
  typename EigenVectorImageType::Pointer m_PrimaryEigenVectorImage;

}; // End class AngleOfIncidenceImageFilter

} // End namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkAngleOfIncidenceImageFilter.hxx"
#endif

#endif // End !defined( __itkAngleOfIncidenceImageFilter_h )
