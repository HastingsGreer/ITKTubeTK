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

#ifndef __itktubeComputeTrainingMaskFilter_h
#define __itktubeComputeTrainingMaskFilter_h

#include <itkImageToImageFilter.h>

#include <itkBinaryThinningImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkBinaryBallStructuringElement.h>
#include <itkDilateObjectMorphologyImageFilter.h>
#include <itkErodeObjectMorphologyImageFilter.h>
#include <itkSubtractImageFilter.h>
#include <itkCastImageFilter.h>
#include <itkMultiplyImageFilter.h>
#include <itkAddImageFilter.h>
#include <itkDivideImageFilter.h>

namespace itk
{

namespace tube
{

/**
 * This class returns expert vessel and not vessel mask.
 *
 * \sa ComputeTrainingMaskFilter
 */

template< class TInputImage >
class ComputeTrainingMaskFilter:
  public ImageToImageFilter< TInputImage,
    itk::Image<short, TInputImage::ImageDimension> >
{
public:
  using Self = ComputeTrainingMaskFilter;
  using Superclass = ImageToImageFilter<TInputImage, TInputImage>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;
  using ImageType = TInputImage;
  using ImageTypeShort = itk::Image< short, ImageType::ImageDimension >;

  static constexpr unsigned int InputImageDimension = TInputImage::ImageDimension;

  itkNewMacro( Self );
  const ImageTypeShort* GetNotVesselMask();
  itkSetMacro( Gap, double );
  itkSetMacro( NotVesselWidth, double );
  itkGetMacro( Gap, double );
  itkGetMacro( NotVesselWidth, double );

protected:
  ComputeTrainingMaskFilter();
  virtual ~ComputeTrainingMaskFilter();
  virtual void GenerateData();
  void PrintSelf( std::ostream & os, Indent indent ) const;

private:
  using BallType = itk::BinaryBallStructuringElement< short,
    ImageType::ImageDimension >;
  using DilateFilterType = itk::DilateObjectMorphologyImageFilter< ImageType, ImageType,
    BallType >;
  using BinaryThinningFilterType = itk::BinaryThinningImageFilter< ImageType, ImageType >;
  using ThresholdFilterType = itk::BinaryThresholdImageFilter< ImageType, ImageType >;
  using SubstractFilterType = itk::SubtractImageFilter<ImageType, ImageType, ImageType>;
  using MultiplyFilterType = itk::MultiplyImageFilter<ImageType, ImageType, ImageType>;
  using DivideFilterType = itk::DivideImageFilter<ImageType, ImageType, ImageType>;
  using AddFilterType = itk::AddImageFilter<ImageType, ImageType, ImageType>;
  using CastFilterType = itk::CastImageFilter< ImageType, ImageTypeShort >;

  ComputeTrainingMaskFilter( const Self& );
  void operator=( const Self& );
  void ApplyDilateMorphologyFilter( typename ImageType::Pointer &input );

  typename AddFilterType::Pointer             m_Add;
  typename ThresholdFilterType::Pointer       m_Threshold;
  typename BinaryThinningFilterType::Pointer  m_BinaryThinning;
  typename DilateFilterType::Pointer          m_Dilate;
  typename SubstractFilterType::Pointer       m_Substract;
  typename MultiplyFilterType::Pointer        m_MultiplyCenterLine;
  typename DivideFilterType::Pointer          m_DivideImage;
  typename CastFilterType::Pointer            m_Cast;
  typename CastFilterType::Pointer            m_CastNotVessel;

  BallType m_Ball;
  double   m_Gap;
  double   m_NotVesselWidth;
};

}//end of tube namespace
}//end of itk namespace

#ifndef ITK_MANUAL_INSTANTIATION
#include "itktubeComputeTrainingMaskFilter.hxx"
#endif

#endif
