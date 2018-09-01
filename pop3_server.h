#include <iostream>
#include <string>
#include <regex>
#include "clx/pop3.h"

#define SUBJECT_NOT_FOUND_MSG "The subject mail could not be found."
#define HEADER_NOT_FOUND_MSG "The header name was not included in that header."

namespace pms {
    class Pop3Server {
        private:
            // #mails in pop3 server
            size_t listSize;
            clx::pop3 session;

        public:
            Pop3Server(){};
            Pop3Server(std::string servername,
                       std::string username,
                       std::string password,
                       int port = 110);
            ~Pop3Server();

            // connect and login pop3 server (default port number: 110)
            void start(std::string servername,
                       std::string username,
                       std::string password,
                       int port = 110);

            // finish server connection
            void finish();

            // show n latest mails
            void showMails(int n);

            // delete n oldest mails
            void deleteMails(int n);

            // get status of pop3 server
            std::string status();

            // get latest mail whose subject include "subject"
            std::string getMailBySubject(std::string subject);

            // get latest mail's header whose subject include "subject"
            std::string getHeaderBySubject(std::string subject);

            // get header content which header name is "headername"
            std::string getHeaderContent(std::string header, std::string headername);

            // get header content by getHeaderContent(header, eadername) after get header by getHeaderBySubject(subject)
            std::string getHeaderContentBySubject(std::string subject, std::string headername);
    };
}
