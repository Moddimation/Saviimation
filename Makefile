#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC")
endif

include $(DEVKITPPC)/wii_rules

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
TARGET		:=	Saviimation
BUILD		:=	build
OUT      :=  dest
SOURCES		:=	src src/menu src/menu/test
DATA		:=	data
INCLUDES	:=
DEBUG := 1

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------

DEFINES =  -include error.h -DDEF_APP_NAME=\"$(TARGET)\"

ifneq ($(strip $(DEVKITPPC)),)
DEFINES += -DDEBUG=1
endif

CFLAGS	= -Ofast -Wall $(MACHDEP) $(INCLUDE) $(DEFINES)
CXXFLAGS	=	$(CFLAGS)

LDFLAGS	=	$(MACHDEP) -Wl,-Map,$(notdir $@).map

ifneq ($(strip $(DEVKITPPC)),)
CFLAGS += -g
CXXFLAGS += -g
LDFLAGS += -g
endif

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS	:=	-lwiiuse -lbte -logc -lm

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(DATA),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

#---------------------------------------------------------------------------------
# automatically build a list of object files for our project
#---------------------------------------------------------------------------------
CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
sFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.S)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
	export LD	:=	$(CC)
else
	export LD	:=	$(CXX)
endif

export OFILES_BIN	:=	$(addsuffix .o,$(BINFILES))
export OFILES_SOURCES := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(sFILES:.s=.o) $(SFILES:.S=.o)
export OFILES := $(OFILES_BIN) $(OFILES_SOURCES)

export HFILES := $(addsuffix .h,$(subst .,_,$(BINFILES)))

#---------------------------------------------------------------------------------
# build a list of include paths
#---------------------------------------------------------------------------------
export INCLUDE	:=	$(foreach dir,$(INCLUDES), -iquote $(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD) \
					-I$(LIBOGC_INC)\
					$(foreach dir,$(SOURCES),-I$(CURDIR)/$(dir))

#---------------------------------------------------------------------------------
# build a list of library paths
#---------------------------------------------------------------------------------
export LIBPATHS	:= -L$(LIBOGC_LIB) $(foreach dir,$(LIBDIRS),-L$(dir)/lib)

export OUTPUT	:=	$(CURDIR)/$(TARGET)
.PHONY: $(BUILD) clean

#---------------------------------------------------------------------------------
$(BUILD): init_compile_commands
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(OUT)/main.dol $(OUTPUT).elf $(OUTPUT).dol

#---------------------------------------------------------------------------------
run:
	wiiload $(TARGET).dol


COMPILE_COMMANDS_FILE := compile_commands.json

# Clean previous commands file
init_compile_commands:
	@if [ -n "$(NEW)" ]; then \
		echo '[' > $(COMPILE_COMMANDS_FILE); \
	fi

#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT).dol: $(OUTPUT).elf fix_compile_commands
$(OUTPUT).elf: $(OFILES)

$(OFILES_SOURCES) : $(HFILES)

#---------------------------------------------------------------------------------
# This rule links in binary data with the .jpg extension
#---------------------------------------------------------------------------------
%.jpg.o	%_jpg.h :	%.jpg
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	$(bin2o)

-include $(DEPENDS)

#---------------------------------------------------------------------------------
COMPILE_COMMANDS_FILE := ../compile_commands.json

# Clean previous commands file
init_compile_commands:
	@if [ -n "$(NEW)" ]; then \
		echo '[' > $(COMPILE_COMMANDS_FILE); \
	fi

define ADD_COMPILE_COMMAND
@if [ -n "$(NEW)" ]; then \
  echo '{' >> $(COMPILE_COMMANDS_FILE); \
  echo '  "directory": "$(CURDIR)",' >> $(COMPILE_COMMANDS_FILE); \
  echo '  "command": "$(CC) $(INCLUDE) $(DEFINES) -I$(DEVKITPPC)/powerpc-eabi/include -I$(DEVKITPRO)/libogc/include -c $< -o $@",' >> $(COMPILE_COMMANDS_FILE); \
  echo '  "file": "$<"' >> $(COMPILE_COMMANDS_FILE); \
  echo '},' >> $(COMPILE_COMMANDS_FILE); \
fi
endef

%.o: %.c
	@echo "   CC   $(notdir $<)"
	@$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d $(CFLAGS) -c $< -o $@ $(ERROR_FILTER)
	@$(call ADD_COMPILE_COMMAND)

%.elf:
	@echo "   LINK"
	@$(LD)  $^ $(LDFLAGS) $(LIBPATHS) $(LIBS) -o $@

%.dol: %.elf
	@echo "   OUT"
	@elf2dol $< $@
	@rm "$<"
	@mkdir -p "../$(OUT)"
	@mv "$@" "../$(OUT)/main.dol"

fix_compile_commands:
	@if [ -n "$(NEW)" ]; then \
		sed -i '$$ s/},/}/' $(COMPILE_COMMANDS_FILE); \
		echo ']' >> $(COMPILE_COMMANDS_FILE); \
	fi



#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------
