#*********************************************************#
#*  File: LinuxNDK.cmake                                 *
#*
#*  Copyright (C) 2002-2012 The PixelLight Team (http://www.pixellight.org/)
#*
#*  This file is part of PixelLight.
#*
#*  PixelLight is free software: you can redistribute it and/or modify
#*  it under the terms of the GNU Lesser General Public License as published by
#*  the Free Software Foundation, either version 3 of the License, or
#*  (at your option) any later version.
#*
#*  PixelLight is distributed in the hope that it will be useful,
#*  but WITHOUT ANY WARRANTY; without even the implied warranty of
#*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#*  GNU Lesser General Public License for more details.
#*
#*  You should have received a copy of the GNU Lesser General Public License
#*  along with PixelLight. If not, see <http://www.gnu.org/licenses/>.
#*********************************************************#


##################################################
## Linux/NDK platform definitions
##
## This file contains compiler and linker settings which are specific to the NDK compiler suit under Linux
##################################################


##################################################
## Preprocessor definitions
##################################################

# Preprocessor definitions
set(LINUX_COMPILE_DEFS
	${LINUX_COMPILE_DEFS}
	__STDC_INT64__						# We need "int64_t" and "uint64_t", so force this preprocessor definition (used in "PLCore/PLCoreLinux.h")
	__STDINT_LIMITS						# We need "UINTMAX_MAX" (used in "PLCore/PLCoreLinux.cpp")
	ANDROID								# We are using the ANDROID NDK toolchain
	HAVE_VISIBILITY_ATTR				# Use visibility attribute (http://gcc.gnu.org/wiki/Visibility)
)


##################################################
## Compiler flags
##################################################

set(LINUX_COMPILE_FLAGS
	${LINUX_COMPILE_FLAGS}
	-fvisibility=hidden					# In order to reduce the binary size, don't put private symbols into the resulting binary (http://gcc.gnu.org/wiki/Visibility)
	-fvisibility-inlines-hidden			# In order to reduce the binary size, don't put private symbols into the resulting binary (http://gcc.gnu.org/wiki/Visibility)
	-Wstrict-null-sentinel				# Warn also about the use of an uncasted NULL as sentinel
	-nostdlib							# Don't use the standard C library of the host system (Android uses a stripped down version of libc, called "bionic")
	-nodefaultlibs						# Do not use the standard system libraries when linking
)

if(NOT CMAKE_BUILD_TYPE MATCHES Debug)
	set(LINUX_COMPILE_FLAGS
		${LINUX_COMPILE_FLAGS}
		-Os								# Optimize for binary size, we're targeting mobile devices so size matters
		-funroll-all-loops				# Perform the optimization of loop unrolling
		-fpeel-loops					# Peels the loops for that there is enough information that they do not roll much (from profile feedback)
		-ftree-vectorize				# Enable the vectorizer
		# The following flag usage is basing on information from http://software.intel.com/en-us/forums/showthread.php?t=66902
		--param max-unroll-times=4
	)
endif()

# Add Android NDK include directory
include_directories(${ANDROID_NDK_SYSROOT}/usr/include/)


##################################################
## Linker flags
##################################################

# Add Android NDK library directory
set(LINUX_LINKER_FLAGS
	${LINUX_LINKER_FLAGS}
	-Wl,-rpath-link=${ANDROID_NDK_SYSROOT}/usr/lib/
	-L${ANDROID_NDK_SYSROOT}/usr/lib/					# If we don't set this, we get "cannot find -lc" later on
)

# Above we set "nostdlib" to avoid using the standard C library of the host system.
# Now we have to tell the linker to use Androids stripped down version of libc ("bionic"),
# present in the location specified by using the "-L" in the "LDFLAGS"-option above.
add_to_list(CMAKETOOLS_CURRENT_LIBS c)
