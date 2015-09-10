file(GLOB SRCS *.cpp)

ADD_EXECUTABLE(testmain ${SRCS})

TARGET_LINK_LIBRARIES(testmain
    libgtest
    libgmock
)

add_test(NAME testmain
         COMMAND testmain)
