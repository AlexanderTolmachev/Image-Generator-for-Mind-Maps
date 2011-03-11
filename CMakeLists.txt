cmake_minimum_required (VERSION 2.6)

set (project ImgGenMM)

set (HEADERS
	mind_map.h
	load_map.h
	map_settings.h
	draw_map.h
	lib/pugixml.hpp
	lib/pugiconfig.hpp)

set (SOURCES
	mind_map.cpp
	load_map.cpp
	map_settings.cpp
	draw_map.cpp
	lib/pugixml.cpp	
	main.cpp)

project ($(PROJECT))

find_package (Qt4 REQUIRED)
include (${QT_USE_FILE})
qt4_wrap_cpp (MOC_SOURCES ${HEADERS})

if (MSVC)
	add_definitions (/W3)
elseif (CMAKE_COMPILER_IS_GNUCXX)
	add_definitions (-Wall -pedantic)
else ()
	message ("Unknown compiler")
endif ()


add_executable (${PROJECT} ${HEADERS} ${SOURCES} ${MOC_SOURCES})
 
target_link_libraries (${PROJECT} ${QT_LIBRARIES})
