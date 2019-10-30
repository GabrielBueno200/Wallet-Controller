#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=MinGW_1-Windows
CND_DLIB_EXT=dll
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/GTKAbas.o \
	${OBJECTDIR}/GTKformAbas.o \
	${OBJECTDIR}/GTKformUsuario.o \
	${OBJECTDIR}/funcsCadastro.o \
	${OBJECTDIR}/funcsCategorias.o \
	${OBJECTDIR}/funcsLogin.o \
	${OBJECTDIR}/funcsTransacoes.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dev_algo.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dev_algo.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dev_algo ${OBJECTFILES} ${LDLIBSOPTIONS} $$(pkg-config --cflags --libs gtk+-3.0)

${OBJECTDIR}/GTKAbas.o: GTKAbas.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../../../msys64/mingw64/include/gtk-3.0 -I../../../../../msys64/mingw64/include/glib-2.0 -I../../../../../msys64/mingw64/include/glib-2.0/glib -I../../../../../msys64/mingw64/include/pango-1.0 -I../../../../../msys64/mingw64/include/cairo -I../../../../../msys64/mingw64/include/gdk-pixbuf-2.0 -I../../../../../msys64/mingw64/include/atk-1.0 -I../../../../../msys64/mingw64/include/glib-2.0/gobject -I../../../../../msys64/mingw64/bin -I../../../../../msys64/usr/share/pkgconfig -I../../../../../msys64/mingw64/lib/pkgconfig -I../../../../../msys64/mingw64/lib/glib-2.0/include -std=c99 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/GTKAbas.o GTKAbas.c

${OBJECTDIR}/GTKformAbas.o: GTKformAbas.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../../../msys64/mingw64/include/gtk-3.0 -I../../../../../msys64/mingw64/include/glib-2.0 -I../../../../../msys64/mingw64/include/glib-2.0/glib -I../../../../../msys64/mingw64/include/pango-1.0 -I../../../../../msys64/mingw64/include/cairo -I../../../../../msys64/mingw64/include/gdk-pixbuf-2.0 -I../../../../../msys64/mingw64/include/atk-1.0 -I../../../../../msys64/mingw64/include/glib-2.0/gobject -I../../../../../msys64/mingw64/bin -I../../../../../msys64/usr/share/pkgconfig -I../../../../../msys64/mingw64/lib/pkgconfig -I../../../../../msys64/mingw64/lib/glib-2.0/include -std=c99 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/GTKformAbas.o GTKformAbas.c

${OBJECTDIR}/GTKformUsuario.o: GTKformUsuario.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../../../msys64/mingw64/include/gtk-3.0 -I../../../../../msys64/mingw64/include/glib-2.0 -I../../../../../msys64/mingw64/include/glib-2.0/glib -I../../../../../msys64/mingw64/include/pango-1.0 -I../../../../../msys64/mingw64/include/cairo -I../../../../../msys64/mingw64/include/gdk-pixbuf-2.0 -I../../../../../msys64/mingw64/include/atk-1.0 -I../../../../../msys64/mingw64/include/glib-2.0/gobject -I../../../../../msys64/mingw64/bin -I../../../../../msys64/usr/share/pkgconfig -I../../../../../msys64/mingw64/lib/pkgconfig -I../../../../../msys64/mingw64/lib/glib-2.0/include -std=c99 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/GTKformUsuario.o GTKformUsuario.c

${OBJECTDIR}/funcsCadastro.o: funcsCadastro.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../../../msys64/mingw64/include/gtk-3.0 -I../../../../../msys64/mingw64/include/glib-2.0 -I../../../../../msys64/mingw64/include/glib-2.0/glib -I../../../../../msys64/mingw64/include/pango-1.0 -I../../../../../msys64/mingw64/include/cairo -I../../../../../msys64/mingw64/include/gdk-pixbuf-2.0 -I../../../../../msys64/mingw64/include/atk-1.0 -I../../../../../msys64/mingw64/include/glib-2.0/gobject -I../../../../../msys64/mingw64/bin -I../../../../../msys64/usr/share/pkgconfig -I../../../../../msys64/mingw64/lib/pkgconfig -I../../../../../msys64/mingw64/lib/glib-2.0/include -std=c99 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/funcsCadastro.o funcsCadastro.c

${OBJECTDIR}/funcsCategorias.o: funcsCategorias.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../../../msys64/mingw64/include/gtk-3.0 -I../../../../../msys64/mingw64/include/glib-2.0 -I../../../../../msys64/mingw64/include/glib-2.0/glib -I../../../../../msys64/mingw64/include/pango-1.0 -I../../../../../msys64/mingw64/include/cairo -I../../../../../msys64/mingw64/include/gdk-pixbuf-2.0 -I../../../../../msys64/mingw64/include/atk-1.0 -I../../../../../msys64/mingw64/include/glib-2.0/gobject -I../../../../../msys64/mingw64/bin -I../../../../../msys64/usr/share/pkgconfig -I../../../../../msys64/mingw64/lib/pkgconfig -I../../../../../msys64/mingw64/lib/glib-2.0/include -std=c99 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/funcsCategorias.o funcsCategorias.c

${OBJECTDIR}/funcsLogin.o: funcsLogin.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../../../msys64/mingw64/include/gtk-3.0 -I../../../../../msys64/mingw64/include/glib-2.0 -I../../../../../msys64/mingw64/include/glib-2.0/glib -I../../../../../msys64/mingw64/include/pango-1.0 -I../../../../../msys64/mingw64/include/cairo -I../../../../../msys64/mingw64/include/gdk-pixbuf-2.0 -I../../../../../msys64/mingw64/include/atk-1.0 -I../../../../../msys64/mingw64/include/glib-2.0/gobject -I../../../../../msys64/mingw64/bin -I../../../../../msys64/usr/share/pkgconfig -I../../../../../msys64/mingw64/lib/pkgconfig -I../../../../../msys64/mingw64/lib/glib-2.0/include -std=c99 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/funcsLogin.o funcsLogin.c

${OBJECTDIR}/funcsTransacoes.o: funcsTransacoes.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../../../msys64/mingw64/include/gtk-3.0 -I../../../../../msys64/mingw64/include/glib-2.0 -I../../../../../msys64/mingw64/include/glib-2.0/glib -I../../../../../msys64/mingw64/include/pango-1.0 -I../../../../../msys64/mingw64/include/cairo -I../../../../../msys64/mingw64/include/gdk-pixbuf-2.0 -I../../../../../msys64/mingw64/include/atk-1.0 -I../../../../../msys64/mingw64/include/glib-2.0/gobject -I../../../../../msys64/mingw64/bin -I../../../../../msys64/usr/share/pkgconfig -I../../../../../msys64/mingw64/lib/pkgconfig -I../../../../../msys64/mingw64/lib/glib-2.0/include -std=c99 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/funcsTransacoes.o funcsTransacoes.c

${OBJECTDIR}/main.o: main.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -I../../../../../msys64/mingw64/include/gtk-3.0 -I../../../../../msys64/mingw64/include/glib-2.0 -I../../../../../msys64/mingw64/include/glib-2.0/glib -I../../../../../msys64/mingw64/include/pango-1.0 -I../../../../../msys64/mingw64/include/cairo -I../../../../../msys64/mingw64/include/gdk-pixbuf-2.0 -I../../../../../msys64/mingw64/include/atk-1.0 -I../../../../../msys64/mingw64/include/glib-2.0/gobject -I../../../../../msys64/mingw64/bin -I../../../../../msys64/usr/share/pkgconfig -I../../../../../msys64/mingw64/lib/pkgconfig -I../../../../../msys64/mingw64/lib/glib-2.0/include -std=c99 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
