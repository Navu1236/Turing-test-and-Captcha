#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <future>
#include <thread>
#include <curl/curl.h>

using namespace std;

const string API_KEY = "YOUR_GROQ_API_KEY";
const string API_URL = "https://api.groq.com/openai/v1/chat/completions";

// Callback to write curl data directly into a std::string
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Replicates Python's threading timeout using C++ std::async
string input_with_timeout(const string& prompt, int timeout_sec) {
    cout << prompt;
    cout.flush();

    // Launch a background task to get input
    future<string> future_input = async(launch::async, []() {
        string input;
        getline(cin, input);
        return input;
    });

    // Wait for the specified time
    if (future_input.wait_for(chrono::seconds(timeout_sec)) == future_status::timeout) {
        return ""; // Timeout occurred
    }
    
    return future_input.get();
}

int main() {
    int score = 0;
    int round_num = 1;
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> role_dist(0, 1);

    curl_global_init(CURL_GLOBAL_ALL);

    cout << "\n==============================\n";
    cout << "       HUMAN OR AI GAME\n";
    cout << "==============================\n";
    cout << "Guess if the response is HUMAN or BOT.\n";
    cout << "You have 10 seconds to answer each round.\n";
    cout << "Game ends when you guess wrong.\n\n";

    while (true) {
        cout << "\n----- ROUND " << round_num << " -----\n";
        bool is_bot = role_dist(gen); 
        string role = is_bot ? "bot" : "human";

        cout << "\nAsk a question: ";
        string question;
        getline(cin, question);

        // Construct JSON Payload safely with std::string
        string prompt;
        if (!is_bot) {
            prompt = "You are pretending to be a casual human. Speak naturally, slightly informal, maybe unsure. You may use casual words like hmm, yeah, maybe. Keep the response under 2 sentences. Question: " + question;
        } else {
            prompt = "You are an AI assistant. Respond logically and formally like a machine. Be structured and clear. Keep the response under 2 sentences. Question: " + question;
        }

        string json_payload = R"({"model": "llama-3.3-70b-versatile", "messages": [{"role": "user", "content": ")" + prompt + R"("}]})";

        // Setup cURL
        CURL* curl = curl_easy_init();
        string response_string;

        if(curl) {
            struct curl_slist* headers = NULL;
            headers = curl_slist_append(headers, "Content-Type: application/json");
            string auth_header = "Authorization: Bearer " + API_KEY;
            headers = curl_slist_append(headers, auth_header.c_str());

            curl_easy_setopt(curl, CURLOPT_URL, API_URL.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_payload.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

            CURLcode res = curl_easy_perform(curl);

            if(res != CURLE_OK) {
                cout << "API Error: " << curl_easy_strerror(res) << "\nRetrying in 10 seconds...\n";
                this_thread::sleep_for(chrono::seconds(10));
                curl_easy_cleanup(curl);
                curl_slist_free_all(headers);
                continue;
            } else {
                // Extract "content" from JSON response
                size_t content_pos = response_string.find("\"content\":\"");
                if (content_pos != string::npos) {
                    content_pos += 11;
                    size_t end_pos = response_string.find("\"", content_pos);
                    string reply = response_string.substr(content_pos, end_pos - content_pos);
                    
                    // Clean up basic JSON newline escaping
                    size_t nlp;
                    while((nlp = reply.find("\\n")) != string::npos) {
                        reply.replace(nlp, 2, " ");
                    }
                    cout << "\nResponse:\n" << reply << "\n";
                }
            }
            curl_easy_cleanup(curl);
            curl_slist_free_all(headers);
        }

        // Handle Timer Input
        string guess = input_with_timeout("\nWas that HUMAN or BOT? (h/b): ", 10);
        
        if (guess.empty()) {
            cout << "\n⏰ Time's up!\n";
            break;
        }

        char g = tolower(guess[0]);
        if ((g == 'h' && !is_bot) || (g == 'b' && is_bot)) {
            score++;
            cout << "\nCorrect!\n";
        } else {
            cout << "\nWrong guess!\nActual role: " << (is_bot ? "BOT" : "HUMAN") << "\n";
            break;
        }

        round_num++;
        this_thread::sleep_for(chrono::seconds(2));
    }

    cout << "\n==============================\n";
    cout << "         GAME OVER\n";
    cout << "==============================\n";
    cout << "Rounds Survived: " << score << "\n";
    cout << "==============================\n";

    curl_global_cleanup();
    return 0;
}