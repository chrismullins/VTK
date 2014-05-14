/*
 * Copyright 2008 Sandia Corporation.
 * Under the terms of Contract DE-AC04-94AL85000, there is a non-exclusive
 * license for use of this work by or on behalf of the
 * U.S. Government. Redistribution and use in source and binary forms, with
 * or without modification, are permitted provided that this Notice and any
 * statement of authorship are reproduced on all copies.
 */
// .SECTION Thanks
// Thanks to Philippe Pebay and David Thompson from Sandia National Laboratories
// for implementing this test.

#include "vtkDataObjectCollection.h"
#include "vtkDoubleArray.h"
#include "vtkMath.h"
#include "vtkMultiBlockDataSet.h"
#include "vtkStringArray.h"
#include "vtkSmartPointer.h"
#include "vtkTable.h"
#include "vtkTimerLog.h"
#include "vtkDescriptiveStatistics.h"

//=============================================================================
int main( int, char *[] )
{
  int testStatus = 0;

  // ************** Test with 3 columns of input data **************

  // Input data
  double mingledData[] =
    {
      46,
      45,
      47,
      49,
      46,
      47,
      46,
      46,
      47,
      46,
      47,
      49,
      49,
      49,
      47,
      45,
      50,
      50,
      46,
      46,
      51,
      50,
      48,
      48,
      52,
      54,
      48,
      47,
      52,
      52,
      49,
      49,
      53,
      54,
      50,
      50,
      53,
      54,
      50,
      52,
      53,
      53,
      50,
      51,
      54,
      54,
      49,
      49,
      52,
      52,
      50,
      51,
      52,
      52,
      49,
      47,
      48,
      48,
      48,
      50,
      46,
      48,
      47,
      47,
    };

  // Test with entire data set
  int nVals1 = 32;

  vtkSmartPointer<vtkDoubleArray> dataset1Arr =
    vtkSmartPointer<vtkDoubleArray>::New();
  dataset1Arr->SetNumberOfComponents( 1 );
  dataset1Arr->SetName( "Metric 0" );

  vtkSmartPointer<vtkDoubleArray> dataset2Arr =
    vtkSmartPointer<vtkDoubleArray>::New();
  dataset2Arr->SetNumberOfComponents( 1 );
  dataset2Arr->SetName( "Metric 1" );

  vtkSmartPointer<vtkDoubleArray> dataset3Arr =
    vtkSmartPointer<vtkDoubleArray>::New();
  dataset3Arr->SetNumberOfComponents( 1 );
  dataset3Arr->SetName( "Metric 2" );

  for ( int i = 0; i < nVals1; ++ i )
    {
    int ti = i << 1;
    dataset1Arr->InsertNextValue( mingledData[ti] );
    dataset2Arr->InsertNextValue( mingledData[ti + 1] );
    dataset3Arr->InsertNextValue( -1. );
    }

  vtkSmartPointer<vtkTable> datasetTable1 =
    vtkSmartPointer<vtkTable>::New();
  datasetTable1->AddColumn( dataset1Arr );
  datasetTable1->AddColumn( dataset2Arr );
  datasetTable1->AddColumn( dataset3Arr );

  // Pairs of interest
  int nMetrics = 3;
  vtkStdString columns[] =
    {
      "Metric 1",
      "Metric 2",
      "Metric 0"
    };

  // Reference values
  // Means for metrics 0, 1, and 2, respectively
  double means1[] = { 49.21875 , 49.5, -1. };

  // Standard deviations for metrics 0, 1, and 2, respectively
  double stdevs1[] = { sqrt( 5.9828629 ), sqrt( 7.548397 ), 0. };

  // Set descriptive statistics algorithm and its input data port
  vtkSmartPointer<vtkDescriptiveStatistics> ds1 =
    vtkSmartPointer<vtkDescriptiveStatistics>::New();

  // First verify that absence of input does not cause trouble
  std::cout << "\n## Verifying that absence of input does not cause trouble... ";
  ds1->Update();
  std::cout << "done.\n";

  // Prepare first test with data
  ds1->SetInputData( vtkStatisticsAlgorithm::INPUT_DATA, datasetTable1 );

  // Select Columns of Interest
  for ( int i = 0; i< nMetrics; ++ i )
    {
    ds1->AddColumn( columns[i] );
    }

  // Test Learn, Derive, Test, and Assess options
  ds1->SetLearnOption( true );
  ds1->SetDeriveOption( true );
  ds1->SetAssessOption( true );
  ds1->SetTestOption( true );
  ds1->SignedDeviationsOff();
  ds1->Update();

  // Get output data and meta tables
  vtkSmartPointer<vtkTable> outputData1 =
    ds1->GetOutput( vtkStatisticsAlgorithm::OUTPUT_DATA );
  vtkSmartPointer<vtkMultiBlockDataSet> outputMetaDS1 =
    vtkMultiBlockDataSet::SafeDownCast( ds1->GetOutputDataObject( vtkStatisticsAlgorithm::OUTPUT_MODEL ) );
  vtkSmartPointer<vtkTable> outputPrimary1 =
    vtkTable::SafeDownCast( outputMetaDS1->GetBlock( 0 ) );
  vtkSmartPointer<vtkTable> outputDerived1 =
    vtkTable::SafeDownCast( outputMetaDS1->GetBlock( 1 ) );
  vtkSmartPointer<vtkTable> outputTest1 =
    ds1->GetOutput( vtkStatisticsAlgorithm::OUTPUT_TEST );

  cout << "\n## Calculated the following primary statistics for first data set:\n";
  for ( vtkIdType r = 0; r < outputPrimary1->GetNumberOfRows(); ++ r )
    {
    std::cout << "   ";
    for ( int i = 0; i < outputPrimary1->GetNumberOfColumns(); ++ i )
      {
      std::cout << outputPrimary1->GetColumnName( i )
           << "="
           << outputPrimary1->GetValue( r, i ).ToString()
           << "  ";
      }

    // Verify some of the calculated primary statistics
    if ( fabs ( outputPrimary1->GetValueByName( r, "Mean" ).ToDouble() - means1[r] ) > 1.e-6 )
      {
      vtkGenericWarningMacro("Incorrect mean");
      testStatus = 1;
      }
    std::cout << "\n";
    }

  std::cout << "\n## Calculated the following derived statistics for first data set:\n";
  for ( vtkIdType r = 0; r < outputDerived1->GetNumberOfRows(); ++ r )
    {
    std::cout << "   ";
    for ( int i = 0; i < outputDerived1->GetNumberOfColumns(); ++ i )
      {
      cout << outputDerived1->GetColumnName( i )
           << "="
           << outputDerived1->GetValue( r, i ).ToString()
           << "  ";
      }

    // Verify some of the calculated derived statistics
    if ( fabs ( outputDerived1->GetValueByName( r, "Standard Deviation" ).ToDouble() - stdevs1[r] ) > 1.e-5 )
      {
      vtkGenericWarningMacro("Incorrect standard deviation");
      testStatus = 1;
      }
    std::cout << "\n";
    }

  // Check some results of the Test option
  std::cout << "\n## Calculated the following Jarque-Bera statistics:\n";
  for ( vtkIdType r = 0; r < outputTest1->GetNumberOfRows(); ++ r )
    {
    std::cout << "   ";
    for ( int i = 0; i < outputTest1->GetNumberOfColumns(); ++ i )
      {
      std::cout << outputTest1->GetColumnName( i )
           << "="
           << outputTest1->GetValue( r, i ).ToString()
           << "  ";
      }

    std::cout << "\n";
    }

  // Search for outliers to check results of Assess option
  double maxdev = 1.5;
  std::cout << "\n## Searching for outliers from mean with relative deviation > "
       << maxdev
       << " for metric 1:\n";

  vtkSmartPointer<vtkDoubleArray> vals0 =
    vtkDoubleArray::SafeDownCast( outputData1->GetColumnByName( "Metric 0" ) );
  vtkSmartPointer<vtkDoubleArray> vals1 =
    vtkDoubleArray::SafeDownCast( outputData1->GetColumnByName( "Metric 1" ) );
  vtkSmartPointer<vtkDoubleArray> devs0 =
    vtkDoubleArray::SafeDownCast( outputData1->GetColumnByName( "d(Metric 0)" ) );
  vtkSmartPointer<vtkDoubleArray> devs1 =
    vtkDoubleArray::SafeDownCast( outputData1->GetColumnByName( "d(Metric 1)" ) );

  if ( ! devs0 || ! devs1 || ! vals0 || ! vals1 )
    {
    vtkGenericWarningMacro("Empty output column(s).\n");
    testStatus = 1;

    return testStatus;
    }

  double dev;
  int m0outliers = 0;
  int m1outliers = 0;
  for ( vtkIdType r = 0; r < outputData1->GetNumberOfRows(); ++ r )
    {
    dev = devs0->GetValue( r );
    if ( dev > maxdev )
      {
      ++ m0outliers;
      std::cout << "   "
           << " row "
           << r
           << ", "
           << devs0->GetName()
           << " = "
           << dev
           << " > "
           << maxdev
           << " (value: "
           << vals0->GetValue( r )
           << ")\n";
      }
    }
  for ( vtkIdType r = 0; r < outputData1->GetNumberOfRows(); ++ r )
    {
    dev = devs1->GetValue( r );
    if ( dev > maxdev )
      {
      ++ m1outliers;
      std::cout << "   "
           << " row "
           << r
           << ", "
           << devs1->GetName()
           << " = "
           << dev
           << " > "
           << maxdev
           << " (value: "
           << vals1->GetValue( r )
           << ")\n";
      }
    }

  std::cout << "  Found "
       << m0outliers
       << " outliers for Metric 0"
       << " and "
       << m1outliers
       << " outliers for Metric 1.\n";

  if ( m0outliers != 4 || m1outliers != 6 )
    {
    vtkGenericWarningMacro("Expected 4 outliers for Metric 0 and 6 outliers for Metric 1.");
    testStatus = 1;
    }

  // Now, used modified output 1 as input 1 to test 0-deviation
  std::cout << "\n## Searching for values not equal to 50 for metric 1:\n";

  vtkSmartPointer<vtkTable> modifiedPrimary =
    vtkSmartPointer<vtkTable>::New();
  modifiedPrimary->ShallowCopy( outputPrimary1 );
  modifiedPrimary->SetValueByName( 1, "Mean", 50. );

  vtkSmartPointer<vtkTable> modifiedDerived =
    vtkSmartPointer<vtkTable>::New();
  modifiedDerived->ShallowCopy( outputDerived1 );
  modifiedDerived->SetValueByName( 1, "Standard Deviation", 0. );

  vtkSmartPointer<vtkMultiBlockDataSet> modifiedModel =
    vtkSmartPointer<vtkMultiBlockDataSet>::New();
  modifiedModel->SetNumberOfBlocks( 2 );
  modifiedModel->SetBlock( 0, modifiedPrimary );
  modifiedModel->SetBlock( 1, modifiedDerived );

  // Run with Assess option only (do not recalculate nor rederive a model)
  ds1->SetInputData( vtkStatisticsAlgorithm::INPUT_MODEL, modifiedModel );
  ds1->SetLearnOption( false );
  ds1->SetDeriveOption( false );
  ds1->SetTestOption( true );
  ds1->SetAssessOption( true );
  ds1->Update();

  vals1 = vtkDoubleArray::SafeDownCast( outputData1->GetColumnByName( "Metric 1" ) );
  devs1 = vtkDoubleArray::SafeDownCast( outputData1->GetColumnByName( "d(Metric 1)" ) );

  if ( ! devs1 || ! vals1 )
    {
    vtkGenericWarningMacro("Empty output column(s).\n");
    testStatus = 1;

    return testStatus;
    }

  m1outliers = 0;
  for ( vtkIdType r = 0; r < outputData1->GetNumberOfRows(); ++ r )
    {
    dev = devs1->GetValue( r );
    if ( dev )
      {
      ++ m1outliers;
      }
    }

  std::cout << "  Found "
       << m1outliers
       << " outliers for Metric 1.\n";

  if ( m1outliers != 28 )
    {
    vtkGenericWarningMacro("Expected 28 outliers for Metric 1, found " << m1outliers << ".");
    testStatus = 1;
    }

  // Clean up (which implies resetting input model to first algorithm parameters table values which were modified to their initial values)
  modifiedPrimary->SetValueByName( 1, "Mean", means1[1] );
  modifiedPrimary->Delete();
  modifiedDerived->SetValueByName( 1, "Standard Deviation", stdevs1[1] );
  modifiedDerived->Delete();
  modifiedModel->Delete();

  // Test with a slight variation of initial data set (to test model aggregation)
  int nVals2 = 32;

  vtkSmartPointer<vtkDoubleArray> dataset4Arr =
    vtkSmartPointer<vtkDoubleArray>::New();
  dataset4Arr->SetNumberOfComponents( 1 );
  dataset4Arr->SetName( "Metric 0" );

  vtkSmartPointer<vtkDoubleArray> dataset5Arr =
    vtkSmartPointer<vtkDoubleArray>::New();
  dataset5Arr->SetNumberOfComponents( 1 );
  dataset5Arr->SetName( "Metric 1" );

  vtkSmartPointer<vtkDoubleArray> dataset6Arr =
    vtkSmartPointer<vtkDoubleArray>::New();
  dataset6Arr->SetNumberOfComponents( 1 );
  dataset6Arr->SetName( "Metric 2" );

  for ( int i = 0; i < nVals2; ++ i )
    {
    int ti = i << 1;
    dataset4Arr->InsertNextValue( mingledData[ti] + 1. );
    dataset5Arr->InsertNextValue( mingledData[ti + 1] );
    dataset6Arr->InsertNextValue( 1. );
    }

  vtkSmartPointer<vtkTable> datasetTable2 =
    vtkSmartPointer<vtkTable>::New();
  datasetTable2->AddColumn( dataset4Arr );
  datasetTable2->AddColumn( dataset5Arr );
  datasetTable2->AddColumn( dataset6Arr );

  // Set descriptive statistics algorithm and its input data port
  vtkSmartPointer<vtkDescriptiveStatistics> ds2 =
    vtkSmartPointer<vtkDescriptiveStatistics>::New();
  ds2->SetInputData( vtkStatisticsAlgorithm::INPUT_DATA, datasetTable2 );

  // Select Columns of Interest (all of them)
  for ( int i = 0; i< nMetrics; ++ i )
    {
    ds2->AddColumn( columns[i] );
    }

  // Update with Learn option only
  ds2->SetLearnOption( true );
  ds2->SetDeriveOption( false );
  ds2->SetTestOption( false );
  ds2->SetAssessOption( false );
  ds2->Update();

  // Get output meta tables
  vtkSmartPointer<vtkMultiBlockDataSet> outputMetaDS2 =
    vtkMultiBlockDataSet::SafeDownCast( ds2->GetOutputDataObject( vtkStatisticsAlgorithm::OUTPUT_MODEL ) );
  vtkSmartPointer<vtkTable> outputPrimary2 =
    vtkTable::SafeDownCast( outputMetaDS2->GetBlock( 0 ) );

  std::cout << "\n## Calculated the following primary statistics for second data set:\n";
  for ( vtkIdType r = 0; r < outputPrimary2->GetNumberOfRows(); ++ r )
    {
    std::cout << "   ";
    for ( int i = 0; i < outputPrimary2->GetNumberOfColumns(); ++ i )
      {
      std::cout << outputPrimary2->GetColumnName( i )
           << "="
           << outputPrimary2->GetValue( r, i ).ToString()
           << "  ";
      }
    std::cout << "\n";
   }

  // Test model aggregation by adding new data to engine which already has a model
  ds1->SetInputData( vtkStatisticsAlgorithm::INPUT_DATA, datasetTable2 );
  vtkSmartPointer<vtkMultiBlockDataSet> model =
    vtkSmartPointer<vtkMultiBlockDataSet>::New();
  model->ShallowCopy( outputMetaDS1 );
  ds1->SetInputData( vtkStatisticsAlgorithm::INPUT_MODEL, model );

  // Update with Learn and Derive options only
  ds1->SetLearnOption( true );
  ds1->SetDeriveOption( true );
  ds1->SetTestOption( false );
  ds1->SetAssessOption( false );
  ds1->Update();

  // Updated reference values
  // Means deviations for metrics 0, 1, and 2, respectively
  double means0[] = { 49.71875 , 49.5, 0. };

  // Standard deviations for metrics 0, 1, and 2, respectively
  double stdevs0[] = { sqrt( 6.1418651 ), sqrt( 7.548397 * 62. / 63. ), sqrt( 64. / 63. ) };

  // Get output data and meta tables
  outputMetaDS1 = vtkMultiBlockDataSet::SafeDownCast( ds1->GetOutputDataObject( vtkStatisticsAlgorithm::OUTPUT_MODEL ) );
  outputPrimary1 = vtkTable::SafeDownCast( outputMetaDS1->GetBlock( 0 ) );
  outputDerived1 = vtkTable::SafeDownCast( outputMetaDS1->GetBlock( 1 ) );

  std::cout << "\n## Calculated the following primary statistics for updated (first + second) data set:\n";
  for ( vtkIdType r = 0; r < outputPrimary1->GetNumberOfRows(); ++ r )
    {
    std::cout << "   ";
    for ( int i = 0; i < outputPrimary1->GetNumberOfColumns(); ++ i )
      {
      std::cout << outputPrimary1->GetColumnName( i )
           << "="
           << outputPrimary1->GetValue( r, i ).ToString()
           << "  ";
      }

    // Verify some of the calculated primary statistics
    if ( fabs ( outputPrimary1->GetValueByName( r, "Mean" ).ToDouble() - means0[r] ) > 1.e-6 )
      {
      vtkGenericWarningMacro("Incorrect mean");
      testStatus = 1;
      }
    std::cout << "\n";
    }

  std::cout << "\n## Calculated the following derived statistics for updated (first + second) data set:\n";
  for ( vtkIdType r = 0; r < outputDerived1->GetNumberOfRows(); ++ r )
    {
    std::cout << "   ";
    for ( int i = 0; i < outputDerived1->GetNumberOfColumns(); ++ i )
      {
      std::cout << outputDerived1->GetColumnName( i )
           << "="
           << outputDerived1->GetValue( r, i ).ToString()
           << "  ";
      }

    // Verify some of the calculated derived statistics
    if ( fabs ( outputDerived1->GetValueByName( r, "Standard Deviation" ).ToDouble() - stdevs0[r] ) > 1.e-5 )
      {
      vtkGenericWarningMacro("Incorrect standard deviation");
      testStatus = 1;
      }
    cout << "\n";
    }

  // ************** Very simple example, for baseline comparison vs. R *********
  double simpleData[] =
    {
      0,
      1,
      2,
      3,
      4,
      5,
      6,
      7,
      8,
      9,
    };
  int nSimpleVals = 10;

  vtkSmartPointer<vtkDoubleArray> datasetArr =
    vtkSmartPointer<vtkDoubleArray>::New();
  datasetArr->SetNumberOfComponents( 1 );
  datasetArr->SetName( "Digits" );

  for ( int i = 0; i < nSimpleVals; ++ i )
    {
    datasetArr->InsertNextValue( simpleData[i] );
    }

  vtkSmartPointer<vtkTable> simpleTable =
    vtkSmartPointer<vtkTable>::New();
  simpleTable->AddColumn( datasetArr );

  double mean = 4.5;
  double variance = 9.16666666666667;
  double skewness = 0.;
  double kurtosis = -1.56163636363636;

  // Set descriptive statistics algorithm and its input data port
  vtkSmartPointer<vtkDescriptiveStatistics> ds3 =
    vtkSmartPointer<vtkDescriptiveStatistics>::New();
  ds3->SetInputData( vtkStatisticsAlgorithm::INPUT_DATA, simpleTable );

  // Select column of interest
  ds3->AddColumn( "Digits" );

  // Add non existing column
  ds3->AddColumn( "Bogus" );

  // Warning for non existing column will mess up output
  std::cout << "\n";

  // Test Learn and Derive options only
  ds3->SetLearnOption( true );
  ds3->SetDeriveOption( true );
  ds3->SetTestOption( false );
  ds3->SetAssessOption( false );
  ds3->Update();

  // Get output data and meta tables
  vtkSmartPointer<vtkMultiBlockDataSet> outputMetaDS3 =
    vtkMultiBlockDataSet::SafeDownCast( ds3->GetOutputDataObject( vtkStatisticsAlgorithm::OUTPUT_MODEL ) );
  vtkSmartPointer<vtkTable> outputPrimary3 =
    vtkTable::SafeDownCast( outputMetaDS3->GetBlock( 0 ) );
  vtkSmartPointer<vtkTable> outputDerived3 =
    vtkTable::SafeDownCast( outputMetaDS3->GetBlock( 1 ) );

  std::cout << "\n## Calculated the following primary statistics for {0,...9} sequence:\n";
  std::cout << "   ";
  for ( int i = 0; i < outputPrimary3->GetNumberOfColumns(); ++ i )
    {
    std::cout << outputPrimary3->GetColumnName( i )
         << "="
         << outputPrimary3->GetValue( 0, i ).ToString()
         << "  ";
    }

  // Verify some of the calculated primary statistics
  if ( fabs ( outputPrimary3->GetValueByName( 0, "Mean" ).ToDouble() - mean ) > 1.e-6 )
    {
    vtkGenericWarningMacro("Incorrect mean");
    testStatus = 1;
    }
  std::cout << "\n";

  std::cout << "\n## Calculated the following derived statistics for {0,...9} sequence:\n";
  std::cout << "   ";
  for ( int i = 0; i < outputDerived3->GetNumberOfColumns(); ++ i )
    {
    std::cout << outputDerived3->GetColumnName( i )
         << "="
         << outputDerived3->GetValue( 0, i ).ToString()
         << "  ";
    }

  // Verify some of the calculated derived statistics
  if ( fabs ( outputDerived3->GetValueByName( 0, "Variance" ).ToDouble() - variance ) > 1.e-6 )
    {
    vtkGenericWarningMacro("Incorrect variance");
    testStatus = 1;
    }

  if ( fabs ( outputDerived3->GetValueByName( 0, "Skewness" ).ToDouble() - skewness ) > 1.e-6 )
    {
    vtkGenericWarningMacro("Incorrect skewness");
    testStatus = 1;
    }

  if ( fabs ( outputDerived3->GetValueByName( 0, "Kurtosis" ).ToDouble() - kurtosis ) > 1.e-6 )
    {
    vtkGenericWarningMacro("Incorrect kurtosis");
    testStatus = 1;
    }
  std::cout << "\n";

  // ************** Pseudo-random sample to exercise Jarque-Bera test *********
  int nVals = 10000;

  vtkSmartPointer<vtkDoubleArray> datasetNormal =
    vtkSmartPointer<vtkDoubleArray>::New();;
  datasetNormal->SetNumberOfComponents( 1 );
  datasetNormal->SetName( "Standard Normal" );

  vtkSmartPointer<vtkDoubleArray> datasetUniform =
    vtkSmartPointer<vtkDoubleArray>::New();
  datasetUniform->SetNumberOfComponents( 1 );
  datasetUniform->SetName( "Standard Uniform" );

  vtkSmartPointer<vtkDoubleArray> datasetLogNormal =
    vtkSmartPointer<vtkDoubleArray>::New();
  datasetLogNormal->SetNumberOfComponents( 1 );
  datasetLogNormal->SetName( "Standard Log-Normal" );

  vtkSmartPointer<vtkDoubleArray> datasetExponential =
    vtkSmartPointer<vtkDoubleArray>::New();
  datasetExponential->SetNumberOfComponents( 1 );
  datasetExponential->SetName( "Standard Exponential" );

  vtkSmartPointer<vtkDoubleArray> datasetLaplace =
    vtkSmartPointer<vtkDoubleArray>::New();
  datasetLaplace->SetNumberOfComponents( 1 );
  datasetLaplace->SetName( "Standard Laplace" );

  // Seed random number generator
  vtkMath::RandomSeed( static_cast<int>( vtkTimerLog::GetUniversalTime() ) );

  for ( int i = 0; i < nVals; ++ i )
    {
    datasetNormal->InsertNextValue( vtkMath::Gaussian() );
    datasetUniform->InsertNextValue( vtkMath::Random() );
    datasetLogNormal->InsertNextValue( exp( vtkMath::Gaussian() ) );
    datasetExponential->InsertNextValue( -log ( vtkMath::Random() ) );
    double u = vtkMath::Random() - .5;
    datasetLaplace->InsertNextValue( ( u < 0. ? 1. : -1. ) * log ( 1. - 2. * fabs( u ) ) );
    }

  vtkSmartPointer<vtkTable> gaussianTable =
    vtkSmartPointer<vtkTable>::New();
  gaussianTable->AddColumn( datasetNormal );
  gaussianTable->AddColumn( datasetUniform );
  gaussianTable->AddColumn( datasetLogNormal );
  gaussianTable->AddColumn( datasetExponential );
  gaussianTable->AddColumn( datasetLaplace );

  // Set descriptive statistics algorithm and its input data port
  vtkSmartPointer<vtkDescriptiveStatistics> ds4 =
    vtkSmartPointer<vtkDescriptiveStatistics>::New();
  ds4->SetInputData( vtkStatisticsAlgorithm::INPUT_DATA, gaussianTable );

  // Select Column of Interest
  ds4->AddColumn( "Standard Normal" );
  ds4->AddColumn( "Standard Uniform" );
  ds4->AddColumn( "Standard Log-Normal" );
  ds4->AddColumn( "Standard Exponential" );
  ds4->AddColumn( "Standard Laplace" );

  // Test Learn, Derive, and Test options only
  ds4->SetLearnOption( true );
  ds4->SetDeriveOption( true );
  ds4->SetTestOption( true );
  ds4->SetAssessOption( false );
  ds4->Update();

  // Get output data and meta tables
  vtkSmartPointer<vtkMultiBlockDataSet> outputMetaDS4 =
    vtkMultiBlockDataSet::SafeDownCast( ds4->GetOutputDataObject( vtkStatisticsAlgorithm::OUTPUT_MODEL ) );
  vtkSmartPointer<vtkTable> outputPrimary4 =
    vtkTable::SafeDownCast( outputMetaDS4->GetBlock( 0 ) );
  vtkSmartPointer<vtkTable> outputDerived4 =
    vtkTable::SafeDownCast( outputMetaDS4->GetBlock( 1 ) );
  vtkSmartPointer<vtkTable> outputTest4 =
    ds4->GetOutput( vtkStatisticsAlgorithm::OUTPUT_TEST );

  std::cout << "\n## Calculated the following primary statistics for pseudo-random variables (n="
       << nVals
       << "):\n";
  for ( vtkIdType r = 0; r < outputPrimary4->GetNumberOfRows(); ++ r )
    {
    std::cout << "   ";
    for ( int i = 0; i < outputPrimary4->GetNumberOfColumns(); ++ i )
      {
      std::cout << outputPrimary4->GetColumnName( i )
           << "="
           << outputPrimary4->GetValue( r, i ).ToString()
           << "  ";
      }

    std::cout << "\n";
    }

  std::cout << "\n## Calculated the following derived statistics for pseudo-random variables (n="
       << nVals
       << "):\n";
  for ( vtkIdType r = 0; r < outputDerived4->GetNumberOfRows(); ++ r )
    {
    std::cout << "   ";
    for ( int i = 0; i < outputDerived4->GetNumberOfColumns(); ++ i )
      {
      std::cout << outputDerived4->GetColumnName( i )
           << "="
           << outputDerived4->GetValue( r, i ).ToString()
           << "  ";
      }

    std::cout << "\n";
    }

  // Check some results of the Test option
  std::cout << "\n## Calculated the following Jarque-Bera statistics for pseudo-random variables (n="
       << nVals;

#ifdef VTK_USE_GNU_R
  int nNonGaussian = 3;
  int nRejected = 0;
  double alpha = .01;

  std::cout << ", null hypothesis: normality, significance level="
       << alpha;
#endif // VTK_USE_GNU_R

  cout << "):\n";

  // Loop over Test table
  for ( vtkIdType r = 0; r < outputTest4->GetNumberOfRows(); ++ r )
    {
    cout << "   ";
    for ( int c = 0; c < outputTest4->GetNumberOfColumns(); ++ c )
      {
      cout << outputTest4->GetColumnName( c )
           << "="
           << outputTest4->GetValue( r, c ).ToString()
           << "  ";
      }

#ifdef VTK_USE_GNU_R
    // Check if null hypothesis is rejected at specified significance level
    double p = outputTest4->GetValueByName( r, "P" ).ToDouble();
    // Must verify that p value is valid (it is set to -1 if R has failed)
    if ( p > -1 && p < alpha )
      {
      std::cout << "N.H. rejected";

      ++ nRejected;
      }
#endif // VTK_USE_GNU_R

    std::cout << "\n";
    }

#ifdef VTK_USE_GNU_R
  if ( nRejected < nNonGaussian )
    {
    vtkGenericWarningMacro("Rejected only "
                           << nRejected
                           << " null hypotheses of normality whereas "
                           << nNonGaussian
                           << " variables are not Gaussian");
    testStatus = 1;
    }
#endif // VTK_USE_GNU_R

  return testStatus;
}
