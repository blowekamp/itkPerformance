itk_module_test()

set(ITK-PerformanceTestTests
  itkOpenMPAdd.cxx
  itkOpenMPTourture.cxx
)

CreateTestDriver_SupportBuildInIOFactories(ITK-PerformanceTest  "${ITK-PerformanceTest-Test_LIBRARIES}" "${ITK-PerformanceTestTests}")

#-------------------------------------------------------------------------
itk_add_test(NAME itkOpenMPAddPerformance
      COMMAND ITK-PerformanceTestTestDriver itkOpenMPAdd)

itk_add_test(NAME itkOpenMPTourture
      COMMAND ITK-PerformanceTestTestDriver itkOpenMPTourture)
