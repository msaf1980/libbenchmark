if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    get_linux_os_release()
    split_linux_lsb_version()
    #message(STATUS "Linux is ${LSB_RELEASE_ID} ${LSB_RELEASE_VERSION_MAJOR} '${LSB_RELEASE_CODENAME}'")
    get_linux_family()

    if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
        if("${LSB_FAMILY}" STREQUAL "redhat")
            set( CMAKE_INSTALL_LIBDIR "lib64" )
            set(CPACK_GENERATOR "RPM")
        elseif("${LSB_FAMILY}" STREQUAL "debian")
            set(CPACK_GENERATOR "DEB")
            set( CMAKE_INSTALL_LIBDIR "lib/x86_64-linux-gnu" )
        endif()
    endif()

    string( TOLOWER "${LSB_RELEASE_ID}" LSB_RELEASE_ID_LOWER )
    if ("${LSB_RELEASE_ID_LOWER}" MATCHES "fedora")
        set( CPACK_OS_RELEASE "fc${LSB_RELEASE_VERSION_MAJOR}" )
    elseif ("${LSB_RELEASE_ID_LOWER}" MATCHES "centos" OR "${LSB_RELEASE_ID_LOWER}" MATCHES "ol" OR "${LSB_RELEASE_ID_LOWER}" MATCHES "rhel" )
        set( CPACK_OS_RELEASE "el${LSB_RELEASE_VERSION_MAJOR}" )
    elseif ("${LSB_RELEASE_ID_LOWER}" MATCHES "debian")
        set( CPACK_OS_RELEASE "deb${LSB_RELEASE_VERSION_MAJOR}" )
    elseif ("${LSB_RELEASE_ID_LOWER}" MATCHES "ubuntu")
        set( CPACK_OS_RELEASE "ubuntu${LSB_RELEASE_VERSION_MAJOR}" )
    endif()
endif()