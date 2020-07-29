#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-recording.mk)" "nbproject/Makefile-local-recording.mk"
include nbproject/Makefile-local-recording.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=recording
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/pArm.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/pArm.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS
SUB_IMAGE_ADDRESS_COMMAND=--image-address $(SUB_IMAGE_ADDRESS)
else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=main.c init.c motor/servo.c motor/motor.c interrupts.c utils/time.c utils/uart.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/main.o ${OBJECTDIR}/init.o ${OBJECTDIR}/motor/servo.o ${OBJECTDIR}/motor/motor.o ${OBJECTDIR}/interrupts.o ${OBJECTDIR}/utils/time.o ${OBJECTDIR}/utils/uart.o
POSSIBLE_DEPFILES=${OBJECTDIR}/main.o.d ${OBJECTDIR}/init.o.d ${OBJECTDIR}/motor/servo.o.d ${OBJECTDIR}/motor/motor.o.d ${OBJECTDIR}/interrupts.o.d ${OBJECTDIR}/utils/time.o.d ${OBJECTDIR}/utils/uart.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/main.o ${OBJECTDIR}/init.o ${OBJECTDIR}/motor/servo.o ${OBJECTDIR}/motor/motor.o ${OBJECTDIR}/interrupts.o ${OBJECTDIR}/utils/time.o ${OBJECTDIR}/utils/uart.o

# Source Files
SOURCEFILES=main.c init.c motor/servo.c motor/motor.c interrupts.c utils/time.c utils/uart.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-recording.mk dist/${CND_CONF}/${IMAGE_TYPE}/pArm.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33EP512GM604
MP_LINKER_FILE_OPTION=,--script=p33EP512GM604.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -fast-math -DXPRJ_recording=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -fno-short-double -O2 -funroll-loops -falign-arrays -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/init.o: init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/init.o.d 
	@${RM} ${OBJECTDIR}/init.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  init.c  -o ${OBJECTDIR}/init.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/init.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -fast-math -DXPRJ_recording=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -fno-short-double -O2 -funroll-loops -falign-arrays -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/init.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/motor/servo.o: motor/servo.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/motor" 
	@${RM} ${OBJECTDIR}/motor/servo.o.d 
	@${RM} ${OBJECTDIR}/motor/servo.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  motor/servo.c  -o ${OBJECTDIR}/motor/servo.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/motor/servo.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -fast-math -DXPRJ_recording=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -fno-short-double -O2 -funroll-loops -falign-arrays -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/motor/servo.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/motor/motor.o: motor/motor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/motor" 
	@${RM} ${OBJECTDIR}/motor/motor.o.d 
	@${RM} ${OBJECTDIR}/motor/motor.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  motor/motor.c  -o ${OBJECTDIR}/motor/motor.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/motor/motor.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -fast-math -DXPRJ_recording=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -fno-short-double -O2 -funroll-loops -falign-arrays -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/motor/motor.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/interrupts.o: interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/interrupts.o.d 
	@${RM} ${OBJECTDIR}/interrupts.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  interrupts.c  -o ${OBJECTDIR}/interrupts.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/interrupts.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -fast-math -DXPRJ_recording=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -fno-short-double -O2 -funroll-loops -falign-arrays -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/interrupts.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/utils/time.o: utils/time.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/utils" 
	@${RM} ${OBJECTDIR}/utils/time.o.d 
	@${RM} ${OBJECTDIR}/utils/time.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  utils/time.c  -o ${OBJECTDIR}/utils/time.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/utils/time.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -fast-math -DXPRJ_recording=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -fno-short-double -O2 -funroll-loops -falign-arrays -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/utils/time.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/utils/uart.o: utils/uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/utils" 
	@${RM} ${OBJECTDIR}/utils/uart.o.d 
	@${RM} ${OBJECTDIR}/utils/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  utils/uart.c  -o ${OBJECTDIR}/utils/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/utils/uart.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -fast-math -DXPRJ_recording=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -fno-short-double -O2 -funroll-loops -falign-arrays -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/utils/uart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d"      -mno-eds-warn  -g -omf=elf -fast-math -DXPRJ_recording=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -fno-short-double -O2 -funroll-loops -falign-arrays -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/init.o: init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/init.o.d 
	@${RM} ${OBJECTDIR}/init.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  init.c  -o ${OBJECTDIR}/init.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/init.o.d"      -mno-eds-warn  -g -omf=elf -fast-math -DXPRJ_recording=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -fno-short-double -O2 -funroll-loops -falign-arrays -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/init.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/motor/servo.o: motor/servo.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/motor" 
	@${RM} ${OBJECTDIR}/motor/servo.o.d 
	@${RM} ${OBJECTDIR}/motor/servo.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  motor/servo.c  -o ${OBJECTDIR}/motor/servo.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/motor/servo.o.d"      -mno-eds-warn  -g -omf=elf -fast-math -DXPRJ_recording=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -fno-short-double -O2 -funroll-loops -falign-arrays -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/motor/servo.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/motor/motor.o: motor/motor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/motor" 
	@${RM} ${OBJECTDIR}/motor/motor.o.d 
	@${RM} ${OBJECTDIR}/motor/motor.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  motor/motor.c  -o ${OBJECTDIR}/motor/motor.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/motor/motor.o.d"      -mno-eds-warn  -g -omf=elf -fast-math -DXPRJ_recording=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -fno-short-double -O2 -funroll-loops -falign-arrays -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/motor/motor.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/interrupts.o: interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/interrupts.o.d 
	@${RM} ${OBJECTDIR}/interrupts.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  interrupts.c  -o ${OBJECTDIR}/interrupts.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/interrupts.o.d"      -mno-eds-warn  -g -omf=elf -fast-math -DXPRJ_recording=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -fno-short-double -O2 -funroll-loops -falign-arrays -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/interrupts.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/utils/time.o: utils/time.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/utils" 
	@${RM} ${OBJECTDIR}/utils/time.o.d 
	@${RM} ${OBJECTDIR}/utils/time.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  utils/time.c  -o ${OBJECTDIR}/utils/time.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/utils/time.o.d"      -mno-eds-warn  -g -omf=elf -fast-math -DXPRJ_recording=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -fno-short-double -O2 -funroll-loops -falign-arrays -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/utils/time.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/utils/uart.o: utils/uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/utils" 
	@${RM} ${OBJECTDIR}/utils/uart.o.d 
	@${RM} ${OBJECTDIR}/utils/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  utils/uart.c  -o ${OBJECTDIR}/utils/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/utils/uart.o.d"      -mno-eds-warn  -g -omf=elf -fast-math -DXPRJ_recording=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -fno-short-double -O2 -funroll-loops -falign-arrays -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	@${FIXDEPS} "${OBJECTDIR}/utils/uart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/pArm.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/pArm.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -fast-math -DXPRJ_recording=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99  -mreserve=data@0x1000:0x101B -mreserve=data@0x101C:0x101D -mreserve=data@0x101E:0x101F -mreserve=data@0x1020:0x1021 -mreserve=data@0x1022:0x1023 -mreserve=data@0x1024:0x1027 -mreserve=data@0x1028:0x104F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/pArm.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/pArm.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -fast-math -DXPRJ_recording=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -std=gnu99 -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}/xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/pArm.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp="${DFP_DIR}/xc16" 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/recording
	${RM} -r dist/recording

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
