#include "database.hpp"
#include "pugixml.hpp"
#include <iostream>
#include <ostream>
#include <set>
#include <vector>


Database::Database(std::string xmlPath) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(xmlPath.c_str());

    // A multiset is used so that the sorting is cheaper
    std::multiset<Message, Database::comparePriority> msg_set;

    if (!result)
        std::cerr << "Error parsing XML file" << std::endl;

    for (pugi::xml_node message: doc.child("fichier").children("message")) 
        msg_set.insert(Message(
                message.child("nom").text().get(),
                message.child("type").text().get(),
                message.child("frequence").text().as_float(),
                message.child("taille_mes").text().as_int(), 
                message.child("emetteur").text().get(), 
                message.child("recepteur").text().get()));

    std::copy(msg_set.begin(), msg_set.end(), std::back_inserter(this->messages));
}


bool Database::comparePriority::operator()(const Message m1, const Message m2) const {
    return m1.get_frequence() > m2.get_frequence();
}

std::vector<Message> Database::get_messages() {
    return this->messages;
}

void Database::calculate_transmission_time() {
    for (auto iter = this->messages.begin(); iter != this->messages.end(); iter++)
        iter->set_transmissionTime(iter->transmission_time());
}

void Database::calculate_WCTT() {
    auto init_msgs = messages.begin(), begin_freq = init_msgs;
    float curr_freq = init_msgs->get_frequence();

    /*
     * The objective function is to calculate for every message
     * W_(n+1)_i = C_i + max(j E lp_i){C_j} + sum(k E hp_i){C_k * (W_(n)_i) * f_i}
    */

    bool last = false;
    // Loops over all the elements of the vector
    for (auto iter = init_msgs; !last; iter++) {

        // A new frequency was found!!! Or it's the last element
        if (iter->get_frequence() != curr_freq || (last = std::next(iter) == messages.end())) {
            curr_freq = iter->get_frequence();

            // Treat edge case (last frequency group)
            if (last) {
                iter = messages.end();
            }
            
            // Get the C_j (maximum transmission time between the lower priority messages)
            double transmission_t = last ? 0 : iter->get_transmissionTime();
            for (auto temp_iter = iter; temp_iter != messages.end(); temp_iter++)
                if (temp_iter->get_transmissionTime() > transmission_t)
                    transmission_t = temp_iter->get_transmissionTime();

            // Looping through the frequency group and updating the WCES
            // begin_freq points to the current element
            // For every message in the frequency group:
            do {
                double sum_factor = 0;

                // Convergence loop for the WCTT
                double curr_WCTT = begin_freq->get_transmissionTime(), previous_WCTT = 0;
                do {
                    previous_WCTT = curr_WCTT;
                    sum_factor = 0;

                    // One has to loop over every element from the beggining to the end of frequency group (pointed by iter)
                    for (auto temp_iter = init_msgs; temp_iter != iter; temp_iter++) {
                        if (temp_iter == begin_freq) continue;

                        sum_factor += ((int)(1 + curr_WCTT * temp_iter->get_frequence() * 1e-6)) * temp_iter->get_transmissionTime();
                    }

                    curr_WCTT = sum_factor;
                    curr_WCTT += begin_freq->get_transmissionTime();
                    curr_WCTT += transmission_t;
                } while (MIN_DIFF < (curr_WCTT - previous_WCTT));

                begin_freq->set_WCTT(curr_WCTT);

            } while (++begin_freq != iter);
        }
    }
}

void Database::calculate_access_time() {
    for (auto iter = this->messages.begin(); iter != this->messages.end(); iter++)
        iter->set_accessTime(iter->get_WCTT() - iter->get_transmissionTime());
}

void Database::print_messsages() {
    for (Message msg: this->messages) {
        std::cout << "Message: " << msg.get_name() << "\n";
        std::cout << "Frequency: " << msg.get_frequence() << "\n";
        std::cout << "Size: " << msg.size_of_message() << "\n";
        std::cout << "Access Time: " << msg.get_accessTime() << "us\n";
        std::cout << "Transmission Time: " << msg.get_transmissionTime() << "us\n";
        std::cout << "WCTT: " << msg.get_WCTT() << "us\n";
        std::cout << std::endl;
    }
}
