#include<iostream>

#include"vtkCompositeDataSet.h"
#include"vtkMultiBlockDataSet.h"
#include"vtkXMLMultiBlockDataReader.h"
#include"vtkUnstructuredGrid.h"
#include"vtkDataSet.h"
#include"vtkDataSetReader.h"

#include"vtkMPIController.h"
#include"vtkDistributedDataFilter.h"

using namespace std; 
//single block
void single(int argc, char **argv) {
	vtkMPIController *contr = vtkMPIController::New();
	contr->Initialize(&argc, &argv, 0);

	int numProcs = contr->GetNumberOfProcesses();
	int me = contr->GetLocalProcessId();

	
	vtkDataSet *ds = nullptr;
	vtkUnstructuredGrid *ug = vtkUnstructuredGrid::New();

	if (me == 0) {
		//read data on the master process
		const char* fname = "../../dataset/curvilinear/comb.vtk";
		vtkDataSetReader *dsr = vtkDataSetReader::New();
		dsr->SetFileName(fname);
		dsr->Update();

		ds = dsr->GetOutput();
	}
	else {
		//the salve process is empty
		ds = static_cast<vtkDataSet*>(ug);
	}
	//d3 object
	vtkDistributedDataFilter *dd = vtkDistributedDataFilter::New();
	dd->SetInputData(ds);
	dd->SetController(contr);
	dd->UseMinimalMemoryOff();
	dd->Update();

	//the output with d3 fitler is unstructured grid;
	ug = vtkUnstructuredGrid::SafeDownCast(dd->GetOutput());
	cout << ug->GetNumberOfCells() << endl;

	if (me == 0) {
		contr->TriggerBreakRMIs();
	}
	else contr->ProcessRMIs();


	contr->Finalize();
}


//composite data
void multiblock(int argc, char **argv) {
	vtkMPIController *contr = vtkMPIController::New();
	contr->Initialize(&argc, &argv, 0);

	int numProcs = contr->GetNumberOfProcesses();
	int me = contr->GetLocalProcessId();


	vtkCompositeDataSet *input = nullptr;
	vtkMultiBlockDataSet *output = nullptr;

	//load data on all process
	const char* fname = "../../dataset/multiblock/yf17.vtm";
	vtkXMLMultiBlockDataReader *reader = vtkXMLMultiBlockDataReader::New();
	reader->SetFileName(fname);
	reader->Update();

	if (me == 0) {
		input = reader->GetOutput();// data on master process
	}
	else {
		input->CopyStructure(reader->GetOutput());// slave process keep same structure with source data
	}
	//d3 object
	vtkDistributedDataFilter *dd = vtkDistributedDataFilter::New();
	dd->SetInputData(input);
	dd->SetController(contr);
	dd->UseMinimalMemoryOff();
	dd->Update();

	//the output with d3 fitler is composited data;
	output = vtkMultiBlockDataSet::SafeDownCast(dd->GetOutput());
	cout << output->GetNumberOfCells() << endl;

	if (me == 0) {
		contr->TriggerBreakRMIs();
	}
	else contr->ProcessRMIs();


	contr->Finalize();
}
int main(int argc, char **argv)
{
	

	return 0;
}
