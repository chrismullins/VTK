#include <vtkArrayPrint.h>
#include <vtkDenseArray.h>
#include <vtkSmartPointer.h>
#include <vtkSparseArray.h>

int main(int vtkNotUsed(argc), char *vtkNotUsed(argv)[])
{
  // Create a dense matrix:
  vtkSmartPointer< vtkDenseArray<double> > matrix =
    vtkSmartPointer< vtkDenseArray<double> >::New();
  matrix->Resize(10, 10);
  matrix->Fill(0.0);

  // Increment every value in a sparse-or-dense array
  // with any number of dimensions:
  for(vtkArray::SizeT n = 0; n != matrix->GetNonNullSize(); ++n)
    {
    matrix->SetValueN(n, matrix->GetValueN(n) + 1);
    }

  // Compute the sum of every column in a sparse-or-dense matrix:
  //vtkDenseArray<double>* sum = vtkDenseArray<double>::New();
  vtkSmartPointer< vtkDenseArray<double> > sum =
    vtkSmartPointer< vtkDenseArray<double> >::New();
  sum->Resize(matrix->GetExtents()[1]);
  sum->Fill(0.0);

  vtkArrayCoordinates coordinates;
  for(vtkArray::SizeT n = 0; n != matrix->GetNonNullSize(); ++n)
    {
    matrix->GetCoordinatesN(n, coordinates);
    sum->SetValue(coordinates[1], sum->GetValue(coordinates[1]) + matrix->GetValueN(n));
    }

  cout << "matrix:\n";
  vtkPrintMatrixFormat(std::cout, vtkTypedArray<double>::SafeDownCast(matrix));
  cout << "\n";

  std::cout << "sum:\n";
  vtkPrintVectorFormat(std::cout, vtkTypedArray<double>::SafeDownCast(sum));
  std::cout << "\n";

  return EXIT_SUCCESS;
}
