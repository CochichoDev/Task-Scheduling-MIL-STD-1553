#ifndef MESSAGE
#define MESSAGE

#include <string>

#define TRANSMISSION_SPEED 1      // Meaning 1MHz

class Database;     // This is not a good design choise but it's faster

class Message {
    std::string name;
    std::string type;
    float frequence;
    int message_size;
    std::string sender;
    std::string receiver;
    double transmissionTime;
    double accessTime;
    double WCTT;

public:
    Message (
        std::string name,
        std::string type,
        float frequence,
        int message_size,
        std::string sender,
        std::string receiver
        );

    int size_of_message() const;
    // Returns the transmission time in us
    double transmission_time() const;
    std::string get_name() const;
    std::string get_type() const;
    float get_frequence() const;
    int get_size() const;
    std::string get_sender() const;
    std::string get_receiver() const;
    double get_transmissionTime() const;
    double get_accessTime() const;
    double get_WCTT() const;

    void set_transmissionTime(double val);
    void set_accessTime(double val);
    void set_WCTT(double val);
};

#endif
