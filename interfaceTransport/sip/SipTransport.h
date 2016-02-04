/*
Copyright 2016 Silent Circle, LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#ifndef SIPTRANSPORT_H
#define SIPTRANSPORT_H

/**
 * @file SipTransport.h
 * @brief Implementation for network transport functions, SIP transport
 * @ingroup Salamander++
 * @{
 */

#include <utility>
#include <vector>
#include <string>

#include "../Transport.h"
#include "../../interfaceApp/AppInterface.h"

using namespace std;

namespace salamander {

class SipTransport: public Transport
{
public:
    SipTransport(AppInterface* appInterface) : appInterface_(appInterface) {}

    ~SipTransport() {}

    void setSendDataFunction(SEND_DATA_FUNC sendData) { sendAxoData_ = sendData; }

    SEND_DATA_FUNC getTransport() { return sendAxoData_; }

    vector<int64_t>* sendAxoMessage(const string& recipient, vector< pair< string, string > >* msgPairs);

    int32_t receiveAxoMessage(uint8_t* data, size_t length);

    void stateReportAxo(int64_t messageIdentifier, int32_t stateCode, uint8_t* data, size_t length);

    void notifyAxo(uint8_t* data, size_t length);

private:

    SipTransport(const SipTransport& other) {}
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type"
    SipTransport& operator= ( const SipTransport& other ) { }
    bool operator== ( const SipTransport& other ) const { }
#pragma clang diagnostic pop

    AppInterface *appInterface_;
    SEND_DATA_FUNC sendAxoData_;
};
}
/**
 * @}
 */

#endif // SIPTRANSPORT_H
