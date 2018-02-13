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
#ifndef __itktubeGPUArrayFireGaussianDerivativeFilter_h
#define __itktubeGPUArrayFireGaussianDerivativeFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkParametricImageSource.h"
#include "itkSize.h"

#include "itktubeGaussianDerivativeFilter.h"
#include "itkFFTShiftImageFilter.h"

#include <arrayfire.h>

namespace itk
{
namespace tube
{

template< typename TInputImage, typename TOutputImage =
Image< float, TInputImage::ImageDimension >  >
class GPUArrayFireGaussianDerivativeFilter :
  public GaussianDerivativeFilter< TInputImage, TOutputImage >
{
public:

  using Self = GPUArrayFireGaussianDerivativeFilter;
  using Superclass = GaussianDerivativeFilter< TInputImage,
          TOutputImage >;
  using Pointer = SmartPointer< Self >;
  using ConstPointer = SmartPointer< const Self >;

  itkNewMacro ( Self );

  itkTypeMacro ( GPUArrayFireGaussianDerivativeFilter,
                 GaussianDerivativeFilter );

  static constexpr unsigned int ImageDimension = TInputImage::ImageDimension;

  using InputImageType = TInputImage;
  using OutputImageType = TOutputImage;
  using RealImageType = Image< double, ImageDimension >;
  using RealImagePointerType = typename RealImageType::Pointer;
  using GaussianDerivativeImageSourceType = GaussianDerivativeImageSource< RealImageType >;
  using OrdersType = typename Superclass::OrdersType;
  using SigmasType = typename Superclass::SigmasType;

  void GenerateNJet ( typename OutputImageType::Pointer & D,
                      std::vector< typename TOutputImage::Pointer > & Dx,
                      std::vector< typename TOutputImage::Pointer > & Dxx );

protected:

  using FFTShiftFilterType = FFTShiftImageFilter< RealImageType, RealImageType >;

  GPUArrayFireGaussianDerivativeFilter ( void );
  virtual ~GPUArrayFireGaussianDerivativeFilter ( void ) {}

  void ComputeInputImageFFT();
  void ComputeKernelImageFFT();
  void ComputeConvolvedImageFFT();
  void ComputeConvolvedImage();

  void GenerateData();

  void PrintSelf ( std::ostream & os, Indent indent ) const;

private:
  // Purposely not implemented
  GPUArrayFireGaussianDerivativeFilter ( const Self & );
  void operator = ( const Self & );

  typename RealImageType::Pointer                     m_PaddedInputImage;

  af::array                                           m_InputImageFFTAfArr;

  af::array                                           m_GaussianFFTAfArr;

  af::array                                           m_KernelImageFFTAfArr;

  af::array                                           m_ConvolvedImageFFTAfArr;

  typename TOutputImage::Pointer                      m_ConvolvedImage;

  const InputImageType *                              m_LastInputImage;
};


// End class GPUArrayFireGaussianDerivativeFilter

} // End namespace tube

} // End namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itktubeGPUArrayFireGaussianDerivativeFilter.hxx"
#endif

#endif
