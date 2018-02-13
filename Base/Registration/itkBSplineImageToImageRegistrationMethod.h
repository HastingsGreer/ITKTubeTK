/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: ITKHeader.h,v $
  Language:  C++
  Date:      $Date: 2007-07-10 11:35:36 -0400 ( Tue, 10 Jul 2007 ) $
  Version:   $Revision: 0 $

  Copyright ( c ) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkBSplineImageToImageRegistrationMethod_h
#define __itkBSplineImageToImageRegistrationMethod_h

#include "itkImage.h"
#include "itkBSplineDeformableTransform.h"

#include "itkOptimizedImageToImageRegistrationMethod.h"

namespace itk
{

template <class TImage>
class BSplineImageToImageRegistrationMethod
  : public OptimizedImageToImageRegistrationMethod<TImage>
{

public:

  using Self = BSplineImageToImageRegistrationMethod;
  using Superclass = OptimizedImageToImageRegistrationMethod<TImage>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  itkTypeMacro( BSplineImageToImageRegistrationMethod,
                OptimizedImageToImageRegistrationMethod );

  itkNewMacro( Self );

  //
  // Typedefs from Superclass
  //
  using ImageType = TImage;
  static constexpr unsigned int ImageDimension = TImage::ImageDimension;

  // Overrides the superclass' TransformType typedef
  using BSplineTransformType = BSplineDeformableTransform<double,
                                     itkGetStaticConstMacro( ImageDimension ),
                                     itkGetStaticConstMacro( ImageDimension )>;

  using BSplineTransformPointer = typename BSplineTransformType::Pointer;

  using TransformType = BSplineTransformType;

  using ParametersType = typename BSplineTransformType::ParametersType;

  //
  // Methods from Superclass
  //

  virtual void GenerateData( void );

  //
  // Custom Methods
  //

  /**
   * The function performs the casting.  This function should only appear
   * once in the class hierarchy.  It is provided so that member
   * functions that exist only in specific transforms ( e.g., SetIdentity )
   * can be called without the caller having to do the casting. */
  virtual TransformType * GetTypedTransform( void );

  virtual const TransformType * GetTypedTransform( void ) const;

  itkSetMacro( ExpectedDeformationMagnitude, double );
  itkGetConstMacro( ExpectedDeformationMagnitude, double );

  itkSetClampMacro( NumberOfControlPoints, unsigned int, 3, 2000 );
  itkGetConstMacro( NumberOfControlPoints, unsigned int );

  itkSetClampMacro( NumberOfLevels, unsigned int, 1, 5 );
  itkGetConstMacro( NumberOfLevels, unsigned int );

  BSplineTransformPointer GetBSplineTransform( void ) const;

  void ComputeGridRegion( int numberOfControlPoints,
                    typename TransformType::RegionType::SizeType & regionSize,
                    typename TransformType::SpacingType & regionSpacing,
                    typename TransformType::OriginType & regionOrigin,
                    typename TransformType::DirectionType & regionDirection );

  void ResampleControlGrid( int newNumberOfControlPoints,
                            ParametersType & newParameters );

  itkSetMacro( GradientOptimizeOnly, bool );
  itkGetMacro( GradientOptimizeOnly, bool );
protected:

  BSplineImageToImageRegistrationMethod( void );
  virtual ~BSplineImageToImageRegistrationMethod( void );

  using InterpolatorType = InterpolateImageFunction<TImage, double>;
  using MetricType = ImageToImageMetric<TImage, TImage>;

  virtual void Optimize( MetricType * metric, InterpolatorType * interpolator );

  virtual void GradientOptimize( MetricType * metric,
                                 InterpolatorType * interpolator );

  virtual void MultiResolutionOptimize( MetricType * metric,
                                        InterpolatorType * interpolator );

  virtual void PrintSelf( std::ostream & os, Indent indent ) const;

private:

  // Purposely not implemented
  BSplineImageToImageRegistrationMethod( const Self & );
  // Purposely not implemented
  void operator =( const Self & );

  double m_ExpectedDeformationMagnitude;

  unsigned int m_NumberOfControlPoints;

  unsigned int m_NumberOfLevels;

  bool m_GradientOptimizeOnly;

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBSplineImageToImageRegistrationMethod.txx"
#endif

#endif // __ImageToImageRegistrationMethod_h
