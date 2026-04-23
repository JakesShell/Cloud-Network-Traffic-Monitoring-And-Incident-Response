# Network Traffic Security Monitor

## Overview

Network Traffic Security Monitor is a C++-based security monitoring prototype designed to simulate network traffic events, classify severity levels, and write structured event output to a local NDJSON log file.

This project is positioned as a recruiter-ready backend and systems-focused security utility that demonstrates how a lightweight monitoring component can generate and persist security-relevant traffic events for later review, ingestion, or dashboard visualization.

The current version focuses on:

- Config-Driven Monitoring Behavior
- Simulated Network Event Generation
- Severity Classification
- Structured NDJSON Output
- Console-Based Monitoring Summary

## Real-World Business Use Case

This project maps to realistic security and infrastructure workflows used by:

- Security Operations Teams
- Infrastructure Engineers
- Backend Systems Developers
- Platform Engineers
- Internal Monitoring Tool Builders

A company may need to answer questions such as:

- How can network events be generated and logged in a structured format?
- How can traffic events be classified by severity before being ingested elsewhere?
- How can a lightweight monitoring component be prototyped in C++?
- How can event output be prepared for later visualization or SIEM-style workflows?

This kind of utility could act as an upstream event producer for log pipelines, dashboards, or alerting systems.

## Key Features

- Config-Driven Monitor Settings
- Simulated Traffic Event Generation
- Protocol And Byte-Based Severity Classification
- NDJSON Output File
- Console Summary Output
- Lightweight C++ Monitoring Prototype

## Tech Stack

- C++
- JSON Configuration
- NDJSON Output
- CMake
- Microsoft C++ Compiler

## Project Structure

```text
Network-Traffic-Security-Monitor/
|-- main.cpp
|-- TrafficMonitor.cpp
|-- TrafficMonitor.h
|-- config.json
|-- CMakeLists.txt
|-- .gitignore
|-- README.md
|-- network_events.ndjson
