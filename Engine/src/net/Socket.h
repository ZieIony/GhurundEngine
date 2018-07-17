#pragma once

#include "winsock2.h"
#include "../Ghurund.h"
#include "../core/Logger.h"
#include "ws2tcpip.h"

namespace Ghurund {
    /**
     * Protok? gniazdka sieciowego
     */
    enum class SocketProtocol {
        TCP = SOCK_STREAM, UDP = SOCK_DGRAM
    };

    /**
     * Klasa gniazdka sieciowego kompaktuj?ca wszystkie potrzebne do po??czenia
     * informacje
     */
    class Socket {
    private:
        static const int BUFFER_SIZE = 4096;

        ::SOCKET id = INVALID_SOCKET;
        unsigned short port = 0;
        tchar *address = nullptr;
        sockaddr_in *addressStruct = nullptr;
        sockaddr_in6 *addressStruct6 = nullptr;
        SocketProtocol protocol;
        WSAEVENT eventHandle = 0;
        WSABUF dataBuffer;

    public:
        Socket() {
            memset(&dataBuffer, 0, sizeof(WSABUF));
        }

        ~Socket() {
            close();
        }

        /**
         * Inicjalizuje gniazdko o podanych parametrach
         * @param SocketProtocol protocol protok? po??czenia
         * @param unsigned long address adres IPv4
         * @param unsigned short port port komunikacyjny
         * @return kod b??du
         */
        Status init(::SOCKET id, SocketProtocol protocol, const tchar *address, unsigned short port);

        Status init(SocketProtocol protocol, const tchar *address, unsigned short port) {
            return init(::socket(AF_INET, (int)protocol, 0), protocol, address, port);
        }

        ::SOCKET getId() const {
            return id;
        }

        __declspec(property(get = getId)) ::SOCKET Id;

        WSAEVENT getEventHandle() const {
            return eventHandle;
        }

        __declspec(property(get = getEventHandle)) WSAEVENT EventHandle;

        sockaddr *getAddressStruct()const;

        Status send(void *data, size_t size, unsigned int flags = 0)const;

        Status receive(void **data = nullptr, size_t *size = nullptr);

        void *getData()const {
            return dataBuffer.buf;
        }

        size_t getDataSize()const {
            return dataBuffer.len;
        }

        Status bind();

        Status select();

        Socket * accept() const;

        void close() {
            delete[] dataBuffer.buf;
            WSACloseEvent(eventHandle);
            closesocket(id);
            delete[] address;
            delete addressStruct;
            delete addressStruct6;
        }
    };
}