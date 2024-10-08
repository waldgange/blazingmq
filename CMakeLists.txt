cmake_minimum_required( VERSION 3.19 )

project( BMQ LANGUAGES C CXX )
    # o We need the above line because, per CMake 'project' command
    #   documentation, "The top-level CMakeLists.txt file for a project must
    #   contain a literal, direct call to the project() command; loading one
    #   through the include() command is not sufficient."

# -----------------------------------------------------------------------------
#                                    MODULES
# -----------------------------------------------------------------------------
# Where to find custom CMake Modules
#
list( APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/etc/cmake" )

include( BBVendor )
include( BMQPlugins )
include( TargetBMQStyleUor )
include( BmqPackageProvider )

setup_package_provider()

# -----------------------------------------------------------------------------
#                                INITIALIZATION
# -----------------------------------------------------------------------------

enable_testing()

set( CMAKE_POSITION_INDEPENDENT_CODE ON )

# Define CXXFlags/LDFlags per build mode, that are OS/compiler specific
if( "${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?Clang" )
  # Enable as much Clang compiler warnings as we can
  add_compile_options(
          "-Weverything"
          "-Wno-covered-switch-default"
          "-Wno-padded"
          "-Wno-global-constructors"    # bmqp_protocol mask constants
          "-Wno-conversion"
          "-Wno-undef"                  # BSLS_COMPILERFEATURES_SIMULATE_CPP11_FEATURES
          "-Wno-float-equal"            # operator== in generated classes
  )



  if( DEFINED CMAKE_CXX_STANDARD AND CMAKE_CXX_STANDARD GREATER_EQUAL 17 )
    # The below warnings are disabled, but should be revisited and properly
    # addressed
    add_compile_options(
            "-Wno-atomic-implicit-seq-cst"
            "-Wno-c++98-compat-pedantic"
            "-Wno-deprecated"
            "-Wno-disabled-macro-expansion"
            "-Wno-extra-semi-stmt"
            "-Wno-zero-as-null-pointer-constant"
            # to compile hello_world: uncomment or add 'override' specifier (C++11)
            # "-Wno-suggest-override"
  )
  endif()
endif()

if( "${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" )
  # Disable the warning about ABI change around exception specifications from
  # C++17 as there is nothing anyone can do about it; we just should not mix
  # C++17 and non C++17 built code.
  set_property( DIRECTORY
                APPEND
                PROPERTY COMPILE_OPTIONS "-Wno-noexcept-type" )
endif()

# Enable '-Werror' for all targets when using GCC or Clang.
# NOTE: o In order to be effective, this must be called before creating the
#         targets (i.e. before the 'bbproject_add_group/application').
#       o Do it here (after 'bbproject_setup' so that overlays are not
#         compiled with -Werror)
set_property( DIRECTORY
              APPEND
              PROPERTY COMPILE_WARNING_AS_ERROR ON )
if( "${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU|(Apple)?Clang" )
  # Disable warnings about unused functions or variables when not in Debug
  # build, so that we don't get warn (and build failure due to warn-as-error)
  # for code only used within BSLS_ASSERT_SAFE.
  if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_property(DIRECTORY APPEND PROPERTY COMPILE_OPTIONS "-Wno-unused-variable")
    set_property(DIRECTORY APPEND PROPERTY COMPILE_OPTIONS "-Wno-unused-function")
    set_property(DIRECTORY APPEND PROPERTY COMPILE_OPTIONS "-Wno-unused-but-set-variable")
  endif()
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  # In Debug build, ensure that BSLS_ASSERT_SAFE macro is enabled
  add_definitions("-DBSLS_ASSERT_LEVEL_ASSERT_SAFE")
endif()

# This repo contains the canonical source of multiple, independently released,
# UORs (mwc, bmq, bmqbrkr, bmqbrkrcfg, bmqtool).  When releasing a UOR, that
# specific target is selected through injection of the 'INSTALL_TARGETS'
# variable.  Many static analysis tool leverage the generated
# 'compile_commands.json' in order to know which sources to work with.
# Therefore, we should only add the necessary targets based on the UOR being
# built.
#
# To that purpose, we have to 'manually' build the reverse dependencies, so
# that we will only add targets to the intended ones, based on what is
# currently being built.  We will set a "BMQ_TARGET_<xyz>_NEEDED" value to
# YES/NO.
# - if INSTALL_TARGETS is empty, this means we are not doing a DPKG build, but
#   just a normal developer build, therefore include all targets
# - otherwise, selectively enable targets based on what is being built

if (NOT DEFINED INSTALL_TARGETS)
  # If no specic install targets has been set, then enable them all
  set(BMQ_TARGET_BMQBRKR_NEEDED        YES)
  set(BMQ_TARGET_BMQBRKRCFG_NEEDED     YES)
  set(BMQ_TARGET_BMQTOOL_NEEDED        YES)
  set(BMQ_TARGET_BMQSTORAGETOOL_NEEDED YES)
  set(BMQ_TARGET_MWC_NEEDED            YES)
  set(BMQ_TARGET_BMQ_NEEDED            YES)
  set(BMQ_TARGET_MQB_NEEDED            YES)
  set(BMQ_TARGET_E_BMQBRKR_NEEDED      YES)
  set(BMQ_TARGET_TUTORIAL_NEEDED       YES)
  set(BMQ_TARGET_PROMETHEUS_NEEDED     NO)
else()
  bbproject_check_install_target("bmqbrkr"         installBMQBRKR)
  bbproject_check_install_target("BMQBRKR_NIGHTLY" installNightly)

  if (installNightly)
    if (NOT installBMQBRKR)
      string(APPEND INSTALL_TARGETS ";BMQBRKR")
      set(installBMQBRKR YES)
    endif()
  endif()

  # Disable all by default, and then we'll enable selectively based on the
  # content of INSTALL_TARGETS
  set(BMQ_TARGET_BMQBRKR_NEEDED        NO)
  set(BMQ_TARGET_BMQBRKRCFG_NEEDED     NO)
  set(BMQ_TARGET_BMQTOOL_NEEDED        NO)
  set(BMQ_TARGET_BMQSTORAGETOOL_NEEDED NO)
  set(BMQ_TARGET_MWC_NEEDED            NO)
  set(BMQ_TARGET_BMQ_NEEDED            NO)
  set(BMQ_TARGET_MQB_NEEDED            NO)
  set(BMQ_TARGET_TUTORIAL_NEEDED       NO)
  set(BMQ_TARGET_PROMETHEUS_NEEDED     NO)

  bbproject_check_install_target("mwc"            installMWC)
  bbproject_check_install_target("bmq"            installBMQ)
  bbproject_check_install_target("mqb"            installMQB)
  bbproject_check_install_target("bmqbrkrcfg"     installBMQBRKRCFG)
  bbproject_check_install_target("bmqtool"        installBMQTOOL)
  bbproject_check_install_target("bmqstoragetool" installBMQSTORAGETOOL)
  bbproject_check_install_target("prometheus"     installPROMETHEUS)

  # NOTE: All targets should get 'mwc' from DPKG, except for the 'mwc' release
  #       itself (and the development work).
  if (installMWC)
    set(BMQ_TARGET_MWC_NEEDED YES)
  endif()

  if (installBMQ)
    set(BMQ_TARGET_BMQ_NEEDED YES)
  endif()

  if (installMQB)
    set(BMQ_TARGET_BMQ_NEEDED YES)
    set(BMQ_TARGET_MQB_NEEDED YES)
  endif()

  if (installBMQBRKR)
    set(BMQ_TARGET_BMQ_NEEDED       YES)
    set(BMQ_TARGET_MQB_NEEDED       YES)
    set(BMQ_TARGET_BMQBRKR_NEEDED   YES)
    set(BMQ_TARGET_E_BMQBRKR_NEEDED YES)
  endif()

  if (installBMQBRKRCFG)
    set(BMQ_TARGET_BMQBRKRCFG_NEEDED YES)
  endif()

  if (installBMQTOOL)
    set(BMQ_TARGET_BMQ_NEEDED     YES)
    set(BMQ_TARGET_MQB_NEEDED     YES)
    set(BMQ_TARGET_BMQTOOL_NEEDED YES)
  endif()

  if (installBMQSTORAGETOOL)
    set(BMQ_TARGET_BMQ_NEEDED            YES)
    set(BMQ_TARGET_MQB_NEEDED            YES)
    set(BMQ_TARGET_BMQSTORAGETOOL_NEEDED YES)
  endif()

  if (installPROMETHEUS)
    set(BMQ_TARGET_MWC_NEEDED        YES)
    set(BMQ_TARGET_BMQ_NEEDED        YES)
    set(BMQ_TARGET_MQB_NEEDED        YES)
    set(BMQ_TARGET_PROMETHEUS_NEEDED YES)
  endif()
endif()

# -----------------------------------------------------------------------------
#                                   PROJECTS
# -----------------------------------------------------------------------------
add_subdirectory( "src/groups" )
add_subdirectory( "src/applications" )
add_subdirectory( "src/tutorials" )
add_subdirectory( "src/plugins" )
