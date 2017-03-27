#define MAXBUF 1024

/*
 * Message types...
 */
#define FILENAME 1
#define DATA  50
#define ERROR 51

/*
 * Message actually carries 2 "types".
 * The first is the "main" type for the msgq functions.
 * The second is used to carry aditional information between
 * client and server. It is only examined internally in the client
 * and server code.
 */
struct message {
    long m_type;
    long m_subtype;
    char m_data[MAXBUF];
};
