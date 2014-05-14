#include <vtkArrayPrint.h>
#include <vtkSmartPointer.h>
#include <vtkSparseArray.h>

#include <vtksys/ios/sstream>

int main(int argc, char* argv[])
{
  if(argc != 2)
    {
    cerr << "Usage: " << argv[0] << " matrix-size\n";
    return EXIT_FAILURE;
    }

  int size = 0;
  vtksys_ios::istringstream buffer(argv[1]);
  buffer >> size;

  if(size < 1)
    {
    cerr << "Matrix size must be an integer greater than zero.\n";
    return EXIT_FAILURE;
    }

  // Create a sparse identity matrix:
  vtkSmartPointer< vtkSparseArray<double> > matrix =
    vtkSmartPointer< vtkSparseArray<double> >::New();
  matrix->Resize(0, 0); // To set the number of dimensions
  for(int n = 0; n != size; ++n)
    {
    matrix->AddValue(vtkArrayCoordinates(n, n), 1);
    }
  matrix->SetExtentsFromContents(); // To synchronize the array extents with newly-added values.

  std::cout << "matrix:\n";
  vtkPrintMatrixFormat(std::cout, vtkTypedArray<double>::SafeDownCast(matrix));
  std::cout << "\n";

  return EXIT_SUCCESS;
}

