#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <sstream>

class Chatbot {
private:
    std::map<std::string, std::string> responseMap;
    std::vector<std::string> conversationLog;

    // Method to convert string to lowercase
    std::string toLowerCase(const std::string& str) {
        std::string lowerStr = str;
        std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
        return lowerStr;
    }

    // Method to load responses from a file
    void loadResponsesFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                std::string pattern, response;
                if (std::getline(iss, pattern, ':') && std::getline(iss, response)) {
                    responseMap[toLowerCase(pattern)] = response;
                }
            }
            file.close();
        }
    }

    // Method to save learned responses to a file
    void saveResponsesToFile(const std::string& filename) {
        std::ofstream file(filename, std::ios::app);
        if (file.is_open()) {
            for (const auto& entry : responseMap) {
                file << entry.first << ":" << entry.second << std::endl;
            }
            file.close();
        }
    }

public:
    Chatbot() {
        // Initialize with some default responses
        responseMap["hello"] = "Hi there! How can I help you today?";
        responseMap["how are you"] = "I'm just a bunch of code, but I'm here to help!";
        responseMap["what is your name"] = "I am a simple C++ chatbot.";
        responseMap["bye"] = "Goodbye! Have a nice day!";
        // Load additional responses from file
        loadResponsesFromFile("responses.txt");
    }

    ~Chatbot() {
        // Save learned responses to file
        saveResponsesToFile("responses.txt");
    }

    // Method to get response based on user input
    std::string getResponse(const std::string& input) {
        std::string lowerInput = toLowerCase(input);
        if (responseMap.find(lowerInput) != responseMap.end()) {
            return responseMap[lowerInput];
        } else {
            return "I'm not sure how to respond to that. Can you teach me?";
        }
    }

    // Method to add new response
    void learnResponse(const std::string& input, const std::string& response) {
        std::string lowerInput = toLowerCase(input);
        responseMap[lowerInput] = response;
    }

    // Method to log conversation
    void logConversation(const std::string& user, const std::string& bot) {
        conversationLog.push_back("You: " + user);
        conversationLog.push_back("Chatbot: " + bot);
    }

    // Method to save conversation log
    void saveConversationLog(const std::string& filename) {
        std::ofstream file(filename);
        if (file.is_open()) {
            for (const auto& entry : conversationLog) {
                file << entry << std::endl;
            }
            file.close();
        }
    }
};

// Main function
int main() {
    Chatbot chatbot;
    std::string userInput;

    std::cout << "Chatbot: Hello! I am a simple C++ chatbot. Type 'bye' to exit." << std::endl;

    while (true) {
        std::cout << "You: ";
        std::getline(std::cin, userInput);

        if (userInput == "bye") {
            std::string response = chatbot.getResponse(userInput);
            chatbot.logConversation(userInput, response);
            std::cout << "Chatbot: " << response << std::endl;
            break;
        }

        std::string response = chatbot.getResponse(userInput);
        std::cout << "Chatbot: " << response << std::endl;
        chatbot.logConversation(userInput, response);

        if (response == "I'm not sure how to respond to that. Can you teach me?") {
            std::cout << "Chatbot: Please provide an appropriate response for \"" << userInput << "\": ";
            std::string newResponse;
            std::getline(std::cin, newResponse);
            chatbot.learnResponse(userInput, newResponse);
        }
    }

    chatbot.saveConversationLog("conversation_log.txt");

    return 0;
}
