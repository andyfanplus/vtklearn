#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkOpenGLRenderWindow.h>
#include <iostream>

int main() {
	// ������Ⱦ��
	auto renderer = vtkSmartPointer<vtkRenderer>::New();

	// ������Ⱦ����
	auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);

	// ����������
	auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);

	// ��Ⱦ��������ת��Ϊ OpenGL ����
	auto openglRenderWindow = vtkOpenGLRenderWindow::SafeDownCast(renderWindow);
	if (!openglRenderWindow) {
		std::cerr << "Error: Could not retrieve OpenGL render window!" << std::endl;
		return -1;
	}

	


	// ����Կ���Ϣ
	int major, minor;
	auto s = openglRenderWindow->GetOpenGLSupportMessage();
	openglRenderWindow->GetOpenGLVersion(major, minor);

	// ��Ⱦ����������
	renderWindow->Render();
	interactor->Start();

	return 0;
}
