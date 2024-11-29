#include <iostream>

#include "vtkSocketController.h"

#include "vtkSphereSource.h"
#include "vtkDataSetSurfaceFilter.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSynchronizedRenderWindows.h"
#include "vtkClientServerSynchronizedRenderers.h"

using namespace std;
int main(int argc, char **argv)
{

  int IsServer;
  cin >> IsServer;

  vtkSmartPointer<vtkSocketController> socketController = vtkSmartPointer<vtkSocketController>::New();
  socketController->Initialize(&argc, &argv);

  if (IsServer)
  {
    cout << "wait for connection" << endl;
    socketController->WaitForConnection(11111);
    cout << "connection established" << endl;
  }
  else
  {
    if (!socketController->ConnectTo("localhost", 11111))
    {
      cout << "connection failed" << endl;
      return EXIT_FAILURE;
    }
    cout << "connect success" << endl;
  }

  vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
  sphere->SetPhiResolution(100);
  sphere->SetThetaResolution(100);

  vtkSmartPointer<vtkDataSetSurfaceFilter> filter = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
  filter->SetInputConnection(sphere->GetOutputPort());

  vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(filter->GetOutputPort());

  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

  vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
  if (IsServer)
  {
    renderer->AddActor(actor);
  }

  vtkSmartPointer<vtkRenderWindow> rw = vtkSmartPointer<vtkRenderWindow>::New();
  rw->AddRenderer(renderer);
  rw->SetWindowName(IsServer ? "server" : "client");

  vtkSmartPointer<vtkSynchronizedRenderWindows> csSynchronizedRenderWindows =
      vtkSmartPointer<vtkSynchronizedRenderWindows>::New();
  csSynchronizedRenderWindows->SetRenderWindow(rw);
  csSynchronizedRenderWindows->SetParallelController(socketController);
  csSynchronizedRenderWindows->SetIdentifier(1);
  csSynchronizedRenderWindows->SetRootProcessId(IsServer);

  vtkSmartPointer<vtkClientServerSynchronizedRenderers> csSynchronizedRenderers =
      vtkSmartPointer<vtkClientServerSynchronizedRenderers>::New();
  csSynchronizedRenderers->SetRenderer(renderer);
  csSynchronizedRenderers->SetParallelController(socketController);
  csSynchronizedRenderers->SetImageReductionFactor(1);
  csSynchronizedRenderers->SetRootProcessId(IsServer);

  if (IsServer)
  {
    //rw->OffScreenRenderingOn();
    socketController->ProcessRMIs();
  }
  else
  {
    vtkRenderWindowInteractor *rwi = vtkRenderWindowInteractor::New();
    rwi->SetRenderWindow(rw);
    rwi->Start();
    socketController->TriggerBreakRMIs();
  }

  return 0;
}