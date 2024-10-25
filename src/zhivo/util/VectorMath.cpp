/*
 * Copyright (c) 2024 - Nathanne Isip
 * This file is part of Zhivo.
 * 
 * Zhivo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 * 
 * Zhivo is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Zhivo. If not, see <https://www.gnu.org/licenses/>.
 */

#include <zhivo/util/VectorMath.hpp>

namespace ZhivoUtil {

DynamicObject vector2Object(const std::vector<double>& vec) {
    std::vector<DynamicObject> objects(vec.size());
    for(size_t i = 0; i < vec.size(); ++i)
        objects[i] = std::move(DynamicObject(vec[i]));

    return DynamicObject(
        std::make_shared<std::vector<DynamicObject>>(std::move(objects))
    );
}

std::vector<double> object2Vector(const DynamicObject object) {
    std::vector<DynamicObject> objects = *object.getArray();
    size_t objSize = objects.size();
    std::vector<double> values(objSize);

    #pragma omp parallel for
    for (size_t i = 0; i < objSize; ++i)
        values[i] = objects[i].getNumber();

    return values;
}

std::vector<double> ZhivoUtil::VectorMath::add(
        std::vector<double> left,
        std::vector<double> right
) {
    size_t size = left.size();
    if(size != right.size())
        throw std::invalid_argument("Vectors must be of the same size.");

    std::vector<double> result(size);

    #ifdef __CUDA__

    double *d_left, *d_right, *d_result;
    cudaMalloc(&d_left, size * sizeof(double));
    cudaMalloc(&d_right, size * sizeof(double));
    cudaMalloc(&d_result, size * sizeof(double));

    cudaMemcpy(d_left, left.data(), size * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_right, right.data(), size * sizeof(double), cudaMemcpyHostToDevice);

    cudaVectorAdd<<<(size + 255) / 256, 256>>>(d_left, d_right, d_result, size);
    cudaMemcpy(result.data(), d_result, size * sizeof(double), cudaMemcpyDeviceToHost);

    cudaFree(d_left);
    cudaFree(d_right);
    cudaFree(d_result);

    #else

    #pragma omp parallel for
    for(size_t i = 0; i < size; ++i)
        result[i] = left[i] + right[i];

    #endif

    return result;
}

std::vector<double> ZhivoUtil::VectorMath::sub(
    std::vector<double> left,
    std::vector<double> right
) {
    size_t size = left.size();
    if(size != right.size())
        throw std::invalid_argument("Vectors must be of the same size.");

    std::vector<double> result(size);

    #ifdef __CUDA__

    double *d_left, *d_right, *d_result;
    cudaMalloc(&d_left, size * sizeof(double));
    cudaMalloc(&d_right, size * sizeof(double));
    cudaMalloc(&d_result, size * sizeof(double));

    cudaMemcpy(d_left, left.data(), size * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_right, right.data(), size * sizeof(double), cudaMemcpyHostToDevice);

    cudaVectorSub<<<(size + 255) / 256, 256>>>(d_left, d_right, d_result, size);
    cudaMemcpy(result.data(), d_result, size * sizeof(double), cudaMemcpyDeviceToHost);

    cudaFree(d_left);
    cudaFree(d_right);
    cudaFree(d_result);

    #else

    #pragma omp parallel for
    for(size_t i = 0; i < size; ++i)
        result[i] = left[i] - right[i];

    #endif

    return result;
}

std::vector<double> ZhivoUtil::VectorMath::div(
    std::vector<double> left,
    std::vector<double> right
) {
    size_t size = left.size();
    if(size != right.size())
        throw std::invalid_argument("Vectors must be of the same size.");

    std::vector<double> result(size);

    #ifdef __CUDA__

    double *d_left, *d_right, *d_result;
    cudaMalloc(&d_left, size * sizeof(double));
    cudaMalloc(&d_right, size * sizeof(double));
    cudaMalloc(&d_result, size * sizeof(double));

    cudaMemcpy(d_left, left.data(), size * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_right, right.data(), size * sizeof(double), cudaMemcpyHostToDevice);

    cudaVectorDiv<<<(size + 255) / 256, 256>>>(d_left, d_right, d_result, size);
    cudaMemcpy(result.data(), d_result, size * sizeof(double), cudaMemcpyDeviceToHost);

    cudaFree(d_left);
    cudaFree(d_right);
    cudaFree(d_result);

    #else

    #pragma omp parallel for
    for(size_t i = 0; i < size; ++i)
        result[i] = left[i] / right[i];

    #endif

    return result;
}

std::vector<double> ZhivoUtil::VectorMath::mul(
    std::vector<double> left,
    std::vector<double> right
) {
    size_t size = left.size();
    if(size != right.size())
        throw std::invalid_argument("Vectors must be of the same size.");

    std::vector<double> result(size);

    #ifdef __CUDA__

    double *d_left, *d_right, *d_result;
    cudaMalloc(&d_left, size * sizeof(double));
    cudaMalloc(&d_right, size * sizeof(double));
    cudaMalloc(&d_result, size * sizeof(double));

    cudaMemcpy(d_left, left.data(), size * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_right, right.data(), size * sizeof(double), cudaMemcpyHostToDevice);

    cudaVectorMul<<<(size + 255) / 256, 256>>>(d_left, d_right, d_result, size);
    cudaMemcpy(result.data(), d_result, size * sizeof(double), cudaMemcpyDeviceToHost);

    cudaFree(d_left);
    cudaFree(d_right);
    cudaFree(d_result);

    #else

    #pragma omp parallel for
    for(size_t i = 0; i < size; ++i)
        result[i] = left[i] * right[i];

    #endif

    return result;
}

std::vector<double> ZhivoUtil::VectorMath::rem(
    std::vector<double> left,
    std::vector<double> right
) {
    size_t size = left.size();
    if(size != right.size())
        throw std::invalid_argument("Vectors must be of the same size.");

    std::vector<double> result(size);

    #ifdef __CUDA__

    double *d_left, *d_right, *d_result;
    cudaMalloc(&d_left, size * sizeof(double));
    cudaMalloc(&d_right, size * sizeof(double));
    cudaMalloc(&d_result, size * sizeof(double));

    cudaMemcpy(d_left, left.data(), size * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_right, right.data(), size * sizeof(double), cudaMemcpyHostToDevice);

    cudaVectorRem<<<(size + 255) / 256, 256>>>(d_left, d_right, d_result, size);
    cudaMemcpy(result.data(), d_result, size * sizeof(double), cudaMemcpyDeviceToHost);

    cudaFree(d_left);
    cudaFree(d_right);
    cudaFree(d_result);

    #else

    #pragma omp parallel for
    for(size_t i = 0; i < size; ++i)
        result[i] = (double) ((long) left[i] % (long) right[i]);

    #endif

    return result;
}

std::vector<double> ZhivoUtil::VectorMath::bitwiseAnd(
    std::vector<double> left,
    std::vector<double> right
) {
    size_t size = left.size();
    if(size != right.size())
        throw std::invalid_argument("Vectors must be of the same size.");

    std::vector<double> result(size);

    #ifdef __CUDA__

    double *d_left, *d_right, *d_result;
    cudaMalloc(&d_left, size * sizeof(double));
    cudaMalloc(&d_right, size * sizeof(double));
    cudaMalloc(&d_result, size * sizeof(double));

    cudaMemcpy(d_left, left.data(), size * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_right, right.data(), size * sizeof(double), cudaMemcpyHostToDevice);

    cudaVectorBitwiseAnd<<<(size + 255) / 256, 256>>>(d_left, d_right, d_result, size);
    cudaMemcpy(result.data(), d_result, size * sizeof(double), cudaMemcpyDeviceToHost);

    cudaFree(d_left);
    cudaFree(d_right);
    cudaFree(d_result);

    #else

    #pragma omp parallel for
    for(size_t i = 0; i < size; ++i)
        result[i] = (double) ((long) left[i] & (long) right[i]);

    #endif

    return result;
}

std::vector<double> ZhivoUtil::VectorMath::bitwiseOr(
    std::vector<double> left,
    std::vector<double> right
) {
    size_t size = left.size();
    if(size != right.size())
        throw std::invalid_argument("Vectors must be of the same size.");

    std::vector<double> result(size);

    #ifdef __CUDA__

    double *d_left, *d_right, *d_result;
    cudaMalloc(&d_left, size * sizeof(double));
    cudaMalloc(&d_right, size * sizeof(double));
    cudaMalloc(&d_result, size * sizeof(double));

    cudaMemcpy(d_left, left.data(), size * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_right, right.data(), size * sizeof(double), cudaMemcpyHostToDevice);

    cudaVectorBitwiseOr<<<(size + 255) / 256, 256>>>(d_left, d_right, d_result, size);
    cudaMemcpy(result.data(), d_result, size * sizeof(double), cudaMemcpyDeviceToHost);

    cudaFree(d_left);
    cudaFree(d_right);
    cudaFree(d_result);

    #else

    #pragma omp parallel for
    for(size_t i = 0; i < size; ++i)
        result[i] = (double) ((long) left[i] | (long) right[i]);

    #endif

    return result;
}

std::vector<double> ZhivoUtil::VectorMath::bitwiseXor(
    std::vector<double> left,
    std::vector<double> right
) {
    size_t size = left.size();
    if(size != right.size())
        throw std::invalid_argument("Vectors must be of the same size.");

    std::vector<double> result(size);

    #ifdef __CUDA__

    double *d_left, *d_right, *d_result;
    cudaMalloc(&d_left, size * sizeof(double));
    cudaMalloc(&d_right, size * sizeof(double));
    cudaMalloc(&d_result, size * sizeof(double));

    cudaMemcpy(d_left, left.data(), size * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_right, right.data(), size * sizeof(double), cudaMemcpyHostToDevice);

    cudaVectorBitwiseXor<<<(size + 255) / 256, 256>>>(d_left, d_right, d_result, size);
    cudaMemcpy(result.data(), d_result, size * sizeof(double), cudaMemcpyDeviceToHost);

    cudaFree(d_left);
    cudaFree(d_right);
    cudaFree(d_result);

    #else

    #pragma omp parallel for
    for(size_t i = 0; i < size; ++i)
        result[i] = (double) ((long) left[i] ^ (long) right[i]);

    #endif

    return result;
}

std::vector<double> ZhivoUtil::VectorMath::shiftLeft(
    std::vector<double> left,
    std::vector<double> right
) {
    size_t size = left.size();
    if(size != right.size())
        throw std::invalid_argument("Vectors must be of the same size.");

    std::vector<double> result(size);

    #ifdef __CUDA__

    double *d_left, *d_right, *d_result;
    cudaMalloc(&d_left, size * sizeof(double));
    cudaMalloc(&d_right, size * sizeof(double));
    cudaMalloc(&d_result, size * sizeof(double));

    cudaMemcpy(d_left, left.data(), size * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_right, right.data(), size * sizeof(double), cudaMemcpyHostToDevice);

    cudaVectorShiftLeft<<<(size + 255) / 256, 256>>>(d_left, d_right, d_result, size);
    cudaMemcpy(result.data(), d_result, size * sizeof(double), cudaMemcpyDeviceToHost);

    cudaFree(d_left);
    cudaFree(d_right);
    cudaFree(d_result);

    #else

    #pragma omp parallel for
    for(size_t i = 0; i < size; ++i)
        result[i] = (double) ((long) left[i] << (long) right[i]);

    #endif

    return result;
}

std::vector<double> ZhivoUtil::VectorMath::shiftRight(
    std::vector<double> left,
    std::vector<double> right
) {
    size_t size = left.size();
    if(size != right.size())
        throw std::invalid_argument("Vectors must be of the same size.");

    std::vector<double> result(size);

    #ifdef __CUDA__

    double *d_left, *d_right, *d_result;
    cudaMalloc(&d_left, size * sizeof(double));
    cudaMalloc(&d_right, size * sizeof(double));
    cudaMalloc(&d_result, size * sizeof(double));

    cudaMemcpy(d_left, left.data(), size * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(d_right, right.data(), size * sizeof(double), cudaMemcpyHostToDevice);

    cudaVectorShift<<<(size + 255) / 256, 256>>>(d_left, d_right, d_result, size);
    cudaMemcpy(result.data(), d_result, size * sizeof(double), cudaMemcpyDeviceToHost);

    cudaFree(d_left);
    cudaFree(d_right);
    cudaFree(d_result);

    #else

    #pragma omp parallel for
    for(size_t i = 0; i < size; ++i)
        result[i] = (double) ((long) left[i] >> (long) right[i]);

    #endif

    return result;
}

#ifdef __CUDA__
static __global__ void cudaVectorAdd(
    const double* left,
    const double* right,
    double* result,
    size_t size
) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if(idx < size)
        result[idx] = left[idx] + right[idx];
}

static __global__ void cudaVectorSub(
    const double* left,
    const double* right,
    double* result,
    size_t size
) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if(idx < size)
        result[idx] = left[idx] - right[idx];
}

static __global__ void cudaVectorDiv(
    const double* left,
    const double* right,
    double* result,
    size_t size
) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if(idx < size)
        result[idx] = left[idx] / right[idx];
}

static __global__ void cudaVectorMul(
    const double* left,
    const double* right,
    double* result,
    size_t size
) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if(idx < size)
        result[idx] = left[idx] * right[idx];
}

static __global__ void cudaVectorRem(
    const double* left,
    const double* right,
    double* result,
    size_t size
) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if(idx < size)
        result[idx] = (double) ((long) left[idx] * (long) right[idx]);
}

static __global__ void cudaVectorBitwiseAnd(
    const double* left,
    const double* right,
    double* result,
    size_t size
) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if(idx < size)
        result[idx] = (double) ((long) left[idx] & (long) right[idx]);
}

static __global__ void cudaVectorBitwiseOr(
    const double* left,
    const double* right,
    double* result,
    size_t size
) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if(idx < size)
        result[idx] = (double) ((long) left[idx] | (long) right[idx]);
}

static __global__ void cudaVectorBitwiseXor(
    const double* left,
    const double* right,
    double* result,
    size_t size
) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if(idx < size)
        result[idx] = (double) ((long) left[idx] ^ (long) right[idx]);
}

static __global__ void cudaVectorShiftLeft(
    const double* left,
    const double* right,
    double* result,
    size_t size
) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if(idx < size)
        result[idx] = (double) ((long) left[idx] << (long) right[idx]);
}

static __global__ void cudaVectorShiftRight(
    const double* left,
    const double* right,
    double* result,
    size_t size
) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if(idx < size)
        result[idx] = (double) ((long) left[idx] >> (long) right[idx]);
}

#endif

}
