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

#ifndef __itktubeImageToTubeRigidMetric_h
#define __itktubeImageToTubeRigidMetric_h

#include <itkEuler3DTransform.h>
#include <itkCompensatedSummation.h>
#include <itkGaussianDerivativeImageFunction.h>
#include <itkImageToSpatialObjectMetric.h>

namespace itk
{

namespace tube
{

/** \class ImageToTubeRigidMetric
 * \brief Computes similarity between two objects to be registered
 * The metric implemented here corresponds to the following paper:
 * \link
 * http://www.cs.unc.edu/Research/MIDAG/pubs/papers/MICCAI01-aylwardVReg.pdf
 * The metric is based on the fact that vessel centerlines are scaled
 * intensity ridges in the image.
 *
 * \tparam TFixedImage Type of the Image to register against.
 * \tparam TMovingSpatialObject Type of the SpatialObject to register with,
 * could be a Tube, Group, etc.
 * \tparam TTubeSpatialObject Type of the tubes contained within the input
 * TMovingSpatialObject to use for the registration.
 *
 * \warning ( Derivative )
 */

template< class TFixedImage,
          class TMovingSpatialObject,
          class TTubeSpatialObject >
class ImageToTubeRigidMetric
  : public ImageToSpatialObjectMetric< TFixedImage, TMovingSpatialObject >
{
public:
  /** Standard class type alias. */
  using Self = ImageToTubeRigidMetric;
  using Superclass = ImageToSpatialObjectMetric< TFixedImage, TMovingSpatialObject >;
  using Pointer = SmartPointer< Self >;
  using ConstPointer = SmartPointer< const Self >;

  /**  Dimension of the image and tube.  */
  static constexpr unsigned int ImageDimension = TFixedImage::ImageDimension;
  static constexpr unsigned int TubeDimension = TTubeSpatialObject::ObjectDimension;

  using FixedImageType = TFixedImage;
  using TubeTreeType = TMovingSpatialObject;
  using TubeType = TTubeSpatialObject;
  using TubePointType = typename TubeType::TubePointType;

  using ScalarType = double;
  using DerivativeImageFunctionType = GaussianDerivativeImageFunction< TFixedImage >;
  using DerivativeType = typename Superclass::DerivativeType;
  using ParametersType = typename Superclass::ParametersType;
  using MeasureType = typename Superclass::MeasureType;

  /** Run-time type information ( and related methods ). */
  itkTypeMacro( ImageToTubeRigidMetric, ImageToSpatialObjectMetric );

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  inline unsigned int GetNumberOfParameters( void ) const
    {
    return this->m_Transform->GetNumberOfParameters();
    }

  /** Type used for representing point components  */
  typedef typename Superclass::CoordinateRepresentationType
                                               CoordinateRepresentationType;

  /** Type definition for the size */
  using SizeType = typename TFixedImage::SizeType;

  /** Type definition for the pixel type */
  using PixelType = typename TFixedImage::PixelType;

  /**  Type of the Transform Base class */
  using TransformType = Euler3DTransform< ScalarType >;
  using TransformPointer = typename TransformType::Pointer;
  using InputPointType = typename TransformType::InputPointType;
  using OutputPointType = typename TransformType::OutputPointType;
  using TransformParametersType = typename TransformType::ParametersType;
  using TransformJacobianType = typename TransformType::JacobianType;
  using FeatureWeightsType = TransformType::ParametersType;

  /** Get the Derivatives of the Match Measure */
  const DerivativeType & GetDerivative( const ParametersType &
    parameters ) const;
  void GetDerivative( const ParametersType & parameters,
    DerivativeType & derivative ) const;

  /** Get the Value for SingleValue Optimizers */
  MeasureType  GetValue( const ParametersType & parameters ) const;

  /** Get Value and Derivatives for MultipleValuedOptimizers */
  void GetValueAndDerivative( const ParametersType & parameters,
    MeasureType & Value, DerivativeType  & Derivative ) const;

  /** Initialize the metric */
  void Initialize( void ) throw ( ExceptionObject );

  /** Control the radius scaling of the metric. */
  itkSetMacro( Kappa, ScalarType );
  itkGetConstMacro( Kappa, ScalarType );

  /** Set/Get the minimum scaling radius. */
  itkSetMacro( MinimumScalingRadius, ScalarType );
  itkGetConstMacro( MinimumScalingRadius, ScalarType );

  /** Set/Get the extent of the blurring calculation given in Gaussian
   * sigma's. */
  itkSetMacro( Extent, ScalarType );
  itkGetConstMacro( Extent, ScalarType );

  /** Set/Get the scalar weights associated with every point in the tube.
   * The index of the point weights should correspond to "standard tube tree
   * interation". */
  void SetFeatureWeights( FeatureWeightsType & featureWeights );
  itkGetConstReferenceMacro( FeatureWeights, FeatureWeightsType )

  TransformPointer GetTransform( void ) const
    {
    return dynamic_cast<TransformType*>( this->m_Transform.GetPointer() );
    }

  /** Downsample the tube points by this integer value. */

protected:
  ImageToTubeRigidMetric( void );
  virtual ~ImageToTubeRigidMetric( void );

  using VectorType = Vector< ScalarType, TubeDimension >;
  using MatrixType = Matrix< ScalarType, TubeDimension, TubeDimension >;

  using PointType = typename TubePointType::PointType;
  using VnlVectorType = vnl_vector< ScalarType >;
  using VnlMatrixType = vnl_matrix< ScalarType >;
  using CompensatedSummationType = CompensatedSummation< ScalarType >;

  virtual void ComputeCenterOfRotation( void );
  SizeValueType CountTubePoints( void );

  void GetDeltaAngles( const OutputPointType & x,
    const VnlVectorType & dx,
    const VectorType & offsets,
    ScalarType angle[3] ) const;

private:
  ImageToTubeRigidMetric( const Self& ); // purposely not implemented
  void operator=( const Self& ); // purposely not implemented

  typename DerivativeImageFunctionType::Pointer m_DerivativeImageFunction;

  ScalarType m_Kappa;
  ScalarType m_MinimumScalingRadius;
  ScalarType m_Extent;

  /** The center of rotation of the weighted tube points. */
  using CenterOfRotationType = PointType;
  CenterOfRotationType m_CenterOfRotation;

  /** Test whether the specified tube point is inside the Image.
   * \param inputPoint the non-transformed tube point.
   * \param outputPoint the transformed tube point.
   * \param transform the transform to apply to the input point. */
  bool IsInside( const InputPointType & inputPoint,
    OutputPointType & outputPoint,
    const TransformType * transform ) const;

  ScalarType ComputeLaplacianMagnitude(
    const typename TubePointType::CovariantVectorType & tubeNormal,
    const ScalarType scale,
    const OutputPointType & currentPoint ) const;
  ScalarType ComputeThirdDerivatives(
    const VectorType & v,
    const ScalarType scale,
    const OutputPointType & currentPoint ) const;

  /**
   * \warning User is responsible for freeing the list, but not the elements
   * of the list.
   */
  typename TubeTreeType::ChildrenListType* GetTubes( void ) const;

  FeatureWeightsType m_FeatureWeights;
}; // End class ImageToTubeRigidMetric

} // End namespace tube

} // End namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itktubeImageToTubeRigidMetric.hxx"
#endif

#endif // End !defined( __itktubeImageToTubeRigidMetric_h )
