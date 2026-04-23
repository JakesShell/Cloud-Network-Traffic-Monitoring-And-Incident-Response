#ifndef TRAFFIC_MONITOR_H
#define TRAFFIC_MONITOR_H

#include <string>

struct MonitorConfig {
    std::string monitorName = "Network Traffic Security Monitor";
    std::string outputFile = "network_events.ndjson";
    int intervalSeconds = 2;
    int maxEvents = 5;
    std::string defaultSource = "10.0.0.15";
    std::string defaultDestination = "10.0.0.25";
};

class TrafficMonitor {
public:
    explicit TrafficMonitor(const std::string& configPath);
    void startMonitoring();

private:
    MonitorConfig config;
    int eventCounter;

    MonitorConfig loadConfig(const std::string& configPath);
    std::string readFile(const std::string& path);
    std::string extractString(const std::string& content, const std::string& key, const std::string& fallback);
    int extractInt(const std::string& content, const std::string& key, int fallback);

    std::string generateTrafficEvent();
    std::string classifySeverity(const std::string& protocol, int bytes) const;
    void persistEvent(const std::string& eventJson);
    void printSummary(const std::string& eventJson) const;
};

#endif
