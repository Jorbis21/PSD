/* soapStub.h
   Generated by gSOAP 2.8.130 for blackJack.h

gSOAP XML Web services tools
Copyright (C) 2000-2023, Robert van Engelen, Genivia Inc. All Rights Reserved.
The soapcpp2 tool and its generated software are released under the GPL.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

#define ERROR_NAME_REPEATED -1
#define ERROR_SERVER_FULL -2
#define ERROR_PLAYER_NOT_FOUND -3
#define PLAYER_STAND 0
#define PLAYER_HIT_CARD 1
#define TURN_PLAY 2
#define TURN_WAIT 3
#define GAME_WIN 4
#define GAME_LOSE 5
#define DECK_SIZE 52
#define SUIT_SIZE 13
#define MAX_BET 5
#define TRUE 1
#define FALSE 0
#define STRING_LENGTH 256

#ifndef soapStub_H
#define soapStub_H
#include "stdsoap2.h"
#if GSOAP_VERSION != 208130
# error "GSOAP VERSION 208130 MISMATCH IN GENERATED CODE VERSUS LIBRARY CODE: PLEASE REINSTALL PACKAGE"
#endif


/******************************************************************************\
 *                                                                            *
 * Types with Custom Serializers                                              *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Structs and Unions                                                         *
 *                                                                            *
\******************************************************************************/

struct tMessage;	/* blackJack.h:57 */
struct tDeck;	/* blackJack.h:63 */
struct tBlock;	/* blackJack.h:69 */
struct blackJackns__registerResponse;	/* blackJack.h:75 */
struct blackJackns__register;	/* blackJack.h:75 */
struct blackJackns__getStatusResponse;	/* blackJack.h:76 */
struct blackJackns__getStatus;	/* blackJack.h:76 */
struct blackJackns__playResponse;	/* blackJack.h:77 */
struct blackJackns__play;	/* blackJack.h:77 */

/* blackJack.h:57 */
#ifndef SOAP_TYPE_tMessage
#define SOAP_TYPE_tMessage (8)
/* complex XML schema type 'tMessage': */
struct tMessage {
        /** Sequence of elements 'msg' of XML schema type 'xsd:string' stored in dynamic array msg of length __size */
        int __size;
        char *msg;
};
#endif

/* blackJack.h:63 */
#ifndef SOAP_TYPE_tDeck
#define SOAP_TYPE_tDeck (10)
/* complex XML schema type 'tDeck': */
struct tDeck {
        /** Sequence of elements 'cards' of XML schema type 'xsd:unsignedInt' stored in dynamic array cards of length __size */
        int __size;
        unsigned int *cards;
};
#endif

/* blackJack.h:69 */
#ifndef SOAP_TYPE_tBlock
#define SOAP_TYPE_tBlock (14)
/* complex XML schema type 'tBlock': */
struct tBlock {
        /** Required element 'code' of XML schema type 'xsd:int' */
        int code;
        /** Required element 'msgStruct' of XML schema type 'blackJackns:tMessage' */
        struct tMessage msgStruct;
        /** Required element 'deck' of XML schema type 'blackJackns:tDeck' */
        struct tDeck deck;
};
#endif

/* blackJack.h:75 */
#ifndef SOAP_TYPE_blackJackns__registerResponse
#define SOAP_TYPE_blackJackns__registerResponse (18)
/* complex XML schema type 'blackJackns:registerResponse': */
struct blackJackns__registerResponse {
        /** Optional element 'result' of XML schema type 'xsd:int' */
        int *result;
};
#endif

/* blackJack.h:75 */
#ifndef SOAP_TYPE_blackJackns__register
#define SOAP_TYPE_blackJackns__register (19)
/* complex XML schema type 'blackJackns:register': */
struct blackJackns__register {
        /** Required element 'playerName' of XML schema type 'blackJackns:tMessage' */
        struct tMessage playerName;
};
#endif

/* blackJack.h:76 */
#ifndef SOAP_TYPE_blackJackns__getStatusResponse
#define SOAP_TYPE_blackJackns__getStatusResponse (22)
/* complex XML schema type 'blackJackns:getStatusResponse': */
struct blackJackns__getStatusResponse {
        /** Optional element 'block' of XML schema type 'blackJackns:tBlock' */
        struct tBlock *block;
};
#endif

/* blackJack.h:76 */
#ifndef SOAP_TYPE_blackJackns__getStatus
#define SOAP_TYPE_blackJackns__getStatus (23)
/* complex XML schema type 'blackJackns:getStatus': */
struct blackJackns__getStatus {
        /** Required element 'gameId' of XML schema type 'xsd:int' */
        int gameId;
        /** Required element 'playerName' of XML schema type 'blackJackns:tMessage' */
        struct tMessage playerName;
};
#endif

/* blackJack.h:77 */
#ifndef SOAP_TYPE_blackJackns__playResponse
#define SOAP_TYPE_blackJackns__playResponse (25)
/* complex XML schema type 'blackJackns:playResponse': */
struct blackJackns__playResponse {
        /** Optional element 'code' of XML schema type 'xsd:int' */
        int *code;
};
#endif

/* blackJack.h:77 */
#ifndef SOAP_TYPE_blackJackns__play
#define SOAP_TYPE_blackJackns__play (26)
/* complex XML schema type 'blackJackns:play': */
struct blackJackns__play {
        /** Required element 'playerName' of XML schema type 'blackJackns:tMessage' */
        struct tMessage playerName;
        /** Required element 'gameId' of XML schema type 'xsd:int' */
        int gameId;
        /** Required element 'playerMove' of XML schema type 'xsd:unsignedInt' */
        unsigned int playerMove;
};
#endif

/* blackJack.h:77 */
#ifndef WITH_NOGLOBAL
#ifndef SOAP_TYPE_SOAP_ENV__Header
#define SOAP_TYPE_SOAP_ENV__Header (27)
/* SOAP_ENV__Header: */
struct SOAP_ENV__Header {
#ifdef WITH_NOEMPTYSTRUCT
	char dummy;	/* empty struct is a GNU extension */
#endif
};
#endif
#endif

/* blackJack.h:77 */
#ifndef WITH_NOGLOBAL
#ifndef SOAP_TYPE_SOAP_ENV__Code
#define SOAP_TYPE_SOAP_ENV__Code (28)
/* Type SOAP_ENV__Code is a recursive data type, (in)directly referencing itself through its (base or derived class) members */
/* SOAP_ENV__Code: */
struct SOAP_ENV__Code {
        /** Optional element 'SOAP-ENV:Value' of XML schema type 'xsd:QName' */
        char *SOAP_ENV__Value;
        /** Optional element 'SOAP-ENV:Subcode' of XML schema type 'SOAP-ENV:Code' */
        struct SOAP_ENV__Code *SOAP_ENV__Subcode;
};
#endif
#endif

/* blackJack.h:77 */
#ifndef WITH_NOGLOBAL
#ifndef SOAP_TYPE_SOAP_ENV__Detail
#define SOAP_TYPE_SOAP_ENV__Detail (30)
/* SOAP_ENV__Detail: */
struct SOAP_ENV__Detail {
        char *__any;
        /** Any type of element 'fault' assigned to fault with its SOAP_TYPE_<typename> assigned to __type */
        /** Do not create a cyclic data structure through this member unless SOAP encoding or SOAP_XML_GRAPH are used for id-ref serialization */
        int __type;
        void *fault;
};
#endif
#endif

/* blackJack.h:77 */
#ifndef WITH_NOGLOBAL
#ifndef SOAP_TYPE_SOAP_ENV__Reason
#define SOAP_TYPE_SOAP_ENV__Reason (33)
/* SOAP_ENV__Reason: */
struct SOAP_ENV__Reason {
        /** Optional element 'SOAP-ENV:Text' of XML schema type 'xsd:string' */
        char *SOAP_ENV__Text;
};
#endif
#endif

/* blackJack.h:77 */
#ifndef WITH_NOGLOBAL
#ifndef SOAP_TYPE_SOAP_ENV__Fault
#define SOAP_TYPE_SOAP_ENV__Fault (34)
/* SOAP_ENV__Fault: */
struct SOAP_ENV__Fault {
        /** Optional element 'faultcode' of XML schema type 'xsd:QName' */
        char *faultcode;
        /** Optional element 'faultstring' of XML schema type 'xsd:string' */
        char *faultstring;
        /** Optional element 'faultactor' of XML schema type 'xsd:string' */
        char *faultactor;
        /** Optional element 'detail' of XML schema type 'SOAP-ENV:Detail' */
        struct SOAP_ENV__Detail *detail;
        /** Optional element 'SOAP-ENV:Code' of XML schema type 'SOAP-ENV:Code' */
        struct SOAP_ENV__Code *SOAP_ENV__Code;
        /** Optional element 'SOAP-ENV:Reason' of XML schema type 'SOAP-ENV:Reason' */
        struct SOAP_ENV__Reason *SOAP_ENV__Reason;
        /** Optional element 'SOAP-ENV:Node' of XML schema type 'xsd:string' */
        char *SOAP_ENV__Node;
        /** Optional element 'SOAP-ENV:Role' of XML schema type 'xsd:string' */
        char *SOAP_ENV__Role;
        /** Optional element 'SOAP-ENV:Detail' of XML schema type 'SOAP-ENV:Detail' */
        struct SOAP_ENV__Detail *SOAP_ENV__Detail;
};
#endif
#endif

/******************************************************************************\
 *                                                                            *
 * Typedefs                                                                   *
 *                                                                            *
\******************************************************************************/


/* (built-in):0 */
#ifndef SOAP_TYPE__XML
#define SOAP_TYPE__XML (5)
typedef char *_XML;
#endif

/* (built-in):0 */
#ifndef SOAP_TYPE__QName
#define SOAP_TYPE__QName (6)
typedef char *_QName;
#endif

/* blackJack.h:54 */
#ifndef SOAP_TYPE_xsd__string
#define SOAP_TYPE_xsd__string (7)
typedef char *xsd__string;
#endif

/* blackJack.h:60 */
#ifndef SOAP_TYPE_blackJackns__tMessage
#define SOAP_TYPE_blackJackns__tMessage (9)
typedef struct tMessage blackJackns__tMessage;
#endif

/* blackJack.h:66 */
#ifndef SOAP_TYPE_blackJackns__tDeck
#define SOAP_TYPE_blackJackns__tDeck (13)
typedef struct tDeck blackJackns__tDeck;
#endif

/* blackJack.h:73 */
#ifndef SOAP_TYPE_blackJackns__tBlock
#define SOAP_TYPE_blackJackns__tBlock (15)
typedef struct tBlock blackJackns__tBlock;
#endif

/******************************************************************************\
 *                                                                            *
 * Serializable Types                                                         *
 *                                                                            *
\******************************************************************************/


/* char has binding name 'byte' for type 'xsd:byte' */
#ifndef SOAP_TYPE_byte
#define SOAP_TYPE_byte (3)
#endif

/* int has binding name 'int' for type 'xsd:int' */
#ifndef SOAP_TYPE_int
#define SOAP_TYPE_int (1)
#endif

/* unsigned int has binding name 'unsignedInt' for type 'xsd:unsignedInt' */
#ifndef SOAP_TYPE_unsignedInt
#define SOAP_TYPE_unsignedInt (11)
#endif

/* struct SOAP_ENV__Fault has binding name 'SOAP_ENV__Fault' for type '' */
#ifndef SOAP_TYPE_SOAP_ENV__Fault
#define SOAP_TYPE_SOAP_ENV__Fault (34)
#endif

/* struct SOAP_ENV__Reason has binding name 'SOAP_ENV__Reason' for type '' */
#ifndef SOAP_TYPE_SOAP_ENV__Reason
#define SOAP_TYPE_SOAP_ENV__Reason (33)
#endif

/* struct SOAP_ENV__Detail has binding name 'SOAP_ENV__Detail' for type '' */
#ifndef SOAP_TYPE_SOAP_ENV__Detail
#define SOAP_TYPE_SOAP_ENV__Detail (30)
#endif

/* struct SOAP_ENV__Code has binding name 'SOAP_ENV__Code' for type '' */
#ifndef SOAP_TYPE_SOAP_ENV__Code
#define SOAP_TYPE_SOAP_ENV__Code (28)
#endif

/* struct SOAP_ENV__Header has binding name 'SOAP_ENV__Header' for type '' */
#ifndef SOAP_TYPE_SOAP_ENV__Header
#define SOAP_TYPE_SOAP_ENV__Header (27)
#endif

/* struct blackJackns__play has binding name 'blackJackns__play' for type 'blackJackns:play' */
#ifndef SOAP_TYPE_blackJackns__play
#define SOAP_TYPE_blackJackns__play (26)
#endif

/* struct blackJackns__playResponse has binding name 'blackJackns__playResponse' for type 'blackJackns:playResponse' */
#ifndef SOAP_TYPE_blackJackns__playResponse
#define SOAP_TYPE_blackJackns__playResponse (25)
#endif

/* struct blackJackns__getStatus has binding name 'blackJackns__getStatus' for type 'blackJackns:getStatus' */
#ifndef SOAP_TYPE_blackJackns__getStatus
#define SOAP_TYPE_blackJackns__getStatus (23)
#endif

/* struct blackJackns__getStatusResponse has binding name 'blackJackns__getStatusResponse' for type 'blackJackns:getStatusResponse' */
#ifndef SOAP_TYPE_blackJackns__getStatusResponse
#define SOAP_TYPE_blackJackns__getStatusResponse (22)
#endif

/* struct blackJackns__register has binding name 'blackJackns__register' for type 'blackJackns:register' */
#ifndef SOAP_TYPE_blackJackns__register
#define SOAP_TYPE_blackJackns__register (19)
#endif

/* struct blackJackns__registerResponse has binding name 'blackJackns__registerResponse' for type 'blackJackns:registerResponse' */
#ifndef SOAP_TYPE_blackJackns__registerResponse
#define SOAP_TYPE_blackJackns__registerResponse (18)
#endif

/* blackJackns__tBlock has binding name 'blackJackns__tBlock' for type 'blackJackns:tBlock' */
#ifndef SOAP_TYPE_blackJackns__tBlock
#define SOAP_TYPE_blackJackns__tBlock (15)
#endif

/* struct tBlock has binding name 'tBlock' for type 'tBlock' */
#ifndef SOAP_TYPE_tBlock
#define SOAP_TYPE_tBlock (14)
#endif

/* blackJackns__tDeck has binding name 'blackJackns__tDeck' for type 'blackJackns:tDeck' */
#ifndef SOAP_TYPE_blackJackns__tDeck
#define SOAP_TYPE_blackJackns__tDeck (13)
#endif

/* struct tDeck has binding name 'tDeck' for type 'tDeck' */
#ifndef SOAP_TYPE_tDeck
#define SOAP_TYPE_tDeck (10)
#endif

/* blackJackns__tMessage has binding name 'blackJackns__tMessage' for type 'blackJackns:tMessage' */
#ifndef SOAP_TYPE_blackJackns__tMessage
#define SOAP_TYPE_blackJackns__tMessage (9)
#endif

/* struct tMessage has binding name 'tMessage' for type 'tMessage' */
#ifndef SOAP_TYPE_tMessage
#define SOAP_TYPE_tMessage (8)
#endif

/* struct SOAP_ENV__Reason * has binding name 'PointerToSOAP_ENV__Reason' for type '' */
#ifndef SOAP_TYPE_PointerToSOAP_ENV__Reason
#define SOAP_TYPE_PointerToSOAP_ENV__Reason (36)
#endif

/* struct SOAP_ENV__Detail * has binding name 'PointerToSOAP_ENV__Detail' for type '' */
#ifndef SOAP_TYPE_PointerToSOAP_ENV__Detail
#define SOAP_TYPE_PointerToSOAP_ENV__Detail (35)
#endif

/* struct SOAP_ENV__Code * has binding name 'PointerToSOAP_ENV__Code' for type '' */
#ifndef SOAP_TYPE_PointerToSOAP_ENV__Code
#define SOAP_TYPE_PointerToSOAP_ENV__Code (29)
#endif

/* struct tBlock * has binding name 'PointerToblackJackns__tBlock' for type 'blackJackns:tBlock' */
#ifndef SOAP_TYPE_PointerToblackJackns__tBlock
#define SOAP_TYPE_PointerToblackJackns__tBlock (20)
#endif

/* int * has binding name 'PointerToint' for type 'xsd:int' */
#ifndef SOAP_TYPE_PointerToint
#define SOAP_TYPE_PointerToint (16)
#endif

/* unsigned int * has binding name 'PointerTounsignedInt' for type 'xsd:unsignedInt' */
#ifndef SOAP_TYPE_PointerTounsignedInt
#define SOAP_TYPE_PointerTounsignedInt (12)
#endif

/* xsd__string has binding name 'xsd__string' for type 'xsd:string' */
#ifndef SOAP_TYPE_xsd__string
#define SOAP_TYPE_xsd__string (7)
#endif

/* _QName has binding name '_QName' for type 'xsd:QName' */
#ifndef SOAP_TYPE__QName
#define SOAP_TYPE__QName (6)
#endif

/* _XML has binding name '_XML' for type '' */
#ifndef SOAP_TYPE__XML
#define SOAP_TYPE__XML (5)
#endif

/* char * has binding name 'string' for type 'xsd:string' */
#ifndef SOAP_TYPE_string
#define SOAP_TYPE_string (4)
#endif

/******************************************************************************\
 *                                                                            *
 * Externals                                                                  *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Client-Side Call Stub Functions                                            *
 *                                                                            *
\******************************************************************************/

    
    /** Web service synchronous operation 'soap_call_blackJackns__register' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 soap_call_blackJackns__register(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct tMessage playerName, int *result);
    /** Web service asynchronous operation 'soap_send_blackJackns__register' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 soap_send_blackJackns__register(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct tMessage playerName);
    /** Web service asynchronous operation 'soap_recv_blackJackns__register' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 soap_recv_blackJackns__register(struct soap *soap, int *result);
    
    /** Web service synchronous operation 'soap_call_blackJackns__getStatus' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 soap_call_blackJackns__getStatus(struct soap *soap, const char *soap_endpoint, const char *soap_action, int gameId, struct tMessage playerName, struct tBlock *block);
    /** Web service asynchronous operation 'soap_send_blackJackns__getStatus' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 soap_send_blackJackns__getStatus(struct soap *soap, const char *soap_endpoint, const char *soap_action, int gameId, struct tMessage playerName);
    /** Web service asynchronous operation 'soap_recv_blackJackns__getStatus' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 soap_recv_blackJackns__getStatus(struct soap *soap, struct tBlock *block);
    
    /** Web service synchronous operation 'soap_call_blackJackns__play' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 soap_call_blackJackns__play(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct tMessage playerName, int gameId, unsigned int playerMove, int *code);
    /** Web service asynchronous operation 'soap_send_blackJackns__play' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 soap_send_blackJackns__play(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct tMessage playerName, int gameId, unsigned int playerMove);
    /** Web service asynchronous operation 'soap_recv_blackJackns__play' to receive a response message from the connected endpoint, returns SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 soap_recv_blackJackns__play(struct soap *soap, int *code);

/******************************************************************************\
 *                                                                            *
 * Server-Side Operations                                                     *
 *                                                                            *
\******************************************************************************/

    /** Web service operation 'blackJackns__register' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 blackJackns__register(struct soap*, struct tMessage playerName, int *result);
    /** Web service operation 'blackJackns__getStatus' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 blackJackns__getStatus(struct soap*, int gameId, struct tMessage playerName, struct tBlock *block);
    /** Web service operation 'blackJackns__play' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 blackJackns__play(struct soap*, struct tMessage playerName, int gameId, unsigned int playerMove, int *code);

/******************************************************************************\
 *                                                                            *
 * Server-Side Skeletons to Invoke Service Operations                         *
 *                                                                            *
\******************************************************************************/

SOAP_FMAC5 int SOAP_FMAC6 soap_serve(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_request(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_blackJackns__register(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_blackJackns__getStatus(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_blackJackns__play(struct soap*);

#endif

/* End of soapStub.h */
