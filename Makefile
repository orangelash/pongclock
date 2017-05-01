#All purpose makefile

# Linux (default)
EXE = main
LDFLAGS = -lGL -lGLU -lglut -I"./audio/include/" ./audio/bin/linux-gcc-64/libIrrKlang.so -pthread#-lm math

# Windows (cygwin)
ifeq "$(OS)" "Windows_NT"
EXE = main.exe
LDFLAGS = -lopengl32 -lglu32 -lglut32
endif

# OS X
ifeq "$(OSTYPE)" "darwin"
LDFLAGS = -framework Carbon -framework OpenGL -framework GLUT
endif

$(EXE) : main.cpp;gcc -o $@ $< $(CFLAGS) $(LDFLAGS)