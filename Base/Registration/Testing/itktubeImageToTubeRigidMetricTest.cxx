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

#include "itktubeImageToTubeRigidMetric.h"
#include "itktubeSubSampleTubeTreeSpatialObjectFilter.h"

#include <itkImageFileReader.h>
#include <itkSpatialObjectReader.h>

/**
 *  This test exercised the metric evaluation methods in the
 *  itktubeImageToTubeRigidMetric class. The distance between
 *  a 3D binary images ( 32x32x32 ) and a .tre image is computed and check with
 *  the reference for the metric.
 */

int itktubeImageToTubeRigidMetricTest( int argc, char * argv[] )
{
  if( argc < 4 )
    {
    std::cerr << "Missing Parameters: "
              << argv[0]
              << " Input_FixedImage "
              << "Input_SpatialObject "
              << "Input_ExpectedValue."
              << std::endl;
    return EXIT_FAILURE;
    }

  using FloatType = double;
  static const unsigned int ImageDimension = 3;
  static const unsigned int TubeDimension = 3;

  using ImageType = itk::Image< FloatType, ImageDimension >;
  using TubeType = itk::VesselTubeSpatialObject< TubeDimension >;
  using TubeNetType = itk::GroupSpatialObject< TubeDimension >;

  using ImageReaderType = itk::ImageFileReader< ImageType >;
  using TubeNetReaderType = itk::SpatialObjectReader< TubeDimension >;

  using MetricType = itk::tube::ImageToTubeRigidMetric< ImageType, TubeNetType,
    TubeType >;
  using TransformType = MetricType::TransformType;

  // read image ( fixedImage )
  ImageReaderType::Pointer imageReader = ImageReaderType::New();
  imageReader->SetFileName( argv[1] );
  try
    {
    imageReader->Update();
    }
  catch( itk::ExceptionObject & err )
    {
    std::cerr << "Exception caught: " << err << std::endl;
    return EXIT_FAILURE;
    }

  // read tube ( spatialObject )
  TubeNetReaderType::Pointer tubeReader = TubeNetReaderType::New();
  tubeReader->SetFileName( argv[2] );
  try
    {
    tubeReader->Update();
    }
  catch( itk::ExceptionObject & err )
    {
    std::cerr << "Exception caught: " << err << std::endl;
    return EXIT_FAILURE;
    }

  // subsample points in vessel
  using SubSampleTubeNetFilterType = itk::tube::SubSampleTubeTreeSpatialObjectFilter< TubeNetType, TubeType >;
  SubSampleTubeNetFilterType::Pointer subSampleTubeNetFilter =
    SubSampleTubeNetFilterType::New();
  subSampleTubeNetFilter->SetInput( tubeReader->GetGroup() );
  subSampleTubeNetFilter->SetSampling( 30 );
  try
    {
    subSampleTubeNetFilter->Update();
    }
  catch( itk::ExceptionObject & err )
    {
    std::cerr << "Exception caught: " << err << std::endl;
    return EXIT_FAILURE;
    }

  //------------------------------------------------------------------
  // Compute the metric for a 3D image susampled at 1/30
  //------------------------------------------------------------------
  MetricType::Pointer metric = MetricType::New();
  metric->SetExtent( 3 );

  TransformType::Pointer transform = TransformType::New();
  TransformType::ParametersType parameters = transform->GetParameters();

  metric->SetFixedImage( imageReader->GetOutput() );
  metric->SetMovingSpatialObject ( subSampleTubeNetFilter->GetOutput() );
  metric->SetTransform( transform );
  try
    {
    metric->Initialize();
    }
  catch( itk::ExceptionObject &excp )
    {
    std::cerr << "Exception caught while initializing metric." << std::endl;
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  const double epsilonReg = 0.05; // Delta threshold on the measure checking.
  MetricType::MeasureType value = metric->GetValue( parameters );
  if( value < ( std::atof( argv[3] ) - epsilonReg ) ||
      value > ( std::atof( argv[3] ) + epsilonReg ) )
    {
    std::cerr << "Distance value different than expected: "
              << value
              << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
