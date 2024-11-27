#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkOpenGLRenderWindow.h>
#include <iostream>

int main() {
	// 创建渲染器
	auto renderer = vtkSmartPointer<vtkRenderer>::New();

	// 创建渲染窗口
	auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);

	// 创建交互器
	auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);

	// 渲染窗口类型转换为 OpenGL 窗口
	auto openglRenderWindow = vtkOpenGLRenderWindow::SafeDownCast(renderWindow);
	if (!openglRenderWindow) {
		std::cerr << "Error: Could not retrieve OpenGL render window!" << std::endl;
		return -1;
	}

	


	// 输出显卡信息
	int major, minor;
	auto s = openglRenderWindow->GetOpenGLSupportMessage();
	openglRenderWindow->GetOpenGLVersion(major, minor);

	// 渲染并启动交互
	renderWindow->Render();
	interactor->Start();

	return 0;
}
