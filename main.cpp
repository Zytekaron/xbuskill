#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
namespace c {
#include <cstdio>
#include <unistd.h>
} // namespace c

void status();
void enable();
void disable();
void execute();
void list();

const char* usage = "usage: xbuskill <enable|disable|status|execute|list>";

auto configFilePath = std::filesystem::path("/etc/xbuskill/config");
auto statusFilePath = std::filesystem::path("/tmp/xbuskill");

int program(std::string action) {
    if (action == "status") {
        status();
    } else if (action == "enable") {
        enable();
    } else if (action == "disable") {
        disable();
    } else if (action == "execute") {
        execute();
    } else if (action == "list") {
        list();
    } else {
        std::cout << usage << std::endl;
        return 1;
    }

    return 0;
}

int main(int argc, char** argv) {
    if (c::getuid() != 0) {
        std::cout << "xbuskill must be run with root privileges" << std::endl;
        return 1;
    }
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

bool is_active() {
    return std::filesystem::exists(statusFilePath);
}

void status() {
    if (is_active()) {
        std::cout << "xbuskill is enabled" << std::endl;
    } else {
        std::cout << "xbuskill is disabled" << std::endl;
    }
}

void enable() {
    if (is_active()) {
        std::cout << "xbuskill is already enabled" << std::endl;
    } else {
        std::ofstream{statusFilePath};
        std::cout << "xbuskill is now enabled" << std::endl;
    }
}

void disable() {
    if (is_active()) {
        std::filesystem::remove(statusFilePath);
        std::cout << "xbuskill is now disabled" << std::endl;
    } else {
        std::cout << "xbuskill is already disabled" << std::endl;
    }
}

void execute() {
    if (!is_active()) {
        return;
    }

    for (auto command : load_commands()) {
        std::cout << "$ " << command << std::endl;
        std::system(command.c_str());
    }
}

void list() {
    for (auto command : load_commands()) {
        std::cout << command << std::endl;
    }
}
