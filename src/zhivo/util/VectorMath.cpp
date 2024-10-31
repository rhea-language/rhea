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

    #pragma omp parallel for
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
    for(long i = 0; i < (long) objSize; ++i)
        values[(size_t) i] = objects[(size_t) i].getNumber();

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

    #pragma omp parallel for
    for(size_t i = 0; i < size; ++i)
        result[i] = left[i] + right[i];
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

    #pragma omp parallel for
    for(size_t i = 0; i < size; ++i)
        result[i] = left[i] - right[i];
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

    #pragma omp parallel for
    for(size_t i = 0; i < size; ++i)
        result[i] = left[i] / right[i];
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

    #pragma omp parallel for
    for(size_t i = 0; i < size; ++i)
        result[i] = left[i] * right[i];
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

    #pragma omp parallel for
    for(size_t i = 0; i < size; ++i)
        result[i] = (double) ((long) left[i] % (long) right[i]);
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

    #pragma omp parallel for
    for(size_t i = 0; i < size; ++i)
        result[i] = (double) ((long) left[i] & (long) right[i]);
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

    #pragma omp parallel for
    for(size_t i = 0; i < size; ++i)
        result[i] = (double) ((long) left[i] | (long) right[i]);
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

    #pragma omp parallel for
    for(size_t i = 0; i < size; ++i)
        result[i] = (double) ((long) left[i] ^ (long) right[i]);
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

    #pragma omp parallel for
    for(size_t i = 0; i < size; ++i)
        result[i] = (double) ((long) left[i] << (long) right[i]);
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

    #pragma omp parallel for
    for(size_t i = 0; i < size; ++i)
        result[i] = (double) ((long) left[i] >> (long) right[i]);
    return result;
}

}
