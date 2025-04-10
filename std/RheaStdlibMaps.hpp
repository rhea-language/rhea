/*
 * Copyright (c) 2025 - Nathanne Isip
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

#ifndef RHEA_STDLIB_MAPS_HPP
#define RHEA_STDLIB_MAPS_HPP

#include <string>
#include <unordered_map>

#include <chisei/neural_network.hpp>
#include <myshell.hpp>
#include <zip.h>

#ifndef __TERMUX__

#include <GLFW/glfw3.h>

// GL
static std::unordered_map<std::string, GLFWwindow*> windowMap;
static std::unordered_map<std::string, GLFWmonitor*> monitorMap;
static std::unordered_map<std::string, GLFWcursor*> cursorMap;

#endif

// Archive
static std::unordered_map<std::string, zip_t*> zipMap;

// ML
static std::unordered_map<
    std::string,
    std::shared_ptr<chisei::NeuralNetwork>
> neuralNetworkMap;

// Sys
static std::unordered_map<std::string, std::shared_ptr<MyShell>> shellMap;

#endif
