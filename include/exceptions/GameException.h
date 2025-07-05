#ifndef GAME_EXCEPTION_H
#define GAME_EXCEPTION_H

#include <exception>
#include <string>


class GameException : public std::exception {
public:

    GameException(const std::string& message)
        : m_message(message)
    {
    }
    

    const char* what() const noexcept override {
        return m_message.c_str();
    }
    
protected:

    std::string m_message;
};


class ResourceException : public GameException {
public:
    ResourceException(const std::string& message)
        : GameException("Resource error: " + message)
    {
    }
};


class InitializationException : public GameException {
public:
    InitializationException(const std::string& message)
        : GameException("Initialization error: " + message)
    {
    }
};


class GameLogicException : public GameException {
public:
    GameLogicException(const std::string& message)
        : GameException("Game logic error: " + message)
    {
    }
};

#endif