#SSL_LIBS=-lssl -lcrypto
#SSL_FLAGS=-DWITH_OPENSSL

SSL_LIBS=
SSL_FLAGS=


all: soapC.c client server

soapC.c:
	soapcpp2 -b -c blackJack.h

client:
	gcc -g $(SSL_FLAGS) -w -o client client.c soapC.c soapClient.c game.c -lgsoap $(SSL_LIBS) -L$(GSOAP_LIB) -I$(GSOAP_INCLUDE)

server:	
	gcc -g $(SSL_FLAGS) -w -o server server.c soapC.c soapServer.c game.c -lgsoap $(SSL_LIBS) -L$(GSOAP_LIB) -I$(GSOAP_INCLUDE) -lpthread


clean:	
	rm client server *.xml *.nsmap *.wsdl *.xsd soapStub.h soapServerLib.* soapH.h soapServer.* soapClientLib.* soapClient.* soapC.*
