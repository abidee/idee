CXXFLAGS =	-O2 -g -Wall -fmessage-length=0
INC = 	-I./
LIBPATH =	
LIBS =	-lpthread
OUT = lib
OBJS =	$(OUT)/Mutex.o $(OUT)/Guard.o $(OUT)/Semaphore.o $(OUT)/Posix_Sync_Func.o $(OUT)/Posix_Thread_Func.o

TARGET =	idee.so

$(TARGET):	$(OBJS)
	$(CXX) -shared $(CXXFLAGS) -o $(OUT)/$(TARGET) $(OBJS) $(LIBS) -Wl,--out-implib=$(OUT)/libidee.a

all:	$(TARGET)

#create lib use ar
#XQLib.a : $(OBJS) 
#	ar -rv XQLib.a $?

#change default rule enable g++ trace into subdirectory	
#.cpp.o:
#	$(CXX) $(CXXFLAGS) -c $(@D)/$(<F) -o $(@D)/$(@F)

$(OUT)/Posix_Sync_Func.o	:	posix/Posix_Sync_Func.h 
	$(CXX) $(CXXFLAGS) -c posix/Posix_Sync_Func.cpp -o $(OUT)/Posix_Sync_Func.o
$(OUT)/Posix_Thread_Func.o	:	posix/Posix_Thread_Func.h 
	$(CXX) $(CXXFLAGS) -c posix/Posix_Thread_Func.cpp -o $(OUT)/Posix_Thread_Func.o
$(OUT)/Mutex.o	:	Mutex.cpp Mutex.h 
	$(CXX) $(CXXFLAGS) -c Mutex.cpp -o $(OUT)/Mutex.o
$(OUT)/Guard.o :	Guard.h	
	$(CXX) $(CXXFLAGS) -c Guard.cpp -o $(OUT)/Guard.o
$(OUT)/Semaphore.o:	Semaphore.h
	$(CXX) $(CXXFLAGS) -c Semaphore.cpp -o $(OUT)/Semaphore.o
	
clean:
	rm -f $(OBJS) $(OUT)/$(TARGET)
