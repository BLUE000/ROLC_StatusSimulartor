cmake_minimum_required(VERSION 3.20)
project(ROLC_StatusSimulator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network Test)

# -----------------------------------------------------------------------------
# TrustChain Security & Provenance Subproject
# -----------------------------------------------------------------------------
add_subdirectory(Lib/TrustChain)

# -----------------------------------------------------------------------------
# ROLC_Core Target (Pure C++20 Domain Engine & Master Data)
# -----------------------------------------------------------------------------
add_library(ROLC_Core STATIC
    src/Core/src/MasterData.cpp
    src/Core/src/CalculatorEngine.cpp
)

target_include_directories(ROLC_Core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/include
)

# -----------------------------------------------------------------------------
# App Target (Qt6 GUI Application & Presenter)
# -----------------------------------------------------------------------------
add_executable(ROLC_StatusSimulator WIN32
    src/App/main.cpp
    src/App/Presenter/SimulatorPresenter.cpp
    src/App/View/MainWindow.cpp
    src/App/View/CharacterWidget.cpp
    src/App/View/StatusWidget.cpp
    src/App/View/EquipmentWidget.cpp
    src/App/View/OutputWidget.cpp
)

target_include_directories(ROLC_StatusSimulator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/App/Presenter
    ${CMAKE_CURRENT_SOURCE_DIR}/src/App/View
    ${CMAKE_CURRENT_SOURCE_DIR}/Lib/TrustChain/include
)

target_link_libraries(ROLC_StatusSimulator PRIVATE
    ROLC_Core
    Qt6::Widgets
    Qt6::Network
    trustchain
)

# Setup TrustChain provenance verification & macro injection for target
trustchain_setup_target(ROLC_StatusSimulator)

# -----------------------------------------------------------------------------
# Automated Test Targets (QTest / GTest Integration)
# -----------------------------------------------------------------------------
enable_testing()

# 1. Core Domain Test
add_executable(ROLC_CoreTest
    test/CoreTest.cpp
)

target_include_directories(ROLC_CoreTest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/include
)

target_link_libraries(ROLC_CoreTest PRIVATE
    ROLC_Core
    Qt6::Test
)

add_test(NAME CoreTest COMMAND ROLC_CoreTest)

# 2. Presenter Integration Test
add_executable(ROLC_PresenterTest
    test/PresenterTest.cpp
    src/App/Presenter/SimulatorPresenter.cpp
)

target_include_directories(ROLC_PresenterTest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/App/Presenter
    ${CMAKE_CURRENT_SOURCE_DIR}/src/App/View
)

target_link_libraries(ROLC_PresenterTest PRIVATE
    ROLC_Core
    Qt6::Widgets
    Qt6::Test
)

add_test(NAME PresenterTest COMMAND ROLC_PresenterTest)
