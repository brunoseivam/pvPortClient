#include <pv/clientFactory.h>
#include "pv/pvPortClient.h"

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace std;
using namespace epics::pvPortClient;

PVPortClient::PVPortClient(const string & providerName, const string & name, const string & request) :
    mProviderName(providerName), mName(name), mRequest(CreateRequest::create()->createRequest(request)),
    mConnected(false)
{
}

void PVPortClient::init (void)
{
    ClientFactory::start();
    mProvider = getChannelProviderRegistry()->getProvider(mProviderName);
    mChannel = mProvider->createChannel(mName, shared_from_this());
    mChannelPut = mChannel->createChannelPut(shared_from_this(), mRequest);
}

PVPortClient::shared_pointer PVPortClient::create (const string & providerName,
        const string & name, const string & request)
{
    PVPortClient::shared_pointer p(new PVPortClient(providerName, name, request));
    p->init();
    return p;
}

PVStructurePtr PVPortClient::get (double timeout)
{
    PVStructurePtr nullPtr;
    mChannelPut->get();
    if(!mEvent.wait(timeout))
        return nullPtr;
    return mPvStructure;
}

void PVPortClient::put (const PVStructurePtr & data, double timeout)
{
    mChannelPut->put(data, mBitSet);
    if(!mEvent.wait(timeout))
        printf("PVPortClient::put timeout\n");
    mEvent.signal();
}

string PVPortClient::getRequesterName (void)
{
    return string("PVPortClient");
}

void PVPortClient::message (string const & message, MessageType messageType)
{
    printf("PVPortClient::message: [provider=%s, record=%s] [type=%s] %s\n",
            mProviderName.c_str(), mName.c_str(),
            getMessageTypeName(messageType).c_str(), message.c_str());
}

void PVPortClient::channelCreated (const Status& status,
        Channel::shared_pointer const & channel)
{
    printf("PVPortClient::channelCreated: %s created\n", channel->getChannelName().c_str());
}

void PVPortClient::channelStateChange (Channel::shared_pointer const & channel,
        Channel::ConnectionState state)
{
    printf("PVPortClient::channelStateChange %s: %s\n",
            channel->getChannelName().c_str(),
            Channel::ConnectionStateNames[state]);
    mConnected = state == Channel::CONNECTED;
}

void PVPortClient::channelPutConnect(const Status& status,
        ChannelPut::shared_pointer const & channelPut,
        Structure::const_shared_pointer const & structure)
{
    printf("PVPortClient::channelPutConnect %s\n", channelPut->getChannel()->getChannelName().c_str());
    channelPut->get();
    mEvent.wait(1.0);
}

void PVPortClient::getDone(const Status& status,
         ChannelPut::shared_pointer const & channelPut,
         PVStructure::shared_pointer const & pvStructure,
         BitSet::shared_pointer const & bitSet)
{
    printf("PVPortClient::getDone %s\n", channelPut->getChannel()->getChannelName().c_str());

    if(status.isSuccess())
    {
        mPvStructure = pvStructure;
        mBitSet = bitSet;
    }
    else
        printf("PVPortClient::getDone %s: failed to get\n", channelPut->getChannel()->getChannelName().c_str());

    mEvent.signal();
}

void PVPortClient::putDone(const Status& status,
        ChannelPut::shared_pointer const & channelPut)
{
    printf("PVPortClient::putDone %s\n", channelPut->getChannel()->getChannelName().c_str());
    mEvent.signal();
}

bool PVPortClient::connected(void)
{
    return mConnected;
}
