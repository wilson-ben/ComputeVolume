cmake_minimum_required(VERSION 2.8)

PROJECT(ComputeVolume)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(ComputeVolume MACOSX_BUNDLE ComputeVolume.cxx)

if(VTK_LIBRARIES)
  target_link_libraries(ComputeVolume ${VTK_LIBRARIES})
else()
  target_link_libraries(ComputeVolume vtkHybrid vtkWidgets)
endif()
