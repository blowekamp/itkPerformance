/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing,
 *  software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 *  implied.
 *  See the License for the specific language governing permissions
 *  and
 *  limitations under the License.
 *
 *=========================================================================*/
#include "itkTimeProbe.h"
#include "itkImage.h"
#include "itkAddImageFilter.h"

#if defined(_MSC_VER) 
#pragma warning ( disable : 4786 )
#endif


template <class TImage >
static void AddPerformance( typename TImage::SizeType &size )
{

  std::cout << "--- For image size: " << size << " ---" << std::endl;

  typedef typename TImage::PixelType PixelType;
  typedef TImage                     ImageType;
  typename ImageType::RegionType     region;


  typename ImageType::IndexType index;
  index.Fill(0);
  region.SetSize(size);
  region.SetIndex(index);
  itk::TimeProbe t;

  for ( unsigned int i = 0; i < 100; ++i )
    {


    typename ImageType::Pointer image1 = ImageType::New();
    image1->SetRegions( region );
    image1->Allocate();
    image1->FillBuffer(12);

    typename ImageType::Pointer image2 = ImageType::New();
    image2->SetRegions( region );
    image2->Allocate();
    image2->FillBuffer(9);



    typedef itk::AddImageFilter< ImageType, ImageType, ImageType> FilterType;

    typename FilterType::Pointer filter = FilterType::New();
    filter->SetInput1( image1 );
    filter->SetInput2( image2 );

    t.Start();

    filter->Update();

    t.Stop();
    }

  const double addsPerUnit = 1.0/t.GetMeanTime();


  std::cout << "Added " << addsPerUnit << " images per " << t.GetUnit() << std::endl;



}



int itkOpenMPAdd( int argc, char *argv[] )
{

  typedef itk::Image<char, 3 > ImageType3D;
  ImageType3D::SizeType size3d;



  size3d.Fill( 512 );
  AddPerformance<ImageType3D>( size3d );


  size3d.Fill( 256 );
  AddPerformance<ImageType3D>( size3d );


  size3d.Fill( 128 );
  AddPerformance<ImageType3D>( size3d );

  typedef itk::Image<char, 2 > ImageType2D;
  ImageType2D::SizeType size2d;

  size2d.Fill( 64 );
  AddPerformance<ImageType2D>( size2d );

  size2d.Fill( 32 );
  AddPerformance<ImageType2D>( size2d );

  return EXIT_SUCCESS;
}
