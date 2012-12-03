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


#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkStatisticsImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageFileReader.h"

#include <boost/ref.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/accumulators/statistics/rolling_mean.hpp>
#include <boost/accumulators/statistics/variance.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/p_square_quantile.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>

#include "tubeMessage.h"
#include "tubeImageQuantilesCLP.h"

enum { TDimensions = 3 };

using namespace std;
using namespace boost;
using namespace boost::accumulators;

typedef itk::Image< float, TDimensions >                  ImageType;
typedef ImageType::IndexType                              ImageIndexType;
typedef ImageType::PixelType                              ImagePixelType;
typedef ImageType::SizeType                               ImageSizeType;
typedef itk::ImageFileReader< ImageType >                 ImageReaderType;
typedef itk::StatisticsImageFilter< ImageType >           StatisticsImageFilterType;
typedef accumulator_set< ImagePixelType,
                         stats<tag::p_square_quantile> >  QuantileAccumulatorType;
typedef itk::ImageRegionConstIterator<ImageType>          ImageIteratorType;


/**
 * Take an image and compute a collection of pixel/voxel quantiles
 * using the BOOST accumulators. The function uses P^2 quantile
 * computation which gives an approximate quantile and is very
 * efficient in terms of storage and runtime complexity. The values
 * are actually never stored.
 */
void computeQuantiles( ImageType::Pointer image,
                       const vector<float> & quantiles,
                       vector<ImagePixelType> & quantileValues)
{
  assert(!quantileValues.size());

  /*
   * Create a and configure a vector of length N of pointers
   * to BOOST accumulators -- Each of the N accumulators will
   * estimate exactly one of the given N desired quantile. If
   * the desired quantile is not within (0,1), throw an exception.
   */
  vector<QuantileAccumulatorType *> accVec;
  BOOST_FOREACH(float q, quantiles)
    {
    if (q <= 0 || q >= 1)
      {
      tube::ErrorMessage("Check quantile range!");
      throw std::exception();
      }
    tube::FmtInfoMessage("Configure accumulator for quantile = %.2f", q);

    QuantileAccumulatorType *acc = new QuantileAccumulatorType(
      quantile_probability = q);
    accVec.push_back(acc);
    }


  /*
   * Use an image iterator to iterate over all pixel/voxel and
   * and then add those values to all the accumulators. Adding
   * the values will incrementally compute the quantile estimates.
   */
  ImageIteratorType imIt( image, image->GetLargestPossibleRegion() );
  imIt.GoToBegin();
  while(!imIt.IsAtEnd())
    {
    ImagePixelType p = imIt.Get();
    BOOST_FOREACH( QuantileAccumulatorType *acc, accVec )
      {
      (*acc)( p );
      }
    ++imIt;
    }


  /*
   * Finally, iterate over the accumulators, query the
   * estimated quantiles and fill the output vector
   */
  BOOST_FOREACH( QuantileAccumulatorType *acc, accVec)
    {
    ImagePixelType qVal = p_square_quantile(*acc);
    quantileValues.push_back(qVal);
    }
  return;
}


/**
 * Writes quantiles and quantile values to a file in JSON
 * format. Example (for quantiles 0.05, 0.5, 0.95):
 *
 *  {
 *     "quantiles":
 *     [
 *        <quantile0>,
 *        <quantile1>,
 *        ...
 *        <quantileN>
 *     ],
 *     "quantileValues":
 *     [
 *        <quantileValue0>,
 *        <quantileValue1>,
 *        ...
 *        <quantileValueN>
 *     ]
 *   }
 */
void writeQuantilesToFile( const vector<float>& quantiles,
                           const vector<ImagePixelType>& quantileValues,
                           const string &outFile )
{
  tube::FmtInfoMessage( "Writing %d quantiles to %s",
        quantiles.size(), outFile.c_str());

  try
    {
    property_tree::ptree root;
    property_tree::ptree quantilesJSON;
    property_tree::ptree quantileValuesJSON;

    for ( int i=0; i<quantiles.size(); ++i )
      {
      property_tree::ptree quantileElementJSON;
      property_tree::ptree quantileValueElementJSON;
      quantileElementJSON.put( "", quantiles[i] );
      quantileValueElementJSON.put( "", quantileValues[i] );
      quantilesJSON.push_back(make_pair( "", quantileElementJSON ) );
      quantileValuesJSON.push_back(make_pair( "", quantileValueElementJSON ) );
      }
    root.add_child( "quantiles", quantilesJSON);
    root.add_child( "quantileValues", quantileValuesJSON );
    property_tree::write_json( outFile, root );
    }
  catch (property_tree::json_parser::json_parser_error &e)
    {
    tube::ErrorMessage( e.message() );
    throw std::exception();
    }
}


int main( int argc, char*argv[] )
{
  PARSE_ARGS;

  tube::FmtInfoMessage("Reading image file %s",
    imageFile.c_str());
  ImageReaderType::Pointer imReader = ImageReaderType::New();
  imReader->SetFileName( imageFile );
  ImageType::Pointer im;

  try
    {
    imReader->Update();
    im = imReader->GetOutput();
    }
  catch (itk::ExceptionObject &ex)
    {
    tube::ErrorMessage( ex.GetDescription() );
    return EXIT_FAILURE;
    }

   vector<float> quantileValues;
   try
    {
    computeQuantiles(im, quantiles, quantileValues);
    writeQuantilesToFile( quantiles, quantileValues, outFile );
    }
  catch (std::exception &e)
    {
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}
