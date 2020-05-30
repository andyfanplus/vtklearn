#include<iostream>
#include<vtkMetaImageReader.h>
#include<vtkImageData.h>
#include<vtkColorTransferFunction.h>
#include<vtkPiecewiseFunction.h>
#include<vtkVolumeProperty.h>
#include<vtkGPUVolumeRayCastMapper.h>
#include<vtkVolume.h>
#include<vtkRenderer.h>
#include<vtkRenderWindow.h>
#include<vtkRenderWindowInteractor.h>

using namespace std; 
int main()
{
	vtkMetaImageReader *reader = vtkMetaImageReader::New();
	reader->SetFileName("../../dataset\\mhd\\CLOUDf.mhd");
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

	vtkGPUVolumeRayCastMapper *mapper = vtkGPUVolumeRayCastMapper::New();
	mapper->SetInputData(reader->GetOutput());
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
	rwi->Render();

	rwi->Initialize();
	rwi->Start();

	return 0;
}