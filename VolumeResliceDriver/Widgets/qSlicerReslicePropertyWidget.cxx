// Qt includes
#include <QButtonGroup>

#include "qSlicerReslicePropertyWidget.h"
#include "ui_qSlicerReslicePropertyWidget.h"

// MRMLWidgets includes
#include <qMRMLSliceWidget.h>
#include <qMRMLSliceControllerWidget.h>
#include <qMRMLThreeDWidget.h>

#include "qMRMLViewControllerBar_p.h"
#include <qMRMLViewControllerBar.h>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QToolButton>

// MRML includes
#include "vtkSmartPointer.h"
#include "vtkMRMLSliceNode.h"
#include "vtkMRMLViewNode.h"

// MRMLLogic includes
#include "vtkMRMLLayoutLogic.h"

#include "vtkSlicerVolumeResliceDriverLogic.h"

// VTK includes
#include <vtkSmartPointer.h>
#include <vtkMatrix4x4.h>
#include <vtkImageData.h>

#include "ctkPopupWidget.h"

#include "qMRMLColors.h"


class qSlicerReslicePropertyWidgetPrivate;
class vtkMRMLNode;
class vtkObject;


//------------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_OpenIGTLinkIF
class qSlicerReslicePropertyWidgetPrivate
  : public qMRMLViewControllerBarPrivate, public Ui_qSlicerReslicePropertyWidget
{
  Q_DECLARE_PUBLIC(qSlicerReslicePropertyWidget);
  
public:
  typedef qMRMLViewControllerBarPrivate Superclass;

  qSlicerReslicePropertyWidgetPrivate(qSlicerReslicePropertyWidget& object);
  virtual ~qSlicerReslicePropertyWidgetPrivate() ;

  virtual void init();
  void SetDriverNodeSelection( const char* nodeID );
  void SetModeSelection( int mode );
  int GetModeSelection() const;
  
  QButtonGroup methodButtonGroup;
  QButtonGroup orientationButtonGroup;

  vtkMRMLScene * scene;
  vtkMRMLSliceNode * sliceNode;
  vtkMRMLNode  * driverNode;

protected:
  virtual void setupPopupUi();
};



qSlicerReslicePropertyWidgetPrivate
::qSlicerReslicePropertyWidgetPrivate( qSlicerReslicePropertyWidget& object )
  : Superclass(object)
{
  this->scene = NULL;
  this->sliceNode = NULL;
  this->driverNode = NULL;
}



qSlicerReslicePropertyWidgetPrivate
::~qSlicerReslicePropertyWidgetPrivate()
{
}



void qSlicerReslicePropertyWidgetPrivate
::setupPopupUi()
{
  //Q_Q(qSlicerReslicePropertyWidget);

  this->Superclass::setupPopupUi();
  this->PopupWidget->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
  this->Ui_qSlicerReslicePropertyWidget::setupUi(this->PopupWidget);

}


void qSlicerReslicePropertyWidgetPrivate
::init()
{
  //Q_Q(qSlicerReslicePropertyWidget);
  
  this->Superclass::init();
  this->ViewLabel->setText(qSlicerReslicePropertyWidget::tr("1"));
  this->BarLayout->addStretch(1);
  this->setColor(qMRMLColors::threeDViewBlue());
}


void qSlicerReslicePropertyWidgetPrivate
::SetDriverNodeSelection( const char* nodeID )
{
  this->driverNodeSelector->setCurrentNodeID( nodeID );
}


void qSlicerReslicePropertyWidgetPrivate
::SetModeSelection( int mode )
{
  this->modeComboBox->setCurrentIndex( mode );
}

int qSlicerReslicePropertyWidgetPrivate
::GetModeSelection() const
{
    return modeComboBox->currentIndex();
}

qSlicerReslicePropertyWidget
::qSlicerReslicePropertyWidget( vtkSlicerVolumeResliceDriverLogic* logic, QWidget *_parent )
  : Superclass( new qSlicerReslicePropertyWidgetPrivate( *this ), _parent )
{
  Q_D(qSlicerReslicePropertyWidget);
  d->init();
  this->setLogic(logic);
}

qSlicerReslicePropertyWidget
  ::qSlicerReslicePropertyWidget( QWidget *_parent )
  : Superclass( new qSlicerReslicePropertyWidgetPrivate( *this ), _parent )
{
  Q_D(qSlicerReslicePropertyWidget);
  d->init();
}

void qSlicerReslicePropertyWidget
::setLogic(vtkSlicerVolumeResliceDriverLogic* logic)
{
  this->Logic = logic;
  if ( this->Logic != NULL )
  {
    qvtkConnect( this->Logic, vtkCommand::ModifiedEvent, this, SLOT( onLogicModified() ) );
  }
}

qSlicerReslicePropertyWidget
::~qSlicerReslicePropertyWidget()
{
  if ( this->Logic != NULL )
    {
    this->Logic = NULL;
    }
}


void qSlicerReslicePropertyWidget
::showAdvanced(int show)
{
    Q_D(qSlicerReslicePropertyWidget);
    _showAdvanced = show;
    if (show)
    {
        d->rotationSlider->show();
        d->rotationLabel->show();
        d->flipCheckBox->show();

        if (d->GetModeSelection() == vtkSlicerVolumeResliceDriverLogic::MODE_PROBE_VIEW)
        {
            d->offsetLabel->show();
            d->offsetSlider->show();
        }
        else
        {
            d->offsetLabel->hide();
            d->offsetSlider->hide();
        }
    }
    else
    {
        d->rotationSlider->hide();
        d->rotationLabel->hide();
        d->flipCheckBox->hide();
        d->offsetLabel->hide();
        d->offsetSlider->hide();
    }
}


void qSlicerReslicePropertyWidget
::setSliceViewName( const QString& newSliceViewName )
{
  Q_D(qSlicerReslicePropertyWidget);
  
  d->ViewLabel->setText(newSliceViewName);
}


void qSlicerReslicePropertyWidget
::setSliceViewColor( const QColor& newSliceViewColor )
{
  Q_D(qSlicerReslicePropertyWidget);
  
  d->setColor( newSliceViewColor );
}



void qSlicerReslicePropertyWidget
::setMRMLSliceNode( vtkMRMLSliceNode* newSliceNode )
{
  Q_D(qSlicerReslicePropertyWidget);
  
  if ( newSliceNode == NULL )
    {
    return;
    }
  
  QObject::disconnect(d->driverNodeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)), this, SLOT(setDriverNode(vtkMRMLNode*)));
  QObject::disconnect(d->modeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onModeChanged(int)));
  QObject::disconnect(d->rotationSlider, SIGNAL(valueChanged(double)), this, SLOT(onRotationChanged(double)));
  QObject::disconnect(d->flipCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onFlipChanged(int)));
  QObject::disconnect(d->offsetSlider, SIGNAL(valueChanged(double)), this, SLOT(onOffsetChanged(double)));

  d->sliceNode = newSliceNode;
  
  if ( newSliceNode->GetScene() )
    {
    this->setMRMLScene( newSliceNode->GetScene() );
    }
  
  QObject::connect(d->driverNodeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)), this, SLOT(setDriverNode(vtkMRMLNode*)));
  QObject::connect(d->modeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onModeChanged(int)));
  QObject::connect(d->rotationSlider, SIGNAL(valueChanged(double)), this, SLOT(onRotationChanged(double)));
  QObject::connect(d->flipCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onFlipChanged(int)));
  QObject::connect(d->offsetSlider, SIGNAL(valueChanged(double)), this, SLOT(onOffsetChanged(double)));
}


void qSlicerReslicePropertyWidget
::setMRMLScene( vtkMRMLScene* newScene )
{
  Q_D(qSlicerReslicePropertyWidget);

  
  if (d->scene != newScene)
    {
    d->scene = newScene;
    if (d->driverNodeSelector)
      {
      d->driverNodeSelector->setMRMLScene(newScene);
      }
    }
}


void qSlicerReslicePropertyWidget
::setDriverNode( vtkMRMLNode* newNode )
{
  Q_D(qSlicerReslicePropertyWidget);
  
  if ( d->sliceNode == NULL )
    {
    return;
    }
  
  if ( newNode == NULL )
    {
    this->Logic->SetDriverForSlice( "", d->sliceNode );
    }
  else
    {
    this->Logic->SetDriverForSlice( newNode->GetID(), d->sliceNode );
    }
}


void qSlicerReslicePropertyWidget
::onModeChanged(int m)
{
  Q_D(qSlicerReslicePropertyWidget);

  this->Logic->SetModeForSlice( m, d->sliceNode );
  showAdvanced(_showAdvanced);
}



void qSlicerReslicePropertyWidget
::onRotationChanged(double r)
{
  Q_D(qSlicerReslicePropertyWidget);

  this->Logic->SetRotationForSlice( r, d->sliceNode );
}



void qSlicerReslicePropertyWidget
::onFlipChanged(int f)
{
  Q_D(qSlicerReslicePropertyWidget);

  this->Logic->SetFlipForSlice( f, d->sliceNode );
}

void qSlicerReslicePropertyWidget
::onOffsetChanged(double f)
{
    Q_D(qSlicerReslicePropertyWidget);

    this->Logic->SetOffsetForSlice(f, d->sliceNode);
}


void qSlicerReslicePropertyWidget
::onLogicModified()
{
  Q_D(qSlicerReslicePropertyWidget);
  
  if ( d->sliceNode == NULL )
    {
    d->SetDriverNodeSelection( NULL );
    return;
    }
  
  const char* driverCC = d->sliceNode->GetAttribute( VOLUMERESLICEDRIVER_DRIVER_ATTRIBUTE );
  d->SetDriverNodeSelection( driverCC );
  
  const char* modeCC = d->sliceNode->GetAttribute( VOLUMERESLICEDRIVER_MODE_ATTRIBUTE );
  if ( modeCC == NULL )
    {
    d->SetModeSelection( this->Logic->MODE_NONE );
    }
  else
    {
    std::stringstream modeSS( modeCC );
    int mode = this->Logic->MODE_NONE;
    modeSS >> mode;
    d->SetModeSelection( mode );
    }

}
