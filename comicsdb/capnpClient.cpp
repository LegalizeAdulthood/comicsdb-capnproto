#include "comic.capnp.h"

#include <capnp/ez-rpc.h>

#include <iostream>

int main()
{
    capnp::EzRpcClient client("localhost:8192");
    auto &waitScope = client.getWaitScope();
    Database::Client db = client.getMain<Database>();
    capnp::Request<Database::ListParams, Database::ListResults> request =
        db.listRequest();
    capnp::Response<Database::ListResults> response =
        request.send().wait(waitScope);

    if (response.getList().size() > 0)
    {
        for (auto id : response.getList())
        {
            std::cout << id << '\n';
            capnp::Request<Database::ReadParams, Database::ReadResults> readRequest = db.readRequest();
            readRequest.setId(id);
            capnp::Response<Database::ReadResults> readResponse = readRequest.send().wait(waitScope);
            auto comic = readResponse.getComic();
            std::cout << "Title:   " << comic.getTitle().cStr() << '\n';
            std::cout << "Issue:   " << comic.getIssue() << '\n';
            std::cout << "Script:  " << comic.getScript().getName().cStr() << '\n';
            std::cout << "Pencils: " << comic.getPencils().getName().cStr() << '\n';
            std::cout << "Inks:    " << comic.getInks().getName().cStr() << '\n';
            std::cout << "Colors:  " << comic.getColors().getName().cStr() << '\n';
            std::cout << "Letters: " << comic.getLetters().getName().cStr() << '\n';
        }
    }
    
    return 0;
}
