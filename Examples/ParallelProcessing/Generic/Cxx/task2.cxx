/*=========================================================================

  Program:   Visualization Toolkit
  Module:    task2.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "TaskParallelism.h"

#include "vtkImageData.h"
#include "vtkPolyDataMapper.h"
#include "vtkSmartPointer.h"

// Task 2 for TaskParallelism.
// See TaskParallelism.cxx for more information.
vtkPolyDataMapper* task2(vtkRenderWindow* renWin, double data,
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
  source1->SetStandardDeviation( 0.5 );
  source1->SetMaximum( 255.0 );
  source1->SetXFreq( 60 );
  source1->SetXMag( 10 );
  source1->SetYFreq( 30 );
  source1->SetYMag( 18 );
  source1->SetZFreq( 40 );
  source1->SetZMag( 5 );
  source1->GetOutput()->SetSpacing(2.0/extent,2.0/extent,2.0/extent);

  // Gradient vector.
  vtkSmartPointer<vtkImageGradient> grad =
    vtkSmartPointer<vtkImageGradient>::New();
  grad->SetDimensionality( 3 );
  grad->SetInputConnection(source1->GetOutputPort());

  vtkSmartPointer<vtkImageShrink3D> mask =
    vtkSmartPointer<vtkImageShrink3D>::New();
  mask->SetInputConnection(grad->GetOutputPort());
  mask->SetShrinkFactors(5, 5, 5);


  // Label the scalar field as the active vectors.
  vtkSmartPointer<vtkAssignAttribute> aa =
    vtkSmartPointer<vtkAssignAttribute>::New();
  aa->SetInputConnection(mask->GetOutputPort());
  aa->Assign(vtkDataSetAttributes::SCALARS, vtkDataSetAttributes::VECTORS,
             vtkAssignAttribute::POINT_DATA);

  vtkSmartPointer<vtkGlyphSource2D> arrow =
    vtkSmartPointer<vtkGlyphSource2D>::New();
  arrow->SetGlyphTypeToArrow();
  arrow->SetScale(1.2);
  arrow->FilledOff();

  // Glyph the gradient vector (with arrows)
  vtkSmartPointer<vtkGlyph3D> glyph =
    vtkSmartPointer<vtkGlyph3D>::New();
  glyph->SetInputConnection(aa->GetOutputPort());
  glyph->SetSourceConnection(arrow->GetOutputPort());
  glyph->ScalingOff();
  glyph->OrientOn();
  glyph->SetVectorModeToUseVector();
  glyph->SetColorModeToColorByVector();

  // Rendering objects.
  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(glyph->GetOutputPort());
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
