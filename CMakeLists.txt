cmake_minimum_required(VERSION 3.16)

project(Gobang VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(STATIC_BUILD)
	if(NOT QT_STATIC_PATH)
		message(FATAL_ERROR "Please specify the path to Qt static build.")
	endif()
	message(STATUS "Building with static Qt at ${QT_STATIC_PATH}")
	set(CMAKE_PREFIX_PATH ${QT_STATIC_PATH}/lib/cmake)
endif()
find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools)

# Base sources
set(PCH_FILE pch.h)
set(BASE_SRC ai.cpp ai.h gobang.h ${PCH_FILE})

# CUI
set(CUI_SRC cui.cpp)
add_executable(gobang_console ${CUI_SRC} ${BASE_SRC})
target_precompile_headers(gobang_console PRIVATE ${PCH_FILE})

# GUI
set(TS_FILE gobang_zh_CN.ts)
set(PCH_GUI_FILE pch_gui.h)
set(GUI_SRC gui.cpp window.cpp window.h window.ui ${PCH_GUI_FILE})
set(QT_LIBS Qt6::Widgets)
set(QT_LINK_LIBS b2 zstd)
qt_add_executable(gobang MANUAL_FINALIZATION ${GUI_SRC} ${BASE_SRC})
qt_add_translations(gobang TS_FILES ${TS_FILE} RESOURCE_PREFIX "/translations")
target_precompile_headers(gobang PRIVATE ${PCH_GUI_FILE})
if(STATIC_BUILD)
	target_link_directories(gobang PRIVATE ${QT_STATIC_PATH}/lib)
	target_link_libraries(gobang PRIVATE -static ${QT_LIBS} ${QT_LINK_LIBS})
else()
	target_link_libraries(gobang PRIVATE ${QT_LIBS} ${QT_LINK_LIBS})
endif()
if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
	set_target_properties(gobang PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
qt_finalize_executable(gobang)

# Static-link
if(STATIC_BUILD)
	if(MSVC)
		set_property(TARGET gobang gobang_console PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s -static -static-libgcc -static-libstdc++")
	endif()
endif()
