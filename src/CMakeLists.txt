set(CMCORETYPE_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(CMCORETYPE_ROOT_INCLUDE_PATH ${CMCORETYPE_ROOT_PATH}/inc)
set(CMCORETYPE_ROOT_SOURCE_PATH ${CMCORETYPE_ROOT_PATH}/src)

set(TYPECORE_HGL_PATH ${CMCORETYPE_ROOT_INCLUDE_PATH}/hgl)
set(TYPECORE_PLATFORM_PATH ${TYPECORE_HGL_PATH}/platform)
set(TYPECORE_PLATFORM_OS_PATH ${TYPECORE_PLATFORM_PATH}/os)
set(TYPECORE_PLATFORM_COMPILER_PATH ${TYPECORE_PLATFORM_PATH}/compiler)
set(TYPECORE_TYPE_PATH ${TYPECORE_HGL_PATH}/type)

set(TYPECORE_HGL_MAIN_HEADERS  ${TYPECORE_HGL_PATH}/CoreType.h
								${TYPECORE_HGL_PATH}/Macro.h)

set(TYPECORE_PLATFORM_MAIN_HEADERS ${TYPECORE_PLATFORM_PATH}/Platform.h
									${TYPECORE_PLATFORM_PATH}/Exit.h
									${TYPECORE_PLATFORM_PATH}/FuncLoad.h)

set(TYPECORE_PLATFORM_OS_HEADERS ${TYPECORE_PLATFORM_OS_PATH}/Android.h
								  ${TYPECORE_PLATFORM_OS_PATH}/Apple.h
								  ${TYPECORE_PLATFORM_OS_PATH}/BSD.h
								  ${TYPECORE_PLATFORM_OS_PATH}/CharType.h
								  ${TYPECORE_PLATFORM_OS_PATH}/Linux.h
								  ${TYPECORE_PLATFORM_OS_PATH}/MSWindows.h
								  ${TYPECORE_PLATFORM_OS_PATH}/PosixThread.h
								  ${TYPECORE_PLATFORM_OS_PATH}/vsprintf.h)

set(TYPECORE_PLATFORM_COMPILER_HEADERS ${TYPECORE_PLATFORM_COMPILER_PATH}/EventFunc.h
										${TYPECORE_PLATFORM_COMPILER_PATH}/GNU.h
										${TYPECORE_PLATFORM_COMPILER_PATH}/Intel.h
										${TYPECORE_PLATFORM_COMPILER_PATH}/LLVM.h
										${TYPECORE_PLATFORM_COMPILER_PATH}/Microsoft.h
										${TYPECORE_PLATFORM_COMPILER_PATH}/Property.h)

set(TYPECORE_TYPE_HEADERS   ${TYPECORE_TYPE_PATH}/_Object.h
                            ${TYPECORE_TYPE_PATH}/AlignUtil.h
                            ${TYPECORE_TYPE_PATH}/ArrayWriter.h
                            ${TYPECORE_TYPE_PATH}/BitOperations.h
                            ${TYPECORE_TYPE_PATH}/CompareUtil.h
                            ${TYPECORE_TYPE_PATH}/Constants.h
                            ${TYPECORE_TYPE_PATH}/EnumUtil.h
                            ${TYPECORE_TYPE_PATH}/MemoryAlloc.h
                            ${TYPECORE_TYPE_PATH}/MemoryUtil.h
                            ${TYPECORE_TYPE_PATH}/MipmapUtil.h
                            ${TYPECORE_TYPE_PATH}/TypeLimits.h
)

source_group("Util" FILES ${TYPECORE_TYPE_HEADERS})

##==================================================================================================
## Math 数学库
##==================================================================================================
SET(MATH_INCLUDE_PATH   ${TYPECORE_HGL_PATH}/math)

SET(MATH_HEADER_FILES   ${MATH_INCLUDE_PATH}/Clamp.h
                        ${MATH_INCLUDE_PATH}/FloatPrecision.h
                        ${MATH_INCLUDE_PATH}/FloatControl.h
                        ${MATH_INCLUDE_PATH}/FloatValidation.h)

source_group("Math" FILES ${MATH_HEADER_FILES})
##==================================================================================================
## Text 文本处理
##==================================================================================================
set(STRCHAR_PATH ${TYPECORE_HGL_PATH}/type)
## StrChar 字符串字符操作
SET(STR_CHAR_FILES  ${STRCHAR_PATH}/CharType.h
                    ${STRCHAR_PATH}/StrChar.h
                    ${STRCHAR_PATH}/Str.Length.h
                    ${STRCHAR_PATH}/Str.Copy.h
                    ${STRCHAR_PATH}/Str.Stat.h
                    ${STRCHAR_PATH}/Str.Search.h
                    ${STRCHAR_PATH}/Str.Comp.h
                    ${STRCHAR_PATH}/Str.TrimClip.h
                    ${STRCHAR_PATH}/Str.Case.h
                    ${STRCHAR_PATH}/Str.Number.h
                    ${STRCHAR_PATH}/Str.NumberArray.h
                    ${STRCHAR_PATH}/Str.StringArray.h
                    ${STRCHAR_PATH}/Str.Between.h
                    ${STRCHAR_PATH}/Str.Hex.h
)

SOURCE_GROUP("Text\\StrChar" FILES ${STR_CHAR_FILES})

##==================================================================================================

SET(TIME_FILES  ${TYPECORE_HGL_PATH}/time/TimeConst.h)

source_group("Time" FILES ${TIME_FILES})
##==================================================================================================
set(TYPECORE_HEADERS ${TYPECORE_HGL_MAIN_HEADERS}
					 ${TYPECORE_PLATFORM_MAIN_HEADERS}
					 ${TYPECORE_PLATFORM_OS_HEADERS}
					 ${TYPECORE_PLATFORM_COMPILER_HEADERS}
					 ${TYPECORE_TYPE_HEADERS}
					 ${MATH_HEADER_FILES}
					 ${STR_CHAR_FILES}
					 ${TIME_FILES})

set(TYPECORE_SOURCE_FILES CMCoreTypeDummy.cpp)

source_group("Platform" FILES ${TYPECORE_PLATFORM_MAIN_HEADERS})
source_group("Platform\\OS" FILES ${TYPECORE_PLATFORM_OS_HEADERS})
source_group("Platform\\Compiler" FILES ${TYPECORE_PLATFORM_COMPILER_HEADERS})
source_group("Source" FILES ${TYPECORE_SOURCE_FILES})

list(REMOVE_DUPLICATES TYPECORE_HEADERS)

add_library(CMCoreType STATIC ${TYPECORE_HEADERS} ${TYPECORE_SOURCE_FILES})

if(WIN32 AND DEFINED HGL_MATH_LIB)
	target_link_libraries(CMCoreType PRIVATE ${HGL_MATH_LIB})
endif()

# Export include directory
set_target_properties(CMCoreType PROPERTIES PUBLIC_HEADER "${TYPECORE_HEADERS}")
target_include_directories(CMCoreType PUBLIC ${CMCORETYPE_ROOT_INCLUDE_PATH})

set_property(TARGET CMCoreType PROPERTY FOLDER "CM")
