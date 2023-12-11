#ifndef DATABASE
#define DATABASE

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "message.hpp"

#define OVERHEAD_CP 56
#define OVERHEAD_CC 106

#define MIN_DIFF 1e-6

class Database {
private:
    struct comparePriority {
        bool operator()(const Message m1, const Message m2) const;
    };

    std::vector<Message> messages;

public:
    Database(std::string xmlPath);

    std::vector<Message> get_messages();
    void calculate_transmission_time();
    void calculate_access_time();
    void calculate_WCTT();
    void print_messsages();
    void print_xml(std::ofstream &file);
};

#endif
