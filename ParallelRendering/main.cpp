#include <iostream>

using namespace std;

#include <vtkDataSet.h>
#include <vtkDataSetReader.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkDistributedDataFilter.h>
#include <vtkMPIController.h>
#include <vtkSynchronizedRenderWindows.h>
#include <vtkCompositedSynchronizedRenderers.h>

int main(int argc, char **argv)
{

  vtkMPIController *contr = vtkMPIController::New();
  contr->Initialize(&argc, &argv, 0);

  int numProcs = contr->GetNumberOfProcesses();
  int me = contr->GetLocalProcessId();

  vtkDataSet *ds = nullptr;
  vtkUnstructuredGrid *ug = vtkUnstructuredGrid::New();

  if (me == 0)
  {
    const char *fname = "comb.vtk";
    vtkDataSetReader *dsr = vtkDataSetReader::New();
    dsr->SetFileName(fname);
    ds = dsr->GetOutput();
    dsr->Update();
  }
  else
    ds = static_cast<vtkDataSet *>(ug);

  vtkDistributedDataFilter *dd = vtkDistributedDataFilter::New();
  dd->SetInputData(ds);
  dd->SetController(contr);
  dd->UseMinimalMemoryOff();
  dd->Update(); //触发d3

  ug = vtkUnstructuredGrid::SafeDownCast(dd->GetOutput());
  //vtk绘制管线
  //mapper
  vtkDataSetMapper *mapper = vtkDataSetMapper::New();
  mapper->SetInputData(ug);
  //actor
  vtkSmartPointer<vtkActor> actor =
      vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  //renderer ,renderWindow.
  vtkSmartPointer<vtkRenderer> renderer =
      vtkSmartPointer<vtkRenderer>::New();
  renderer->AddActor(actor);
  vtkSmartPointer<vtkRenderWindow> renderWindow =
      vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  renderWindow->SetWindowName(to_string(me).c_str());

  //同步窗口
  vtkSmartPointer<vtkSynchronizedRenderWindows> synRenderWndows =
      vtkSmartPointer<vtkSynchronizedRenderWindows>::New();
  synRenderWndows->SetParallelController(contr);
  synRenderWndows->SetRenderWindow(renderWindow);
  synRenderWndows->SetIdentifier(123); //设置一个标识
                                       //同步render
  vtkSmartPointer<vtkCompositedSynchronizedRenderers> synRenderers =
      vtkSmartPointer<vtkCompositedSynchronizedRenderers>::New();
  synRenderers->SetParallelController(contr);
  synRenderers->SetRenderer(renderer);
  synRenderers->SetImageReductionFactor(1); //无损画质

  if (me == 0)
  {
    //在主进程上添加交互器
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);
    renderWindowInteractor->Start();
    contr->TriggerBreakRMIs(); //触发远程调用
  }
  else
  {
    // renderWindow->OffScreenRenderingOn(); //离屏渲染
    contr->ProcessRMIs(); //处理远程调用
  }

  contr->Barrier();

  return EXIT_SUCCESS;
}