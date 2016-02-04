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
#include "DhKeyPair.h"
#include "EcCurveTypes.h"
#include "Ec255PrivateKey.h"
#include "Ec255PublicKey.h"

using namespace salamander;

DhKeyPair::DhKeyPair(const DhPublicKey& publicKey, const DhPrivateKey& privateKey)
{
    if (publicKey.getType() == EcCurveTypes::Curve25519)
        publicKey_  = new Ec255PublicKey(publicKey.getPublicKeyPointer());
    else
        publicKey_ = NULL;

    if (privateKey.getType() == EcCurveTypes::Curve25519)
        privateKey_  = new Ec255PrivateKey(privateKey.privateData());
    else
        privateKey_ = NULL;
}

DhKeyPair::DhKeyPair (const DhKeyPair& otherPair)
{
    if (otherPair.publicKey_->getType() == EcCurveTypes::Curve25519)
        publicKey_  = new Ec255PublicKey(otherPair.publicKey_->getPublicKeyPointer());
    else
        publicKey_ = NULL;

    if (otherPair.privateKey_->getType() == EcCurveTypes::Curve25519)
        privateKey_  = new Ec255PrivateKey(otherPair.privateKey_->privateData());
    else
        privateKey_ = NULL;
}

DhKeyPair::~DhKeyPair()
{
    delete publicKey_;
    delete privateKey_;
}

DhKeyPair& DhKeyPair::operator=(const DhKeyPair& otherPair)
{
    if (this == &otherPair)
        return *this;

    delete privateKey_;
    delete publicKey_;
    if (otherPair.publicKey_->getType() == EcCurveTypes::Curve25519)
        publicKey_  = new Ec255PublicKey(otherPair.publicKey_->getPublicKeyPointer());
    else
        publicKey_ = NULL;

    if (otherPair.privateKey_->getType() == EcCurveTypes::Curve25519)
        privateKey_  = new Ec255PrivateKey(otherPair.privateKey_->privateData());
    else
        privateKey_ = NULL;
    return *this;
}
