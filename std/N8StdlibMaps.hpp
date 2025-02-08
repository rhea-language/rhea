/*
 * Copyright (c) 2025 - Nathanne Isip
 * This file is part of N8.
 * 
 * N8 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 * 
 * N8 is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with N8. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef N8_STDLIB_MAPS_HPP
#define N8_STDLIB_MAPS_HPP

#include <string>
#include <unordered_map>

#include <chisei/neural_network.hpp>
#include <GLFW/glfw3.h>
#include <myshell.hpp>

// GL
static std::unordered_map<std::string, GLFWwindow*> windowMap;
static std::unordered_map<std::string, GLFWmonitor*> monitorMap;
static std::unordered_map<std::string, GLFWcursor*> cursorMap;

// ML
static std::unordered_map<std::string, chisei::NeuralNetwork> neuralNetworkMap;

// Sys
static std::unordered_map<std::string, std::shared_ptr<MyShell>> shellMap;

#endif
