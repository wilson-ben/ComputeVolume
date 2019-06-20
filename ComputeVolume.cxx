#include <vtkSmartPointer.h>
#include <vtkMassProperties.h>
#include <vtkTriangleFilter.h>
#include <vtkFillHolesFilter.h>
#include <vtkPolyDataNormals.h>

#include <vtkBYUReader.h>
#include <vtkOBJReader.h>
#include <vtkPLYReader.h>
#include <vtkPolyDataReader.h>
#include <vtkSTLReader.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkSphereSource.h>
#include <vtksys/SystemTools.hxx>
#include <cstdlib>

namespace
{
vtkSmartPointer<vtkPolyData> ReadPolyData(const char *fileName);
}

int main (int argc, char *argv[])
{
  if(argc < 2){
  std::cerr << "Usage: ";
  std::cerr << "./ComputeVolume ";
  std::cerr << " <File Path> "<< std::endl;
  std::cerr <<"Supported File Extensions: .ply, .vtp, .obj, .stl, .vtk, .g";
  std::cerr << std::endl;
  return EXIT_FAILURE;
}
  // Method of accessing polyData
  vtkSmartPointer<vtkPolyData> polyData = ReadPolyData(argc > 1 ? argv[1] : "");;


  vtkSmartPointer<vtkFillHolesFilter> fillHolesFilter =
    vtkSmartPointer<vtkFillHolesFilter>::New();
  fillHolesFilter->SetInputData(polyData);
  fillHolesFilter->SetHoleSize(1000.0);

  vtkSmartPointer<vtkTriangleFilter> triangleFilter =
    vtkSmartPointer<vtkTriangleFilter>::New();
  triangleFilter->SetInputConnection(fillHolesFilter->GetOutputPort());

  // Make the triangle windong order consistent
  vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
  normals->SetInputConnection(triangleFilter->GetOutputPort());
  normals->ConsistencyOn();
  normals->SplittingOff();

  vtkSmartPointer<vtkMassProperties> massProperties = vtkSmartPointer<vtkMassProperties>::New();
  massProperties->SetInputConnection(normals->GetOutputPort());
  massProperties->Update();
  std::cout << "Volume: " << massProperties->GetVolume() << std::endl
            << "    VolumeX: " << massProperties->GetVolumeX() << std::endl
            << "    VolumeY: " << massProperties->GetVolumeY() << std::endl
            << "    VolumeZ: " << massProperties->GetVolumeZ() << std::endl;

  return EXIT_SUCCESS;
}

namespace
{
vtkSmartPointer<vtkPolyData> ReadPolyData(const char *fileName)
{
  vtkSmartPointer<vtkPolyData> polyData;
  std::string extension = vtksys::SystemTools::GetFilenameExtension(std::string(fileName));
  if (extension == ".ply")
  {
    vtkSmartPointer<vtkPLYReader> reader =
      vtkSmartPointer<vtkPLYReader>::New();
    reader->SetFileName (fileName);
    reader->Update();
    polyData = reader->GetOutput();
  }
  else if (extension == ".vtp")
  {
    vtkSmartPointer<vtkXMLPolyDataReader> reader =
      vtkSmartPointer<vtkXMLPolyDataReader>::New();
    reader->SetFileName (fileName);
    reader->Update();
    polyData = reader->GetOutput();
  }
  else if (extension == ".obj")
  {
    vtkSmartPointer<vtkOBJReader> reader =
      vtkSmartPointer<vtkOBJReader>::New();
    reader->SetFileName (fileName);
    reader->Update();
    polyData = reader->GetOutput();
  }
  else if (extension == ".stl")
  {
    vtkSmartPointer<vtkSTLReader> reader =
      vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName (fileName);
    reader->Update();
    polyData = reader->GetOutput();
  }
  else if (extension == ".vtk")
  {
    vtkSmartPointer<vtkPolyDataReader> reader =
      vtkSmartPointer<vtkPolyDataReader>::New();
    reader->SetFileName (fileName);
    reader->Update();
    polyData = reader->GetOutput();
  }
  else if (extension == ".g")
  {
    vtkSmartPointer<vtkBYUReader> reader =
      vtkSmartPointer<vtkBYUReader>::New();
    reader->SetGeometryFileName (fileName);
    reader->Update();
    polyData = reader->GetOutput();
  }
  else
  {

    std::cerr << "Invalid file Type" << std::endl;
    std::cerr <<"Supported File Extensions: .ply, .vtp, .obj, .stl, .vtk, .g" << std::endl;
    exit(0);
  }
  return polyData;
}
}
