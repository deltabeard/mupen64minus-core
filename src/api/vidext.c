/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus-core - api/vidext.c                                       *
 *   Mupen64Plus homepage: https://mupen64plus.org/                        *
 *   Copyright (C) 2009 Richard Goedeken                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       * 
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
                       
/* This file contains the Core video extension functions which will be exported
 * outside of the core library.
 */

#include <stdlib.h>
#include <string.h>

#define M64P_CORE_PROTOTYPES 1
#include "osal/preproc.h"
#include "../osd/osd.h"
#include "callbacks.h"
#include "m64p_types.h"
#include "m64p_vidext.h"
#include "vidext.h"

/* local variables */
static m64p_video_extension_functions l_ExternalVideoFuncTable = {17, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
static int l_VideoExtensionActive = 0;
static int l_VideoOutputActive = 0;
static int l_Fullscreen = 0;
static int l_SwapControl = 0;
static m64p_render_mode l_RenderMode = M64P_RENDER_OPENGL;

#ifdef VIDEXT_VULKAN
static const char** l_VulkanExtensionNames = NULL;
#endif

/* global function for use by frontend.c */
m64p_error OverrideVideoFunctions(m64p_video_extension_functions *VideoFunctionStruct)
{
    /* check input data */
    if (VideoFunctionStruct == NULL)
        return M64ERR_INPUT_ASSERT;
    if (VideoFunctionStruct->Functions < 17)
        return M64ERR_INPUT_INVALID;

    /* disable video extension if any of the function pointers are NULL */
    if (VideoFunctionStruct->VidExtFuncInit == NULL ||
        VideoFunctionStruct->VidExtFuncInitWithRenderMode == NULL ||
        VideoFunctionStruct->VidExtFuncQuit == NULL ||
        VideoFunctionStruct->VidExtFuncListModes == NULL ||
        VideoFunctionStruct->VidExtFuncListRates == NULL ||
        VideoFunctionStruct->VidExtFuncSetMode == NULL ||
        VideoFunctionStruct->VidExtFuncSetModeWithRate == NULL ||
        VideoFunctionStruct->VidExtFuncGLGetProc == NULL ||
        VideoFunctionStruct->VidExtFuncGLSetAttr == NULL ||
        VideoFunctionStruct->VidExtFuncGLGetAttr == NULL ||
        VideoFunctionStruct->VidExtFuncGLSwapBuf == NULL ||
        VideoFunctionStruct->VidExtFuncSetCaption == NULL ||
        VideoFunctionStruct->VidExtFuncToggleFS == NULL ||
        VideoFunctionStruct->VidExtFuncResizeWindow == NULL ||
        VideoFunctionStruct->VidExtFuncGLGetDefaultFramebuffer == NULL ||
        VideoFunctionStruct->VidExtFuncVKGetSurface == NULL ||
        VideoFunctionStruct->VidExtFuncVKGetInstanceExtensions == NULL)
    {
        l_ExternalVideoFuncTable.Functions = 17;
        memset(&l_ExternalVideoFuncTable.VidExtFuncInit, 0, 17 * sizeof(void *));
        l_VideoExtensionActive = 0;
        return M64ERR_SUCCESS;
    }

    /* otherwise copy in the override function pointers */
    memcpy(&l_ExternalVideoFuncTable, VideoFunctionStruct, sizeof(m64p_video_extension_functions));
    l_VideoExtensionActive = 1;
    return M64ERR_SUCCESS;
}

int VidExt_InFullscreenMode(void)
{
    return l_Fullscreen;
}

int VidExt_VideoRunning(void)
{
    return l_VideoOutputActive;
}

/* video extension functions to be called by the video plugin */
EXPORT m64p_error CALL VidExt_Init(void)
{
    /* call video extension override if necessary */
    if (l_VideoExtensionActive)
        return (*l_ExternalVideoFuncTable.VidExtFuncInit)();

    /* redirect to VidExt_InitWithRenderMode with OpenGL render mode */
    return VidExt_InitWithRenderMode(M64P_RENDER_OPENGL);
}

EXPORT m64p_error CALL VidExt_InitWithRenderMode(m64p_render_mode RenderMode)
{
    /* call video extension override if necessary */
    if (l_VideoExtensionActive)
        return (*l_ExternalVideoFuncTable.VidExtFuncInitWithRenderMode)(RenderMode);

    /* set global render mode */
#ifdef VIDEXT_VULKAN
    l_RenderMode = RenderMode;
#endif

    return M64ERR_SUCCESS;
}

EXPORT m64p_error CALL VidExt_Quit(void)
{
    /* call video extension override if necessary */
    if (l_VideoExtensionActive)
    {
        m64p_error rval = (*l_ExternalVideoFuncTable.VidExtFuncQuit)();
        if (rval == M64ERR_SUCCESS)
        {
            l_VideoOutputActive = 0;
            StateChanged(M64CORE_VIDEO_MODE, M64VIDEO_NONE);
        }
        return rval;
    }

    l_VideoOutputActive = 0;
    StateChanged(M64CORE_VIDEO_MODE, M64VIDEO_NONE);

    return M64ERR_SUCCESS;
}

EXPORT m64p_error CALL VidExt_ListFullscreenModes(m64p_2d_size *SizeArray, int *NumSizes)
{
    return M64ERR_SYSTEM_FAIL;
}

EXPORT m64p_error CALL VidExt_ListFullscreenRates(m64p_2d_size Size, int *NumRates, int *Rates)
{
    return M64ERR_SYSTEM_FAIL;
}

EXPORT m64p_error CALL VidExt_SetVideoMode(int Width, int Height, int BitsPerPixel, m64p_video_mode ScreenMode, m64p_video_flags Flags)
{
    return M64ERR_SYSTEM_FAIL;
}

EXPORT m64p_error CALL VidExt_SetVideoModeWithRate(int Width, int Height, int RefreshRate, int BitsPerPixel, m64p_video_mode ScreenMode, m64p_video_flags Flags)
{
    return M64ERR_UNSUPPORTED;
}

EXPORT m64p_error CALL VidExt_ResizeWindow(int Width, int Height)
{
    StateChanged(M64CORE_VIDEO_SIZE, (Width << 16) | Height);
    return M64ERR_SUCCESS;
}

EXPORT m64p_error CALL VidExt_SetCaption(const char *Title)
{
    return M64ERR_SUCCESS;
}

EXPORT m64p_error CALL VidExt_ToggleFullScreen(void)
{
    return M64ERR_SYSTEM_FAIL;
}

EXPORT m64p_function CALL VidExt_GL_GetProcAddress(const char* Proc)
{
    return NULL;
}

EXPORT m64p_error CALL VidExt_GL_SetAttribute(m64p_GLattr Attr, int Value)
{
    return M64ERR_INPUT_INVALID;
}

EXPORT m64p_error CALL VidExt_GL_GetAttribute(m64p_GLattr Attr, int *pValue)
{
    return M64ERR_INPUT_INVALID;
}

EXPORT m64p_error CALL VidExt_GL_SwapBuffers(void)
{
    return M64ERR_UNSUPPORTED;
}

EXPORT uint32_t CALL VidExt_GL_GetDefaultFramebuffer(void)
{
    if (l_VideoExtensionActive)
        return (*l_ExternalVideoFuncTable.VidExtFuncGLGetDefaultFramebuffer)();

    return 0;
}

EXPORT m64p_error CALL VidExt_VK_GetSurface(void** Surface, void* Instance)
{
    if (l_VideoExtensionActive)
        return (*l_ExternalVideoFuncTable.VidExtFuncVKGetSurface)(Surface, Instance);

    return M64ERR_UNSUPPORTED;
}

EXPORT m64p_error CALL VidExt_VK_GetInstanceExtensions(const char** Extensions[], uint32_t* NumExtensions)
{
    if (l_VideoExtensionActive)
        return (*l_ExternalVideoFuncTable.VidExtFuncVKGetInstanceExtensions)(Extensions, NumExtensions);

    return M64ERR_UNSUPPORTED;
}
