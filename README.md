# Encrypted Chat Application

This project aims to develop a secure chat application with end-to-end encryption, persistent message storage, and a modern user interface.

## Project Status

This project is currently under active development. The foundational client-server communication structure is in place, with ongoing work to integrate advanced features like encryption and database functionalities.

## Features

### Current Features
*   **Client-Server Architecture**: Basic communication established between a chat client and server.
*   **Packet Handling**: Custom packet structure for message transmission.

### Planned Features
*   **End-to-End Encryption**: Implement robust cryptographic protocols to ensure secure communication between users.
*   **Persistent Message Storage**:
    *   **Server-side Database**: Store chat histories and user data securely on the server.
    *   **Client-side Database**: Cache messages locally on the client for offline access and improved performance.
*   **Terminal User Interface (TUI)**: Develop an interactive and user-friendly command-line interface for an enhanced chat experience.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

*   A C++ compiler (e.g., g++)
*   Make (or similar build system)

### Building the Project

1.  Clone the repository:
    ```bash
    git clone https://github.com/User-4675/Terminal-Chat.git
    cd Terminal-Chat/bins
    ```
2.  Build the client and server executables:
    ```bash
    ./build.sh
    ```
    This script should compile the source files and place the executables (`client.out` and `server.out`) in the `bins/` directory.

### Running the Application

1.  **Start the Server**:
    In one terminal, navigate to the project root and run:
    ```bash
    ./server.out
    ```
2.  **Start the Client**:
    In another terminal, navigate to the project root and run:
    ```bash
    ./client.out <client-id>
    ```
    You can open multiple client instances to simulate a chat.

## Project Structure

*   `bins/`: Contains build scripts and compiled executables.
*   `client/`: Source code for the chat client.
*   `clientJSON/`: Intended for client-side JSON related files or configurations (currently empty).
*   `packet/`: Defines the packet structure used for network communication.
*   `server/`: Source code for the chat server.
