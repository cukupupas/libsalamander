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
#include <limits.h>
#include "gtest/gtest.h"

#include "../salamander/state/SalConversation.h"
#include "../storage/sqlite/SQLiteStoreConv.h"
#include "../salamander/crypto/EcCurve.h"
#include "../salamander/crypto/EcCurveTypes.h"
#include "../salamander/crypto/Ec255PublicKey.h"
#include "../util/UUID.h"

#include <iostream>
using namespace salamander;
using namespace std;

static std::string aliceName("alice@wonderland.org");
static std::string bobName("bob@milkyway.com");

static std::string aliceDev("aliceDevId");
static std::string bobDev("BobDevId");

static const uint8_t keyInDataC[] = {"1234567890098765432112345678901"}; // 32 bytes (incl. '\0')
static const uint8_t keyInDataD[] = {"aaaaaaaaaabbbbbbbbbbccccccccccd"};
static const uint8_t keyInDataE[] = {"AAAAAAAAAABBBBBBBBBBCCCCCCCCCCD"};
static const uint8_t keyInDataF[] = {"ZZZZZZZZZZYYYYYYYYYYXXXXXXXXXXW"};

static const uint8_t keyInData[] = {0,1,2,3,4,5,6,7,8,9,19,18,17,16,15,14,13,12,11,10,20,21,22,23,24,25,26,27,28,20,31,30};

void prepareStore()
{
    SQLiteStoreConv* store = SQLiteStoreConv::getStore();
    if (store->isReady())
        return;
    store->setKey(std::string((const char*)keyInData, 32));
    store->openStore(std::string());
    if (SQL_FAIL(store->getSqlCode())) {
        cerr << store->getLastError() << endl;
        exit(1);
    }
}

TEST(StagedKeys, Basic) 
{
    prepareStore();
    SQLiteStoreConv* store = SQLiteStoreConv::getStore();
    string mkiv((const char*)keyInData, 32);

    store->insertStagedMk(bobName, bobDev, aliceName, mkiv);
    ASSERT_FALSE(SQL_FAIL(store->getSqlCode())) << store->getLastError();

    list<string>* keys = store->loadStagedMks(bobName, bobDev, aliceName);
    ASSERT_FALSE(SQL_FAIL(store->getSqlCode())) << store->getLastError();
    ASSERT_TRUE(keys != NULL);
    ASSERT_EQ(1, keys->size());
    string both = keys->front();
    keys->pop_front();
    ASSERT_EQ(mkiv, both);
    delete keys; keys = NULL;

    store->deleteStagedMk(bobName, bobDev, aliceName, both);
    ASSERT_FALSE(SQL_FAIL(store->getSqlCode())) << store->getLastError();

    keys = store->loadStagedMks(bobName, bobDev, aliceName);
    ASSERT_FALSE(SQL_FAIL(store->getSqlCode())) << store->getLastError();
    ASSERT_TRUE(keys == NULL);
    delete keys;
}

TEST(StagedKeys, TimeDelete) 
{
    prepareStore();
    SQLiteStoreConv* store = SQLiteStoreConv::getStore();
    string mkiv((const char*)keyInDataC, 32);
    string mkiv_1((const char*)keyInDataD, 32);
    string mkiv_2((const char*)keyInDataE, 32);
    string mkiv_3((const char*)keyInDataF, 32);

    store->insertStagedMk(bobName, bobDev, aliceName, mkiv);
    ASSERT_FALSE(SQL_FAIL(store->getSqlCode())) << store->getLastError();

    store->insertStagedMk(bobName, bobDev, aliceName, mkiv_1);
    ASSERT_FALSE(SQL_FAIL(store->getSqlCode())) << store->getLastError();

    list<string>* keys = store->loadStagedMks(bobName, bobDev, aliceName);
    ASSERT_FALSE(SQL_FAIL(store->getSqlCode())) << store->getLastError();
    ASSERT_TRUE(keys != NULL);
    ASSERT_EQ(2, keys->size());
    delete keys; keys = NULL;

    sqlite3_sleep(5000);

    store->insertStagedMk(bobName, bobDev, aliceName, mkiv_2);
    ASSERT_FALSE(SQL_FAIL(store->getSqlCode())) << store->getLastError();

    store->insertStagedMk(bobName, bobDev, aliceName, mkiv_3);
    ASSERT_FALSE(SQL_FAIL(store->getSqlCode())) << store->getLastError();

    keys = store->loadStagedMks(bobName, bobDev, aliceName);
    ASSERT_FALSE(SQL_FAIL(store->getSqlCode())) << store->getLastError();
    ASSERT_TRUE(keys != NULL);
    ASSERT_EQ(4, keys->size());
    delete keys;

    time_t now_4 = time(0) - 4;
    store->deleteStagedMk(now_4);
    keys = store->loadStagedMks(bobName, bobDev, aliceName);

    ASSERT_FALSE(SQL_FAIL(store->getSqlCode())) << store->getLastError();
    ASSERT_TRUE(keys != NULL);
    ASSERT_EQ(2, keys->size());
    delete keys;
}

TEST(UUID, Basic)
{
    uuid_t uuid1;
    struct timeval tv;

    time_t tm = time(0);
    uuid_generate_time(uuid1);
    time_t tm1 = uuid_time(uuid1, &tv);

    uuid_string_t uuidString;
    uuid_unparse(uuid1, uuidString);

    cerr << "tm: " << tm << ", tm1: " << tm1 << endl;
    cerr << uuidString << endl;
}

