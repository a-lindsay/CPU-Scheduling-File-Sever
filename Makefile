CC	= g++
LDFLAGS	= -lm -lnsl -lpthread
CFLAGS	= -ggdb
TARGET	= server client mserver

default: $(TARGET)

mserver:
	g++ $(CFLAGS) -o mserver mserver.cpp

server:
	g++ $(CFLAGS) -o server server.cpp sem.cpp FileGet.cpp commandLineCheck.cpp dispatch.cpp scheduler_factory.cpp task.cpp scheduler.cpp scheduler_FIFO.cpp scheduler_RANDOM.cpp scheduler_SJF.cpp header.cpp logger.cpp $(LDFLAGS)

client:
	g++ $(CFLAGS) -o client client.cpp commandLineCheck.cpp lineParser.cpp request.cpp getLine.cpp excessParser.cpp $(LDFLAGS)

clean:
	-rm -f *.o *~

cleanall: clean
	-rm -f $(TARGET) 


