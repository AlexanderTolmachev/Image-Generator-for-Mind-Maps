cmake_minimum_required (VERSION 2.6)

set (PROJECT 
	ImgGenMM)

set (HEADERS
	mind_map.h
	map_loader.h
	map_settings.h
	map_drawer.h
	lib/pugixml.hpp
	lib/pugiconfig.hpp)

set (SOURCES
	mind_map.cpp
	map_loader.cpp
	map_settings.cpp
	map_drawer.cpp
	lib/pugixml.cpp	
	main.cpp)

project (${PROJECT})

include_directories (D:/Программы/Qt/4.7.2/lib/)

set (LIBRARIES
	QtGui4.lib
	QtCore4.lib)

set (QT_HEADERS
	D:/Программы/Qt/4.7.2/include/QtGui/qimage.h
	D:/Программы/Qt/4.7.2/include/QtGui/qbrush.h
	D:/Программы/Qt/4.7.2/include/QtGui/qpainter.h
	D:/Программы/Qt/4.7.2/include/QtGui/qapplication.h)

source_group ("Header Files" FILES ${HEADERS})
source_group ("Source Files" FILES ${SOURCES})

if (MSVC)
	add_definitions (/W3)
elseif (CMAKE_COMPILER_IS_GNUCXX)
	add_definitions (-Wall -pedantic)
else ()
	message ("Unknown compiler")
endif ()


add_executable (${PROJECT} ${HEADERS} ${SOURCES} ${QT_HEADERS})
 
target_link_libraries (${PROJECT} ${LIBRARIES})
