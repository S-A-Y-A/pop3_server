#include "pop3_server.h"


int main(int argc, char const *argv[]) {

    // connect and login pop3 server (port number: 995)
    pms::Pop3Server server(argv[1], argv[2], argv[3], 995);

    // show 2 latest mails in pop3 server
    server.showMails(2);

    // get "Date" header name content in the mail which include "goat" in subject
    server.getHeaderContentBySubject("goat", "Date");

    // delete 3 oldest mail in pop3 server
    server.deleteMails(3);

    return 0;
}
