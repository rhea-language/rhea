/*
 * Copyright (c) 2024 - Nathanne Isip
 * This file is part of Rhea.
 * 
 * Rhea is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 * 
 * Rhea is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Rhea. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef RHEA_STDLIB_ML_CC
#define RHEA_STDLIB_ML_CC

#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
#endif

#include "RheaLibrary.hpp"

RHEA_LIB_START

RHEA_FUNC(ml_trendline_calculate);
RHEA_FUNC(ml_trendline_calculateRmse);
RHEA_FUNC(ml_trendline_predict);

RHEA_FUNC(ml_ann_create);
RHEA_FUNC(ml_ann_fromMnist);
RHEA_FUNC(ml_ann_fromModelFile);
RHEA_FUNC(ml_ann_train);
RHEA_FUNC(ml_ann_predict);
RHEA_FUNC(ml_ann_calculateMseLoss);
RHEA_FUNC(ml_ann_computeOutputGradient);
RHEA_FUNC(ml_ann_computeAccuracy);
RHEA_FUNC(ml_ann_isCorrectPrediction);
RHEA_FUNC(ml_ann_saveModel);

RHEA_LIB_END

#ifdef __clang__
#   pragma clang diagnostic pop
#endif

#endif
