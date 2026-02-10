#pragma once
#include "pch.h"

#include "types.h"
#include "print.h"
#include "convert.h"
#include "fps.h"
#include "with_default_value.h"
#include "path.h"
#include "custom_warning.h"

#define keyword_to_string(keyword) #keyword

#ifdef _DEBUG
#define DEBUG_VAL(deb_val, rel_val) deb_val
#else
#define DEBUG_VAL(deb_val, rel_val) rel_val
#endif