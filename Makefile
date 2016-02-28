COMPILER = g++
CFLAGS   = -std=c++14 -fno-exceptions -fno-rtti
LDFLAGS  = -lpthread 

LIBS     =
INCLUDE  = -Ishogi -Ishogi/util -Ishogi/engine -Ishogi/shogilib -Ishogi/shogilib/Notation

ifndef debug
debug = yes
endif

ifndef test
test = no
endif

ifeq ($(debug),no)
else
	CFLAGS += -D_DEBUG
endif


ifeq ($(test), yes)
ifeq ($(OS),Windows_NT)
  EXE    = shogi-test.exe
  LDFLAGS += -static
else
  EXE = shogi-test.out
endif
  TARGET = test/$(EXE)

OBJDIR   = test

else
ifeq ($(OS),Windows_NT)
  TARGET = shogi.exe
  LDFLAGS += -static
else
  TARGET = shogi.out
endif

OBJDIR   = obj
endif


SOURCES  =  shogi/Game.cpp shogi/EnginePlayer.cpp shogi/GameTimer.cpp shogi/Logger.cpp shogi/PvInfo.cpp \
			shogi/util/KeyValue.cpp shogi/engine/CProcess.cpp shogi/engine/Path.cpp shogi/engine/PipeStream.cpp \
			shogi/engine/StringQueue.cpp shogi/engine/USIEngine.cpp shogi/engine/USIOption.cpp shogi/engine/USIString.cpp \
			shogi/shogilib/Handicap.cpp shogi/shogilib/History.cpp shogi/shogilib/Key.cpp shogi/shogilib/Move.cpp \
			shogi/shogilib/MovesEnumerator.cpp shogi/shogilib/Notation.cpp shogi/shogilib/Position.cpp shogi/shogilib/StringUtil.cpp \
			shogi/shogilib/Tokenizer.cpp shogi/shogilib/MoveCheck.cpp \
			shogi/shogilib/Notation/Kif.cpp shogi/shogilib/Notation/Sfen.cpp

ifeq ($(test), no)
SOURCES += shogi/shogi.cpp
else

SOURCES += shogi-test/Test.cpp shogi-test/EnginePlayerTest.cpp shogi-test/MessageQueueTest.cpp \
			shogi-test/ProcessTest.cpp shogi-test/SemaphoreTest.cpp shogi-test/StringQueueTest.cpp shogi-test/USIEngineTest.cpp \
			shogi-test/USIOptionTest.cpp shogi-test/USIStringTest.cpp \
			shogi-test/ColorTest.cpp shogi-test/FileTest.cpp shogi-test/HandicapTest.cpp shogi-test/KeyTest.cpp shogi-test/MoveCheckTest.cpp \
			shogi-test/MoveTest.cpp shogi-test/NotationTest.cpp shogi-test/PieceTest.cpp shogi-test/PositionTest.cpp \
			shogi-test/RankTest.cpp shogi-test/SfenTest.cpp shogi-test/SquareTest.cpp shogi-test/StringUtilTest.cpp \
			shogi-test/DateTimeTest.cpp shogi-test/KeyValueTest.cpp shogi-test/TimerTest.cpp \
			shogi-test/PathTest.cpp shogi-test/HistoryTest.cpp shogi-test/KifTest.cpp

ifeq ($(OS),Windows_NT)
LIBS	+=  external/googletest/lib/gtest.a external/googletest/lib/gtest_main.a
else
LIBS	+=  external/googletest/lib/libgtest.a external/googletest/lib/libgtest_main.a
endif

INCLUDE  += -Iexternal/googletest/include
endif


OBJECTS  = $(addprefix $(OBJDIR)/, $(SOURCES:.cpp=.o))
DEPENDS  = $(OBJECTS:.o=.d)


all: $(TARGET)
	
$(TARGET): $(OBJECTS) $(LIBS)
	$(COMPILER) -o $@ $^ $(LDFLAGS) $(CFLAGS)

$(OBJDIR)/%.o: %.cpp
	@[ -d $(dir $@) ] || mkdir -p $(dir $@)
	$(COMPILER) $(CFLAGS) $(INCLUDE) -o $@ -c $<

.PHONY: release debug test

release:
	$(MAKE) debug=no all

debug:
	$(MAKE) debug=yes all

test:
	$(MAKE) debug=yes test=yes all

test-run:
	cd Test
	$(EXE)
	cd ..

test-child:
	g++ tchild/child.cpp -o test/tchild.out

hiyoko:
	$(EXE) Debug/hiyoko.exe Debug/hiyoko.exe -T 0 0.2 -G 2

clean:
	rm -f $(OBJECTS) $(DEPENDS) $(TARGET) ${OBJECTS:.o=.gcda}

-include $(DEPENDS)
