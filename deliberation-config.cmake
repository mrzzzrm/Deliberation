# DELIBERATION_DIR 
# DELIBERATION_FOUND 
# DELIBERATION_LIBRARIES 
# DELIBERATION_INCLUDE_DIRS 

set(DELIBERATION_INCLUDE_DIRS "")
set(DELIBERATION_LIBRARIES "")

# Definition of function "find" with two mandatory arguments, "LIB_NAME" and "HEADER".
macro (find LIB_NAME HEADER)

    set(HINT_PATHS ${ARGN})

    if (${LIB_NAME} STREQUAL "Deliberation")
        set(LIB_NAME_UPPER DELIBERATION)
        set(LIBNAME Deliberation)
    else()
        string(TOUPPER DELIBERATION_${LIB_NAME} LIB_NAME_UPPER)
        set(LIBNAME ${LIB_NAME})
    endif()

    find_path(
	${LIB_NAME_UPPER}_INCLUDE_DIR
	${HEADER}
        ${ENV_DELIBERATION_DIR}/include
        ${ENV_DELIBERATION_DIR}/Source/${LIB_NAME}/include
        ${DELIBERATION_DIR}/include
        ${DELIBERATION_DIR}/Source/${LIB_NAME}/include
        ${ENV_PROGRAMFILES}/Deliberation/include
        /usr/include
        /usr/local/include
        /sw/include
        /opt/local/include
        DOC "The directory where ${HEADER} resides"
    )


    find_library(
	${LIB_NAME_UPPER}_LIBRARY_RELEASE
        NAMES ${LIBNAME}
        PATHS ${HINT_PATHS}
        DOC "The ${LIB_NAME} library"
    )
    find_library(
	${LIB_NAME_UPPER}_LIBRARY_DEBUG
        NAMES ${LIBNAME}d
        PATHS ${HINT_PATHS}
        DOC "The ${LIB_NAME} debug library"
    )
    

    if(${LIB_NAME_UPPER}_LIBRARY_RELEASE AND ${LIB_NAME_UPPER}_LIBRARY_DEBUG)
        set(${LIB_NAME_UPPER}_LIBRARY "optimized" ${${LIB_NAME_UPPER}_LIBRARY_RELEASE} "debug" ${${LIB_NAME_UPPER}_LIBRARY_DEBUG})
    elseif(${LIB_NAME_UPPER}_LIBRARY_RELEASE)
        set(${LIB_NAME_UPPER}_LIBRARY ${${LIB_NAME_UPPER}_LIBRARY_RELEASE})
    elseif(${LIB_NAME_UPPER}_LIBRARY_DEBUG)
        set(${LIB_NAME_UPPER}_LIBRARY ${${LIB_NAME_UPPER}_LIBRARY_DEBUG})
    endif()

    list(APPEND DELIBERATION_INCLUDE_DIRS ${${LIB_NAME_UPPER}_INCLUDE_DIR})
    list(APPEND DELIBERATION_LIBRARIES ${${LIB_NAME_UPPER}_LIBRARY})

    # DEBUG MESSAGES
    # message("${LIB_NAME_UPPER}_INCLUDE_DIR     = ${${LIB_NAME_UPPER}_INCLUDE_DIR}")
    # message("${LIB_NAME_UPPER}_LIBRARY_RELEASE = ${${LIB_NAME_UPPER}_LIBRARY_RELEASE}")
    # message("${LIB_NAME_UPPER}_LIBRARY_DEBUG   = ${${LIB_NAME_UPPER}_LIBRARY_DEBUG}")
    # message("${LIB_NAME_UPPER}_LIBRARY         = ${${LIB_NAME_UPPER}_LIBRARY}")

endmacro(find)


# load standard CMake arguments (c.f. http://stackoverflow.com/questions/7005782/cmake-include-findpackagehandlestandardargs-cmake)
include(FindPackageHandleStandardArgs)

if(CMAKE_CURRENT_LIST_FILE)
    get_filename_component(DELIBERATION_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
endif()

file(TO_CMAKE_PATH "$ENV{PROGRAMFILES}" ENV_PROGRAMFILES)
file(TO_CMAKE_PATH "$ENV{DELIBERATION_DIR}" ENV_DELIBERATION_DIR)

set(LIB_PATHS
    ${DELIBERATION_DIR}/build
    ${DELIBERATION_DIR}/build/Release
    ${DELIBERATION_DIR}/build/Debug
    ${DELIBERATION_DIR}/build-release
    ${DELIBERATION_DIR}/build-debug
    ${DELIBERATION_DIR}/Build
    ${DELIBERATION_DIR}/Build/Release
    ${DELIBERATION_DIR}/Build/Debug
    ${DELIBERATION_DIR}/Build-release
    ${DELIBERATION_DIR}/Build-debug
    ${DELIBERATION_DIR}/lib
    ${ENV_DELIBERATION_DIR}/lib
    ${ENV_PROGRAMFILES}/Deliberation/lib
    /usr/lib
    /usr/local/lib
    /sw/lib
    /opt/local/lib
    /usr/lib64
    /usr/local/lib64
    /sw/lib64
    /opt/local/lib64
)

# Find libraries
find(Deliberation Deliberation/Deliberation_API.h ${LIB_PATHS})

if(DELIBERATION_FIBLIB_LIBRARY)
  # add dependencies
endif()


# DEBUG
message("DELIBERATION_INCLUDE_DIR  = ${DELIBERATION_INCLUDE_DIR}")
message("DELIBERATION_INCLUDE_DIRS  = ${DELIBERATION_INCLUDE_DIRS}")
message("DELIBERATION_LIBRARIES = ${DELIBERATION_LIBRARIES}")

find_package_handle_standard_args(DELIBERATION DEFAULT_MSG DELIBERATION_LIBRARIES DELIBERATION_INCLUDE_DIRS)
mark_as_advanced(DELIBERATION_FOUND)
