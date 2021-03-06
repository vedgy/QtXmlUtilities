cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Depends on TemplateUtilities, Qt, QtCoreUtilities.
# NOTE: projects that use this library must
# include_directories(${TemplateUtilities_PublicHeaders_Path})
set(Target_Name QtXmlUtilities)
set(CAP_Target_Name QT_XML_UTILITIES)
message(<${Target_Name}>)

## vedgTools/CMakeModules path boilerplate.
include(vedgTools/CMakeLicense OPTIONAL RESULT_VARIABLE CMakeModulesWasIncluded)
if(CMakeModulesWasIncluded STREQUAL NOTFOUND)
    set(PATH_TO_CMAKE_MODULES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PATH_TO_CMAKE_MODULES})
    message("Including CMakeModules.")
    include(vedgTools/CMakeLicense)
endif()

# NOTE: QtCore and QtXml modules are used by this library.
include(vedgTools/LibraryQtStart)

include(vedgTools/LibraryTemplateUtilitiesStart)
include(vedgTools/LibraryQtCoreUtilitiesStart)

include(vedgTools/LibraryWithQtInit)


include(vedgTools/LibraryFindQt)
libraryFindQt(Qt5Core Qt5Xml . QTCORE QTXML)

include(vedgTools/LibraryGetTemplateUtilitiesPublicHeaders)
include(vedgTools/LibraryGetQtCoreUtilitiesPublicHeaders)


set(Sources_Path src)

include_directories(
    ${PATH_TO_CMAKE_MODULES}/include
    ${TemplateUtilities_PublicHeaders_Path}
    ${QtCoreUtilities_PublicHeaders_Path}
    ${${Target_Name}_PublicHeaders_Path}
    ${${Target_Name}_PublicHeaders_Path}/${Target_Name}
)

set(Sources
    ${Sources_Path}/ReadingShortcuts.cpp ${Sources_Path}/WritingShortcuts.cpp
)


include(vedgTools/LibraryAddTarget)

include(vedgTools/LinkQt)
linkQt(${Target_Name} Core Xml . ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY})

include(vedgTools/LibraryLinkQtCoreUtilitiesToTarget)


set_target_properties(${Target_Name} PROPERTIES
                PUBLIC_HEADER "ReadingShortcuts.hpp;WritingShortcuts.hpp")

message(</${Target_Name}>)
