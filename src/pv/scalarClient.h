#ifndef SCALARCLIENT_H
#define SCALARCLIENT_H

#include <pv/ntscalar.h>
#include "pvPortClient.h"

namespace epics { namespace pvPortClient {

template<typename T>
class epicsShareClass NTScalarClient : public PVPortClient
{
private:
    NTScalarClient(std::string const & providerName, std::string const & name) :
            PVPortClient(providerName, name, "field()")
        {}
public:
    POINTER_DEFINITIONS(NTScalarClient);

    static std::tr1::shared_ptr<NTScalarClient> create (std::string const & providerName,
            std::string const & name)
    {
        std::tr1::shared_ptr<NTScalarClient> client(new NTScalarClient(providerName, name));
        client->init();
        return client;
    }

    T get (void)
    {
        epics::nt::NTScalarPtr ntScalar(epics::nt::NTScalar::wrap(PVPortClient::get()));
        std::tr1::shared_ptr< epics::pvData::PVScalarValue<T> >
        pvScalar(ntScalar->getValue< epics::pvData::PVScalarValue<T> >());
        return pvScalar->get();
    }

    void put (T value)
    {
        epics::nt::NTScalarPtr ntScalar(epics::nt::NTScalar::wrap(mPvStructure));
        std::tr1::shared_ptr< epics::pvData::PVScalarValue<T> >
        pvScalar(ntScalar->getValue< epics::pvData::PVScalarValue<T> >());
        pvScalar->put(value);
        PVPortClient::put(mPvStructure);
    }
};

typedef NTScalarClient<epics::pvData::boolean> BooleanParamClient;
typedef NTScalarClient<epics::pvData::int8> ByteParamClient;
typedef NTScalarClient<epics::pvData::uint8> UByteParamClient;
typedef NTScalarClient<epics::pvData::int16> ShortParamClient;
typedef NTScalarClient<epics::pvData::uint16> UShortParamClient;
typedef NTScalarClient<epics::pvData::int32> IntParamClient;
typedef NTScalarClient<epics::pvData::uint32> UIntParamClient;
typedef NTScalarClient<epics::pvData::int64> LongParamClient;
typedef NTScalarClient<epics::pvData::uint64> ULongParamClient;
typedef NTScalarClient<float> FloatParamClient;
typedef NTScalarClient<double> DoubleParamClient;
typedef NTScalarClient<std::string> StringParamClient;

typedef std::tr1::shared_ptr<BooleanParamClient> BooleanParamClientPtr;
typedef std::tr1::shared_ptr<ByteParamClient> ByteParamClientPtr;
typedef std::tr1::shared_ptr<UByteParamClient> UByteParamClientPtr;
typedef std::tr1::shared_ptr<ShortParamClient> ShortParamClientPtr;
typedef std::tr1::shared_ptr<UShortParamClient> UShortParamClientPtr;
typedef std::tr1::shared_ptr<IntParamClient> IntParamClientPtr;
typedef std::tr1::shared_ptr<UIntParamClient> UIntParamClientPtr;
typedef std::tr1::shared_ptr<LongParamClient> LongParamClientPtr;
typedef std::tr1::shared_ptr<ULongParamClient> ULongParamClientPtr;
typedef std::tr1::shared_ptr<FloatParamClient> FloatParamClientPtr;
typedef std::tr1::shared_ptr<DoubleParamClient> DoubleParamClientPtr;
typedef std::tr1::shared_ptr<StringParamClient> StringParamClientPtr;

}}

#endif // SCALARCLIENT_H
