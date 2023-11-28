cmake_minimum_required(VERSION 3.12)
project(log)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SOURCES
    AsyncLogging.cc
    Condition.h
    CountDownLatch.cc
    CurrentThread.h
    FileUtil.cc
    LogFile.cc
    Logging.cc
    LogStream.cc
    MutexLock.h
    noncopyable.h
    Thread.cc
)
set(HEADERS
    AsyncLogging.h
    CountDownLatch.h
    FileUtil.h
    LogFile.h
    Logging.h
    LogStream.h
    Thread.h
)
add_executable(test test.cc ${SOURCES} ${HEADERS})