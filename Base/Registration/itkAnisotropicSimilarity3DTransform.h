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
#ifndef __itkAnisotropicSimilarity3DTransform_h
#define __itkAnisotropicSimilarity3DTransform_h

#include <iostream>
#include "itkVersorRigid3DTransform.h"

namespace itk
{

/** \brief AnisotropicSimilarity3DTransform of a vector space ( e.g. space
 * coordinates )
 *
 * This transform applies a rotation, translation and anisotropic scaling
 * to the space.
 *
 * The parameters for this transform can be set either using individual
 * Set methods or in serialized form using SetParameters() and
 * SetFixedParameters().
 *
 * The serialization of the optimizable parameters is an array of 9
 * elements. The first 3 elements are the components of the versor
 * representation of 3D rotation. The next 3 parameters defines the
 * translation in each dimension. The last parameter defines the
 * anisotropic scaling.
 *
 * The serialization of the fixed parameters is an array of 3 elements
 * defining the center of rotation.
 *
 * \ingroup Transforms
 *
 * \sa VersorRigid3DTransform
 */
template <class TScalarType = double>
// Data type for scalars ( float or double )
class AnisotropicSimilarity3DTransform :
  public VersorRigid3DTransform<TScalarType>
{
public:
  /** Standard class type alias. */
  using Self = AnisotropicSimilarity3DTransform;
  using Superclass = VersorRigid3DTransform<TScalarType>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** New macro for creation of through a Smart Pointer. */
  itkNewMacro( Self );

  /** Run-time type information ( and related methods ). */
  itkTypeMacro( AnisotropicSimilarity3DTransform, VersorRigid3DTransform );

  /** Dimension of parameters. */
  static constexpr unsigned int SpaceDimension = 3;
  static constexpr unsigned int InputSpaceDimension = 3;
  static constexpr unsigned int OutputSpaceDimension = 3;
  static constexpr unsigned int ParametersDimension = 9;

  /** Parameters Type   */
  using ParametersType = typename Superclass::ParametersType;
  using JacobianType = typename Superclass::JacobianType;
  using ScalarType = typename Superclass::ScalarType;
  using InputPointType = typename Superclass::InputPointType;
  using OutputPointType = typename Superclass::OutputPointType;
  using InputVectorType = typename Superclass::InputVectorType;
  using OutputVectorType = typename Superclass::OutputVectorType;
  using InputVnlVectorType = typename Superclass::InputVnlVectorType;
  using OutputVnlVectorType = typename Superclass::OutputVnlVectorType;

  typedef typename Superclass::InputCovariantVectorType
                                                 InputCovariantVectorType;
  typedef typename Superclass::OutputCovariantVectorType
                                                 OutputCovariantVectorType;
  using MatrixType = typename Superclass::MatrixType;
  using InverseMatrixType = typename Superclass::InverseMatrixType;
  using CenterType = typename Superclass::CenterType;
  using OffsetType = typename Superclass::OffsetType;
  using TranslationType = typename Superclass::TranslationType;

  /** Versor type. */
  using VersorType = typename Superclass::VersorType;
  using AxisType = typename Superclass::AxisType;
  using AngleType = typename Superclass::AngleType;
  using VectorType = typename Superclass::InputVectorType;
  using ScaleType = TScalarType;

  /** Directly set the rotation matrix of the transform.
   * \warning The input matrix must be orthogonal with isotropic scaling
   * to within a specified tolerance, else an exception is thrown.
   *
   * \sa MatrixOffsetTransformBase::SetMatrix() */
  virtual void SetMatrix( const MatrixType & matrix );
  virtual void SetMatrix( const MatrixType & matrix, const double tolerance );

  /** Set the transformation from a container of parameters This is
   * typically used by optimizers.  There are 7 parameters. The first
   * three represent the versor, the next three represent the translation
   * and the last one represents the scaling factor. */
  void SetParameters( const ParametersType & parameters );

  virtual const ParametersType & GetParameters( void ) const;

  /** Set/Get the value of the isotropic scaling factor */
  void SetScale( ScaleType scale );

  void SetScale( VectorType scale );

  itkGetConstReferenceMacro( Scale, VectorType );

  /** This method computes the Jacobian matrix of the transformation.
   * given point or vector, returning the transformed point or
   * vector. The rank of the Jacobian will also indicate if the
   * transform is invertible at this point. */
  virtual const JacobianType & GetJacobian( const InputPointType  & point )
    const;

  virtual void ComputeJacobianWithRespectToParameters(
    const InputPointType & p, JacobianType & jacobian ) const;

protected:
  AnisotropicSimilarity3DTransform( const MatrixType & matrix,
    const OutputVectorType & offset );
  AnisotropicSimilarity3DTransform( unsigned int paramDim );
  AnisotropicSimilarity3DTransform();
  ~AnisotropicSimilarity3DTransform()
    {
    };

  void PrintSelf( std::ostream & os, Indent indent ) const;

  /** Recomputes the matrix by calling the Superclass::ComputeMatrix() and
   * then applying the scale factor. */
  void ComputeMatrix();

  /** Computes the parameters from an input matrix. */
  void ComputeMatrixParameters();

private:
  // purposely not implemented
  AnisotropicSimilarity3DTransform( const Self & );
  // purposely not implemented
  void operator=( const Self & );

  VectorType           m_Scale;
  mutable JacobianType m_NonThreadsafeSharedJacobian;

}; // class AnisotropicSimilarity3DTransform

}  // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkAnisotropicSimilarity3DTransform.txx"
#endif

#endif /* __itkAnisotropicSimilarity3DTransform_h */
