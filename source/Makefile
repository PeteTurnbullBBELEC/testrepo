CC = arm-linux-gcc
CFLAGS = -O2 -Wall -W -Wmissing-declarations -Wshadow
CPPFLAGS = -I./MQTTClient -I../include
LDFLAGS = -s -L./MQTTClient -L../lib
LDLIBS = -lcommon -ldl -lssl -lcrypto -lpthread -lprotobuf-c

MODULE_EXE = module
MODULE_OBJ = module.o edcpayload.pb-c.o
MQTT_OBJ = MQTTClient/Clients.o MQTTClient/Heap.o MQTTClient/LinkedList.o MQTTClient/Log.o MQTTClient/Messages.o MQTTClient/MQTTClient.o MQTTClient/MQTTPacket.o MQTTClient/MQTTPacketOut.o MQTTClient/MQTTPersistence.o MQTTClient/MQTTPersistenceDefault.o MQTTClient/MQTTProtocolClient.o MQTTClient/MQTTProtocolOut.o MQTTClient/SocketBuffer.o MQTTClient/Socket.o MQTTClient/SSLSocket.o MQTTClient/StackTrace.o MQTTClient/Thread.o MQTTClient/Tree.o MQTTClient/utf-8.o

all: $(MODULE_EXE)

$(MODULE_EXE): $(MODULE_OBJ) $(MQTT_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f $(MODULE_EXE) *.o
	rm -f MQTTClient/*.o

install:
