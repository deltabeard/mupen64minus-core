cmake_minimum_required(VERSION 3.13)
project(mupen64minux-core C)

add_library(mupen64minux-core SHARED ./src/api/callbacks.c
        ./src/api/common.c
        ./src/api/config.c
        ./src/api/debugger.c
        ./src/api/frontend.c
        ./src/api/vidext.c
        ./src/backends/api/video_capture_backend.c
        ./src/backends/plugins_compat/audio_plugin_compat.c
        ./src/backends/plugins_compat/input_plugin_compat.c
        ./src/backends/clock_ctime_plus_delta.c
        ./src/backends/dummy_video_capture.c
        ./src/backends/file_storage.c
        ./src/device/cart/cart.c
        ./src/device/cart/af_rtc.c
        ./src/device/cart/cart_rom.c
        ./src/device/cart/eeprom.c
        ./src/device/cart/flashram.c
        ./src/device/cart/is_viewer.c
        ./src/device/cart/sram.c
        ./src/device/controllers/game_controller.c
        ./src/device/controllers/vru_controller.c
        ./src/device/controllers/paks/biopak.c
        ./src/device/controllers/paks/mempak.c
        ./src/device/controllers/paks/rumblepak.c
        ./src/device/controllers/paks/transferpak.c
        ./src/device/dd/dd_controller.c
        ./src/device/dd/disk.c
        ./src/device/device.c
        ./src/device/gb/gb_cart.c
        ./src/device/gb/mbc3_rtc.c
        ./src/device/gb/m64282fp.c
        ./src/device/memory/memory.c
        ./src/device/pif/bootrom_hle.c
        ./src/device/pif/cic.c
        ./src/device/pif/n64_cic_nus_6105.c
        ./src/device/pif/pif.c
        ./src/device/r4300/cached_interp.c
        ./src/device/r4300/cp0.c
        ./src/device/r4300/cp1.c
        ./src/device/r4300/cp2.c
        ./src/device/r4300/idec.c
        ./src/device/r4300/interrupt.c
        ./src/device/r4300/pure_interp.c
        ./src/device/r4300/r4300_core.c
        ./src/device/r4300/tlb.c
        ./src/device/rcp/ai/ai_controller.c
        ./src/device/rcp/mi/mi_controller.c
        ./src/device/rcp/pi/pi_controller.c
        ./src/device/rcp/rdp/fb.c
        ./src/device/rcp/rdp/rdp_core.c
        ./src/device/rcp/ri/ri_controller.c
        ./src/device/rcp/rsp/rsp_core.c
        ./src/device/rcp/si/si_controller.c
        ./src/device/rcp/vi/vi_controller.c
        ./src/device/rdram/rdram.c
        ./src/main/main.c
        ./src/main/util.c
        ./src/main/cheat.c
        ./src/main/eventloop.c
        ./src/main/rom.c
        ./src/main/savestates.c
        ./src/main/sdl_key_converter.c
        ./src/main/workqueue.c
        ./src/plugin/plugin.c
        ./src/plugin/dummy_video.c
        ./src/plugin/dummy_audio.c
        ./src/plugin/dummy_input.c
        ./src/plugin/dummy_rsp.c
        ./src/osal/files_win32.c
        ./src/osal/dynamiclib_win32.c
        ./subprojects/md5/md5.c)

target_include_directories(mupen64minux-core PUBLIC
        ./src
        ./src/asm_defines
        ./subprojects/xxhash
        ./subprojects/md5)

#include(FindPkgConfig)
#pkg_search_module(SDL2 REQUIRED sdl2)
#target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})
#target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2_INCLUDE_DIR})
target_compile_definitions(${PROJECT_NAME} PUBLIC M64P_PARALLEL NO_ASM)
target_compile_options(${PROJECT_NAME} PUBLIC -fdump-rtl-expand)
