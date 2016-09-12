/*==============================================================================

  Program: 3D Slicer

  Copyright (c) 2011 Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#ifndef __qSlicerReslicePropertyWidget_h
#define __qSlicerReslicePropertyWidget_h

// Qt includes
#include <QWidget>

// CTK includes
#include <ctkVTKObject.h>

#include "qMRMLViewControllerBar.h"

// Export includes
#include "qSlicerVolumeResliceDriverModuleWidgetsExport.h"

class qSlicerReslicePropertyWidgetPrivate;
class vtkSlicerVolumeResliceDriverLogic;
class vtkMRMLScene;
class vtkMRMLNode;
class vtkMRMLSliceNode;
class vtkObject;


/// \ingroup Slicer_QtModules_OpenIGTLinkIF
class Q_SLICER_MODULE_VOLUMERESLICEDRIVER_WIDGETS_EXPORT qSlicerReslicePropertyWidget : public qMRMLViewControllerBar
{
  Q_OBJECT
  QVTK_OBJECT
  
public:
  
  typedef qMRMLViewControllerBar Superclass;
  
  qSlicerReslicePropertyWidget( vtkSlicerVolumeResliceDriverLogic* logic, QWidget *parent = 0);
  qSlicerReslicePropertyWidget( QWidget *parent = 0);
  virtual ~qSlicerReslicePropertyWidget();

  void showAdvanced( int show );
  
  void setLogic(vtkSlicerVolumeResliceDriverLogic* logic);
  
public slots:
  
  void setSliceViewName(const QString& newSliceViewName);
  void setSliceViewColor(const QColor& newSliceViewColor);
  void setMRMLSliceNode(vtkMRMLSliceNode* newSliceNode);

  void setMRMLScene(vtkMRMLScene * newScene);
  
protected slots:
  
  void setDriverNode(vtkMRMLNode * newNode);
  void onModeChanged(int);
  void onRotationChanged(double);
  void onFlipChanged(int);
  void onOffsetChanged(double);
  void onLogicModified();
  
  
private:
  
  Q_DECLARE_PRIVATE(qSlicerReslicePropertyWidget);
  Q_DISABLE_COPY(qSlicerReslicePropertyWidget);
  
  vtkSlicerVolumeResliceDriverLogic* Logic;
  bool _showAdvanced;
};

#endif

