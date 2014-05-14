/*=========================================================================

  Program:   Visualization Toolkit
  Module:    task1.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "TaskParallelism.h"

#include "vtkImageData.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkSmartPointer.h"

// Task 1 for TaskParallelism.
// See TaskParallelism.cxx for more information.
vtkPolyDataMapper* task1(vtkRenderWindow* renWin, double data,
                         vtkCamera* cam)
{
  double extent = data;
  int iextent = static_cast<int>(data);
  // The pipeline

  // Synthetic image source.
  vtkSmartPointer<vtkRTAnalyticSource> source1 =
    vtkSmartPointer<vtkRTAnalyticSource>::New();
  source1->SetWholeExtent (-1*iextent, iextent, -1*iextent, iextent,
                           -1*iextent, iextent );
  source1->SetCenter(0, 0, 0);
  source1->SetStandardDeviation( .05 );
  source1->SetMaximum( 255.0 );
  source1->SetXFreq( 60 );
  source1->SetXMag( 10 );
  source1->SetYFreq( 30 );
  source1->SetYMag( 18 );
  source1->SetZFreq( 40 );
  source1->SetZMag( 5 );
  source1->GetOutput()->SetSpacing(2.0/extent,2.0/extent,2.0/extent);

  // Iso-surfacing.
  vtkSmartPointer<vtkContourFilter> contour =
    vtkSmartPointer<vtkContourFilter>::New();
  contour->SetInputConnection(source1->GetOutputPort());
  contour->SetNumberOfContours(1);
  contour->SetValue(0, 220);

  // Magnitude of the gradient vector.
  vtkSmartPointer<vtkImageGradientMagnitude> magn =
    vtkSmartPointer<vtkImageGradientMagnitude>::New();
  magn->SetDimensionality(3);
  magn->SetInputConnection(source1->GetOutputPort());

  // Probe magnitude with iso-surface.
  vtkSmartPointer<vtkProbeFilter> probe =
    vtkSmartPointer<vtkProbeFilter>::New();
  probe->SetInputConnection(contour->GetOutputPort());
  probe->SetSourceConnection(magn->GetOutputPort());
  probe->SpatialMatchOn();

  // Rendering objects.
  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputData(probe->GetPolyDataOutput());
  mapper->SetScalarRange(50, 180);
  mapper->ImmediateModeRenderingOn();

  vtkSmartPointer<vtkActor> actor =
    vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

  vtkSmartPointer<vtkRenderer> ren =
    vtkSmartPointer<vtkRenderer>::New();
  renWin->AddRenderer(ren);

  ren->AddActor(actor);
  ren->SetActiveCamera( cam );

  return mapper;

}
