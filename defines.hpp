#pragma once

#ifndef WM_NCUAHDRAWCAPTION
#define WM_NCUAHDRAWCAPTION (0x00ae)
#endif

#ifndef WM_NCUAHDRAWFRAME
#define WM_NCUAHDRAWFRAME (0x00af)
#endif

#define START_SCOPE(x) namespace x {
#define END_SCOPE }
#define START_BLOCK {
#define END_BLOCK }

#pragma warning(disable:4189)
#pragma warning(disable:4191)
#pragma warning(disable:4100)
#pragma warning(disable:5039)

#define NOT_USING(x) (x)
