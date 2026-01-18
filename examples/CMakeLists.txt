# CMCoreType Examples
# Lightweight wrapper for cm_example_project with module-specific settings

macro(cm_example_project sub_folder project_name)
    cm_example_project_base(
        PROJECT_NAME ${project_name}
        FOLDER_PATH "Examples/CMCoreType/${sub_folder}"
        SOURCES ${ARGN}
        PRIVATE_LIBRARIES CMCoreType
    )
endmacro()

# Add subdirectories
add_subdirectory(typeinfo)
