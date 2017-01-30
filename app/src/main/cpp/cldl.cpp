#include <dlfcn.h>
#include "cldl.h"


int LoadImports(struct _CL* cl)
{
    cl->handle = dlopen("libOpenCL.so", RTLD_LAZY);

    if (!cl->handle)
    {
        return 0;
    }

    cl->ptr_clGetPlatformIDs = dlsym(cl->handle, "clGetPlatformIDs");
    if (!cl->ptr_clGetPlatformIDs) return 0;
    cl->ptr_clGetPlatformInfo = dlsym(cl->handle, "clGetPlatformInfo");
    if (!cl->ptr_clGetPlatformInfo) return 0;
    cl->ptr_clGetDeviceIDs = dlsym(cl->handle, "clGetDeviceIDs");
    if (!cl->ptr_clGetDeviceIDs) return 0;
    cl->ptr_clGetDeviceInfo = dlsym(cl->handle, "clGetDeviceInfo");
    if (!cl->ptr_clGetDeviceInfo) return 0;
    cl->ptr_clCreateContext = dlsym(cl->handle, "clCreateContext");
    if (!cl->ptr_clCreateContext) return 0;
    cl->ptr_clCreateContextFromType = dlsym(cl->handle, "clCreateContextFromType");
    if (!cl->ptr_clCreateContextFromType) return 0;
    cl->ptr_clRetainContext = dlsym(cl->handle, "clRetainContext");
    if (!cl->ptr_clRetainContext) return 0;
    cl->ptr_clReleaseContext = dlsym(cl->handle, "clReleaseContext");
    if (!cl->ptr_clReleaseContext) return 0;
    cl->ptr_clGetContextInfo = dlsym(cl->handle, "clGetContextInfo");
    if (!cl->ptr_clGetContextInfo) return 0;
    cl->ptr_clCreateCommandQueue = dlsym(cl->handle, "clCreateCommandQueue");
    if (!cl->ptr_clCreateCommandQueue) return 0;
    cl->ptr_clRetainCommandQueue = dlsym(cl->handle, "clRetainCommandQueue");
    if (!cl->ptr_clRetainCommandQueue) return 0;
    cl->ptr_clReleaseCommandQueue = dlsym(cl->handle, "clReleaseCommandQueue");
    if (!cl->ptr_clReleaseCommandQueue) return 0;
    cl->ptr_clGetCommandQueueInfo = dlsym(cl->handle, "clGetCommandQueueInfo");
    if (!cl->ptr_clGetCommandQueueInfo) return 0;
    cl->ptr_clSetCommandQueueProperty = dlsym(cl->handle, "clSetCommandQueueProperty");
    if (!cl->ptr_clSetCommandQueueProperty) return 0;
    cl->ptr_clCreateBuffer = dlsym(cl->handle, "clCreateBuffer");
    if (!cl->ptr_clCreateBuffer) return 0;
    cl->ptr_clCreateSubBuffer = dlsym(cl->handle, "clCreateSubBuffer");
    if (!cl->ptr_clCreateSubBuffer) return 0;
    cl->ptr_clCreateImage2D = dlsym(cl->handle, "clCreateImage2D");
    if (!cl->ptr_clCreateImage2D) return 0;
    cl->ptr_clCreateImage3D = dlsym(cl->handle, "clCreateImage3D");
    if (!cl->ptr_clCreateImage3D) return 0;
    cl->ptr_clRetainMemObject = dlsym(cl->handle, "clRetainMemObject");
    if (!cl->ptr_clRetainMemObject) return 0;
    cl->ptr_clReleaseMemObject = dlsym(cl->handle, "clReleaseMemObject");
    if (!cl->ptr_clReleaseMemObject) return 0;
    cl->ptr_clGetSupportedImageFormats = dlsym(cl->handle, "clGetSupportedImageFormats");
    if (!cl->ptr_clGetSupportedImageFormats) return 0;
    cl->ptr_clGetMemObjectInfo = dlsym(cl->handle, "clGetMemObjectInfo");
    if (!cl->ptr_clGetMemObjectInfo) return 0;
    cl->ptr_clGetImageInfo = dlsym(cl->handle, "clGetImageInfo");
    if (!cl->ptr_clGetImageInfo) return 0;
    cl->ptr_clSetMemObjectDestructorCallback = dlsym(cl->handle, "clSetMemObjectDestructorCallback");
    if (!cl->ptr_clSetMemObjectDestructorCallback) return 0;
    cl->ptr_clCreateSampler = dlsym(cl->handle, "clCreateSampler");
    if (!cl->ptr_clCreateSampler) return 0;
    cl->ptr_clRetainSampler = dlsym(cl->handle, "clRetainSampler");
    if (!cl->ptr_clRetainSampler) return 0;
    cl->ptr_clReleaseSampler = dlsym(cl->handle, "clReleaseSampler");
    if (!cl->ptr_clReleaseSampler) return 0;
    cl->ptr_clGetSamplerInfo = dlsym(cl->handle, "clGetSamplerInfo");
    if (!cl->ptr_clGetSamplerInfo) return 0;
    cl->ptr_clCreateProgramWithSource = dlsym(cl->handle, "clCreateProgramWithSource");
    if (!cl->ptr_clCreateProgramWithSource) return 0;
    cl->ptr_clCreateProgramWithBinary = dlsym(cl->handle, "clCreateProgramWithBinary");
    if (!cl->ptr_clCreateProgramWithBinary) return 0;
    cl->ptr_clRetainProgram = dlsym(cl->handle, "clRetainProgram");
    if (!cl->ptr_clRetainProgram) return 0;
    cl->ptr_clReleaseProgram = dlsym(cl->handle, "clReleaseProgram");
    if (!cl->ptr_clReleaseProgram) return 0;
    cl->ptr_clBuildProgram = dlsym(cl->handle, "clBuildProgram");
    if (!cl->ptr_clBuildProgram) return 0;
    cl->ptr_clUnloadCompiler = dlsym(cl->handle, "clUnloadCompiler");
    if (!cl->ptr_clUnloadCompiler) return 0;
    cl->ptr_clGetProgramInfo = dlsym(cl->handle, "clGetProgramInfo");
    if (!cl->ptr_clGetProgramInfo) return 0;
    cl->ptr_clGetProgramBuildInfo = dlsym(cl->handle, "clGetProgramBuildInfo");
    if (!cl->ptr_clGetProgramBuildInfo) return 0;
    cl->ptr_clCreateKernel = dlsym(cl->handle, "clCreateKernel");
    if (!cl->ptr_clCreateKernel) return 0;
    cl->ptr_clCreateKernelsInProgram = dlsym(cl->handle, "clCreateKernelsInProgram");
    if (!cl->ptr_clCreateKernelsInProgram) return 0;
    cl->ptr_clRetainKernel = dlsym(cl->handle, "clRetainKernel");
    if (!cl->ptr_clRetainKernel) return 0;
    cl->ptr_clReleaseKernel = dlsym(cl->handle, "clReleaseKernel");
    if (!cl->ptr_clReleaseKernel) return 0;
    cl->ptr_clSetKernelArg = dlsym(cl->handle, "clSetKernelArg");
    if (!cl->ptr_clSetKernelArg) return 0;
    cl->ptr_clGetKernelInfo = dlsym(cl->handle, "clGetKernelInfo");
    if (!cl->ptr_clGetKernelInfo) return 0;
    cl->ptr_clGetKernelWorkGroupInfo = dlsym(cl->handle, "clGetKernelWorkGroupInfo");
    if (!cl->ptr_clGetKernelWorkGroupInfo) return 0;
    cl->ptr_clWaitForEvents = dlsym(cl->handle, "clWaitForEvents");
    if (!cl->ptr_clWaitForEvents) return 0;
    cl->ptr_clGetEventInfo = dlsym(cl->handle, "clGetEventInfo");
    if (!cl->ptr_clGetEventInfo) return 0;
    cl->ptr_clCreateUserEvent = dlsym(cl->handle, "clCreateUserEvent");
    if (!cl->ptr_clCreateUserEvent) return 0;
    cl->ptr_clRetainEvent = dlsym(cl->handle, "clRetainEvent");
    if (!cl->ptr_clRetainEvent) return 0;
    cl->ptr_clReleaseEvent = dlsym(cl->handle, "clReleaseEvent");
    if (!cl->ptr_clReleaseEvent) return 0;
    cl->ptr_clSetUserEventStatus = dlsym(cl->handle, "clSetUserEventStatus");
    if (!cl->ptr_clSetUserEventStatus) return 0;
    cl->ptr_clSetEventCallback = dlsym(cl->handle, "clSetEventCallback");
    if (!cl->ptr_clSetEventCallback) return 0;
    cl->ptr_clGetEventProfilingInfo = dlsym(cl->handle, "clGetEventProfilingInfo");
    if (!cl->ptr_clGetEventProfilingInfo) return 0;
    cl->ptr_clFlush = dlsym(cl->handle, "clFlush");
    if (!cl->ptr_clFlush) return 0;
    cl->ptr_clFinish = dlsym(cl->handle, "clFinish");
    if (!cl->ptr_clFinish) return 0;
    cl->ptr_clEnqueueReadBuffer = dlsym(cl->handle, "clEnqueueReadBuffer");
    if (!cl->ptr_clEnqueueReadBuffer) return 0;
    cl->ptr_clEnqueueReadBufferRect = dlsym(cl->handle, "clEnqueueReadBufferRect");
    if (!cl->ptr_clEnqueueReadBufferRect) return 0;
    cl->ptr_clEnqueueWriteBuffer = dlsym(cl->handle, "clEnqueueWriteBuffer");
    if (!cl->ptr_clEnqueueWriteBuffer) return 0;
    cl->ptr_clEnqueueWriteBufferRect = dlsym(cl->handle, "clEnqueueWriteBufferRect");
    if (!cl->ptr_clEnqueueWriteBufferRect) return 0;
    cl->ptr_clEnqueueCopyBuffer = dlsym(cl->handle, "clEnqueueCopyBuffer");
    if (!cl->ptr_clEnqueueCopyBuffer) return 0;
    cl->ptr_clEnqueueCopyBufferRect = dlsym(cl->handle, "clEnqueueCopyBufferRect");
    if (!cl->ptr_clEnqueueCopyBufferRect) return 0;
    cl->ptr_clEnqueueReadImage = dlsym(cl->handle, "clEnqueueReadImage");
    if (!cl->ptr_clEnqueueReadImage) return 0;
    cl->ptr_clEnqueueWriteImage = dlsym(cl->handle, "clEnqueueWriteImage");
    if (!cl->ptr_clEnqueueWriteImage) return 0;
    cl->ptr_clEnqueueCopyImage = dlsym(cl->handle, "clEnqueueCopyImage");
    if (!cl->ptr_clEnqueueCopyImage) return 0;
    cl->ptr_clEnqueueCopyImageToBuffer = dlsym(cl->handle, "clEnqueueCopyImageToBuffer");
    if (!cl->ptr_clEnqueueCopyImageToBuffer) return 0;
    cl->ptr_clEnqueueCopyBufferToImage = dlsym(cl->handle, "clEnqueueCopyBufferToImage");
    if (!cl->ptr_clEnqueueCopyBufferToImage) return 0;
    cl->ptr_clEnqueueMapBuffer = dlsym(cl->handle, "clEnqueueMapBuffer");
    if (!cl->ptr_clEnqueueMapBuffer) return 0;
    cl->ptr_clEnqueueMapImage = dlsym(cl->handle, "clEnqueueMapImage");
    if (!cl->ptr_clEnqueueMapImage) return 0;
    cl->ptr_clEnqueueUnmapMemObject = dlsym(cl->handle, "clEnqueueUnmapMemObject");
    if (!cl->ptr_clEnqueueUnmapMemObject) return 0;
    cl->ptr_clEnqueueNDRangeKernel = dlsym(cl->handle, "clEnqueueNDRangeKernel");
    if (!cl->ptr_clEnqueueNDRangeKernel) return 0;
    cl->ptr_clEnqueueTask = dlsym(cl->handle, "clEnqueueTask");
    if (!cl->ptr_clEnqueueTask) return 0;
    cl->ptr_clEnqueueNativeKernel = dlsym(cl->handle, "clEnqueueNativeKernel");
    if (!cl->ptr_clEnqueueNativeKernel) return 0;
    cl->ptr_clEnqueueMarker = dlsym(cl->handle, "clEnqueueMarker");
    if (!cl->ptr_clEnqueueMarker) return 0;
    cl->ptr_clEnqueueWaitForEvents = dlsym(cl->handle, "clEnqueueWaitForEvents");
    if (!cl->ptr_clEnqueueWaitForEvents) return 0;
    cl->ptr_clEnqueueBarrier = dlsym(cl->handle, "clEnqueueBarrier");
    if (!cl->ptr_clEnqueueBarrier) return 0;
    cl->ptr_clGetExtensionFunctionAddress = dlsym(cl->handle, "clGetExtensionFunctionAddress");
    if (!cl->ptr_clGetExtensionFunctionAddress) return 0;

    return 1;
}

void ReleaseImports(struct _CL* cl)
{
    dlclose(cl->handle);
}
