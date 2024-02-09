/*
proto_seriale.h

protocollo per collegamento seriale, sia cavo che wireless
Sringa composta:
PREAMBOLO   inizio stringa
sepa        separatore
COMANDO     comando
sepa        separatore
TYPE        tipo di comando: Richiesta, Conferma, INdicazione, Responso
sepa        separatore
DATI        campi dati separati da un separatore
*/
#ifndef PROTO_H
#define PROTO_H

#define COM_PORT_USB        0
#define COM_PORT_BLUE       1

#define P_PREAMBOLO         '$'
#define P_SEPARA            ','
#define P_SEPARA_FILE       TAB   //';'
#define P_REQUEST_TYPE      'R'
#define P_CONFIRM_TYPE      'C'
#define P_INDICATION_TYPE   'I'
#define P_RESPONSE_TYPE     'r'
#define P_END_STRING        CR
#define BLUE_SEPARA         ','

#define RENOTE_FLAG_NONE      0
#define REMOTE_FLAG_REQ       1
#define REMOTE_FLAG_CFM       2
#define REMOTE_FLAG_IND       3

/* procedure */
extern bool proto_gest(char* p_from, char com_port);

#endif //PRORO_H
