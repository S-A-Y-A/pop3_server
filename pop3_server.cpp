#include "pop3_server.h"


namespace pms {

    Pop3Server::Pop3Server(std::string servername,
                           std::string username,
                           std::string password,
                           int port) {
        this->start(servername, username, password, port);
    }


    Pop3Server::~Pop3Server() {
        this->finish();
    }


    void Pop3Server::start(std::string servername,
                           std::string username,
                           std::string password,
                           int port) {
        try {
            session.start(servername, port);
            std::cout << "start" << std::endl;
            session.login(username, password, session.apop_support());
            std::cout << "login" << std::endl;
            listSize = session.list().size();
        }
        catch (clx::pop3_error& e) {
            std::cerr << e.what() << std::endl;
            std::exit(-1);
        }
    }


    void Pop3Server::finish() {
        session.finish();
    }


    std::string Pop3Server::status() {
        return session.status();
    }


    void Pop3Server::showMails(int n) {
        std::cout << "~~~~~ Mails ~~~~~" << std::endl;
        for(size_t i = listSize; i > 0; i--) {
            std::string mail = session.retrieve(i);
            std::cout << mail << std::endl;
            std::cout << "----------" << std::endl;
        }
    }


    std::string Pop3Server::getMailBySubject(std::string subject) {
        std::regex regexSubject("(^|\n)Subject: " + subject + "($|\n)");

        for(size_t i = listSize; i > 0; i--) {
            std::string header = session.top(i, 0);
            if(std::regex_search(header, regexSubject)){
                return session.retrieve(i);
            }
        }
        return SUBJECT_NOT_FOUND_MSG;
    }


    std::string Pop3Server::getHeaderBySubject(std::string subject) {
        std::regex regexSubject("(^|\n)Subject: " + subject + "($|\n)");

        for(size_t i = listSize; i > 0; i--) {
            std::string header = session.top(i, 0);
            if(std::regex_search(header, regexSubject)){
                return header;
            }
        }
        return SUBJECT_NOT_FOUND_MSG;
    }


    std::string Pop3Server::getHeaderContent(std::string header, std::string headername) {
        std::regex regexHeadername("(^|\n)" + headername + ": .*($|\n)");
        std::smatch results;

        if(std::regex_search(header, results, regexHeadername)){
            return results[0].str();
        }
        else {
            return HEADER_NOT_FOUND_MSG;
        }
    }

    std::string Pop3Server::getHeaderContentBySubject(std::string subject, std::string headername) {
        std::string header = getHeaderBySubject(subject);
        if(header == SUBJECT_NOT_FOUND_MSG) {
            return SUBJECT_NOT_FOUND_MSG;
        }
        else {
            return getHeaderContent(header, headername);
        }
    }

    void Pop3Server::deleteMails(int n) {
        for(int i = n; i > 0; i--) {
            session.erase(i);
        }
    }
}
