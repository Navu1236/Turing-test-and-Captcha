#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <cstdlib>

using namespace std;

void generate_captcha() {
    const string charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string text = "";
    
    // Modern C++ Random Number Generation
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, charset.length() - 1);

    for (int i = 0; i < 5; ++i) {
        text += charset[dis(gen)];
    }

    // Write to file using C++ streams
    ofstream file("captcha.txt");
    if (file.is_open()) {
        file << "=== CAPTCHA ===\n" << text << "\n===============\n";
        file.close();
    } else {
        cerr << "Error creating CAPTCHA file.\n";
        return;
    }

    cout << "A CAPTCHA file has been generated.\n";
    cout << "Open captcha.txt and type the text.\n";
    cout << "Enter CAPTCHA: ";

    string user_input;
    cin >> user_input;

    if (user_input == text) {
        cout << "CAPTCHA correct. Redirecting to Google...\n";
        
        #ifdef _WIN32
            system("start https://www.google.com");
        #elif __APPLE__
            system("open https://www.google.com");
        #else
            system("xdg-open https://www.google.com");
        #endif
    } else {
        cout << "CAPTCHA incorrect. Try again.\n";
    }
}

int main() {
    generate_captcha();
    return 0;
}