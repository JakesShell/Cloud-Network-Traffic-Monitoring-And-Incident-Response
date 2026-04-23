#include <iostream>
#include "TrafficMonitor.h"

int main() {
    try {
        TrafficMonitor monitor("config.json");
        monitor.startMonitoring();
    } catch (const std::exception& ex) {
        std::cerr << "Application Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
