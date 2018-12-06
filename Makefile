#Compiler and Linker
CC          := g++

#The Target Binary Program
TARGET      := treebench

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR      := src
INCDIR      := inc
BUILDDIR    := build
TARGETDIR   := bin
SRCEXT      := cc
DEPEXT      := d
OBJEXT      := o

VERSION			:= 1.0
PKG         := treebench

INSTDIR = /usr/local/bin/$(EXE)
CONFDIR = ~/$(TARGET)

#Flags, Libraries and Includes

#PROFILING
#CFLAGS      := -Wall -O0 -pg -ggdb -c
#LFLAGS      := -pg
#DEBUGGING
CFLAGS      := -std=c++14 -Wall -O0 -ggdb -c -finstrument-functions
#OPTIMIZED
#CFLAGS      := -std=c++14 -Wall -O3 -c
CFLAGS 		+= $(CURL_CFLAGS)

LIB 				:=
INC         := -I$(INCDIR) -I/usr/local/include
INCDEP      := -I$(INCDIR)

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
SOURCES     := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS     := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

#Defauilt Make
all: directories $(TARGET)

#Remake
remake: cleaner all

#Make the Directories
directories:
		@mkdir -p $(TARGETDIR)
		@mkdir -p $(BUILDDIR)

package: all
	@echo " Building a package"
	mkdir -p "pkg/$(PKG)/$(PKG)-$(VERSION)/etc/$(PKG)"
	mkdir -p "pkg/$(PKG)/$(PKG)-$(VERSION)/usr/bin/"
	@cp -rf $(RESDIR)/* "pkg/$(PKG)/$(PKG)-$(VERSION)/etc/$(PKG)"
	@cp $(TARGETDIR)/$(TARGET) "pkg/$(PKG)/$(PKG)-$(VERSION)/usr/bin/$(TARGET)"
	chown -R root:root "pkg/$(PKG)"
	dpkg --build "pkg/$(PKG)/$(PKG)-$(VERSION)"
	mv "pkg/$(PKG)/$(PKG)-$(VERSION).deb" .

install: all
	@echo " o Creating install directory '$(INSTDIR)'"
	mkdir -p "$(INSTDIR)"
	chmod 777 "$(INSTDIR)"

#Clean only Objecst
clean:
		@$(RM) -rf $(BUILDDIR)
		@$(RM) -rf $(TARGETDIR)
		mkdir -p $(TARGETDIR)

#Full Clean, Objects and Binaries
cleaner: clean
		@$(RM) -rf $(TARGETDIR)
		@$(RM) $(TARGET)-$(VERSION).deb

#Pull in dependency info for *existing* .o files
-include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))

#Link

$(TARGET): $(OBJECTS)
		$(CC) $(LFLAGS) -o $(TARGETDIR)/$(TARGET) $^ $(LIB)

#Compile
$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
		@mkdir -p $(dir $@)
		$(CC) $(CFLAGS) $(INC) -c -o $@ $<
		@$(CC) $(CFLAGS) $(INCDEP) -MM $(SRCDIR)/$*.$(SRCEXT) > $(BUILDDIR)/$*.$(DEPEXT)
		@cp -f $(BUILDDIR)/$*.$(DEPEXT) $(BUILDDIR)/$*.$(DEPEXT).tmp
		@sed -e 's|.*:|$(BUILDDIR)/$*.$(OBJEXT):|' < $(BUILDDIR)/$*.$(DEPEXT).tmp > $(BUILDDIR)/$*.$(DEPEXT)
		@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/$*.$(DEPEXT)
		@rm -f $(BUILDDIR)/$*.$(DEPEXT).tmp

#Non-File Targets
.PHONY: all remake clean cleaner

