## This file should be placed in the root directory of your project.
## Then modify the CMakeLists.txt file in the root directory of your
## project to incorporate the testing dashboard.
##
## # The following are required to submit to the CDash dashboard:
##   ENABLE_TESTING()
##   INCLUDE(CTest)

set(CTEST_PROJECT_NAME "Wintermute")
set(CTEST_NIGHTLY_START_TIME "00:00:00 EST")
set(CTEST_DROP_METHOD "http")
set(CTEST_DROP_SITE "my.cdash.org")
set(CTEST_DROP_LOCATION "/submit.php?project=Wintermute")
set(CTEST_DROP_SITE_CDASH false)

# From this line down, this script may be customized
# on the Clients tab of the CDash createProject page.
#
if(JOB_MODULE)
  set(SOURCE_NAME ${JOB_MODULE})
  if(JOB_TAG)
    set(SOURCE_NAME ${SOURCE_NAME}-${JOB_TAG})
  endif()
else()
  set(SOURCE_NAME ${PROJECT_NAME})
  if(JOB_BUILDNAME_SUFFIX)
    set(SOURCE_NAME ${SOURCE_NAME}-${JOB_BUILDNAME_SUFFIX})
  endif()
endif()

set(CTEST_SOURCE_NAME ${SOURCE_NAME})
set(CTEST_BINARY_NAME ${SOURCE_NAME}-bin)
set(CTEST_DASHBOARD_ROOT "${CLIENT_BASE_DIRECTORY}")
set(CTEST_SOURCE_DIRECTORY "${CTEST_DASHBOARD_ROOT}/${CTEST_SOURCE_NAME}")
set(CTEST_BINARY_DIRECTORY "${CTEST_DASHBOARD_ROOT}/${CTEST_BINARY_NAME}")
set(CTEST_CMAKE_GENERATOR "${JOB_CMAKE_GENERATOR}")
set(CTEST_BUILD_CONFIGURATION "${JOB_BUILD_CONFIGURATION}")

set(CTEST_SITE "${CLIENT_SITE}")
set(CTEST_BUILD_NAME "${JOB_OS_NAME}-${JOB_OS_VERSION}-${JOB_OS_BITS}-${JOB_COMPILER_NAME}-${JOB_COMPILER_VERSION}")
if(JOB_BUILDNAME_SUFFIX)
  set(CTEST_BUILD_NAME ${CTEST_BUILD_NAME}-${JOB_BUILDNAME_SUFFIX})
endif()

if(NOT EXISTS "${CTEST_SOURCE_DIRECTORY}")
  set(CTEST_CHECKOUT_COMMAND "git clone ${JOB_REPOSITORY} ${SOURCE_NAME}")
endif()
set(CTEST_UPDATE_COMMAND "git")

file(WRITE "${CTEST_BINARY_DIRECTORY}/CMakeCache.txt" "${JOB_INITIAL_CACHE}")

ctest_start(${JOB_BUILDTYPE})
ctest_update(SOURCE ${CTEST_SOURCE_DIRECTORY})
ctest_configure(BUILD "${CTEST_BINARY_DIRECTORY}" RETURN_VALUE res)
ctest_build(BUILD "${CTEST_BINARY_DIRECTORY}" RETURN_VALUE res)
ctest_test(BUILD "${CTEST_BINARY_DIRECTORY}" RETURN_VALUE res)
# The following lines are used to associate a build id with this job.
set(CTEST_DROP_SITE ${JOB_DROP_SITE})
set(CTEST_DROP_LOCATION ${JOB_DROP_LOCATION})
ctest_submit(RETURN_VALUE res)

message("DONE")

