IF (CMAKE_COMPILER_IS_GNUCXX)


	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-long-long")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable")
	#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weffc++")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pipe")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
	#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -funit-at-a-time")
	#####SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
	# Too much is too much
	#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdisabled-optimization -Winline -Wredundant-decls -Wconversion -Wsign-compare")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wredundant-decls -Wconversion -Wsign-compare")

# RELEASE:

	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS}")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fomit-frame-pointer")

	#SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=i686")
	#SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=pentium4")
	#SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=athlon64")
	#SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mtune=pentium4")
	#SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mtune=i686")

	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfpmath=sse")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -msse")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -msse2")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mssse3")
	#SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -funroll-loops")

	#SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fvisibility=hidden -fvisibility-inlines-hidden")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fvisibility-inlines-hidden")

	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -D NDEBUG")
	SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -D NDEBUG")
# DEBUG:

	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS}")
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O3")
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

	# Enable this for profiling with gprof
	#SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pg")

	# ADD DEBUG FLAGS FOR EACH BUILD TYPE (IT CANNOT BE SET BY ADD_DEFINITIONS (BECAUSE OF MSVS))
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D DEBUG")
	SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D DEBUG")

ENDIF (CMAKE_COMPILER_IS_GNUCXX)

IF(MSVC)
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /arch:SSE")
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /arch:SSE2")
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /arch:SSSE3")
	#SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /fp:fast /Wp64")


	SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${CMAKE_C_FLAGS}")
	SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /O2")
	SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Ob2")
	SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Og")
	SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Ot")
	SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Oy")
	SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /G7")

	# This enables "whole program optimization, which leads to a huge delay when compiling in release mode",
	# thus enable it only on your local copy for the case you need it.
	##SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /GL")

	#SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Oi")
	#SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /GS-")
	#SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /GR-")
	#SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Wall")

	SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${CMAKE_C_FLAGS}")
	#SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /GR-")
	#SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /Wall")
#-- --
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE2")
	#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wp64")
	#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fp:fast")
	
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_CXX_FLAGS}")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ob2")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Og")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ot")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Oy")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /G7")

	# This enables "whole program optimization, which leads to a huge delay when compiling in release mode",
	# thus enable it only on your local copy for the case you need it.
	##SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /GL")

	#SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Oi")
	#SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /GS-")
	#SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /GR-")
	#SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Wall")

	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_CXX_FLAGS}")
	#SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /GR-")
	#SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Wall")
#-- --
	#SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG /IGNORE:4089")
	#SET(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /LTCG /IGNORE:4089")
	#SET(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS_RELEASE} /LTCG /IGNORE:4089")

	# add msvc definitions.
	ADD_DEFINITIONS(-D__SSE2__ -D__SSE3__ -D__SSE__ -D__MMX__)

	# disable deprecated warnings (most of them come from OpenCV)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
	ADD_DEFINITIONS(-D_CRT_NONSTDC_NO_WARNINGS)


	# ADD DEBUG FLAGS FOR EACH BUILD TYPE (IT CANNOT BE SET BY ADD_DEFINITIONS (BECAUSE OF MSVS))
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /D DEBUG")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /D NDEBUG")
	SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /D DEBUG")
	SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /D NDEBUG")

ENDIF(MSVC)
