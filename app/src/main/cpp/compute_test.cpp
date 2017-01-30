#include <jni.h>
#include <dlfcn.h>

#include <android/log.h>

#define APPNAME "ComputeTest"

#include <vector>
#include <string>
#include <cstdio>
#include <algorithm>
#include <random>
#include <iostream>
#include <chrono>
#include <sstream>

#include "CL/cl.h"
#include "cldl.h"

#define CHECK_ERROR(err, msg) if (err != CL_SUCCESS) { return env->NewStringUTF(msg);}
#define CHECK_ERROR_TEST(err) if (err != CL_SUCCESS) { return err; }

_CL CL;

extern cl_int TestAdd(cl_context context, cl_device_id device, cl_command_queue cmdqueue, int size, float& time);
extern cl_int TestCopy(cl_context context, cl_device_id device, cl_command_queue cmdqueue, int size, float& time);
extern cl_int TestCopyKernel(cl_context context, cl_device_id device, cl_command_queue cmdqueue, int size, float& time);
extern cl_int TestSgemmNaive(cl_context context, cl_device_id device, cl_command_queue cmdqueue, unsigned int size, float& time);


extern "C"
jstring
Java_com_yozhijk_computetest_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */)
{
    // Try loading OpenCL imports
    if (!LoadImports(&CL))
    {
        return env->NewStringUTF("OpenCL is not supported on this device");
    }

    // Initialize OpenCL
    cl_platform_id platform;
    auto status = CL.clGetPlatformIDs(1, &platform, nullptr);

    CHECK_ERROR(status, "clGetPlatformIDs failed");

    cl_uint num_devices = 0;
    status = CL.clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, nullptr, &num_devices);

    CHECK_ERROR(status, "clGetDeviceIDs failed");

    if (num_devices == 0)
    {
        return env->NewStringUTF("No OpenCL devices detected\n");
    }

    std::vector<cl_device_id> devices(num_devices);

    status = CL.clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, num_devices, &devices[0], nullptr);

    CHECK_ERROR(status, "clGetDeviceIDs failed");

    auto context = CL.clCreateContext(nullptr, 1, &devices[0], nullptr, nullptr, &status);

    CHECK_ERROR(status, "clCreateContext failed");

    auto cmdqueue = CL.clCreateCommandQueue(context, devices[0], 0, &status);

    CHECK_ERROR(status, "clCreateCommandQueue failed");

    float time = 0.f;

    std::ostringstream oss;

    // Start running tests
    for (int size = 1024; size <= 1024; size *= 4)
    {
        oss << "Testing with size = " << size << '\n';

//        auto res = TestAdd(context, devices[0], cmdqueue, size, time);
//        oss << "Test ADD result: " << (res == CL_SUCCESS ? "PASSED\n" : "FAILED\n");
//        oss << "Test average time: " << time << "ms\n";
//        oss << "Test average throughput: " << (3.f * size * sizeof(float)) / (time / 1000.f) / 1024 / 1024 / 1024
//        << "Gb/s\n\n";
//
//        res = TestCopy(context, devices[0], cmdqueue, size, time);
//        oss << "Test COPY result: " << (res == CL_SUCCESS ? "PASSED\n" : "FAILED\n");
//        oss << "Test average time: " << time << "ms\n";
//        oss << "Test average throughput: " << (2.f * size * sizeof(float)) / (time / 1000.f) / 1024 / 1024 / 1024
//        << "Gb/s\n\n";
//
//        res = TestCopyKernel(context, devices[0], cmdqueue, size, time);
//        oss << "Test COPY KERNEL result: " << (res == CL_SUCCESS ? "PASSED\n" : "FAILED\n");
//        oss << "Test average time: " << time << "ms\n";
//        oss << "Test average throughput: " << (2.f * size * sizeof(float)) / (time / 1000.f) / 1024 / 1024 / 1024
//        << "Gb/s\n\n\n\n\n";

        auto res = TestSgemmNaive(context, devices[0], cmdqueue, size, time);
        oss << "Test SGEMM result: " << (res == CL_SUCCESS ? "PASSED\n" : "FAILED\n");
        oss << "Test average time: " << time << "ms\n";
        oss << "Test average throughput: " << (size * size * (size + 3)) / (time / 1000.f) / 1024 / 1024 / 1024
        << "Gflops/s\n\n";

        __android_log_print(ANDROID_LOG_VERBOSE, "ComputeTest", "Tests finished for size %d\n", size);
    }

    ReleaseImports(&CL);

    return env->NewStringUTF(oss.str().c_str());
}


cl_int TestAdd(cl_context context, cl_device_id device, cl_command_queue cmdqueue, int size, float& time)
{
    char const* kernel_add =
            "__kernel void add(__global float4* a, __global float4* b, __global float4* c)\n"
                    "{\n int idx = get_global_id(0);\n "
                    "int size = get_global_size(0);\n"
                    "c[idx] = a[idx] + b[idx];\n"
                    "c[idx + size] = a[idx + size] + b[idx + size];\n"
                    "c[idx + 2*size] = a[idx + 2*size] + b[idx + 2*size];\n"
                    "c[idx + 3*size] = a[idx + 3*size] + b[idx + 3*size];\n"
                    "}\n";

    cl_int status = CL_SUCCESS;
    std::size_t kernel_len = strlen(kernel_add);

    auto program = CL.clCreateProgramWithSource(context, 1, &kernel_add, &kernel_len, &status);

    CHECK_ERROR_TEST(status);

    status = CL.clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr);

    CHECK_ERROR_TEST(status);

    auto kernel = CL.clCreateKernel(program, "add", &status);

    CHECK_ERROR_TEST(status);


    std::vector<float> a(size);
    std::vector<float> b(size);
    std::vector<float> c(size);
    std::vector<float> c_ref(size);

    std::random_device rand;
    std::mt19937 rng(rand());
    std::uniform_real_distribution<> rand_dist(0.f, 100.f);

    std::generate(a.begin(), a.end(), [&rng, &rand_dist](){ return rand_dist(rng);});
    std::generate(b.begin(), b.end(), [&rng, &rand_dist](){ return rand_dist(rng);});

    for (auto i = 0U; i < size; ++i)
    {
        c_ref[i] = a[i] + b[i];
    }

    cl_mem dev_a = CL.clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                     size * sizeof(float), &a[0], &status);

    CHECK_ERROR_TEST(status);

    cl_mem dev_b = CL.clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                     size * sizeof(float), &b[0], &status);

    CHECK_ERROR_TEST(status);

    cl_mem dev_c = CL.clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                     size * sizeof(float), nullptr, &status);

    CHECK_ERROR_TEST(status);

    CL.clSetKernelArg(kernel, 0, sizeof(cl_mem), &dev_a);
    CL.clSetKernelArg(kernel, 1, sizeof(cl_mem), &dev_b);
    CL.clSetKernelArg(kernel, 2, sizeof(cl_mem), &dev_c);

    std::size_t global_size = size >> 4;
    std::size_t local_size = 256;

    auto time_start = std::chrono::high_resolution_clock::now();

    auto constexpr kNumPasses = 100U;
    for (auto i = 0U; i < kNumPasses; ++i)
    {
        status = CL.clEnqueueNDRangeKernel(cmdqueue, kernel, 1, nullptr, &global_size, &local_size,
                                           0, nullptr, nullptr);
        CHECK_ERROR_TEST(status);
    }

    CL.clFinish(cmdqueue);

    CHECK_ERROR_TEST(status);

    auto time_end = std::chrono::high_resolution_clock::now();

    auto delta = (float)(std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start).count()) / kNumPasses;

    time = delta;

    // Check first 1M elements, too slow
    size = std::min(size, 1024 * 1024);

    status = CL.clEnqueueReadBuffer(cmdqueue, dev_c, CL_TRUE, 0, size * sizeof(float), &c[0], 0, nullptr, nullptr);

    CHECK_ERROR_TEST(status);

    for (auto i = 0U; i < size; ++i)
    {
        if (std::fabs(c_ref[i] - c[i]) > 0.002f)
            return -1;
    }

    return CL_SUCCESS;
}

cl_int TestCopyKernel(cl_context context, cl_device_id device, cl_command_queue cmdqueue, int size, float& time)
{
    char const* kernel_copy =
            "__kernel void copy(__global float4 const* a, __global float4* c)\n"
                    "{\n int idx = get_global_id(0);\n"
                    "int size = get_global_size(0);\n"
                    "c[idx] = a[idx];\n"
                    "c[idx + size] = a[idx + size];\n"
                    "c[idx + size * 2] = a[idx + size * 2];\n"
                    "c[idx + size * 3] = a[idx + size * 3];\n"
                    "}\n";

    cl_int status = CL_SUCCESS;
    std::size_t kernel_len = strlen(kernel_copy);

    auto program = CL.clCreateProgramWithSource(context, 1, &kernel_copy, &kernel_len, &status);

    CHECK_ERROR_TEST(status);

    status = CL.clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr);

    CHECK_ERROR_TEST(status);

    auto kernel = CL.clCreateKernel(program, "copy", &status);

    CHECK_ERROR_TEST(status);

    std::vector<float> a(size);
    std::vector<float> c(size);

    std::random_device rand;
    std::mt19937 rng(rand());
    std::uniform_real_distribution<> rand_dist(0.f, 100.f);

    std::generate(a.begin(), a.end(), [&rng, &rand_dist](){ return rand_dist(rng);});

    cl_mem dev_a = CL.clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                     size * sizeof(float), &a[0], &status);

    CHECK_ERROR_TEST(status);


    cl_mem dev_c = CL.clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                     size * sizeof(float), nullptr, &status);

    CHECK_ERROR_TEST(status);

    CL.clSetKernelArg(kernel, 0, sizeof(cl_mem), &dev_a);
    CL.clSetKernelArg(kernel, 1, sizeof(cl_mem), &dev_c);

    std::size_t global_size = size >> 4;
    std::size_t local_size = 256;

    auto time_start = std::chrono::high_resolution_clock::now();

    auto constexpr kNumPasses = 100U;
    for (auto i = 0U; i < kNumPasses; ++i)
    {
        status = CL.clEnqueueNDRangeKernel(cmdqueue, kernel, 1, nullptr, &global_size, &local_size,
                                           0, nullptr, nullptr);
        CHECK_ERROR_TEST(status);
    }

    CL.clFinish(cmdqueue);

    CHECK_ERROR_TEST(status);

    auto time_end = std::chrono::high_resolution_clock::now();

    auto delta = (float)(std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start).count()) / kNumPasses;

    time = delta;

    // Check first 1M elements, too slow
    size = std::min(size, 1024 * 1024);

    status = CL.clEnqueueReadBuffer(cmdqueue, dev_c, CL_TRUE, 0, size * sizeof(float), &c[0], 0, nullptr, nullptr);

    CHECK_ERROR_TEST(status);

    for (auto i = 0U; i < size; ++i)
    {
        if (c[i] != a[i])
            return -1;
    }

    return CL_SUCCESS;
}

cl_int TestCopy(cl_context context, cl_device_id device, cl_command_queue cmdqueue, int size, float& time)
{
    std::vector<float> a(size);
    std::vector<float> c(size);

    std::random_device rand;
    std::mt19937 rng(rand());
    std::uniform_real_distribution<> rand_dist(0.f, 100.f);

    std::generate(a.begin(), a.end(), [&rng, &rand_dist](){ return rand_dist(rng);});

    cl_int status = CL_SUCCESS;
    cl_mem dev_a = CL.clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                     size * sizeof(float), &a[0], &status);

    CHECK_ERROR_TEST(status);


    cl_mem dev_c = CL.clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                     size * sizeof(float), nullptr, &status);

    CHECK_ERROR_TEST(status);

    auto time_start = std::chrono::high_resolution_clock::now();

    auto constexpr kNumPasses = 100U;
    for (auto i = 0U; i < kNumPasses; ++i)
    {
        status = CL.clEnqueueCopyBuffer(cmdqueue, dev_a, dev_c, 0, 0, size * sizeof(float), 0, nullptr, nullptr);
        CHECK_ERROR_TEST(status);
    }

    CL.clFinish(cmdqueue);

    CHECK_ERROR_TEST(status);

    auto time_end = std::chrono::high_resolution_clock::now();

    auto delta = (float)(std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start).count()) / kNumPasses;

    time = delta;

    //Check first 1M elements, too slow
    size = std::min(size, 1024 * 1024);

    status = CL.clEnqueueReadBuffer(cmdqueue, dev_c, CL_TRUE, 0, size * sizeof(float), &c[0], 0, nullptr, nullptr);

    CHECK_ERROR_TEST(status);

    for (auto i = 0U; i < size; ++i)
    {
        if (c[i] != a[i])
            return -1;
    }

    return CL_SUCCESS;
}


cl_int TestSgemmNaive(cl_context context, cl_device_id device, cl_command_queue cmdqueue, unsigned int size, float& time)
{
    char const* kernel_sgemm =
            "__kernel void sgemm(__global float const* A, __global float const* B, "
                    "__global float* C/*, float alpha, float beta*/ , uint N)\n"
                    "{ int j = get_global_id(0);\n"
                    "int i = get_global_id(1);\n"
                    "float sum = 0.f;\n"
                    "for (uint k = 0U; k < N; ++k) {\n"
                    "sum += A[i * N + k] * B[k * N + j];\n"
                    "}\n"
                    "C[i * N + j] = sum;\n"
                    "}\n";

    cl_int status = CL_SUCCESS;
    std::size_t kernel_len = strlen(kernel_sgemm);

    auto program = CL.clCreateProgramWithSource(context, 1, &kernel_sgemm, &kernel_len, &status);

    CHECK_ERROR_TEST(status);

    status = CL.clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr);

    CHECK_ERROR_TEST(status);

    auto kernel = CL.clCreateKernel(program, "sgemm", &status);

    CHECK_ERROR_TEST(status);

    std::vector<float> A(size * size);
    std::vector<float> B(size * size);
    std::vector<float> C(size * size);
    std::vector<float> C_ref(size * size);

    std::random_device rand;
    std::mt19937 rng(rand());
    std::uniform_real_distribution<> rand_dist(0.f, 100.f);

    std::generate(A.begin(), A.end(), [&rng, &rand_dist](){ return rand_dist(rng);});
    std::generate(B.begin(), B.end(), [&rng, &rand_dist](){ return rand_dist(rng);});
    std::generate(C_ref.begin(), C_ref.end(), [&rng, &rand_dist](){ return rand_dist(rng);});
    std::copy(C_ref.cbegin(), C_ref.cend(), C.begin());

    //float alpha = 2.5f;
    //float beta = 1.5f;
    for (auto i = 0U; i < size; ++i)
        for (auto j = 0U; j < size; ++j)
        {
            float sum = 0.f;
            for (auto k = 0U; k < size; ++k)
                sum += A[i * size + k] * B[k * size + j];

            C_ref[i * size + j] = sum;
        }



    cl_mem dev_A = CL.clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                     size * size * sizeof(float), &A[0], &status);

    CHECK_ERROR_TEST(status);

    cl_mem dev_B = CL.clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                     size * size * sizeof(float), &B[0], &status);

    CHECK_ERROR_TEST(status);


    cl_mem dev_C = CL.clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
                                     size * size * sizeof(float), &C[0], &status);

    CHECK_ERROR_TEST(status);

    CL.clSetKernelArg(kernel, 0, sizeof(cl_mem), &dev_A);
    CL.clSetKernelArg(kernel, 1, sizeof(cl_mem), &dev_B);
    CL.clSetKernelArg(kernel, 2, sizeof(cl_mem), &dev_C);
    //CL.clSetKernelArg(kernel, 3, sizeof(float), &alpha);
    //CL.clSetKernelArg(kernel, 4, sizeof(float), &beta);
    CL.clSetKernelArg(kernel, 3, sizeof(unsigned), &size);

    std::size_t global_size[] = {size, size};
    std::size_t local_size[] = {16, 16};

    auto time_start = std::chrono::high_resolution_clock::now();

    auto constexpr kNumPasses = 100U;
    for (auto i = 0U; i < kNumPasses; ++i)
    {
        status = CL.clEnqueueNDRangeKernel(cmdqueue, kernel, 2, nullptr, global_size, local_size,
                                           0, nullptr, nullptr);
        CHECK_ERROR_TEST(status);
    }

    CL.clFinish(cmdqueue);

    CHECK_ERROR_TEST(status);

    auto time_end = std::chrono::high_resolution_clock::now();

    auto delta = (float)(std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start).count()) / kNumPasses;

    time = delta;

    // Check first 1M elements, too slow
    // size = std::min(size, 1024 * 1024);

    status = CL.clEnqueueReadBuffer(cmdqueue, dev_C, CL_TRUE, 0, size * size * sizeof(float), &C[0], 0, nullptr, nullptr);

    CHECK_ERROR_TEST(status);

    for (auto i = 0U; i < size * size; ++i)
    {
        if (std::fabs(C[i] - C_ref[i]) > 0.02f)
            return -1;
    }

    return CL_SUCCESS;
}




