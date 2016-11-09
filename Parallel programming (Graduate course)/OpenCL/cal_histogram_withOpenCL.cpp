#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include <fstream>
#include <iostream>
#include <CL/opencl.h>
#include <math.h>


		         /* Student ID: 0440061*/		

				 
// OpenCL kernel. Each work item takes care of one element of c
const char *kernelSource =                                      "\n" \
"#pragma OPENCL EXTENSION cl_khr_fp64 : enable                   \n" \
"__kernel void histogram(  __global unsigned int *img,           \n" \
"                       __global unsigned int *ptr,              \n" \
"                       const unsigned int input_size)           \n" \
"{                                                               \n" \
"    //Get our global thread ID                                  \n" \
"    int id = get_global_id(0);                                  \n" \
"                                                                \n" \
"    //make sure we do not go out of bounds                      \n" \
"    if(id<input_size){                                          \n" \
"    int distinguish=id%3;                                       \n" \   
"    int index=distinguish*256+img[id];                          \n" \  
"       atomic_inc(ptr+index);                                            \n" \  
"     }                                                          \n" \    
"}                                                               \n" \
                                                                  "\n" ;																
 
int main( int argc, char* argv[] )
{
	// declare input
	unsigned int a, input_size;
	unsigned i=0;
	unsigned int *ptr;
	std::fstream inFile("input", std::ios_base::in);
	std::ofstream outFile("0440061.out", std::ios_base::out);
	
	
	
	
	//create memory and init input  (input_size,image,ptr)
	inFile >> input_size;
	unsigned int *image = new unsigned int[input_size];
	while( inFile >> a ) {
		image[i++] = a;
	}
	ptr=(unsigned int *)malloc(256*3*sizeof(int));
    memset (ptr, 0x0, 256 * 3 * sizeof(unsigned int));
	
	// Device input buffers
	cl_mem d_image;
	cl_mem d_input_size;
	// Device output buffers
	cl_mem d_ptr;
	
	cl_platform_id cpPlatform;        // OpenCL platform
    cl_device_id device_id;           // device ID
    cl_context context;               // context
    cl_command_queue queue;           // command queue
    cl_program program;               // program
    cl_kernel kernel;                 // kernel
	
	//size, in bytes,of image
	size_t bytes_image=input_size*sizeof(int);
	size_t bytes_ptr=256*3*sizeof(int);
	
	// declare globalsize and work-items
	size_t globalSize, localSize;
    cl_int err;
 
    // Number of work items in each local work group
    localSize = 64;
 
    // Number of total work items - localSize must be devisor
    globalSize = ceil(input_size/(float)localSize)*localSize;
	
	// Bind to platform
    err = clGetPlatformIDs(1, &cpPlatform, NULL);
 
    // Get ID for the device
    err = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
 
    // Create a context  
    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
 
    // Create a command queue 
    queue = clCreateCommandQueue(context, device_id, 0, &err);
 
    // Create the compute program from the source buffer
    program = clCreateProgramWithSource(context, 1,
                            (const char **) & kernelSource, NULL, &err);
 
    // Build the program executable 
    clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
 
    // Create the compute kernel in the program we wish to run
    kernel = clCreateKernel(program, "histogram", &err);
 
    // Create the input and output arrays in device memory for our calculation
    d_image = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes_image, NULL, NULL);
    d_ptr = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes_ptr, NULL, NULL);
	
	// Write our data set into the input array in device memory
    err  = clEnqueueWriteBuffer(queue, d_image, CL_TRUE, 0,
                                   bytes_image, image, 0, NULL, NULL);							   
	err  = clEnqueueWriteBuffer(queue, d_ptr, CL_TRUE, 0,
                                   bytes_ptr, ptr, 0, NULL, NULL);
	
			
	// Set the arguments to our compute kernel		
	err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_image);
    err  = clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_ptr);
	err  = clSetKernelArg(kernel, 2, sizeof(unsigned int), &input_size);
	
	// Execute the kernel over the entire range of the data set  
    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, &localSize,
                                                              0, NULL, NULL);
															  
    // Wait for the command queue to get serviced before reading back results
    clFinish(queue);
 
    // Read the results from the device
    clEnqueueReadBuffer(queue, d_ptr, CL_TRUE, 0,
                                bytes_ptr, ptr, 0, NULL, NULL );
								
	// write out to 0440061.out
	for(unsigned int i = 0; i < 256 * 3; ++i) {
		if (i % 256 == 0 && i != 0)
			outFile << std::endl;
		outFile << ptr[i]<< ' ';
	}
	
	// release OpenCL resources
    clReleaseMemObject(d_image);
    clReleaseMemObject(d_ptr);
    clReleaseMemObject(d_input_size);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
 
    //release host memory
    free(image);
    free(ptr);
  //  free(input_size);
	
	
	inFile.close();
	outFile.close();

	return 0;
}	
