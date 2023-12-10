#include "database.hpp"

int main()  {
    Database messages("../xmlB1-periodique.xml");

    messages.calculate_transmission_time();
    messages.calculate_WCTT();
    messages.calculate_access_time();

    messages.print_messsages();

    return 0;
}
