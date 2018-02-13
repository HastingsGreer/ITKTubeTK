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

#ifndef __itktubeTubeSpatialObjectToImageFilter_hxx
#define __itktubeTubeSpatialObjectToImageFilter_hxx

#include "itktubeTubeSpatialObjectToImageFilter.h"

#include <itkImageRegionIteratorWithIndex.h>

#include <vnl/vnl_vector.h>

/** Constructor */
template< unsigned int ObjectDimension, class TOutputImage,
  class TRadiusImage, class TTangentImage >
TubeSpatialObjectToImageFilter< ObjectDimension, TOutputImage, TRadiusImage,
  TTangentImage>
::TubeSpatialObjectToImageFilter( void )
{
  m_UseRadius = false;
  m_Cumulative = false;
  m_BuildRadiusImage = false;
  m_BuildTangentImage = false;
  m_FallOff = 0.0;

  // This is a little bit tricky since the 2nd an 3rd outputs are
  //   not always computed
  this->SetNumberOfRequiredOutputs( 3 );
  m_RadiusImage = TRadiusImage::New();
  this->SetNthOutput( 1, m_RadiusImage );

  m_TangentImage = TTangentImage::New();
  this->SetNthOutput( 2, m_TangentImage );
}

/** Destructor */
template< unsigned int ObjectDimension, class TOutputImage,
  class TRadiusImage, class TTangentImage >
TubeSpatialObjectToImageFilter< ObjectDimension, TOutputImage, TRadiusImage,
  TTangentImage >
::~TubeSpatialObjectToImageFilter( void )
{
}

/** Return the Radius Image */
template< unsigned int ObjectDimension, class TOutputImage,
  class TRadiusImage, class TTangentImage >
typename TubeSpatialObjectToImageFilter< ObjectDimension, TOutputImage,
  TRadiusImage, TTangentImage >::RadiusImagePointer
TubeSpatialObjectToImageFilter< ObjectDimension, TOutputImage, TRadiusImage,
  TTangentImage>
::GetRadiusImage( void )
{
  return dynamic_cast< TRadiusImage * >( this->ProcessObject::GetOutput( 1 ) );
}

/** Return the tangent Image */
template< unsigned int ObjectDimension, class TOutputImage,
  class TRadiusImage, class TTangentImage >
typename TubeSpatialObjectToImageFilter< ObjectDimension, TOutputImage,
  TRadiusImage, TTangentImage >::TangentImagePointer
TubeSpatialObjectToImageFilter< ObjectDimension, TOutputImage, TRadiusImage,
  TTangentImage >
::GetTangentImage( void )
{
  return dynamic_cast< TTangentImage * >(
    this->ProcessObject::GetOutput( 2 ) );
}

/** Update */
template< unsigned int ObjectDimension, class TOutputImage,
  class TRadiusImage, class TTangentImage >
void
TubeSpatialObjectToImageFilter< ObjectDimension, TOutputImage, TRadiusImage,
  TTangentImage >
::GenerateData( void )
{
  itkDebugMacro( << "TubeSpatialObjectToImageFilter::Update() called." );

  //Get the input and output pointers
  const typename SuperClass::InputSpatialObjectType   * InputTube =
    this->GetInput();
  typename SuperClass::OutputImagePointer             OutputImage =
    this->GetOutput();

  // Generate the image
  typename OutputImageType::RegionType region;
  if( this->m_Size[0] == 0 )
    {
    std::cout << "WARNING: Size not set." << std::endl;
    std::cout << "   Reverting to an incorrect method to compute region."
      << std::endl;
    SizeType size;

    typename SuperClass::InputSpatialObjectType::BoundingBoxType::PointType
      maxPoint;
    maxPoint = InputTube->GetBoundingBox()->GetMaximum();

    typename OutputImageType::PointType   physicalSize;

    unsigned int buffer = 4;

    for( unsigned int i=0; i<ObjectDimension; i++ )
      {
      maxPoint[i] = maxPoint[i] *
                    ( InputTube->GetIndexToObjectTransform() )
                      ->GetScaleComponent()[i];
      physicalSize[i] = maxPoint[i] - this->m_Origin[i];

      /** Get the origin point within the image so that the object
       * remains in the image **/
      size[i] = ( long unsigned int )(
        physicalSize[i] / this->m_Spacing[i] ) + buffer;
      }
    region.SetSize( size );
    }
  else
    {
    region.SetSize( this->m_Size );
    }

  typename OutputImageType::IndexType index;
  index.Fill( 0 );
  region.SetIndex( index );

  OutputImage->SetRegions( region );
  OutputImage->SetSpacing( this->m_Spacing );
  OutputImage->SetOrigin( this->m_Origin );
  OutputImage->SetDirection( this->m_Direction );
  OutputImage->Allocate();
  OutputImage->FillBuffer( 0 );

  using ContinuousIndexType = itk::ContinuousIndex<double, ObjectDimension>;
  ContinuousIndexType point;

  m_RadiusImage = this->GetRadiusImage();
  //Build radius image for processing
  if( m_BuildRadiusImage )
    {
    m_RadiusImage->SetRegions( region );
    m_RadiusImage->SetSpacing( this->m_Spacing );
    m_RadiusImage->SetOrigin( this->m_Origin );
    m_RadiusImage->SetDirection( this->m_Direction );
    m_RadiusImage->Allocate();
    m_RadiusImage->FillBuffer( 0 );
    }

  m_TangentImage = this->GetTangentImage();
  //Build radius image for processing
  if( m_BuildTangentImage )
    {
    m_TangentImage->SetRegions( region );
    m_TangentImage->SetSpacing( this->m_Spacing );
    m_TangentImage->SetOrigin( this->m_Origin );
    m_TangentImage->SetDirection( this->m_Direction );
    m_TangentImage->Allocate();
    TangentPixelType v;
    v.Fill( 0 );
    m_TangentImage->FillBuffer( v );
    }

  // Get the list of tubes
  char tubeName[] = "Tube";
  ChildrenListType* tubeList = InputTube->GetChildren(
    this->m_ChildrenDepth, tubeName );

  //int size = tubeList->size();

  using ChildrenIteratorType = typename ChildrenListType::iterator;
  ChildrenIteratorType TubeIterator = tubeList->begin();

  typename OutputImageType::IndexType index2;

  while( TubeIterator != tubeList->end() )
    {
    TubeType * tube = ( TubeType * )TubeIterator->GetPointer();

    tube->ComputeObjectToWorldTransform();

    typename TubeType::TransformType * tubeIndexPhysTransform =
      tube->GetIndexToWorldTransform();

    // Force the computation of the tangents
    if( m_BuildTangentImage )
      {
      tube->RemoveDuplicatePoints();
      tube->ComputeTangentAndNormals();
      }

    for( unsigned int k=0; k < tube->GetNumberOfPoints(); k++ )
      {
      using TubePointType = typename TubeType::TubePointType;
      const TubePointType* tubePoint = static_cast<const TubePointType*>(
        tube->GetPoint( k ) );
      OutputImage->TransformPhysicalPointToContinuousIndex(
        tubeIndexPhysTransform->TransformPoint( tubePoint->GetPosition() ),
        point );
      for( unsigned int i=0; i<ObjectDimension; i++ )
        {
        index[i] = ( long int )( point[i]+0.5 );
        }
      bool IsInside = OutputImage->GetLargestPossibleRegion().IsInside( index );

      if( IsInside )
        {
        // Density Image
        if( m_Cumulative )
          {
          OutputImage->SetPixel( index, OutputImage->GetPixel( index )+1 );
          }
        else
          {
          OutputImage->SetPixel( index, 1 );
          }

        // Tangent Image
        if( m_BuildTangentImage )
          {
          // Convert the tangent type to the actual tangent image pixel type
          typename TubeType::VectorType t = tubePoint->GetTangent();
          TangentPixelType tp;
          for( unsigned int tpind = 0;tpind<ObjectDimension;tpind++ )
            {
            tp[tpind] = t[tpind];
            }
          m_TangentImage->SetPixel( index, tp );
          }

        // Radius Image and Density image with radius
        if( m_UseRadius )
          {
          typename TubePointType::PointType radius, zero;
          radius.Fill( tubePoint->GetRadius() );
          zero.Fill( 0 );
          // Convert to an index vector, working around the lack of an
          // appropriate transformation function
          ContinuousIndexType cix_radius, cix_zero;
          OutputImage->TransformPhysicalPointToContinuousIndex(
            tubeIndexPhysTransform->TransformPoint( radius ),
            cix_radius );
          OutputImage->TransformPhysicalPointToContinuousIndex(
            tubeIndexPhysTransform->TransformPoint( zero ),
            cix_zero );
          itk::Vector<double, ObjectDimension> v_radius = cix_radius - cix_zero;

          // This is inherently broken for anisotropic images since
          // the radius image is scalar-valued
          double phys_pt_radius = tubePoint->GetRadius() *
                                      tube
                                      ->GetIndexToObjectTransform()
                                      ->GetScaleComponent()[0];
          if( m_BuildRadiusImage )
            {
            m_RadiusImage->SetPixel( index,
                                  static_cast<RadiusPixelType>(
                                      phys_pt_radius ) );
            }

          double step[ObjectDimension];
          for( unsigned int i = 0; i < ObjectDimension; i++ )
            {
            double s = v_radius[i] / 2;

            while( s > 1 )
              {
              s /= 2;
              }
            if( s < 0.5 )
              {
              s = 0.5;
              }

            step[i] = s;
            }

          if( ObjectDimension == 2 )
            {
            for( double x=-v_radius[0]; x<=v_radius[0]+step[0]/2; x+=step[0] )
              {
              for( double y=-v_radius[1]; y<=v_radius[1]+step[1]/2; y+=step[1] )
                {
                double xr = x / v_radius[0], yr = y / v_radius[1];
                if( ( ( xr*xr ) +( yr*yr ) ) <= 1 )
                  // test  inside the sphere
                  {
                  index2[0]=( long )( point[0]+x+0.5 );
                  index2[1]=( long )( point[1]+y+0.5 );
                  if( OutputImage->GetLargestPossibleRegion().IsInside(
                    index2 ) )
                    {
                    using PixelType = typename OutputImageType::PixelType;
                    if( m_Cumulative )
                      {
                      OutputImage->SetPixel( index2,
                                            ( PixelType )( OutputImage
                                                        ->GetPixel( index2 )
                                            + 0.5 ) );
                      }
                    else
                      {
                      OutputImage->SetPixel( index2, 1 );
                      }
                    if( m_BuildRadiusImage )
                      {
                      m_RadiusImage->SetPixel( index2, phys_pt_radius );
                      }
                    }
                  }
                }
              }
            }
          else if( ObjectDimension == 3 )
            {
            for( double x=-v_radius[0]; x<=v_radius[0]+step[0]/2; x+=step[0] )
              {
              for( double y=-v_radius[1]; y<=v_radius[1]+step[1]/2; y+=step[1] )
                {
                for( double z=-v_radius[2]; z<=v_radius[2]+step[2]/2;
                  z+=step[2] )
                  {
                  double xr = x / v_radius[0];
                  double yr = y / v_radius[1];
                  double zr = z / v_radius[2];
                  if( ( ( xr*xr ) +( yr*yr ) +( zr*zr ) ) <= 1 )
                    // test  inside the sphere
                    {
                    index2[0]=( long )( point[0]+x+0.5 );
                    index2[1]=( long )( point[1]+y+0.5 );
                    index2[2]=( long )( point[2]+z+0.5 );

                    // Test that point is within the output image boundries
                    if( OutputImage->GetLargestPossibleRegion().IsInside(
                      index2 ) )
                      {
                      OutputImage->SetPixel( index2, 1 );
                      if( m_BuildRadiusImage )
                        {
                        m_RadiusImage->SetPixel( index2, phys_pt_radius );
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    ++TubeIterator;
    }

  delete tubeList;

  itkDebugMacro( << "TubeSpatialObjectToImageFilter::Update() finished." );

} // End update function

#endif // End !defined( __itktubeTubeSpatialObjectToImageFilter_hxx )
