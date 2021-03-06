#np# Typically the only line that needs to be edited in this file is the last 
#np# line.  Set 'SUBDIRS= (list all subdirectories that are autotool'ed here)'
#np# List all subdirectories even if one or more are configured/built only
#np# conditionally.

#@HEADER
# ***********************************************************************
#
#                           Rythmos Package
#                 Copyright (2006) Sandia Corporation
#
# Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
# license for use of this work by or on behalf of the U.S. Government.
#
# This library is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation; either version 2.1 of the
# License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
# USA
# Questions? Contact Todd S. Coffey (tscoffe@sandia.gov)
#
# ***********************************************************************
#@HEADER

## #######################################################################
## Options to automake (rarely used - don't worry about it)
## #######################################################################
AUTOMAKE_OPTIONS = foreign

## #######################################################################
## Aclocal command (rarely used - don't worry about it)
## #######################################################################
ACLOCAL_AMFLAGS = -I config

#
#       I believe that by switching to AUX_DIR(../../config) one 
#       could get rid of these.
#
## #######################################################################
## Additional files to be included in distribution for 'make dist'
## #######################################################################
#np# For a typical package, there is no reason to distribute these files
#np# because users should not have to bootstrap.  We distribute them with
#np# new package so that the files can be used in creating the 
#np# configure script for other packages. 
EXTRA_DIST = \
config/ac_cxx_bool.m4                   config/depcomp \
config/ac_cxx_have_std.m4               config/install-sh \
config/ac_cxx_have_stl.m4               config/missing \
config/ac_cxx_mutable.m4                config/mkinstalldirs \
config/ac_cxx_namespaces.m4             config/tac_arg_check_mpi.m4 \
config/ac_cxx_new_for_scoping.m4        config/tac_arg_config_mpi.m4 \
config/ac_cxx_reinterpret_cast.m4 \
config/tac_arg_enable_compiler_options.m4 \
config/ac_cxx_std_ios_base_fmtflags.m4  config/tac_arg_enable_feature.m4 \
config/ac_cxx_std_sprintf.m4            config/tac_arg_enable_option.m4 \
config/ac_prompt_user.m4                config/tac_arg_with_ar.m4 \
config/ac_prompt_user_no_define.m4      config/tac_arg_with_compiler.m4 \
config/acx_check_dos_filesys.m4         config/tac_arg_with_flags.m4 \
config/tac_arg_with_incdirs.m4          config/acx_pthread.m4 \
config/config.guess                     config/tac_arg_with_libdirs.m4 \
config/config.sub                       config/tac_arg_with_package.m4 \
config/strip_dup_incl_paths.pl		config/strip_dup_libs.pl \
config/replace-install-prefix.pl        config/string-replace.pl \
config/token-replace.pl \
config/generate-makeoptions.pl		bootstrap 

#
#       I believe that by switching to AUX_DIR(../../config) one 
#       could get rid of these.
#
## #######################################################################
## Tools in the auxillary directory 
## #######################################################################
AUX_DIST = config/install-sh config/missing config/mkinstalldirs 
#
#  Again, I hope that AUX_DIR(../../config) eliminates these
#  config/install-sh config/missing config/mkinstalldirs 

## #######################################################################
## Files to be deleted by 'make maintainer-clean'
## #######################################################################
MAINTAINERCLEANFILES = Makefile.in aclocal.m4 autom4te.cache/* \
	configure config.status config.log \
	src/common/config-h.in src/common/stamp-h.in \
	$(AUX_DIST) 

#The following line helps the test harness recover from build errors.
                                                                                
all-local:
	@echo "Trilinos package rythmos built successfully"

## #######################################################################
## Subdirectories to be make'd recursively
## #######################################################################

#We now build tests and examples through separate make targets, rather than
#during "make".  We still need to conditionally include the test and example
#in SUBDIRS, even though BUILD_TESTS and BUILD_EXAMPLES will never be
#defined, so that the tests and examples are included in the distribution
#tarball.

if SUB_TEST
TEST_SUBDIR=test
endif

if SUB_EXAMPLE
EXAMPLE_SUBDIR=example
endif

if ENABLE_RYTHMOS_GAASP
BUILD_RYTHMOS_GAASP_TESTS=cd adapters/gaasp/test && $(MAKE)
BUILD_RYTHMOS_GAASP_EXAMPLES=cd adapters/gaasp/example && $(MAKE)
BUILD_RYTHMOS_GAASP_CLEAN_CMD=clean
BUILD_RYTHMOS_GAASP_INSTALL_CMD=install
endif

if BUILD_TESTS
tests: examples
	@echo ""
	@echo "Now building rythmos tests."
	@echo ""
	cd $(top_builddir)/test && $(MAKE)
	$(BUILD_RYTHMOS_GAASP_TESTS)
	@echo ""
	@echo "Finished building rythmos tests."
	@echo ""
else
tests:
	@echo "rythmos tests were disabled at configure time"
endif

if BUILD_EXAMPLES
examples:
	@echo ""
	@echo "Now building rythmos examples."
	@echo ""
	cd $(top_builddir)/example && $(MAKE)
	$(BUILD_RYTHMOS_GAASP_EXAMPLES)
	@echo ""
	@echo "Finished building rythmos examples."
	@echo ""

install-examples:
	cd $(top_builddir)/example && $(MAKE) install
	$(BUILD_RYTHMOS_GAASP_EXAMPLES) $(BUILD_RYTHMOS_GAASP_INSTALL_CMD)
else
examples:
	@echo "rythmos examples were disabled at configure time"

install-examples:
	@echo "rythmos examples were disabled at configure time"
endif

clean-tests:
	cd $(top_builddir)/test && $(MAKE) clean
	$(BUILD_RYTHMOS_GAASP_TESTS) $(BUILD_RYTHMOS_GAASP_CLEAN_CMD)

clean-examples:
	cd $(top_builddir)/example && $(MAKE) clean
	$(BUILD_RYTHMOS_GAASP_EXAMPLES) $(BUILD_RYTHMOS_GAASP_CLEAN_CMD)

everything:
	$(MAKE) && $(MAKE) examples && $(MAKE) tests

clean-everything:
	$(MAKE) clean-examples && $(MAKE) clean-tests && $(MAKE) clean

install-everything:
	$(MAKE) install && $(MAKE) install-examples

SUBDIRS = src adapters $(TEST_SUBDIR) $(EXAMPLE_SUBDIR)


if USING_EXPORT_MAKEFILES

install-exec-hook:	
	mkdir -p $(DESTDIR)$(includedir)
	cp $(top_builddir)/Makefile.export.rythmos $(DESTDIR)$(includedir)/.
	$(PERL_EXE) $(top_srcdir)/config/replace-install-prefix.pl \
		--exec-prefix=$(exec_prefix) \
		--my-export-makefile=Makefile.export.rythmos \
		--my-abs-top-srcdir=@abs_top_srcdir@ \
		--my-abs-incl-dirs=@abs_top_builddir@/src:@abs_top_srcdir@/src \
		--my-abs-lib-dirs=@abs_top_builddir@/src \
		--dep-package-abs-builddirs=@abs_top_builddir@/../thyra:@abs_top_builddir@/../epetraext:@abs_top_builddir@/../stratimikos:@abs_top_builddir@/../nox:@abs_top_builddir@/../sacado
	$(PERL_EXE) $(top_srcdir)/config/generate-makeoptions.pl $(top_builddir)/src/Makefile RYTHMOS > $(DESTDIR)$(includedir)/Makefile.export.rythmos.macros

uninstall-hook:
	rm -f $(includedir)/Makefile.export.rythmos
	rm -f $(includedir)/Makefile.export.rythmos.macros

else 

install-exec-hook:

uninstall-hook:

endif


## #######################################################################
## The below targets allow you to use the new
## testharness to run the test suite as make targets
## #######################################################################

TRILINOS_HOME_DIR=@abs_top_srcdir@/../..
TRILINOS_BUILD_DIR=@abs_top_builddir@/../..
TRILINOS_MPI_MAX_PROC=4
TRILINOS_TEST_CATEGORY=INSTALL

runtests-serial :
	$(PERL_EXE) $(TRILINOS_HOME_DIR)/commonTools/test/utilities/runtests \
  --trilinos-dir=$(TRILINOS_HOME_DIR) \
  --comm=serial \
  --build-dir=$(TRILINOS_BUILD_DIR) \
  --category=$(TRILINOS_TEST_CATEGORY) \
  --output-dir=@abs_top_builddir@/test/runtests-results \
  --verbosity=1 \
  --packages=rythmos

runtests-mpi :
	$(PERL_EXE) $(TRILINOS_HOME_DIR)/commonTools/test/utilities/runtests \
  --trilinos-dir=$(TRILINOS_HOME_DIR) \
  --comm=mpi \
  --mpi-go=$(TRILINOS_MPI_GO) \
  --build-dir=$(TRILINOS_BUILD_DIR) \
  --max-proc=$(TRILINOS_MPI_MAX_PROC) \
  --category=$(TRILINOS_TEST_CATEGORY) \
  --output-dir=@abs_top_builddir@/test/runtests-results \
  --verbosity=1 \
  --packages=rythmos
