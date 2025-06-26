#pragma once

#include "ix.hpp"

#if ix_PLATFORM(WIN)

#if !defined(NOGDICAPMASKS) && !defined(ix_WIN_INCLUDE_GDICAPMASKS)
#define NOGDICAPMASKS // CC_*, LC_*, PC_*, CP_*, TC_*, RC_
#endif
#if !defined(NOVIRTUALKEYCODES) && !defined(ix_WIN_INCLUDE_VIRTUALKEYCODES)
#define NOVIRTUALKEYCODES // VK_*
#endif
#if !defined(NOWINMESSAGES) && !defined(ix_WIN_INCLUDE_WINMESSAGES)
#define NOWINMESSAGES // WM_*, EM_*, LB_*, CB_*
#endif
#if !defined(NOWINSTYLES) && !defined(ix_WIN_INCLUDE_WINSTYLES)
#define NOWINSTYLES // WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
#endif
#if !defined(NOSYSMETRICS) && !defined(ix_WIN_INCLUDE_SYSMETRICS)
#define NOSYSMETRICS // SM_*
#endif
#if !defined(NOMENUS) && !defined(ix_WIN_INCLUDE_MENUS)
#define NOMENUS // MF_*
#endif
#if !defined(NOICONS) && !defined(ix_WIN_INCLUDE_ICONS)
#define NOICONS // IDI_*
#endif
#if !defined(NOKEYSTATES) && !defined(ix_WIN_INCLUDE_KEYSTATES)
#define NOKEYSTATES // MK_*
#endif
#if !defined(NOSYSCOMMANDS) && !defined(ix_WIN_INCLUDE_SYSCOMMANDS)
#define NOSYSCOMMANDS // SC_*
#endif
#if !defined(NORASTEROPS) && !defined(ix_WIN_INCLUDE_RASTEROPS)
#define NORASTEROPS // Binary and Tertiary raster ops
#endif
#if !defined(NOSHOWWINDOW) && !defined(ix_WIN_INCLUDE_SHOWWINDOW)
#define NOSHOWWINDOW // SW_*
#endif
#if !defined(OEMRESOURCE)
#define OEMRESOURCE // OEM Resource values
#endif
#if !defined(NOATOM) && !defined(ix_WIN_INCLUDE_ATOM)
#define NOATOM // Atom Manager routines
#endif
#if !defined(NOCLIPBOARD) && !defined(ix_WIN_INCLUDE_CLIPBOARD)
#define NOCLIPBOARD // Clipboard routines
#endif
#if !defined(NOCOLOR) && !defined(ix_WIN_INCLUDE_COLOR)
#define NOCOLOR // Screen colors
#endif
#if !defined(NOCTLMGR) && !defined(ix_WIN_INCLUDE_CTLMGR)
#define NOCTLMGR // Control and Dialog routines
#endif
#if !defined(NODRAWTEXT) && !defined(ix_WIN_INCLUDE_DRAWTEXT)
#define NODRAWTEXT // DrawText() and DT_*
#endif
#if !defined(NOGDI) && !defined(ix_WIN_INCLUDE_GDI)
#define NOGDI // All GDI defines and routines
#endif
#if !defined(NOKERNEL) && !defined(ix_WIN_INCLUDE_KERNEL)
#define NOKERNEL // All KERNEL defines and routines
#endif
#if !defined(NOUSER) && !defined(ix_WIN_INCLUDE_USER)
#define NOUSER // All USER defines and routines
#endif
#if !defined(NONLS) && !defined(ix_WIN_INCLUDE_NLS)
#define NONLS // All NLS defines and routines
#endif
#if !defined(NOMB) && !defined(ix_WIN_INCLUDE_MB)
#define NOMB // MB_* and MessageBox()
#endif
#if !defined(NOMEMMGR) && !defined(ix_WIN_INCLUDE_MEMMGR)
#define NOMEMMGR // GMEM_*, LMEM_*, GHND, LHND, associated routines
#endif
#if !defined(NOMETAFILE) && !defined(ix_WIN_INCLUDE_METAFILE)
#define NOMETAFILE // typedef METAFILEPICT
#endif
#if !defined(NOMINMAX) && !defined(ix_WIN_INCLUDE_MINMAX)
#define NOMINMAX // Macros min(a,b) and max(a,b)
#endif
#if !defined(NOMSG) && !defined(ix_WIN_INCLUDE_MSG)
#define NOMSG // typedef MSG and associated routines
#endif
#if !defined(NOOPENFILE) && !defined(ix_WIN_INCLUDE_OPENFILE)
#define NOOPENFILE // OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#endif
#if !defined(NOSCROLL) && !defined(ix_WIN_INCLUDE_SCROLL)
#define NOSCROLL // SB_* and scrolling routines
#endif
#if !defined(NOSERVICE) && !defined(ix_WIN_INCLUDE_SERVICE)
#define NOSERVICE // All Service Controller routines, SERVICE_ equates, etc.
#endif
#if !defined(NOSOUND) && !defined(ix_WIN_INCLUDE_SOUND)
#define NOSOUND // Sound driver routines
#endif
#if !defined(NOTEXTMETRIC) && !defined(ix_WIN_INCLUDE_TEXTMETRIC)
#define NOTEXTMETRIC // typedef TEXTMETRIC and associated routines
#endif
#if !defined(NOWH) && !defined(ix_WIN_INCLUDE_WH)
#define NOWH // SetWindowsHook and WH_*
#endif
#if !defined(NOWINOFFSETS) && !defined(ix_WIN_INCLUDE_WINOFFSETS)
#define NOWINOFFSETS // GWL_*, GCL_*, associated routines
#endif
#if !defined(NOCOMM) && !defined(ix_WIN_INCLUDE_COMM)
#define NOCOMM // COMM driver routines
#endif
#if !defined(NOKANJI) && !defined(ix_WIN_INCLUDE_KANJI)
#define NOKANJI // Kanji support stuff.
#endif
#if !defined(NOHELP) && !defined(ix_WIN_INCLUDE_HELP)
#define NOHELP // Help engine interface.
#endif
#if !defined(NOPROFILER) && !defined(ix_WIN_INCLUDE_PROFILER)
#define NOPROFILER // Profiler interface.
#endif
#if !defined(NODEFERWINDOWPOS) && !defined(ix_WIN_INCLUDE_DEFERWINDOWPOS)
#define NODEFERWINDOWPOS // DeferWindowPos routines
#endif
#if !defined(NOMCX) && !defined(ix_WIN_INCLUDE_MCX)
#define NOMCX // Modem Configuration Extensions
#endif

ix_DISABLE_CLANG_WARNING_BEGIN
ix_DISABLE_CLANG_WARNING("-Wreserved-macro-identifier")

#if ix_ARCH(x64)
#define _AMD64_ 1 // NOLINT
#elif ix_ARCH(ARM64)
#define _ARM64_ 1 // NOLINT
#else
#error "Unknown architecture"
#endif

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

ix_DISABLE_CLANG_WARNING_END

#include <windef.h>

#if 0
// For copy and paste.
#define ix_WIN_INCLUDE_GDICAPMASKS
#define ix_WIN_INCLUDE_VIRTUALKEYCODES
#define ix_WIN_INCLUDE_WINMESSAGES
#define ix_WIN_INCLUDE_WINSTYLES
#define ix_WIN_INCLUDE_SYSMETRICS
#define ix_WIN_INCLUDE_MENUS
#define ix_WIN_INCLUDE_ICONS
#define ix_WIN_INCLUDE_KEYSTATES
#define ix_WIN_INCLUDE_SYSCOMMANDS
#define ix_WIN_INCLUDE_RASTEROPS
#define ix_WIN_INCLUDE_SHOWWINDOW
#define ix_WIN_INCLUDE_ATOM
#define ix_WIN_INCLUDE_CLIPBOARD
#define ix_WIN_INCLUDE_COLOR
#define ix_WIN_INCLUDE_CTLMGR
#define ix_WIN_INCLUDE_DRAWTEXT
#define ix_WIN_INCLUDE_GDI
#define ix_WIN_INCLUDE_KERNEL
#define ix_WIN_INCLUDE_USER
#define ix_WIN_INCLUDE_NLS
#define ix_WIN_INCLUDE_MB
#define ix_WIN_INCLUDE_MEMMGR
#define ix_WIN_INCLUDE_METAFILE
#define ix_WIN_INCLUDE_MINMAX
#define ix_WIN_INCLUDE_MSG
#define ix_WIN_INCLUDE_OPENFILE
#define ix_WIN_INCLUDE_SCROLL
#define ix_WIN_INCLUDE_SERVICE
#define ix_WIN_INCLUDE_SOUND
#define ix_WIN_INCLUDE_TEXTMETRIC
#define ix_WIN_INCLUDE_WH
#define ix_WIN_INCLUDE_WINOFFSETS
#define ix_WIN_INCLUDE_COMM
#define ix_WIN_INCLUDE_KANJI
#define ix_WIN_INCLUDE_HELP
#define ix_WIN_INCLUDE_PROFILER
#define ix_WIN_INCLUDE_DEFERWINDOWPOS
#define ix_WIN_INCLUDE_MCX
#define ix_WIN_INCLUDE_GDICAPMASKS
#define ix_WIN_INCLUDE_VIRTUALKEYCODES
#define ix_WIN_INCLUDE_WINMESSAGES
#define ix_WIN_INCLUDE_WINSTYLES
#define ix_WIN_INCLUDE_SYSMETRICS
#define ix_WIN_INCLUDE_MENUS
#define ix_WIN_INCLUDE_ICONS
#define ix_WIN_INCLUDE_KEYSTATES
#define ix_WIN_INCLUDE_SYSCOMMANDS
#define ix_WIN_INCLUDE_RASTEROPS
#define ix_WIN_INCLUDE_SHOWWINDOW
#define ix_WIN_INCLUDE_ATOM
#define ix_WIN_INCLUDE_CLIPBOARD
#define ix_WIN_INCLUDE_COLOR
#define ix_WIN_INCLUDE_CTLMGR
#define ix_WIN_INCLUDE_DRAWTEXT
#define ix_WIN_INCLUDE_GDI
#define ix_WIN_INCLUDE_KERNEL
#define ix_WIN_INCLUDE_USER
#define ix_WIN_INCLUDE_NLS
#define ix_WIN_INCLUDE_MB
#define ix_WIN_INCLUDE_MEMMGR
#define ix_WIN_INCLUDE_METAFILE
#define ix_WIN_INCLUDE_MINMAX
#define ix_WIN_INCLUDE_MSG
#define ix_WIN_INCLUDE_OPENFILE
#define ix_WIN_INCLUDE_SCROLL
#define ix_WIN_INCLUDE_SERVICE
#define ix_WIN_INCLUDE_SOUND
#define ix_WIN_INCLUDE_TEXTMETRIC
#define ix_WIN_INCLUDE_WH
#define ix_WIN_INCLUDE_WINOFFSETS
#define ix_WIN_INCLUDE_COMM
#define ix_WIN_INCLUDE_KANJI
#define ix_WIN_INCLUDE_HELP
#define ix_WIN_INCLUDE_PROFILER
#define ix_WIN_INCLUDE_DEFERWINDOWPOS
#define ix_WIN_INCLUDE_MCX
#define ix_WIN_INCLUDE_GDICAPMASKS
#define ix_WIN_INCLUDE_VIRTUALKEYCODES
#define ix_WIN_INCLUDE_WINMESSAGES
#define ix_WIN_INCLUDE_WINSTYLES
#define ix_WIN_INCLUDE_SYSMETRICS
#define ix_WIN_INCLUDE_MENUS
#define ix_WIN_INCLUDE_ICONS
#define ix_WIN_INCLUDE_KEYSTATES
#define ix_WIN_INCLUDE_SYSCOMMANDS
#define ix_WIN_INCLUDE_RASTEROPS
#define ix_WIN_INCLUDE_SHOWWINDOW
#define ix_WIN_INCLUDE_ATOM
#define ix_WIN_INCLUDE_CLIPBOARD
#define ix_WIN_INCLUDE_COLOR
#define ix_WIN_INCLUDE_CTLMGR
#define ix_WIN_INCLUDE_DRAWTEXT
#define ix_WIN_INCLUDE_GDI
#define ix_WIN_INCLUDE_KERNEL
#define ix_WIN_INCLUDE_USER
#define ix_WIN_INCLUDE_NLS
#define ix_WIN_INCLUDE_MB
#define ix_WIN_INCLUDE_MEMMGR
#define ix_WIN_INCLUDE_METAFILE
#define ix_WIN_INCLUDE_MINMAX
#define ix_WIN_INCLUDE_MSG
#define ix_WIN_INCLUDE_OPENFILE
#define ix_WIN_INCLUDE_SCROLL
#define ix_WIN_INCLUDE_SERVICE
#define ix_WIN_INCLUDE_SOUND
#define ix_WIN_INCLUDE_TEXTMETRIC
#define ix_WIN_INCLUDE_WH
#define ix_WIN_INCLUDE_WINOFFSETS
#define ix_WIN_INCLUDE_COMM
#define ix_WIN_INCLUDE_KANJI
#define ix_WIN_INCLUDE_HELP
#define ix_WIN_INCLUDE_PROFILER
#define ix_WIN_INCLUDE_DEFERWINDOWPOS
#define ix_WIN_INCLUDE_MCX
#define ix_WIN_INCLUDE_GDICAPMASKS
#define ix_WIN_INCLUDE_VIRTUALKEYCODES
#define ix_WIN_INCLUDE_WINMESSAGES
#define ix_WIN_INCLUDE_WINSTYLES
#define ix_WIN_INCLUDE_SYSMETRICS
#define ix_WIN_INCLUDE_MENUS
#define ix_WIN_INCLUDE_ICONS
#define ix_WIN_INCLUDE_KEYSTATES
#define ix_WIN_INCLUDE_SYSCOMMANDS
#define ix_WIN_INCLUDE_RASTEROPS
#define ix_WIN_INCLUDE_SHOWWINDOW
#define ix_WIN_INCLUDE_ATOM
#define ix_WIN_INCLUDE_CLIPBOARD
#define ix_WIN_INCLUDE_COLOR
#define ix_WIN_INCLUDE_CTLMGR
#define ix_WIN_INCLUDE_DRAWTEXT
#define ix_WIN_INCLUDE_GDI
#define ix_WIN_INCLUDE_KERNEL
#define ix_WIN_INCLUDE_USER
#define ix_WIN_INCLUDE_NLS
#define ix_WIN_INCLUDE_MB
#define ix_WIN_INCLUDE_MEMMGR
#define ix_WIN_INCLUDE_METAFILE
#define ix_WIN_INCLUDE_MINMAX
#define ix_WIN_INCLUDE_MSG
#define ix_WIN_INCLUDE_OPENFILE
#define ix_WIN_INCLUDE_SCROLL
#define ix_WIN_INCLUDE_SERVICE
#define ix_WIN_INCLUDE_SOUND
#define ix_WIN_INCLUDE_TEXTMETRIC
#define ix_WIN_INCLUDE_WH
#define ix_WIN_INCLUDE_WINOFFSETS
#define ix_WIN_INCLUDE_COMM
#define ix_WIN_INCLUDE_KANJI
#define ix_WIN_INCLUDE_HELP
#define ix_WIN_INCLUDE_PROFILER
#define ix_WIN_INCLUDE_DEFERWINDOWPOS
#define ix_WIN_INCLUDE_MCX
#define ix_WIN_INCLUDE_GDICAPMASKS
#define ix_WIN_INCLUDE_VIRTUALKEYCODES
#define ix_WIN_INCLUDE_WINMESSAGES
#define ix_WIN_INCLUDE_WINSTYLES
#define ix_WIN_INCLUDE_SYSMETRICS
#define ix_WIN_INCLUDE_MENUS
#define ix_WIN_INCLUDE_ICONS
#define ix_WIN_INCLUDE_KEYSTATES
#define ix_WIN_INCLUDE_SYSCOMMANDS
#define ix_WIN_INCLUDE_RASTEROPS
#define ix_WIN_INCLUDE_SHOWWINDOW
#define ix_WIN_INCLUDE_ATOM
#define ix_WIN_INCLUDE_CLIPBOARD
#define ix_WIN_INCLUDE_COLOR
#define ix_WIN_INCLUDE_CTLMGR
#define ix_WIN_INCLUDE_DRAWTEXT
#define ix_WIN_INCLUDE_GDI
#define ix_WIN_INCLUDE_KERNEL
#define ix_WIN_INCLUDE_USER
#define ix_WIN_INCLUDE_NLS
#define ix_WIN_INCLUDE_MB
#define ix_WIN_INCLUDE_MEMMGR
#define ix_WIN_INCLUDE_METAFILE
#define ix_WIN_INCLUDE_MINMAX
#define ix_WIN_INCLUDE_MSG
#define ix_WIN_INCLUDE_OPENFILE
#define ix_WIN_INCLUDE_SCROLL
#define ix_WIN_INCLUDE_SERVICE
#define ix_WIN_INCLUDE_SOUND
#define ix_WIN_INCLUDE_TEXTMETRIC
#define ix_WIN_INCLUDE_WH
#define ix_WIN_INCLUDE_WINOFFSETS
#define ix_WIN_INCLUDE_COMM
#define ix_WIN_INCLUDE_KANJI
#define ix_WIN_INCLUDE_HELP
#define ix_WIN_INCLUDE_PROFILER
#define ix_WIN_INCLUDE_DEFERWINDOWPOS
#define ix_WIN_INCLUDE_MCX
#endif

#endif
