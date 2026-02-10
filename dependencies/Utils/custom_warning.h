#pragma once

#if defined(__GNUC__) || defined(__clang__)
#define ___COMPILER_WARNING_IMPL___(ARG) _Pragma(#ARG)
#define COMPILER_WARNING(MSG) ___COMPILER_WARNING_IMPL___(warning MSG)
#elif defined(_MSC_VER)
#define COMPILER_WARNING(MSG) __pragma(message("Warning: " MSG))
#endif