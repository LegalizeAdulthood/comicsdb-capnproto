#include "comicsdb.h"
#include "comic.capnp.h"

#include <capnp/ez-rpc.h>

#include <algorithm>
#include <iostream>

namespace comicsDatabase = comicsdb::v2;

class DatabaseImpl : public Database::Server
{
  public:
    DatabaseImpl(comicsDatabase::ComicDb &db) : m_db(db) {}

    kj::Promise<void> list(ListContext context) override;
    kj::Promise<void> read(ReadContext context) override;
    kj::Promise<void> create(CreateContext context) override;
    kj::Promise<void> update(UpdateContext context) override;
    kj::Promise<void> delete_(DeleteContext context) override;

private:
    comicsDatabase::ComicDb &m_db;
};

kj::Promise<void> DatabaseImpl::list(ListContext context)
{
    std::vector<int64_t> ids;
    for (size_t i = 0; i < m_db.size(); ++i)
    {
        if (m_db[i].issue != comicsDatabase::Comic::DELETED_ISSUE)
        {
            ids.push_back(i);
        }
    }
    ListResults::Builder result = context.getResults();
    result.setList(kj::ArrayPtr<const int64_t>(ids.data(), ids.size()));
    return kj::READY_NOW;
}

kj::Promise<void> DatabaseImpl::read(ReadContext context)
{
    int64_t id = context.getParams().getId();
    try
    {
        comicsDatabase::Comic comic = readComic(m_db, id);
        auto builder = context.getResults();
        auto result = builder.getComic();
        result.setTitle(comic.title);
        result.setIssue(static_cast<int16_t>(comic.issue));
        result.getScript().setName(comic.script->name);
        result.getPencils().setName(comic.pencils->name);
        result.getInks().setName(comic.inks->name);
        result.getColors().setName(comic.colors->name);
        result.getLetters().setName(comic.letters->name);
    }
    catch (...)
    {
    }
    return kj::READY_NOW;
}

kj::Promise<void> DatabaseImpl::create(CreateContext context)
{
    return Server::create(context);
}

kj::Promise<void> DatabaseImpl::update(UpdateContext context)
{
    return Server::update(context);
}

kj::Promise<void> DatabaseImpl::delete_(DeleteContext context)
{
    return Server::delete_(context);
}

int main()
{
    comicsDatabase::ComicDb db = comicsDatabase::load();
    capnp::EzRpcServer server(kj::heap<DatabaseImpl>(db), "localhost", 8192);
    auto &waitScope = server.getWaitScope();

    kj::NEVER_DONE.wait(waitScope);
}
