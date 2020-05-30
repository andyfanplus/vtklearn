#include<iostream>
#include<vtkStructuredGridReader.h>
#include<vtkStructuredGrid.h>
#include<vtkPointData.h>
#include<vtkContourFilter.h>
#include<vtkPolyData.h>
#include<vtkPolyDataMapper.h>
#include<vtkActor.h>
#include<vtkRenderer.h>
#include<vtkRenderWindow.h>
#include<vtkRenderWindowInteractor.h>

#define vtk_create()
using namespace std; 
int main()
{
	vtkStructuredGridReader *reader = vtkStructuredGridReader::New();
	reader->SetFileName("../../dataset/curvilinear/comb.vtk");
	reader->Update();

	double *range= reader->GetOutput()->GetScalarRange();

	vtkContourFilter *filter = vtkContourFilter::New();
	filter->SetInputData(reader->GetOutput());
	filter->SetValue(0, (range[0] + range[1]) / 2);
	filter->Update();

	vtkPolyData *contour = filter->GetOutput();
	
	vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
	mapper->SetInputData(contour);

	vtkActor* actor = vtkActor::New();
	actor->SetMapper(mapper);

	vtkRenderer *renderer = vtkRenderer::New();
	renderer->AddActor(actor);

	vtkRenderWindow *rWin = vtkRenderWindow::New();
	rWin->AddRenderer(renderer);

	vtkRenderWindowInteractor *rwi = vtkRenderWindowInteractor::New();
	rwi->SetRenderWindow(rWin);
	rwi->SetSize(500, 500);
	rwi->Render();

	rwi->Initialize();
	rwi->Start();

	return 0;
}