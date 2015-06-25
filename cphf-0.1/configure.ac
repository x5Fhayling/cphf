#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([cphf], [0.1])
AM_INIT_AUTOMAKE([-Wall])
AC_CONFIG_SRCDIR([src/CPHFFile.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_LANG(C++)
AX_CXX_COMPILE_STDCXX_11(,mandatory)
AC_PROG_INSTALL
AC_CHECK_PROG(LSOF_CHECK,lsof,yes,no)
if test x"$LSOF_CHECK" != x"yes" ; then
    AC_MSG_ERROR([ERROR: lsof is missing.])
fi

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h limits.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_MODE_T
AC_TYPE_SIZE_T
AC_CHECK_MEMBERS([struct stat.st_rdev])
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_FORK
AC_HEADER_MAJOR
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([dup2 memset])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
