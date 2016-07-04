
# GLM_FOUND
# GLM_INCLUDE_DIR

include(FindPackageHandleStandardArgs)

FIND_PATH(GLM_INCLUDE_DIR glm/glm.hpp
    ${PROJECT_SOURCE_DIR}/../glm
    $ENV{GLM_HOME}
    $ENV{GLMDIR}
    /usr/include
    /usr/local/include
    /sw/include
    /opt/local/include
    DOC "The directory where glm/glm.hpp resides.")
    
find_package_handle_standard_args(GLM REQUIRED_VARS GLM_INCLUDE_DIR)

mark_as_advanced(GLM_INCLUDE_DIR)
