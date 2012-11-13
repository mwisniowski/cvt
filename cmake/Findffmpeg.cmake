# On success this script defines
# FFMPEG_INCLUDES	locations of avformat.h
# FFMPEG_LIBRARIES	needed ffmpeg libraries 
# FFMPEG_FOUND		whether the library has been found or not 


FIND_PATH(AVFORMAT_INCLUDE_DIR libavformat/avformat.h
		  ${CMAKE_INCLUDE_PATH}/
		  /usr/include/
		  /usr/local/include/
		  /opt/local/include/
)

FIND_PATH(AVCODEC_INCLUDE_DIR  libavcodec/avcodec.h
		  ${CMAKE_INCLUDE_PATH}
		  /usr/include/
		  /usr/local/include/
		  /opt/local/include/
)

FIND_PATH(AVUTIL_INCLUDE_DIR  libavutil/avutil.h
		  ${CMAKE_INCLUDE_PATH}
		  /usr/include/
		  /usr/local/include/
		  /opt/local/include/
)

FIND_LIBRARY(AVFORMAT_LIB
	NAMES
	avformat
	PATHS
	${CMAKE_LIBRARY_PATH}
	/usr/lib
	/usr/local/lib
	/opt/local/lib
)

FIND_LIBRARY(AVCODEC_LIB
	NAMES
	avcodec
	PATHS
	${CMAKE_LIBRARY_PATH}
	/usr/lib
	/usr/local/lib
	/opt/local/lib
)

FIND_LIBRARY(AVUTIL_LIB
	NAMES
	avutil
	PATHS
	${CMAKE_LIBRARY_PATH}
	/usr/lib
	/usr/local/lib
	/opt/local/lib
)

IF(AVFORMAT_LIB AND AVCODEC_LIB AND AVUTIL_LIB)
	SET(FFMPEG_FOUND TRUE)
ENDIF()


SET(FFMPEG_INCLUDES
	${AVFORMAT_INCLUDE_DIR}
	${AVCODEC_INCLUDE_DIR}
	${AVUTIL_INCLUDE_DIR}
)

SET(FFMPEG_LIBRARIES
	${AVFORMAT_LIB}
	${AVCODEC_LIB}
	${AVUTIL_LIB}
)

MARK_AS_ADVANCED(
	FFMPEG_FOUND
	FFMPEG_INCLUDES
	FFMPEG_LIBRARIES
) 
