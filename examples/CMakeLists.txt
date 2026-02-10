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

cm_example_project("" ArrayItemProcessTest      ArrayItemProcessTest.cpp)
cm_example_project("" ArrayRearrangeHelperTest  ArrayRearrangeHelperTest.cpp)
cm_example_project("" ObjectUtilTest            ObjectUtilTest.cpp)

cm_example_project("" TypeCastTest              TypeCastTest.cpp)

cm_example_project("Cereal" CerealBasicTest           CerealBasicTest.cpp)
cm_example_project("Cereal" CerealVersionedTest       CerealVersionedTest.cpp)

cm_example_project("Hash" WyHashTest                WyHashTest.cpp)

add_subdirectory(StrNumber)
