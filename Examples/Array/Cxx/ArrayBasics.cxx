#include <vtkArrayPrint.h>
#include <vtkDenseArray.h>
#include <vtkSmartPointer.h>
#include <vtkSparseArray.h>

int main(int vtkNotUsed(argc), char *vtkNotUsed(argv)[])
{
  // Creating N-Way Arrays

  // Creating a dense array of 10 integers:
  vtkSmartPointer< vtkDenseArray<vtkIdType> > array =
    vtkSmartPointer< vtkDenseArray<vtkIdType> >::New();
  array->Resize(10);

  // Creating a dense 20 x 30 matrix:
  vtkSmartPointer< vtkDenseArray<double> > matrix =
    vtkSmartPointer< vtkDenseArray<double> >::New();
  matrix->Resize(20, 30);

  // Creating a sparse 10 x 20 x 30 x 40 tensor:
  vtkArrayExtents extents;
  extents.SetDimensions(4);
  extents[0] = vtkArrayRange(0, 10);
  extents[1] = vtkArrayRange(0, 20);
  extents[2] = vtkArrayRange(0, 30);
  extents[3] = vtkArrayRange(0, 40);

  vtkSmartPointer< vtkSparseArray<vtkIdType> > tensor =
    vtkSmartPointer< vtkSparseArray<vtkIdType> >::New();
  tensor->Resize(extents);

  // Initializing N-Way Arrays

  // Filling a dense array with ones:
  array->Fill(1);

  // Filling a dense matrix with zeros:
  matrix->Fill(0.0);

  // There's nothing to do for a sparse array - it's already empty.

  // Assigning N-Way Array Values

  // Assign array value [5]:
  array->SetValue(5, 42);

  // Assign matrix value [4, 3]:
  matrix->SetValue(4, 3, 1970);

  // Assign tensor value [3, 7, 1, 2]:
  vtkArrayCoordinates coordinates;
  coordinates.SetDimensions(4);
  coordinates[0] = 3;
  coordinates[1] = 7;
  coordinates[2] = 1;
  coordinates[3] = 2;
  tensor->SetValue(coordinates, 38);

  ////////////////////////////////////////////////////////
  // Accessing N-Way Array Values

  // Access array value [5]:
  std::cout << "array[5]: " << array->GetValue(5) << "\n\n";

  // Access matrix value [4, 3]:
  std::cout << "matrix[4, 3]: " << matrix->GetValue(4, 3) << "\n\n";

  // Access tensor value [3, 7, 1, 2]:
  std::cout << "tensor[3, 7, 1, 2]: " << tensor->GetValue(coordinates) << "\n\n";

  // Printing N-Way Arrays

  std::cout << "array:\n";
  vtkPrintVectorFormat(std::cout, vtkTypedArray<vtkIdType>::SafeDownCast(array));
  std::cout << "\n";

  std::cout << "matrix:\n";
  vtkPrintMatrixFormat(std::cout, vtkTypedArray<double>::SafeDownCast(matrix));
  std::cout << "\n";

  std::cout << "tensor:\n";
  vtkPrintCoordinateFormat(std::cout, vtkTypedArray<vtkIdType>::SafeDownCast(tensor));
  std::cout << "\n";

  return EXIT_SUCCESS;
}
