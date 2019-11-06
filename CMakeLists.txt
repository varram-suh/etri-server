cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)
project(testproject)
if(POLICY CMP0071)
  cmake_policy(SET CMP0071 NEW)
endif()
find_package(Protobuf REQUIRED)
find_package(TensorflowCC COMPONENTS Shared)
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

# find_package(Boost 1.67.0 REQUIRED COMPONENTS
#              date_time filesystem system)
# target_link_libraries(foo Boost::date_time Boost::filesystem
#                           Boost::iostreams)
message(${Protobuf_LIBRARIES})

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package( OpenCV REQUIRED )
find_package(Qt5PrintSupport REQUIRED )

MESSAGE(${OpenCV_LIBS})
# Populate a CMake variable with the sources
set(
    helloworld_SRCS
    mainwindow.ui
    mdichild.ui
    mainwindow.cpp
    mdi-child.cpp
    #tensor_test.cpp
    main.cpp
    circular-buffer.cpp
    license-session.cpp
    protobuf-c/protobuf-c.c
    qcustomplot.cpp
    simple.pb-c.c
)

# Tell CMake to create the helloworld executable
add_executable(helloworld WIN32 ${helloworld_SRCS})

# Use the Widgets module from Qt 5
#qt5_use_modules(helloworld Core SerialPort)
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${Protobuf_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}/src
    ${OpenCV_INCLUDE_DIR}
    )

target_link_libraries(helloworld
Qt5::Widgets
Qt5::Multimedia
boost_filesystem
boost_date_time
pthread
${Protobuf_LIBRARIES}
${OpenCV_LIBS}
Qt5::PrintSupport
TensorflowCC::Shared
)
