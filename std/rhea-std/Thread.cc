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

#include "rhea-std/Thread.hpp"

#include <Rhea.hpp>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <rhea/ast/TerminativeSignal.hpp>
#include <rhea/ast/expression/FunctionDeclarationExpression.hpp>
#include <rhea/util/RandomUtil.hpp>
#include <thread>
#include <unordered_map>

static std::unordered_map<std::string, std::thread*> threadMap;
static std::unordered_map<std::string, std::mutex*> mutexMap;
static std::unordered_map<std::string, std::condition_variable*> conditionMap;
static std::mutex threadMapMutex;
static std::mutex mutexMapMutex;
static std::mutex conditionMapMutex;

RHEA_FUNC(thread_create) {
    if(args.size() != 0)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 0 arguments, got " + std::to_string(args.size()));

    std::string key = RheaUtil::uniqueKey();
    std::thread* newThread = new std::thread([]() {});

    {
        std::lock_guard<std::mutex> lock(threadMapMutex);
        threadMap[key] = newThread;
    }

    return DynamicObject(key);
}

RHEA_FUNC(thread_join) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    std::thread* threadPtr = nullptr;
    {
        std::lock_guard<std::mutex> lock(threadMapMutex);
        if(threadMap.count(keyStr) == 0)
            throw TerminativeThrowSignal(std::move(address),
                                         "Thread key does not exist");
        threadPtr = threadMap[keyStr];
    }

    if(threadPtr && threadPtr->joinable()) threadPtr->join();

    return {};
}

RHEA_FUNC(thread_detach) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    std::thread* threadPtr = nullptr;
    {
        std::lock_guard<std::mutex> lock(threadMapMutex);
        if(threadMap.count(keyStr) == 0)
            throw TerminativeThrowSignal(std::move(address),
                                         "Thread key does not exist");
        threadPtr = threadMap[keyStr];
    }

    if(threadPtr && threadPtr->joinable()) threadPtr->detach();

    return {};
}

RHEA_FUNC(thread_sleep) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject milliseconds = args.at(0);
    if(!milliseconds.isNumber())
        throw TerminativeThrowSignal(std::move(address),
                                     "Milliseconds must be a number");

    std::this_thread::sleep_for(
        std::chrono::milliseconds(static_cast<int>(milliseconds.getNumber())));

    return {};
}

RHEA_FUNC(thread_yield) {
    std::this_thread::yield();
    return {};
}

RHEA_FUNC(thread_id) {
    std::hash<std::thread::id> hasher;
    size_t threadId = hasher(std::this_thread::get_id());
    return DynamicObject(static_cast<double>(threadId));
}

RHEA_FUNC(thread_exit) {
    std::thread* currentThread = nullptr;
    std::string currentKey;

    {
        std::lock_guard<std::mutex> lock(threadMapMutex);
        for(auto& pair : threadMap) {
            if(pair.second->get_id() == std::this_thread::get_id()) {
                currentThread = pair.second;
                currentKey = pair.first;
                break;
            }
        }
    }

    if(currentThread) {
        currentThread->detach();
        delete currentThread;

        std::lock_guard<std::mutex> lock(threadMapMutex);
        threadMap.erase(currentKey);
    }

    return {};
}

RHEA_FUNC(thread_mutex_create) {
    std::string key = RheaUtil::uniqueKey();
    std::mutex* newMutex = new std::mutex();

    {
        std::lock_guard<std::mutex> lock(mutexMapMutex);
        mutexMap[key] = newMutex;
    }

    return DynamicObject(key);
}

RHEA_FUNC(thread_mutex_lockMut) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    std::mutex* mutexPtr = nullptr;
    {
        std::lock_guard<std::mutex> lock(mutexMapMutex);
        if(mutexMap.count(keyStr) == 0)
            throw TerminativeThrowSignal(std::move(address),
                                         "Mutex key does not exist");
        mutexPtr = mutexMap[keyStr];
    }

    if(mutexPtr) mutexPtr->lock();

    return {};
}

RHEA_FUNC(thread_mutex_tryLock) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    std::mutex* mutexPtr = nullptr;
    {
        std::lock_guard<std::mutex> lock(mutexMapMutex);
        if(mutexMap.count(keyStr) == 0)
            throw TerminativeThrowSignal(std::move(address),
                                         "Mutex key does not exist");
        mutexPtr = mutexMap[keyStr];
    }

    if(mutexPtr) return DynamicObject(mutexPtr->try_lock());

    return DynamicObject(false);
}

RHEA_FUNC(thread_mutex_unlock) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    std::mutex* mutexPtr = nullptr;
    {
        std::lock_guard<std::mutex> lock(mutexMapMutex);
        if(mutexMap.count(keyStr) == 0)
            throw TerminativeThrowSignal(std::move(address),
                                         "Mutex key does not exist");
        mutexPtr = mutexMap[keyStr];
    }

    if(mutexPtr) mutexPtr->unlock();

    return {};
}

RHEA_FUNC(thread_mutex_destroy) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    std::mutex* mutexPtr = nullptr;
    {
        std::lock_guard<std::mutex> lock(mutexMapMutex);
        if(mutexMap.count(keyStr) == 0)
            throw TerminativeThrowSignal(std::move(address),
                                         "Mutex key does not exist");
        mutexPtr = mutexMap[keyStr];
        mutexMap.erase(keyStr);
    }

    if(mutexPtr) delete mutexPtr;

    return {};
}

RHEA_FUNC(thread_condition_create) {
    std::string key = RheaUtil::uniqueKey();
    std::condition_variable* newCondition = new std::condition_variable();

    {
        std::lock_guard<std::mutex> lock(conditionMapMutex);
        conditionMap[key] = newCondition;
    }

    return DynamicObject(key);
}

RHEA_FUNC(thread_condition_hold) {
    if(args.size() != 2)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 2 arguments, got " + std::to_string(args.size()));

    DynamicObject condKey = args.at(0), mutexKey = args.at(1);
    std::string condStr = condKey.toString(), mutexStr = mutexKey.toString();

    std::condition_variable* condPtr = nullptr;
    std::mutex* mutexPtr = nullptr;

    {
        std::lock_guard<std::mutex> lockCond(conditionMapMutex);
        std::lock_guard<std::mutex> lockMutex(mutexMapMutex);

        if(conditionMap.count(condStr) == 0)
            throw TerminativeThrowSignal(std::move(address),
                                         "Condition key does not exist");
        if(mutexMap.count(mutexStr) == 0)
            throw TerminativeThrowSignal(std::move(address),
                                         "Mutex key does not exist");

        condPtr = conditionMap[condStr];
        mutexPtr = mutexMap[mutexStr];
    }

    if(condPtr && mutexPtr) {
        std::unique_lock<std::mutex> lock(*mutexPtr, std::adopt_lock);
        condPtr->wait(lock);
        lock.release();
    }

    return {};
}

RHEA_FUNC(thread_condition_signal) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    std::condition_variable* condPtr = nullptr;
    {
        std::lock_guard<std::mutex> lock(conditionMapMutex);
        if(conditionMap.count(keyStr) == 0)
            throw TerminativeThrowSignal(std::move(address),
                                         "Condition key does not exist");
        condPtr = conditionMap[keyStr];
    }

    if(condPtr) condPtr->notify_one();

    return {};
}

RHEA_FUNC(thread_condition_broadcast) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    std::condition_variable* condPtr = nullptr;
    {
        std::lock_guard<std::mutex> lock(conditionMapMutex);
        if(conditionMap.count(keyStr) == 0)
            throw TerminativeThrowSignal(std::move(address),
                                         "Condition key does not exist");
        condPtr = conditionMap[keyStr];
    }

    if(condPtr) condPtr->notify_all();

    return {};
}

RHEA_FUNC(thread_condition_destroy) {
    if(args.size() != 1)
        throw TerminativeThrowSignal(
            std::move(address),
            "Expecting 1 argument, got " + std::to_string(args.size()));

    DynamicObject key = args.at(0);
    std::string keyStr = key.toString();

    std::condition_variable* condPtr = nullptr;
    {
        std::lock_guard<std::mutex> lock(conditionMapMutex);
        if(conditionMap.count(keyStr) == 0)
            throw TerminativeThrowSignal(std::move(address),
                                         "Condition key does not exist");
        condPtr = conditionMap[keyStr];
        conditionMap.erase(keyStr);
    }

    if(condPtr) delete condPtr;

    return {};
}
