#include "database.hpp"
#include <iterator>

Message::Message (
    std::string name,
    std::string type,
    float frequence,
    int message_size,
    std::string sender,
    std::string receiver
    ) {
    this->name = name;
    this->type = type;
    this->frequence = frequence;
    this->message_size = message_size;
    this->sender = sender;
    this->receiver = receiver;
}

int Message::size_of_message() const {
    if (this->receiver == "SXJJ" or this->sender == "SXJJ") {
        return OVERHEAD_CP + this->message_size*20;
    }
    return OVERHEAD_CC + this->message_size*20;
}

// Gives result in us
double Message::transmission_time() const {
    return this->size_of_message() / 1.0f * TRANSMISSION_SPEED;
}

std::string Message::get_name() const {
    return this->name;
}

std::string Message::get_type() const {
    return this->type;
}

float Message::get_frequence() const {
    return this->frequence;
}

int Message::get_size() const {
    return this->message_size;
}

std::string Message::get_sender() const {
    return this->sender;
} 

std::string Message::get_receiver() const {
    return this->receiver;
}

double Message::get_transmissionTime() const {
    return this->transmissionTime;
}

double Message::get_accessTime() const {
    return this->accessTime;
}

double Message::get_WCTT() const {
    return this->WCTT;
}

void Message::set_transmissionTime(double val) {
    this->transmissionTime = val;
}

void Message::set_accessTime(double val) {
    this->accessTime = val;
}

void Message::set_WCTT(double val) {
    this->WCTT = val;
}
