#ifndef PVPORTCLIENT_H
#define PVPORTCLIENT_H

#include <pv/pvAccess.h>
#include <pv/event.h>

namespace epics { namespace pvPortClient {

class epicsShareClass PVPortClient :
    public virtual epics::pvAccess::ChannelRequester,
    public epics::pvAccess::ChannelPutRequester,
    public std::tr1::enable_shared_from_this<PVPortClient>
{
private:
    std::string mProviderName;
    std::string mName;
    epics::pvData::PVStructurePtr mRequest;
    bool mConnected;
    epics::pvData::Event mEvent;
    epics::pvAccess::ChannelProvider::shared_pointer mProvider;
    epics::pvAccess::Channel::shared_pointer mChannel;
    epics::pvAccess::ChannelPut::shared_pointer mChannelPut;

    // Implemented for pvData::Requester
    std::string getRequesterName (void);
    void message (std::string const & message,
            epics::pvData::MessageType messageType);

    // Implemented for pvAccess::ChannelRequester
    void channelCreated (const epics::pvData::Status& status,
            epics::pvAccess::Channel::shared_pointer const & channel);
    void channelStateChange (epics::pvAccess::Channel::shared_pointer const & channel,
            epics::pvAccess::Channel::ConnectionState state);

    // Implemented for pvAccess::ChannelPutRequester
    virtual void channelPutConnect(const epics::pvData::Status& status,
            epics::pvAccess::ChannelPut::shared_pointer const & channelPut,
            epics::pvData::Structure::const_shared_pointer const & structure);

    virtual void getDone(const epics::pvData::Status& status,
             epics::pvAccess::ChannelPut::shared_pointer const & channelPut,
             epics::pvData::PVStructure::shared_pointer const & pvStructure,
             epics::pvData::BitSet::shared_pointer const & bitSet);

    virtual void putDone(const epics::pvData::Status& status,
            epics::pvAccess::ChannelPut::shared_pointer const & channelPut);

protected:
    epics::pvData::Mutex mMutex;
    epics::pvData::PVStructurePtr mPvStructure;
    epics::pvData::BitSetPtr mBitSet;

    PVPortClient(std::string const & providerName, std::string const & name,
            std::string const & request);

    void init (void);

public:
    POINTER_DEFINITIONS(PVPortClient);
    static PVPortClient::shared_pointer create (std::string const & providerName,
            std::string const & name, std::string const & request = "field()");
    virtual ~PVPortClient() {}

    bool connected (void);

    epics::pvData::PVStructurePtr get (double timeout = 1.0);
    void put (const epics::pvData::PVStructurePtr & data, double timeout = 1.0);

};

}}

#endif  /* PVPORTDRIVER_H */

