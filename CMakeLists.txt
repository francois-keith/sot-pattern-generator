# Copyright 2010, François Bleibel, Olivier Stasse, JRL, CNRS/AIST
#
# This file is part of sot-pattern-generator.
# sot-pattern-generator is free software: you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public License
# as published by the Free Software Foundation, either version 3 of
# the License, or (at your option) any later version.
#
# sot-pattern-generator is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Lesser Public License for more details.  You should have
# received a copy of the GNU Lesser General Public License along with
# sot-pattern-generator. If not, see <http://www.gnu.org/licenses/>.

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

INCLUDE(cmake/base.cmake)
INCLUDE(cmake/boost.cmake)
INCLUDE(cmake/lapack.cmake)
INCLUDE(cmake/cpack.cmake)

SET(PROJECT_NAME sot-pattern-generator)
SET(PROJECT_DESCRIPTION "jrl-walkgen bindings for dynamic-graph.")
SET(PROJECT_URL "https://github.com/jrl-umi3218/sot-pattern-generator")

SET(CUSTOM_HEADER_DIR "${PROJECT_NAME}")

# Disable -Werror on Unix for now.
SET(CXX_DISABLE_WERROR True)

SET(PKG_CONFIG_ADDITIONAL_VARIABLES
  ${PKG_CONFIG_ADDITIONAL_VARIABLES}
  plugindirname
  plugindir
  )

SETUP_PROJECT()

# Search for dependencies.
ADD_REQUIRED_DEPENDENCY("jrl-walkgen >= 3.0.0")
ADD_REQUIRED_DEPENDENCY("jrl-dynamics >= 1.16.1")
ADD_REQUIRED_DEPENDENCY("jrl-dynamics-urdf")

ADD_REQUIRED_DEPENDENCY("dynamic-graph >= 1.0.0")
ADD_REQUIRED_DEPENDENCY("sot-core >= 1.0")

# Search for closed-source packages.
ADD_OPTIONAL_DEPENDENCY("hrp2-dynamics >= 1.3.0")
ADD_OPTIONAL_DEPENDENCY("hrp2-10-optimized >= 1.0")

ADD_OPTIONAL_DEPENDENCY("hrp2-10 >= 1.0.0")

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(python)
ADD_SUBDIRECTORY(unitTesting)

SETUP_PROJECT_FINALIZE()
SETUP_PROJECT_CPACK()
