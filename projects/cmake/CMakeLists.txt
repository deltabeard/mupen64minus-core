# vim: ts=4:sw=4:expandtab
CMAKE_MINIMUM_REQUIRED(VERSION 3.18...3.23)

## Check user set options.
IF(NOT CMAKE_BUILD_TYPE)
    MESSAGE(STATUS "CMAKE_BUILD_TYPE was not set by user; setting build type to Debug")
    SET(CMAKE_BUILD_TYPE "Debug")
ELSE()
    # List of valid build types
    SET(VALID_BUILD_TYPES Debug Release RelWithDebInfo MinSizeRel)
    LIST(FIND VALID_BUILD_TYPES ${CMAKE_BUILD_TYPE} IS_VALID_BUILD_TYPE)
    IF(IS_VALID_BUILD_TYPE EQUAL -1)
        MESSAGE(FATAL_ERROR "CMAKE_BUILD_TYPE was '${CMAKE_BUILD_TYPE}' but can only be set to one of ${VALID_BUILD_TYPES}")
    ENDIF()
ENDIF()

PROJECT(mupen64minus-core LANGUAGES C CXX ASM VERSION 2.0.0)

set(SRCDIR ../../src)
set(SUBDIR ../../subprojects)
ADD_LIBRARY(mupen64minus-core SHARED ${SRCDIR}/api/callbacks.c
	${SRCDIR}/api/common.c ${SRCDIR}/api/config.c ${SRCDIR}/api/debugger.c
	${SRCDIR}/api/frontend.c ${SRCDIR}/api/vidext.c
	${SRCDIR}/backends/api/video_capture_backend.c
	${SRCDIR}/backends/plugins_compat/audio_plugin_compat.c
	${SRCDIR}/backends/plugins_compat/input_plugin_compat.c
	${SRCDIR}/backends/clock_ctime_plus_delta.c
	${SRCDIR}/backends/dummy_video_capture.c
	${SRCDIR}/backends/file_storage.c ${SRCDIR}/device/cart/cart.c
	${SRCDIR}/device/cart/af_rtc.c ${SRCDIR}/device/cart/cart_rom.c
	${SRCDIR}/device/cart/eeprom.c ${SRCDIR}/device/cart/flashram.c
	${SRCDIR}/device/cart/is_viewer.c ${SRCDIR}/device/cart/sram.c
	${SRCDIR}/device/controllers/game_controller.c
	${SRCDIR}/device/controllers/vru_controller.c
	${SRCDIR}/device/controllers/paks/biopak.c
	${SRCDIR}/device/controllers/paks/mempak.c
	${SRCDIR}/device/controllers/paks/rumblepak.c
	${SRCDIR}/device/controllers/paks/transferpak.c
	${SRCDIR}/device/dd/dd_controller.c ${SRCDIR}/device/dd/disk.c
	${SRCDIR}/device/device.c ${SRCDIR}/device/gb/gb_cart.c
	${SRCDIR}/device/gb/mbc3_rtc.c ${SRCDIR}/device/gb/m64282fp.c
	${SRCDIR}/device/memory/memory.c ${SRCDIR}/device/pif/bootrom_hle.c
	${SRCDIR}/device/pif/cic.c ${SRCDIR}/device/pif/n64_cic_nus_6105.c
	${SRCDIR}/device/pif/pif.c ${SRCDIR}/device/r4300/cached_interp.c
	${SRCDIR}/device/r4300/cp0.c ${SRCDIR}/device/r4300/cp1.c
	${SRCDIR}/device/r4300/cp2.c ${SRCDIR}/device/r4300/idec.c
	${SRCDIR}/device/r4300/interrupt.c ${SRCDIR}/device/r4300/pure_interp.c
	${SRCDIR}/device/r4300/r4300_core.c ${SRCDIR}/device/r4300/tlb.c
	${SRCDIR}/device/rcp/ai/ai_controller.c
	${SRCDIR}/device/rcp/mi/mi_controller.c
	${SRCDIR}/device/rcp/pi/pi_controller.c ${SRCDIR}/device/rcp/rdp/fb.c
	${SRCDIR}/device/rcp/rdp/rdp_core.c
	${SRCDIR}/device/rcp/ri/ri_controller.c
	${SRCDIR}/device/rcp/rsp/rsp_core.c
	${SRCDIR}/device/rcp/si/si_controller.c
	${SRCDIR}/device/rcp/vi/vi_controller.c ${SRCDIR}/device/rdram/rdram.c
	${SRCDIR}/main/main.c ${SRCDIR}/main/util.c ${SRCDIR}/main/cheat.c
	${SRCDIR}/main/eventloop.c ${SRCDIR}/main/rom.c
	${SRCDIR}/main/savestates.c ${SRCDIR}/main/sdl_key_converter.c
	${SRCDIR}/main/workqueue.c ${SRCDIR}/plugin/plugin.c
	${SRCDIR}/plugin/dummy_video.c ${SRCDIR}/plugin/dummy_audio.c
	${SRCDIR}/plugin/dummy_input.c ${SRCDIR}/plugin/dummy_rsp.c
	${SRCDIR}/device/r4300/new_dynarec/new_dynarec.c
    ${SRCDIR}/device/r4300/new_dynarec/x64/linkage_x64.asm
	${SRCDIR}/asm_defines/asm_defines.c
    #${SRCDIR}/device/r4300/new_dynarec/x64/linkage_x64.s
    #    ${SRCDIR}/osd/osd.c
    #    ${SRCDIR}/osd/oglft_c.cpp
    #    ${SUBDIR}/oglft/OGLFT.cpp
    ${SRCDIR}/osal/dynamiclib_win32.c
    ${SRCDIR}/osal/files_win32.c
	${SUBDIR}/md5/md5.c
	${SUBDIR}/minizip/ioapi.c
	${SUBDIR}/minizip/zip.c
	${SUBDIR}/minizip/unzip.c
	)

include(FindPkgConfig)
find_package(PkgConfig)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(ZLIB REQUIRED zlib)

target_link_libraries(mupen64minus-core PUBLIC ${SDL2_LIBRARIES} ${ZLIB_LIBRARIES})
target_include_directories(mupen64minus-core PUBLIC ${SDL2_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIRS})
target_compile_options(mupen64minus-core PUBLIC ${SDL2_CFLAGS} ${ZLIB_CFLAGS})

target_include_directories(mupen64minus-core PRIVATE ../../src ../../src/asm_defines ${SUBDIR}/minizip ${SUBDIR}/xxhash ${SUBDIR}/md5 ${SUBDIR}/oglft)
target_link_libraries(mupen64minus-core PRIVATE m opengl32 glu32)
target_compile_definitions(mupen64minus-core PRIVATE DYNAREC=x86_64 NEW_DYNAREC=2 NOCRYPT NOUNCRYPT M64P_PARALLEL)
add_compile_options(-fvisibility=hidden -fvisibility-inlines-hidden -fno-strict-aliasing -fPIC  -Wl,-m,i386pep -Wl,-Bsymbolic -shared -Wl,-export-all-symbols -Wl,-version-script,${SRCDIR}/api/api_export.ver)

# TODO
# bash ../../tools/gen_asm_script.sh ../../src/asm_defines build/CMakeFiles/mupen64minus-core.dir/${SRCDIR}/src/asm_defines/asm_defines.c.obj
# nasm -f win64 -d WIN64 -d PIC -I../../src -I../../src/asm_defines/ -o build/CMakeFiles/mupen64minus-core.dir/${SRCDIR}/device/r4300/new_dynarec/x64/linkage_x64.asm.obj ../../src/device/r4300/new_dynarec/x64/linkage_x64.asm

