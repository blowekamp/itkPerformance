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
#include "itkMultiThreader.h"

#include "itkBarrier.h"
#include "itkFastMutexLock.h"
#include <unistd.h>

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif




void *DoNothing( void *)
{
  return NULL;
}


static itk::Barrier::Pointer barrier;

static itk::FastMutexLock::Pointer mutex;


void *DoSomethingWithABarrier( void *)
{
  barrier->Wait();
  return NULL;
}

void *DoSomethingWithAMutex( void *)
{
  mutex->Lock();
  sleep(0);
  mutex->Unlock();
  return NULL;
}

static int depth = 0;


void *OpenMPRecusiveTest( void *)
{

  itk::MultiThreader::Pointer multiThreader = itk::MultiThreader::New();

  multiThreader->SetNumberOfThreads( 2 );
  multiThreader->SetSingleMethod( OpenMPRecusiveTest, NULL );
  multiThreader->SingleMethodExecute();

}


int itkOpenMPTourture( int argc, char *argv[] )
{



  itk::MultiThreader::Pointer multiThreader = itk::MultiThreader::New();

  itk::TimeProbe t1;

  for ( unsigned int i = 0; i < 100; ++i )
    {
    t1.Start();
    for ( unsigned int j = 1; j < multiThreader->GetGlobalMaximumNumberOfThreads(); ++j )
      {
      const unsigned int numberOfThreads = j;
      multiThreader->SetNumberOfThreads( numberOfThreads );
      multiThreader->SetSingleMethod( DoNothing, NULL );
      multiThreader->SingleMethodExecute();
      }

    t1.Stop();
    }

  std::cout << "MultiThreaded Average Time: " << t1.GetMeanTime()/100.0 << t1.GetUnit() << std::endl;


  itk::TimeProbe t2;
  for ( unsigned int i = 0; i < 100; ++i )
    {
    t2.Start();
    for ( unsigned int j = 1; j <multiThreader->GetGlobalMaximumNumberOfThreads(); ++j )
      {


      const unsigned int numberOfThreads = j;

      barrier = itk::Barrier::New();
      barrier->Initialize( numberOfThreads );

      multiThreader->SetNumberOfThreads( numberOfThreads );
      multiThreader->SetSingleMethod( DoSomethingWithABarrier, NULL );
      multiThreader->SingleMethodExecute();
      }

    t2.Stop();
    }

  std::cout << "MultiThreaded with Barrier Average Time: " << t2.GetMeanTime()/100.0 << t2.GetUnit() << std::endl;


  itk::TimeProbe t3;

  for ( unsigned int i = 0; i < 100; ++i )
    {
    t3.Start();
    for ( unsigned int j = 1; j < multiThreader->GetGlobalMaximumNumberOfThreads(); ++j )
      {


      const unsigned int numberOfThreads = j;

      mutex = itk::FastMutexLock::New();

      multiThreader->SetNumberOfThreads( numberOfThreads );
      multiThreader->SetSingleMethod( DoSomethingWithAMutex, NULL );
      multiThreader->SingleMethodExecute();
      }

    t3.Stop();
    }

  std::cout << "MultiThreaded with Mutex Average Time: " << t3.GetMeanTime()/100.0 << t3.GetUnit() << std::endl;



//   for ( unsigned int j = 1; j < multiThreader->GetGlobalMaximumNumberOfThreads(); ++j )
//     {
//     const unsigned int numberOfThreads = j;

//     multiThreader->SetNumberOfThreads( numberOfThreads );
//     multiThreader->SetSingleMethod( OpenMPRecusiveTest, NULL );
//     multiThreader->SingleMethodExecute();
//     }


  return EXIT_SUCCESS;
}
