#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
namespace c {
#include <cstdio>
#include <unistd.h>
} // namespace c

namespace fs = std::filesystem;

int program(std::string action);
void status();
void execute();
void enable();
void disable();
void config();

const char* usage = "usage: xbuskill <enable|disable|status|execute|config>";

auto configFilePath = fs::path("/etc/xbuskill/config");
auto statusFilePath = fs::path("/tmp/xbuskill");

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << usage << std::endl;
        return 1;
    }

    try {
        return program(std::string(argv[1]));
    } catch (const char* error) {
        std::cout << error << std::endl;
        return 1;
    }
}

int program(std::string action) {
    if (action == "status") {
        status();
    } else if (action == "execute") {
        execute();
    } else if (action == "enable") {
        enable();
    } else if (action == "disable") {
        disable();
    } else if (action == "config") {
        config();
    } else {
        std::cout << usage << std::endl;
        return 1;
    }

    return 0;
}

std::vector<std::string> load_commands() {
    std::ifstream file;
    file.open(configFilePath);
    if (!file) {
        throw "config file is missing";
    }
    if (!file.is_open()) {
        throw "could not open config file";
    }

    std::vector<std::string> vec;

    std::string line;
    while (getline(file, line)) {
        if (line.length() == 0) {
            continue;
        }
        vec.push_back(line);
    }

    return vec;
}

bool is_enabled() {
    return fs::exists(statusFilePath);
}

void status() {
    if (is_enabled()) {
        std::cout << "xbuskill is enabled" << std::endl;
    } else {
        std::cout << "xbuskill is disabled" << std::endl;
    }
}

void execute() {
    if (!is_enabled()) {
        return;
    }

    std::cout << "executing kill script" << std::endl;

    for (auto command : load_commands()) {
        std::system(command.c_str());
    }
}

void enable() {
    if (is_enabled()) {
        std::cout << "xbuskill is already enabled" << std::endl;
    } else {
        std::ofstream{statusFilePath};
    }
}

void disable() {
    if (is_enabled()) {
        fs::remove(statusFilePath);
    } else {
        std::cout << "xbuskill is already disabled" << std::endl;
    }
}

void config() {
    for (auto command : load_commands()) {
        std::cout << command << std::endl;
    }
}
