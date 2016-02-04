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
#ifndef AXORATCHET_H
#define AXORATCHET_H

/**
 * @file SalRatchet.h
 * @brief Salamander ratchet functions
 * @ingroup Salamander++
 * @{
 */

#include "../crypto/DhKeyPair.h"
#include "../crypto/DhPublicKey.h"
#include "../state/SalConversation.h"

using namespace std;

namespace salamander {
class AxoRatchet
{
public:
    AxoRatchet();
    ~AxoRatchet();

    /**
     * @brief Encrypt a message and message supplements, assemble a wire message.
     *
     * @param conv The Salamander conversation
     * @param message The plaintext message bytes.
     * @param supplements Additional data for the message, will be encrypted with the message key
     * @param idHashes The sender's and receiver's id hashes to send with the message, can be @c NULL if
     *                 not required
     * @return An encrypted wire message, ready to send to the recipient+device tuple.
     */
    static const string* encrypt(AxoConversation& conv, const string& message, const string& supplements, 
                                 string* supplementsEncrypted, pair<string, string>* idHashes = NULL);

    /**
     * @brief Parse a wire message and decrypt the payload.
     * 
     * @param conv The Salamander conversation
     * @param wire The wire message.
     * @param supplements Encrypted additional data for the message
     * @param supplementsPlain Additional data for the message if available and decryption was successful.
     * @param idHashes The sender's and receiver's id hashes contained in the message, can be @c NULL if
     *                 not available
     * @return Plaintext or @c NULL if decryption failed
     */
    static string* decrypt( salamander::AxoConversation* conv, const string& wire, const string& supplements, 
                            string* supplementsPlain, pair<string, string>* idHashes = NULL);
};
}
/**
 * @}
 */

#endif // AXORATCHET_H
