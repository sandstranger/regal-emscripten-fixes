#
# Common command-line options
# ===========================
#
# To build in debug mode:
#   - use MODE=debug
#
# To build using ccache (http://ccache.samba.org/)
#   - use CCACHE=ccache
#
# To disable stripping of binaries:
#   - use MODE=debug
#   - use STRIP=
#
# To disable symlinks:
#   - use LN=
#
# To see verbose output
#   - use V=1 on gmake command-line

include build/common.inc

include make/Makefile.snappy
include make/Makefile.apitrace
include make/Makefile.glslopt
include make/Makefile.regal
include make/Makefile.glu
include make/Makefile.glut
include make/Makefile.glew
include make/Makefile.glewinfo

# Examples

include make/Makefile.dreamtorus
include make/Makefile.dreamtorus_static
include make/Makefile.tiger

# Testing

include make/Makefile.gtest
include make/Makefile.regaltest

# Misc

# include Makefile.expat

all::

clean::

clobber:
	$(RM) -r tmp
	$(RM) -r lib
	$(RM) -r bin

.PHONY: all clean clobber
