#include "TrafficMonitor.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <thread>
#include <vector>

TrafficMonitor::TrafficMonitor(const std::string& configPath)
    : config(loadConfig(configPath)), eventCounter(0) {
}

std::string TrafficMonitor::readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string TrafficMonitor::extractString(const std::string& content, const std::string& key, const std::string& fallback) {
    std::string quotedKey = "\"" + key + "\"";
    size_t keyPos = content.find(quotedKey);
    if (keyPos == std::string::npos) {
        return fallback;
    }

    size_t colonPos = content.find(':', keyPos);
    size_t firstQuote = content.find('"', colonPos + 1);
    size_t secondQuote = content.find('"', firstQuote + 1);

    if (colonPos == std::string::npos || firstQuote == std::string::npos || secondQuote == std::string::npos) {
        return fallback;
    }

    return content.substr(firstQuote + 1, secondQuote - firstQuote - 1);
}

int TrafficMonitor::extractInt(const std::string& content, const std::string& key, int fallback) {
    std::string quotedKey = "\"" + key + "\"";
    size_t keyPos = content.find(quotedKey);
    if (keyPos == std::string::npos) {
        return fallback;
    }

    size_t colonPos = content.find(':', keyPos);
    if (colonPos == std::string::npos) {
        return fallback;
    }

    size_t numberStart = content.find_first_of("0123456789", colonPos + 1);
    size_t numberEnd = content.find_first_not_of("0123456789", numberStart);

    if (numberStart == std::string::npos) {
        return fallback;
    }

    return std::stoi(content.substr(numberStart, numberEnd - numberStart));
}

MonitorConfig TrafficMonitor::loadConfig(const std::string& configPath) {
    MonitorConfig loaded;
    std::string content = readFile(configPath);

    if (content.empty()) {
        return loaded;
    }

    loaded.monitorName = extractString(content, "monitor_name", loaded.monitorName);
    loaded.outputFile = extractString(content, "output_file", loaded.outputFile);
    loaded.intervalSeconds = extractInt(content, "interval_seconds", loaded.intervalSeconds);
    loaded.maxEvents = extractInt(content, "max_events", loaded.maxEvents);
    loaded.defaultSource = extractString(content, "default_source", loaded.defaultSource);
    loaded.defaultDestination = extractString(content, "default_destination", loaded.defaultDestination);

    return loaded;
}

std::string TrafficMonitor::classifySeverity(const std::string& protocol, int bytes) const {
    if (protocol == "ICMP" && bytes > 1200) {
        return "High";
    }
    if (protocol == "TCP" && bytes > 900) {
        return "Medium";
    }
    if (protocol == "UDP" && bytes > 700) {
        return "Medium";
    }
    return "Low";
}

std::string TrafficMonitor::generateTrafficEvent() {
    static std::vector<std::string> protocols = {"TCP", "UDP", "ICMP"};
    static std::vector<std::string> destinations = {
        config.defaultDestination,
        "10.0.0.40",
        "172.16.0.12",
        "192.168.1.50"
    };

    static std::mt19937 rng(static_cast<unsigned int>(
        std::chrono::system_clock::now().time_since_epoch().count()
    ));

    std::uniform_int_distribution<int> protocolDist(0, static_cast<int>(protocols.size() - 1));
    std::uniform_int_distribution<int> destinationDist(0, static_cast<int>(destinations.size() - 1));
    std::uniform_int_distribution<int> bytesDist(120, 1500);

    std::string protocol = protocols[protocolDist(rng)];
    std::string destination = destinations[destinationDist(rng)];
    int bytes = bytesDist(rng);
    std::string severity = classifySeverity(protocol, bytes);

    eventCounter++;

    std::ostringstream json;
    json << "{"
         << "\"event_id\":" << eventCounter << ","
         << "\"source\":\"" << config.defaultSource << "\","
         << "\"destination\":\"" << destination << "\","
         << "\"protocol\":\"" << protocol << "\","
         << "\"bytes\":" << bytes << ","
         << "\"severity\":\"" << severity << "\""
         << "}";

    return json.str();
}

void TrafficMonitor::persistEvent(const std::string& eventJson) {
    std::ofstream output(config.outputFile, std::ios::app);
    output << eventJson << std::endl;
}

void TrafficMonitor::printSummary(const std::string& eventJson) const {
    std::cout << "[Monitor] " << eventJson << std::endl;
}

void TrafficMonitor::startMonitoring() {
    std::cout << config.monitorName << std::endl;
    std::cout << "Writing Events To: " << config.outputFile << std::endl;
    std::cout << "Max Events: " << config.maxEvents << std::endl;
    std::cout << "Interval: " << config.intervalSeconds << " Seconds" << std::endl;
    std::cout << std::endl;

    for (int i = 0; i < config.maxEvents; ++i) {
        std::string trafficEvent = generateTrafficEvent();
        persistEvent(trafficEvent);
        printSummary(trafficEvent);

        if (i < config.maxEvents - 1) {
            std::this_thread::sleep_for(std::chrono::seconds(config.intervalSeconds));
        }
    }

    std::cout << std::endl;
    std::cout << "Monitoring Complete. Events saved to " << config.outputFile << std::endl;
}
