# Human or AI Game (C++ Turing Test)

## Description
This is a command-line game written in C++ that simulates a Turing Test. It connects to the Groq API (using the Llama 3 model) to generate responses. Each round, the program secretly assigns the API a persona: either a logical, formal "AI Assistant" or a casual, unsure "Human". 

You get to ask one question per round. Based on the response, you have **10 seconds** to guess whether you are talking to the Human or the Bot. The game continues until you guess incorrectly.

## Features
* **Live API Integration:** Uses `libcurl` to fetch real-time responses from Groq's fast LLM endpoints.
* **JSON Escaping:** Safely handles user input (like quotes and backslashes) to prevent API crashes.
* **Asynchronous Timeout:** Uses OS-level POSIX `select()` to enforce a strict 10-second timer for user guesses without freezing the program.

## Prerequisites
Before you can compile and run this game, you need:
1. A modern C++ compiler (like `g++` or `clang++`).
2. **libcurl** installed on your system.
   * *Ubuntu/Debian:* `sudo apt-get install libcurl4-openssl-dev`
   * *macOS (Homebrew):* `brew install curl`
3. A free API key from [Groq](https://console.groq.com/).

## Setup Instructions
1. Save the provided C++ code into a file named `turing_test.cpp`.
2. Open the file and locate line 19:
   ```cpp
   const string API_KEY = "YOUR_GROQ_API_KEY";