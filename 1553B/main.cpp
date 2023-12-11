#include "database.hpp"

int main()  {
    Database messages("../xmlB1-periodique.xml");

    messages.calculate_transmission_time();
    messages.calculate_WCTT();
    messages.calculate_access_time();

    std::ofstream new_xml;
    new_xml.open("results.xml");
    messages.print_xml(new_xml);
    new_xml.close();

    return 0;
}
