#include <libcga/blind_signature/client.hpp>
#include <libcga/blind_signature/server.hpp>

#include <cassert>
#include <iostream>

int main() {
    cga::blindsign::Client client;
    cga::blindsign::Server server;

    const auto h = client.vote(server.d(), server.n());
    const auto s = server.sign(h);
    const auto pair = client.bulletin(s, server.n());
    server.verify(pair.first, pair.second);
    assert(server.verify(pair.first, pair.second));
}