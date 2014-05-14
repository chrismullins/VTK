/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkmyEx1.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
//
// This example creates a couple of class instances and print them to
// the standard output. No rendering window is created.
//

//
// First include the required header files for the vtk classes we are using
//
#include "vtkBar.h"
#include "vtkBar2.h"
#include "vtkImageFoo.h"
#include "vtkSmartPointer.h"

int main()
{

  //
  // Next we create an instance of vtkBar
  //
  std::cout << "Create vtkBar object and print it." << std::endl;

  vtkSmartPointer<vtkBar> bar =
    vtkSmartPointer<vtkBar>::New();
  bar->Print(std::cout);

  //
  // Then we create an instance of vtkBar2
  //
  std::cout << "Create vtkBar2 object and print it." << std::endl;

  vtkSmartPointer<vtkBar2> bar2 =
    vtkSmartPointer<vtkBar2>::New();
  bar2->Print(std::cout);

  //
  // And we create an instance of vtkImageFoo
  //
  std::cout << "Create vtkImageFoo object and print it." << std::endl;

  vtkSmartPointer<vtkImageFoo> imagefoo =
    vtkSmartPointer<vtkImageFoo>::New();
  imagefoo->Print(std::cout);

  std::cout << "Looks good ?" << std::endl;

  return EXIT_SUCCESS;
}
