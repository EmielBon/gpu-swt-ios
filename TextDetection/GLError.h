#pragma once

bool _check_gl_error(const char *file, int line);

///
/// Usage
/// [... some opengl calls]
/// 
///
#define check_gl_error() _check_gl_error(__FILE__,__LINE__)