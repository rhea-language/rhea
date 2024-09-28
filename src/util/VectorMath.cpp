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

#include <util/VectorMath.hpp>

#include <mutex>

namespace ZhivoUtil {

DynamicObject vector2Object(const std::vector<double>& vec) {
    std::vector<DynamicObject> objects(vec.size());
    std::mutex mutex;

    #pragma omp parallel for
    for(size_t i = 0; i < vec.size(); ++i) {
        std::lock_guard<std::mutex> lock(mutex);
        DynamicObject obj(vec[i]);

        objects[i] = std::move(obj);
    }

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

    #ifdef __CUDACC__

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

#ifdef __CUDACC__
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

#endif

}
