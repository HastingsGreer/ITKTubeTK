/*=========================================================================

Library:   TubeTK

Copyright 2010 Kitware Inc. 28 Corporate Drive,
Clifton Park, NY, 12065, USA.

All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

=========================================================================*/

#include "itktubeShrinkUsingMaxImageFilter.h"

#include <itkFilterWatcher.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkImageRegionIteratorWithIndex.h>

int itktubeShrinkUsingMaxImageFilterTest( int argc, char * argv[] )
{
  if( argc != 4 )
    {
    std::cout << "Missing arguments." << std::endl;
    std::cout << "Usage: " << std::endl;
    std::cout << argv[0] << " inputImage outputImage indexImage"
      << std::endl;
    return EXIT_FAILURE;
    }

  // Define the dimension of the images
  enum { Dimension = 2 };

  // Define the pixel type
  typedef float PixelType;

  // Declare the types of the images
  typedef itk::Image< PixelType, Dimension >  ImageType;

  // Declare the reader and writer
  typedef itk::ImageFileReader< ImageType > ReaderType;
  typedef itk::ImageFileWriter< ImageType > WriterType;

  typedef itk::Vector< unsigned int, Dimension >   IndexPixelType;
  typedef itk::Image< IndexPixelType, Dimension >  IndexImageType;
  typedef itk::ImageFileWriter< IndexImageType >   IndexImageWriterType;

  // Declare the type for the Filter
  typedef itk::tube::ShrinkUsingMaxImageFilter< ImageType, ImageType >
    FilterType;

  // Create the reader and writer
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  try
    {
    reader->Update();
    }
  catch( itk::ExceptionObject & e )
    {
    std::cout << "Exception caught during input read:" << std::endl << e
      << std::endl;
    return EXIT_FAILURE;
    }
  ImageType::Pointer inputImage = reader->GetOutput();

  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( inputImage );
  FilterType::ShrinkFactorsType shrinkFactors;
  shrinkFactors.Fill( 4 );
  filter->SetShrinkFactors( shrinkFactors );
  filter->Update();

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( argv[2] );
  writer->SetUseCompression( true );
  writer->SetInput( filter->GetOutput() );
  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & e )
    {
    std::cout << "Exception caught during write:" << std::endl << e
      << std::endl;
    return EXIT_FAILURE;
    }

  IndexImageWriterType::Pointer indexImageWriter =
    IndexImageWriterType::New();
  indexImageWriter->SetFileName( argv[3] );
  indexImageWriter->SetUseCompression( true );
  indexImageWriter->SetInput( filter->GetIndexImage() );
  try
    {
    indexImageWriter->Update();
    }
  catch( itk::ExceptionObject & e )
    {
    std::cout << "Exception caught during image index write:" << std::endl
      << e << std::endl;
    return EXIT_FAILURE;
    }

  // All objects should be automatically destroyed at this point
  return EXIT_SUCCESS;
}