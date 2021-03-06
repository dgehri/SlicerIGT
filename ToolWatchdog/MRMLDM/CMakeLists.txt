project(vtkSlicer${MODULE_NAME}ModuleMRMLDisplayableManager)

set(KIT ${PROJECT_NAME})

set(${KIT}_EXPORT_DIRECTIVE "VTK_SLICER_${MODULE_NAME_UPPER}_MODULE_MRMLDISPLAYABLEMANAGER_EXPORT")

set(${KIT}_INCLUDE_DIRECTORIES
  ${vtkSlicer${MODULE_NAME}ModuleLogic_SOURCE_DIR}
  ${vtkSlicer${MODULE_NAME}ModuleLogic_BINARY_DIR}
  ${vtkSlicer${MODULE_NAME}ModuleMRML_SOURCE_DIR}
  ${vtkSlicer${MODULE_NAME}ModuleMRML_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  )

set(displayable_manager_SRCS
  vtkMRML${MODULE_NAME}DisplayableManager.cxx
  )

set(VTK_USE_INSTANTIATOR_NEW 1)
if(${VTK_VERSION_MAJOR} GREATER 5)
  include(${VTK_CMAKE_DIR}/vtkMakeInstantiator.cmake)
endif()
VTK_MAKE_INSTANTIATOR3("${MODULE_NAME}Instantiator"
  displayable_manager_instantiator_SRCS
  "${displayable_manager_SRCS}"
  "${${KIT}_EXPORT_DIRECTIVE}"
  ${CMAKE_CURRENT_BINARY_DIR}
  "${KIT}Export.h"
  )

set(${KIT}_SRCS
  ${displayable_manager_instantiator_SRCS}
  ${displayable_manager_SRCS}
  )

set(${KIT}_VTK_LIBRARIES)
if(${VTK_VERSION_MAJOR} LESS 6)
  set(${KIT}_VTK_LIBRARIES vtkRendering)
endif()

set(${KIT}_TARGET_LIBRARIES
  ${MRML_LIBRARIES}
  ${${KIT}_VTK_LIBRARIES}
  vtkSlicerWatchdogModuleMRML
  )

#-----------------------------------------------------------------------------
SlicerMacroBuildModuleLogic(
  NAME ${KIT}
  EXPORT_DIRECTIVE ${${KIT}_EXPORT_DIRECTIVE}
  INCLUDE_DIRECTORIES ${${KIT}_INCLUDE_DIRECTORIES}
  SRCS ${${KIT}_SRCS}
  TARGET_LIBRARIES ${${KIT}_TARGET_LIBRARIES}
  )
