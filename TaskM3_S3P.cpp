#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <chrono>


#define PRINT 1 // Defining a macro for printing vectors


int SZ = 100000000; // Default size of vectors


int *v1, *v2, *v_out; // Declaring pointers for vectors


// Declaring OpenCL memory buffers
cl_mem bufV1, bufV2, bufV_out;


// Declaring OpenCL objects
cl_device_id device_id;
cl_context context;
cl_program program;
cl_kernel kernel;
cl_command_queue queue;
cl_event event = NULL;
int err;


// Function for creating an OpenCL device
cl_device_id create_device();


// Function for setting up OpenCL device, context, queue, and kernel
void setup_openCL_device_context_queue_kernel(char *filename, char *kernelname);


// Function for building OpenCL program
cl_program build_program(cl_context ctx, cl_device_id dev, const char *filename);


// Function for setting up OpenCL memory buffers
void setup_kernel_memory();


// Function for copying kernel arguments
void copy_kernel_args();


// Function for freeing allocated memory
void free_memory();


// Function for initializing vectors with random values
void init(int *&A, int size);


// Function for printing vectors
void print(int *A, int size);


int main(int argc, char **argv)
{
    if (argc > 1)
    {
        SZ = atoi(argv[1]); // Setting the size of vectors to user input if provided
    }


    init(v1, SZ);    // Initializing vector 1
    init(v2, SZ);    // Initializing vector 2
    init(v_out, SZ); // Initializing the output vector


    size_t global[1] = {(size_t)SZ}; // Defining global work size for OpenCL kernel


    print(v1, SZ); // Printing vector 1
    print(v2, SZ); // Printing vector 2


    setup_openCL_device_context_queue_kernel((char *)"./vector_ops_ocl.cl", (char *)"vector_add_ocl"); // Setting up the OpenCL environment
    setup_kernel_memory();                                                                             // Setting up kernel memory buffers
    copy_kernel_args();                                                                                // Copying kernel arguments
    auto start = std::chrono::high_resolution_clock::now();                                            // Starting to measure time


    // Enqueueing kernel for execution
    clEnqueueNDRangeKernel(queue, kernel, 1, NULL, global, NULL, 0, NULL, &event);
    clWaitForEvents(1, &event); // Waiting for kernel execution to finish


    // Reading the output buffer
    clEnqueueReadBuffer(queue, bufV_out, CL_TRUE, 0, SZ * sizeof(int), &v_out[0], 0, NULL, NULL);
    print(v_out, SZ);                                                      // Printing the output vector
    auto stop = std::chrono::high_resolution_clock::now();                 // Stopping to measure time
    std::chrono::duration<double, std::milli> elapsed_time = stop - start; // Calculating elapsed time


    printf("Kernel Execution Time: %f ms\n", elapsed_time.count()); // Printing kernel execution time
    free_memory();                                                  // Freeing allocated memory
}


// Function for initializing vectors with random values
void init(int *&A, int size)
{
    A = (int *)malloc(sizeof(int) * size); // Allocating memory for the vector


    // Initializing vector elements with random values
    for (long i = 0; i < size; i++)
    {
        A[i] = rand() % 100;
    }
}


// Function for printing vectors
void print(int *A, int size)
{
    if (PRINT == 0)
    {
        return;
    }


    // Printing only the first and last five elements if the vector size is larger than 15
    if (PRINT == 1 && size > 15)
    {
        for (long i = 0; i < 5; i++)
        {
            printf("%d ", A[i]);
        }
        printf(" ..... ");
        for (long i = size - 5; i < size; i++)
        {
            printf("%d ", A[i]);
        }
    }
    else
    {
        // Printing all elements of the vector
        for (long i = 0; i < size; i++)
        {
            printf("%d ", A[i]);
        }
    }
    printf("\n----------------------------\n");
}


// Function for freeing allocated memory
void free_memory()
{
    // Releasing OpenCL memory objects
    clReleaseMemObject(bufV1);
    clReleaseMemObject(bufV2);
    clReleaseMemObject(bufV_out);


    // Releasing kernel, command queue, program, and context
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
    clReleaseProgram(program);
    clReleaseContext(context);


    // Freeing allocated memory for vectors
    free(v1);
    free(v2);
    free(v_out);
}


// Function for copying kernel arguments
void copy_kernel_args()
{
    // Setting kernel arguments
    clSetKernelArg(kernel, 0, sizeof(int), (void *)&SZ);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&bufV1);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&bufV2);
    clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&bufV_out);


    if (err < 0)
    {
        perror("Couldn't create a kernel argument");
        printf("error = %d", err);
        exit(1);
    }
}


// Function for setting up OpenCL memory buffers
void setup_kernel_memory()
{
    // Creating memory buffers for vectors on OpenCL device
    bufV1 = clCreateBuffer(context, CL_MEM_READ_WRITE, SZ * sizeof(int), NULL, NULL);
    bufV2 = clCreateBuffer(context, CL_MEM_READ_WRITE, SZ * sizeof(int), NULL, NULL);
    bufV_out = clCreateBuffer(context, CL_MEM_READ_WRITE, SZ * sizeof(int), NULL, NULL);


    // Writing data from host to device buffers
    clEnqueueWriteBuffer(queue, bufV1, CL_TRUE, 0, SZ * sizeof(int), &v1[0], 0, NULL, NULL);
    clEnqueueWriteBuffer(queue, bufV2, CL_TRUE, 0, SZ * sizeof(int), &v2[0], 0, NULL, NULL);
}


// Function for setting up OpenCL device, context, queue, and kernel
void setup_openCL_device_context_queue_kernel(char *filename, char *kernelname)
{
    device_id = create_device(); // Creating OpenCL device
    cl_int err;


    // Creating OpenCL context
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &err);
    if (err < 0)
    {
        perror("Couldn't create a context");
        exit(1);
    }


    // Building OpenCL program
    program = build_program(context, device_id, filename);


    // Creating command queue for the device
    queue = clCreateCommandQueueWithProperties(context, device_id, 0, &err);
    if (err < 0)
    {
        perror("Couldn't create a command queue");
        exit(1);
    };


    // Creating OpenCL kernel
    kernel = clCreateKernel(program, kernelname, &err);
    if (err < 0)
    {
        perror("Couldn't create a kernel");
        printf("error =%d", err);
        exit(1);
    };
}


// Function for building OpenCL program
cl_program build_program(cl_context ctx, cl_device_id dev, const char *filename)
{


    cl_program program;
    FILE *program_handle;
    char *program_buffer, *program_log;
    size_t program_size, log_size;


    // Reading the OpenCL program source code from file
    program_handle = fopen(filename, "r");
    if (program_handle == NULL)
    {
        perror("Couldn't find the program file");
        exit(1);
    }
    fseek(program_handle, 0, SEEK_END);
    program_size = ftell(program_handle);
    rewind(program_handle);
    program_buffer = (char *)malloc(program_size + 1);
    program_buffer[program_size] = '\0';
    fread(program_buffer, sizeof(char), program_size, program_handle);
    fclose(program_handle);


    // Creating OpenCL program from the source code
    program = clCreateProgramWithSource(ctx, 1,
                                        (const char **)&program_buffer, &program_size, &err);
    if (err < 0)
    {
        perror("Couldn't create the program");
        exit(1);
    }
    free(program_buffer);


    // Building the OpenCL program
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err < 0)
    {


        // If build fails, retrieving build log and printing it
        clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG,
                              0, NULL, &log_size);
        program_log = (char *)malloc(log_size + 1);
        program_log[log_size] = '\0';
        clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG,
                              log_size + 1, program_log, NULL);
        printf("%s\n", program_log);
        free(program_log);
        exit(1);
    }


    return program;
}


// Function for creating an OpenCL device
cl_device_id create_device()
{


    cl_platform_id platform;
    cl_device_id dev;
    int err;


    // Getting the first available OpenCL platform
    err = clGetPlatformIDs(1, &platform, NULL);
    if (err < 0)
    {
        perror("Couldn't identify a platform");
        exit(1);
    }


    // Trying to get a GPU device, if not available, trying CPU
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &dev, NULL);
    if (err == CL_DEVICE_NOT_FOUND)
    {


        printf("GPU not found\n");
        err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &dev, NULL);
    }
    if (err < 0)
    {
        perror("Couldn't access any devices");
        exit(1);
    }


    return dev;
}
