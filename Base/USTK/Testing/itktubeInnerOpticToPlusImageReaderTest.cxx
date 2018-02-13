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

#include <itkImageFileWriter.h>
#include <itkArchetypeSeriesFileNames.h>
#include <itkTestingMacros.h>

#include "itktubeInnerOpticToPlusImageReader.h"

int itktubeInnerOpticToPlusImageReaderTest( int argc, char * argv[] )
{
  if( argc < 3 )
    {
    std::cerr << "Missing arguments." << std::endl;
    std::cerr << "Usage: "
              << argv[0]
              << "innerOpticMetadata "
              << "outputImage.mha "
              << std::endl;
    return EXIT_FAILURE;
    }
  const char * innerOpticMetadata = argv[1];
  const char * outputImageFile = argv[2];

  using ReaderType = itk::tube::InnerOpticToPlusImageReader;
  ReaderType::Pointer reader = ReaderType::New();

  TRY_EXPECT_EXCEPTION( reader->Update() );
  reader->SetFileName( innerOpticMetadata );

  using RGBImageType = ReaderType::OutputImageType;

  using WriterType = itk::ImageFileWriter< RGBImageType >;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( outputImageFile );
  writer->SetInput( reader->GetOutput() );
  TRY_EXPECT_NO_EXCEPTION( writer->Update() );

  reader->GetOutput()->Print( std::cout );
  TEST_EXPECT_EQUAL( reader->GetOutput()->GetMetaDataDictionary().GetKeys().size(), 9 );

  reader->SetStartIndex( 3 );
  TEST_EXPECT_EQUAL( reader->GetStartIndex(), 3 );
  reader->SetEndIndex( 4 );
  TEST_EXPECT_EQUAL( reader->GetEndIndex(), 4 );
  reader->SetIncrementIndex( 5 );
  TEST_EXPECT_EQUAL( reader->GetIncrementIndex(), 5 );

  return EXIT_SUCCESS;
}
