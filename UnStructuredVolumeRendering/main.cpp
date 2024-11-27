#include<iostream>
#include<vtkStructuredGridReader.h>
#include<vtkStructuredGrid.h>
#include<vtkUnstructuredGrid.h>
#include<vtkColorTransferFunction.h>
#include<vtkPiecewiseFunction.h>
#include<vtkVolumeProperty.h>
#include<vtkDataSetTriangleFilter.h>
#include<vtkProjectedTetrahedraMapper.h>
#include<vtkVolume.h>
#include<vtkRenderer.h>
#include<vtkRenderWindow.h>
#include<vtkRenderWindowInteractor.h>


using namespace std; 
int main()
{
	vtkStructuredGridReader *reader = vtkStructuredGridReader::New();
	reader->SetFileName("../../dataset\\curvilinear\\comb.vtk");
	reader->Update();

	double *range = reader->GetOutput()->GetScalarRange();

	vtkColorTransferFunction *color = vtkColorTransferFunction::New();
	color->AddRGBPoint(range[0], 1, 1, 0);
	color->AddRGBPoint((range[0] + range[1]) / 2, 1, 0, 1);
	color->AddRGBPoint(range[1], 0, 1, 1);

	vtkPiecewiseFunction *opacity = vtkPiecewiseFunction::New();
	opacity->AddPoint(range[0], 0);
	opacity->AddPoint(range[1], 1);

	vtkVolumeProperty *property = vtkVolumeProperty::New();
	property->SetColor(color);
	property->SetScalarOpacity(opacity);

	//unstructured volume rendering algorithms only supported tetrahedral grid
	vtkDataSetTriangleFilter *filter = vtkDataSetTriangleFilter::New();
	filter->SetInputData(reader->GetOutput());
	filter->Update();

	vtkProjectedTetrahedraMapper *mapper = vtkProjectedTetrahedraMapper::New();
	mapper->SetInputData(filter->GetOutput());
	mapper->Update();

	vtkVolume *volume = vtkVolume::New();
	volume->SetMapper(mapper);
	volume->SetProperty(property);

	vtkRenderer *renderer = vtkRenderer::New();
	renderer->AddVolume(volume);

	vtkRenderWindow *rWin = vtkRenderWindow::New();
	rWin->AddRenderer(renderer);

	vtkRenderWindowInteractor *rwi = vtkRenderWindowInteractor::New();
	rwi->SetRenderWindow(rWin);
	rwi->SetSize(500, 500);

	//rwi->Render();
	//rwi->Initialize();
	//rwi->Start();

	rWin->Render();
	rwi->Start();

	return 0;
}