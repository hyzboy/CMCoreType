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
                            ${TYPECORE_TYPE_PATH}/ObjectUtil.h
                            ${TYPECORE_TYPE_PATH}/MipmapUtil.h
                            ${TYPECORE_TYPE_PATH}/TypeLimits.h
)

source_group("Util" FILES ${TYPECORE_TYPE_HEADERS})

##==================================================================================================
## Math 数学库
##==================================================================================================
SET(MATH_INCLUDE_PATH   ${TYPECORE_HGL_PATH}/math)

SET(MATH_HEADER_FILES   ${MATH_INCLUDE_PATH}/ClampUtil.h
                        ${MATH_INCLUDE_PATH}/FloatPrecision.h
                        ${MATH_INCLUDE_PATH}/FloatControl.h
                        ${MATH_INCLUDE_PATH}/FloatValidation.h

                        ${MATH_INCLUDE_PATH}/PhysicsConstants.h
                        ${MATH_INCLUDE_PATH}/BinaryConstants.h)

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
## Color 颜色
##==================================================================================================
## Color\\Basic 基础颜色类
SET(COLOR_BASIC_FILES ${CMCORETYPE_ROOT_INCLUDE_PATH}/hgl/color/Color.h
                      ${CMCORETYPE_ROOT_INCLUDE_PATH}/hgl/color/Color3f.h
                      ${CMCORETYPE_ROOT_INCLUDE_PATH}/hgl/color/Color3ub.h
                      ${CMCORETYPE_ROOT_INCLUDE_PATH}/hgl/color/Color4f.h
                      ${CMCORETYPE_ROOT_INCLUDE_PATH}/hgl/color/Color4ub.h
                      Color/Color.cpp
                      Color/Color3f.cpp
                      Color/Color3ub.cpp
                      Color/Color4f.cpp
                      Color/Color4ub.cpp)
SOURCE_GROUP("Color\\Basic" FILES ${COLOR_BASIC_FILES})

## Color\\Models 色彩模型
SET(COLOR_MODELS_FILES ${CMCORETYPE_ROOT_INCLUDE_PATH}/hgl/color/CMYKf.h
                       ${CMCORETYPE_ROOT_INCLUDE_PATH}/hgl/color/CMYKub.h
                       ${CMCORETYPE_ROOT_INCLUDE_PATH}/hgl/color/HSL.h
                       ${CMCORETYPE_ROOT_INCLUDE_PATH}/hgl/color/HSV.h
                       ${CMCORETYPE_ROOT_INCLUDE_PATH}/hgl/color/OKLab.h
                       ${CMCORETYPE_ROOT_INCLUDE_PATH}/hgl/color/XYZ.h
                       ${CMCORETYPE_ROOT_INCLUDE_PATH}/hgl/color/YCbCr.h
                       ${CMCORETYPE_ROOT_INCLUDE_PATH}/hgl/color/YCoCg.h
                       Color/CMYKf.cpp
                       Color/CMYKub.cpp
                       Color/HSL.cpp)
SOURCE_GROUP("Color\\Models" FILES ${COLOR_MODELS_FILES})

## Color\\Linear 线性颜色
SET(COLOR_LINEAR_FILES ${CMCORETYPE_ROOT_INCLUDE_PATH}/hgl/color/LinearColor3f.h
                       ${CMCORETYPE_ROOT_INCLUDE_PATH}/hgl/color/LinearColor4f.h
                       Color/LinearColor3f.cpp
                       Color/LinearColor4f.cpp)
SOURCE_GROUP("Color\\Linear" FILES ${COLOR_LINEAR_FILES})

## Color\\Conversion 颜色转换
SET(COLOR_CONVERSION_FILES ${CMCORETYPE_ROOT_INCLUDE_PATH}/hgl/color/sRGBConvert.h)
SOURCE_GROUP("Color\\Conversion" FILES ${COLOR_CONVERSION_FILES})

## Color\\Operations 颜色操作
SET(COLOR_OPERATIONS_FILES ${CMCORETYPE_ROOT_INCLUDE_PATH}/hgl/color/ColorFormat.h
                           ${CMCORETYPE_ROOT_INCLUDE_PATH}/hgl/color/ColorLerp.h
                           ${CMCORETYPE_ROOT_INCLUDE_PATH}/hgl/color/ColorPacking.h
                           ${CMCORETYPE_ROOT_INCLUDE_PATH}/hgl/color/Lum.h)
SOURCE_GROUP("Color\\Operations" FILES ${COLOR_OPERATIONS_FILES})

## Color 汇总
SET(COLOR_ALL_FILES ${COLOR_BASIC_FILES}
                    ${COLOR_MODELS_FILES}
                    ${COLOR_LINEAR_FILES}
                    ${COLOR_CONVERSION_FILES}
                    ${COLOR_OPERATIONS_FILES})

set(TYPECORE_HEADERS ${TYPECORE_HGL_MAIN_HEADERS}
					 ${TYPECORE_PLATFORM_MAIN_HEADERS}
					 ${TYPECORE_PLATFORM_OS_HEADERS}
					 ${TYPECORE_PLATFORM_COMPILER_HEADERS}
					 ${TYPECORE_TYPE_HEADERS}
					 ${MATH_HEADER_FILES}
					 ${STR_CHAR_FILES}
					 ${TIME_FILES}
)

source_group("Platform" FILES ${TYPECORE_PLATFORM_MAIN_HEADERS})
source_group("Platform\\OS" FILES ${TYPECORE_PLATFORM_OS_HEADERS})
source_group("Platform\\Compiler" FILES ${TYPECORE_PLATFORM_COMPILER_HEADERS})

list(REMOVE_DUPLICATES TYPECORE_HEADERS)

add_cm_library(CMCoreType "CM" ${CMCORETYPE_ROOT_INCLUDE_PATH}
                    ${TYPECORE_HEADERS}
                    ${TYPECORE_SOURCE_FILES}
                    ${COLOR_ALL_FILES})

# Set C++ standard to C++20
set_target_properties(CMCoreType PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)

if(WIN32 AND DEFINED HGL_MATH_LIB)
	target_link_libraries(CMCoreType PRIVATE ${HGL_MATH_LIB})
endif()

# Export include directory
set_target_properties(CMCoreType PROPERTIES PUBLIC_HEADER "${TYPECORE_HEADERS}")
